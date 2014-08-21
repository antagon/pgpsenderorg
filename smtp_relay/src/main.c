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
#include "smtp.h"
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


int
main (int argc, char *argv[])
{
	smtpd_t smtpd;
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

		while ( 1 ){
			unsigned char msg_buff[256];
			struct smtp_request smtp_request;
			struct timeval tv;
			ssize_t recv_len;
			fd_set rdset;

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

			if ( smtp_parse_request ((char*) msg_buff, recv_len, &smtp_request) == SMTP_E_OK ){

				switch ( smtp_request.type ){
					case SMTP_C_HELO:
						fprintf (stderr, "HELO!!!\n");
						break;
					case SMTP_C_EHLO:
						fprintf (stderr, "EHLO!!!\n");
						break;
					case SMTP_C_MAILFROM:
						fprintf (stderr, "MAILFROM!!!\n");
						break;
					case SMTP_C_RCPTTO:
						fprintf (stderr, "RCPTTO!!!\n");
						break;
					case SMTP_C_DATA:
						fprintf (stderr, "DATA!!!\n");
						break;
					case SMTP_C_RSET:
						fprintf (stderr, "RSET!!!\n");
						break;
					case SMTP_C_VRFY:
						fprintf (stderr, "VRFY!!!\n");
						break;
					case SMTP_C_NOOP:
						fprintf (stderr, "NOOP!!!\n");
						break;
					case SMTP_C_QUIT:
						fprintf (stderr, "QUIT!!!\n");
						break;
				}
			}
		}
	}

	log_info ("Daemon killed.");

	smtpd_close (&smtpd);

	return EXIT_SUCCESS;;
}

