#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "smtpd.h"
#include "smtp_parser.h"
#include "log.h"

#define SMTPD_HOST "0.0.0.0"
#define SMTPD_PORT 2525
#define CONNECTIONS_MAX 32
#define CONNECTION_TIMEOUT 60

#define QUEUE_DIR "./queue"

static volatile int daemon_loop = 1;
static int connections_cnt = 0;

static void
sigdie_handler (int signo)
{
	daemon_loop = 0;
}

static void
sigchld_handler (int signo)
{
	pid_t pid;
	int status;

	while ( (pid = waitpid (-1, &status, WNOHANG)) != -1 ){
		connections_cnt--;
	}
}

//
// Callbacks for smtp parser
//
static int
parser_on_helo_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	fprintf (stderr, "HELO\n");
	return 1;
}

static int
parser_on_ehlo_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	fprintf (stderr, "EHLO\n");
	return 1;
}

static int
parser_on_mail_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	fprintf (stderr, "MAIL\n");
	return 1;
}

static int
parser_on_rcpt_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	fprintf (stderr, "RCPT\n");
	return 1;

}

static int
parser_on_data_cb (void *user_data)
{
	fprintf (stderr, "DATA\n");
	return 1;
}

static int
parser_on_eof_cb (void *user_data)
{
	fprintf (stderr, "EOF\n");
	return 1;

}

static int
parser_on_rset_cb (void *user_data)
{
	fprintf (stderr, "RSET\n");
	return 1;
}

static int
parser_on_vrfy_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	fprintf (stderr, "VRFY\n");
	return 1;
}

static int
parser_on_noop_cb (void *user_data)
{
	fprintf (stderr, "NOOP\n");
	return 1;
}

static int
parser_on_quit_cb (void *user_data)
{
	fprintf (stderr, "QUIT\n");
	return 1;
}

int
main (int argc, char *argv[])
{
	smtpd_t smtpd;
	struct smtp_parser parser;
	struct smtpd_config smtpd_config;
	struct sigaction sa;
	int rval;

	openlog ("smtpd", LOG_PID | LOG_PERROR, LOG_USER);

	// Setup signals
	sa.sa_handler = sigdie_handler;
	sigemptyset (&(sa.sa_mask));
	sa.sa_flags = 0;

	rval = 0;
	rval &= sigaction (SIGINT, &sa, NULL);
	rval &= sigaction (SIGQUIT, &sa, NULL);
	rval &= sigaction (SIGTERM, &sa, NULL);

	sa.sa_handler = sigchld_handler;
	sigemptyset (&(sa.sa_mask));
	sa.sa_flags = 0;

	rval &= sigaction (SIGCHLD, &sa, NULL);

	sa.sa_handler = SIG_IGN;
	sigemptyset (&(sa.sa_mask));
	sa.sa_flags = 0;

	rval &= sigaction (SIGPIPE, &sa, NULL);

	if ( rval ){
		log_error ("Cannot setup signal handlers: %s", strerror (errno));
		return EXIT_FAILURE;
	}

	// Configure smtp parser
	parser.on_helo = parser_on_helo_cb;
	parser.on_ehlo = parser_on_ehlo_cb;
	parser.on_mail = parser_on_mail_cb;
	parser.on_rcpt = parser_on_rcpt_cb;
	parser.on_data = parser_on_data_cb;
	parser.on_eof = parser_on_eof_cb;
	parser.on_rset = parser_on_rset_cb;
	parser.on_vrfy = parser_on_vrfy_cb;
	parser.on_noop = parser_on_noop_cb;
	parser.on_quit = parser_on_quit_cb;

	smtp_parser_init (&parser);

	// Configure smtpd
	smtpd_config.address = SMTPD_HOST;
	smtpd_config.port = SMTPD_PORT;

	rval = smtpd_open (&smtpd);

	if ( rval != 0 ){
		log_error ("Cannot create socket: %s", strerror (errno));
		return EXIT_FAILURE;
	}

	rval = smtpd_listen (&smtpd, &smtpd_config);

	if ( rval != 0 ){
		log_error ("Cannot start listening on %s:%u: %s", smtpd_config.address,
															smtpd_config.port, strerror (errno));
		smtpd_close (&smtpd);
		return EXIT_FAILURE;
	}

	log_info ("Daemon started, listening on %s:%u", smtpd_config.address, smtpd_config.port);

	while ( daemon_loop ){
		struct smtpd_client smtpd_client;
		pid_t pid;

		rval = smtpd_accept (&smtpd, &smtpd_client);

		if ( rval != 0 ){

			// If accept failed due to signal handler, jump at the beginning of the loop and try again
			if ( errno == EINTR ){
				errno = 0;
				continue;
			}

			log_error ("Cannot accept new connection from: %s", strerror (errno));
			smtpd_close (&smtpd);
			return EXIT_FAILURE;
		}

		if ( ++connections_cnt > CONNECTIONS_MAX ){
			log_info ("Cannot accept new connection: too many concurrent connections (%u/%u).", --connections_cnt, CONNECTIONS_MAX);
			smtpd_client_close (&smtpd_client);
			continue;
		}

		log_info ("New connection accepted for %s:%u", smtpd_client.address, smtpd_client.port);

		pid = fork ();

		if ( pid == -1 ){
			log_error ("Cannot fork the parent process: %s", strerror (errno));
			smtpd_client_close (&smtpd_client);	
			smtpd_close (&smtpd);
			return EXIT_FAILURE;

		} else if ( pid > 0 ){
			continue;
		}

		// Client IO loop
		for ( ;; ){
			unsigned char msg_buff[256];
			struct timeval tv;
			ssize_t recv_len;
			size_t parsed_len;
			fd_set rdset;

			parsed_len = 0;

			tv.tv_sec = CONNECTION_TIMEOUT;
			tv.tv_usec = 0;

			FD_ZERO (&rdset);
			FD_SET (smtpd_client.sock, &rdset);

			rval = select (smtpd_client.sock + 1, &rdset, NULL, NULL, &tv);

			if ( rval == -1 ){
				log_error ("Select failed: %s", strerror (errno));
				smtpd_client_close (&smtpd_client);
				smtpd_close (&smtpd);
				return EXIT_FAILURE;

			} else if ( rval == 0 ){
				log_info ("Connection timeout.");
				smtpd_client_close (&smtpd_client);
				smtpd_close (&smtpd);
				return EXIT_FAILURE;
			}

			recv_len = recv (smtpd_client.sock, msg_buff, sizeof (msg_buff), 0);

			if ( recv_len <= 0 ){

				if ( errno ){
					log_error ("Cannot receive data from connected client: %s", strerror (errno));
					smtpd_client_close (&smtpd_client);
					smtpd_close (&smtpd);
					return EXIT_FAILURE;
				}
				
				log_info ("Connection closed.");
				smtpd_client_close (&smtpd_client);
				smtpd_close (&smtpd);
				return EXIT_SUCCESS;
			}

			// Parser loop
			do {
				parsed_len += smtp_parser_exec (&parser, (const char*)  msg_buff, recv_len);
			} while ( parsed_len != recv_len );
		}
	}

	log_info ("Daemon killed.");

	smtpd_close (&smtpd);

	return EXIT_SUCCESS;;
}

