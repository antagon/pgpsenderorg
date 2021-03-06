#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <getopt.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "smtpd.h"
#include "smtp_parser.h"
#include "smtp_envelope.h"
#include "log.h"

#define SMTPD_HOST "0.0.0.0"
#define SMTPD_PORT 2525
#define CONNECTIONS_MAX 32
#define CONNECTION_TIMEOUT 60

struct config
{
	uint16_t port;
	char *sqlite_db_path;
	char *mail_queue_path;
};

static volatile int daemon_loop = 1;
static int connections_cnt = 0;
static sqlite3 *sqlite;

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

static void
print_usage (const char *p)
{
	fprintf (stderr, "Usage: %s -d <DIR> -f <FILE> [-p <PORT>]\n\nOptions:\n\
  -d <DIR> mail queue directory path\n\
  -f <FILE> sqlite database file\n\
  -p <PORT> listening TCP port\n\
  -h show usage\n", p);
}

//
// Callbacks for smtp parser
//
static int
parser_on_helo_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	struct smtp_env *envelope;
	size_t copy_len;

	envelope = (struct smtp_env*) user_data;
	copy_len = argv->len;

	if ( argc < 1 ){
		envelope->codeno = SMTP_EARGSYNTAX;
		return 1;
	}

	if ( argv->len > SMTP_ADDR_MAX )
		copy_len = SMTP_ADDR_MAX - 1;

	strncpy (envelope->hostname, argv->val, copy_len);
	envelope->hostname[copy_len] = '\0';

	envelope->codeno = SMTP_MAILOK;
	envelope->sequence |= SMTP_C_HELO;

	return 1;
}

static int
parser_on_ehlo_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	return parser_on_helo_cb (argv, argc, user_data);
}

static int
parser_on_mail_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	struct smtp_env *envelope;
	char buff[SMTP_ADDR_MAX];
	size_t copy_len;

	envelope = (struct smtp_env*) user_data;
	copy_len = argv->len;

	if ( argc < 1 ){
		envelope->codeno = SMTP_EARGSYNTAX;
		return 1;
	}

	if ( (envelope->sequence & SMTP_C_HELO) == 0 ){
		envelope->codeno = SMTP_EBADSEQ;
		return 1;
	}

	if ( argv->len > SMTP_ADDR_MAX )
		copy_len = SMTP_ADDR_MAX - 1;
	
	strncpy (buff, argv->val, copy_len);
	buff[copy_len] = '\0';

	if ( smtp_env_add_recipient (envelope, buff) == NULL )
		return 0;
	
	envelope->codeno = SMTP_MAILOK;
	envelope->sequence |= SMTP_C_MAILFROM;

	return 1;
}

static int
parser_on_rcpt_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	struct smtp_env *envelope;
	char buff[SMTP_ADDR_MAX];
	size_t copy_len;

	envelope = (struct smtp_env*) user_data;
	copy_len = argv->len;

	if ( argc < 1 ){
		envelope->codeno = SMTP_EARGSYNTAX;
		return 1;
	}

	if ( ((envelope->sequence & SMTP_C_HELO) == 0)
			|| ((envelope->sequence & SMTP_C_MAILFROM) == 0) ){
		envelope->codeno = SMTP_EBADSEQ;
		return 1;
	}

	if ( argv->len > SMTP_ADDR_MAX )
		copy_len = SMTP_ADDR_MAX - 1;
	
	strncpy (buff, argv->val, copy_len);
	buff[copy_len] = '\0';

	if ( smtp_env_add_recipient (envelope, buff) == NULL )
		return 0;

	envelope->codeno = SMTP_MAILOK;
	envelope->sequence |= SMTP_C_RCPTTO;

	return 1;
}

static int
parser_on_data_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	if ( ((envelope->sequence & SMTP_C_HELO) == 0)
			|| ((envelope->sequence & SMTP_C_MAILFROM) == 0)
			|| ((envelope->sequence & SMTP_C_RCPTTO) == 0) ){
		envelope->codeno = SMTP_EBADSEQ;
		return 1;
	}

	envelope->file_data = tmpfile ();

	if ( envelope->file_data == NULL )
		return 0;

	envelope->codeno = SMTP_STARTMAIL;
	envelope->sequence |= SMTP_C_DATA;

	return 1;
}

static int
parser_on_eof_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	if ( ((envelope->sequence & SMTP_C_HELO) == 0)
			|| ((envelope->sequence & SMTP_C_MAILFROM) == 0)
			|| ((envelope->sequence & SMTP_C_RCPTTO) == 0)
			|| ((envelope->sequence & SMTP_C_DATA) == 0) ){
		envelope->codeno = SMTP_EBADSEQ;
		return 1;
	}

	smtp_env_free (envelope);

	envelope->codeno = SMTP_MAILOK;

	return 1;
}

static int
parser_on_rset_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	smtp_env_free (envelope);

	envelope->codeno = SMTP_MAILOK;
	envelope->sequence |= SMTP_C_HELO;

	return 1;
}

static int
parser_on_vrfy_cb (struct smtp_req_arg *argv, size_t argc, void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	envelope->codeno = SMTP_ECMDNIMPL;

	return 1;
}

static int
parser_on_noop_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	envelope->codeno = SMTP_MAILOK;

	return 1;
}

static int
parser_on_quit_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	envelope->codeno = SMTP_BYE;

	return 1;
}

static int
parser_on_unknown_cb (void *user_data)
{
	struct smtp_env *envelope;

	envelope = (struct smtp_env*) user_data;

	envelope->codeno = SMTP_ESYNTAX;

	return 1;
}

int
main (int argc, char *argv[])
{
	smtpd_t smtpd;
	struct smtpd_config smtpd_config;
	struct smtp_parser parser;
	struct smtp_env envelope;
	struct config config;
	struct sigaction sa;
	int rval;

	memset (&config, 0, sizeof (struct config));

	while ( (rval = getopt (argc, argv, "d:f:p:h")) != -1 ){

		switch ( rval ){
			case 'd':
				config.mail_queue_path = strdup (optarg);
				break;
			case 'f':
				config.sqlite_db_path = strdup (optarg);
				break;
			case 'p':
				config.port = strtoll (optarg, NULL, 10);
				break;
			case 'h':
				print_usage (argv[0]);
				return EXIT_SUCCESS;
			default:
				print_usage (argv[0]);
				return EXIT_FAILURE;
		}
	}

	if ( config.mail_queue_path == NULL ){
		print_usage (argv[0]);
		return EXIT_FAILURE;
	}

	if ( config.sqlite_db_path == NULL ){
		print_usage (argv[0]);	
		return EXIT_FAILURE;
	}

	if ( config.port == 0 )
		config.port = SMTPD_PORT;

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
	parser.user_data = &envelope;
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
	parser.on_unknown = parser_on_unknown_cb;

	smtp_parser_init (&parser);

	// Configure smtpd
	smtpd_config.address = SMTPD_HOST;
	smtpd_config.port = config.port;

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

	// Initialize envelope
	smtp_env_init (&envelope);

	// Open sqlite database
	rval = sqlite3_open (config.sqlite_db_path, &sqlite);

	if ( rval != SQLITE_OK ){
		log_error ("Cannot open sqlite3 database in file '%s'", "TODO");
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

		log_info ("New connection accepted from %s:%u", smtpd_client.address, smtpd_client.port);

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
				char res_buff[256];
				ssize_t send_len;
				
				parsed_len += smtp_parser_exec (&parser, (const char*) msg_buff, recv_len);

				if ( smtp_strresponse (envelope.codeno, res_buff, sizeof (res_buff)) == -1 ){
					log_error ("Unknown SMTP code %d, connection closed", envelope.codeno);
					smtpd_client_close (&smtpd_client);
					smtpd_close (&smtpd);
					return EXIT_FAILURE;
				}

				send_len = send (smtpd_client.sock, res_buff, strlen (res_buff), 0);

				if ( send_len <= 0 ){
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

			} while ( parsed_len != recv_len );
		}
	}

	log_info ("Daemon killed.");

	sqlite3_close (sqlite);
	smtpd_close (&smtpd);

	free (config.mail_queue_path);
	free (config.sqlite_db_path);

	return EXIT_SUCCESS;;
}

