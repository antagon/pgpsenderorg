#ifndef _SMTPD_H
#define _SMTPD_H

#include <stdint.h>
#include <linux/limits.h>
#include <arpa/inet.h>

struct smtpd_config
{
	char queue_dir[PATH_MAX];
	char *address;
	uint16_t port;
};

struct smtpd_client
{
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_len;
	char address[INET_ADDRSTRLEN];
	uint16_t port;
};

typedef struct
{
	int sock;
	pid_t pid;
	struct smtpd_config *config;
} smtpd_t;

extern int smtpd_open (smtpd_t *resource);

extern int smtpd_close (smtpd_t *resource);

extern int smtpd_listen (smtpd_t *resource, struct smtpd_config *config);

extern int smtpd_accept (smtpd_t *resource, struct smtpd_client *client);

extern int smtpd_client_close (struct smtpd_client *client);

#endif

