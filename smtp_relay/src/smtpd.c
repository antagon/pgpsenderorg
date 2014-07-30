#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include "smtpd.h"

int
smtpd_open (smtpd_t *resource)
{
	int opt;

	memset (resource, 0, sizeof (smtpd_t));

	resource->pid = getpid ();

	resource->sock = socket (AF_INET, SOCK_STREAM, 0);

	if ( resource->sock == -1 )
		return errno;

	if ( setsockopt (resource->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1 )
		return errno;

	return 0;
}

int
smtpd_close (smtpd_t *resource)
{
	return (close (resource->sock) == -1)? errno:0;
}

int
smtpd_listen (smtpd_t *resource, struct smtpd_config *config)
{
	struct sockaddr_in sock_addr;
	int rval;

	memset (&sock_addr, 0, sizeof (struct sockaddr_in));

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons (config->port);

	if ( inet_pton (AF_INET, config->address, &(sock_addr.sin_addr)) == -1 )
		return errno;

	rval = bind (resource->sock, (struct sockaddr*) &(sock_addr), sizeof (struct sockaddr_in));

	if ( rval == -1 )
		return errno;

	rval = listen (resource->sock, config->port);

	if ( rval == -1 )
		return errno;

	return 0;
}

int
smtpd_accept (smtpd_t *resource, struct smtpd_client *client)
{
	memset (client, 0, sizeof (struct smtpd_client));

	client->addr_len = sizeof (struct sockaddr_in);

	client->sock = accept (resource->sock, (struct sockaddr*) &(client->addr), &(client->addr_len));

	if ( client->sock == -1 && errno )
		return errno;

	if ( inet_ntop (AF_INET, &(client->addr.sin_addr), client->address, sizeof (client->address)) == NULL )
		return errno;

	client->port = ntohs (client->addr.sin_port);

	return 0;
}

int
smtpd_client_close (struct smtpd_client *client)
{
	close (client->sock);
	return 0;
}

