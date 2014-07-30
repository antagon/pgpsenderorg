#include <string.h>
#include <strings.h>

#include "smtp.h"

#define CMD_MATCHES(str, cmd) (strcasecmp ((str), (cmd)) == 0)

int
smtp_parse_request (const char *str, size_t str_len, struct smtp_request *req)
{
	const char *cmd_pos;
	char cmd_name[32];
	size_t cmd_len;
	int cmd_type;

	cmd_pos = (const char*) memchr (str, ' ', str_len);

	if ( cmd_pos == NULL )
		cmd_pos = str + str_len;

	cmd_len = cmd_pos - str;

	if ( cmd_len > sizeof (cmd_name) )
		cmd_len = sizeof (cmd_name);

	memcpy (cmd_name, str, cmd_len);
	cmd_name[sizeof (cmd_name) - 1] = '\0';

	if ( CMD_MATCHES (cmd_name, "HELO") )
		cmd_type = SMTP_C_HELO;
	else if ( CMD_MATCHES (cmd_name, "EHLO") )
		cmd_type = SMTP_C_EHLO;
	else if ( CMD_MATCHES (cmd_name, "MAIL") )
		cmd_type = SMTP_C_MAILFROM;
	else if ( CMD_MATCHES (cmd_name, "RCPT") )
		cmd_type = SMTP_C_RCPTTO;
	else if ( CMD_MATCHES (cmd_name, "DATA") )
		cmd_type = SMTP_C_DATA;
	else if ( CMD_MATCHES (cmd_name, "RSET") )
		cmd_type = SMTP_C_RSET;
	else if ( CMD_MATCHES (cmd_name, "VRFY") )
		cmd_type = SMTP_C_VRFY;
	else if ( CMD_MATCHES (cmd_name, "NOOP") )
		cmd_type = SMTP_C_NOOP;
	else if ( CMD_MATCHES (cmd_name, "QUIT") )
		cmd_type = SMTP_C_QUIT;
	else
		return SMTP_E_UNKNWN;

	memset (req, 0, sizeof (struct smtp_request));

	req->type = cmd_type;

	return SMTP_E_OK;
}

