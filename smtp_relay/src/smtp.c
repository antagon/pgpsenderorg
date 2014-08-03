#include <string.h>
#include <strings.h>
#include <regex.h>

#include "smtp.h"

#define CMD_MATCHES(str, cmd) (strcasecmp ((str), (cmd)) == 0)

/*static int
smtp_is_email_addr (const char *str, size_t str_len)
{
	regex_t preg;
	char email_addr[320];
	int rval, cpy_max;

	cpy_max = str_len;

	if ( cpy_max > sizeof (email_addr) )
		cpy_max = sizeof (email_addr);

	strncpy (email_addr, str, cpy_max);
	email_addr[cpy_max - 1] = '\0';

	// Source: https://fightingforalostcause.net/content/misc/2006/compare-email-regex.php
	rval = regcomp (&preg, "/^(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){255,})(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){65,}@)(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22))(?:\.(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z0-9]+(?:-[a-z0-9]+)*\.){1,126}){1,}(?:(?:[a-z][a-z0-9]*)|(?:(?:xn--)[a-z0-9]+))(?:-[a-z0-9]+)*)|(?:\[(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9][:\]]){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))\]))$/iD", REG_EXTENDED);

	if ( rval != 0 )
		return rval;

	rval = regexec (&preg, email_addr, 0, NULL, 0);

	return rval;
}*/

int
smtp_parse_request (const char *str, size_t str_len, struct smtp_request *req)
{
	const char *eol_pos, *cmd_pos;
	char cmd_name[32];
	size_t cmd_len;
	int cmd_type;

	eol_pos = (const char*) memmem (str, str_len, "\r\n", 2);

	cmd_pos = (const char*) memchr (str, ' ', str_len);

	if ( cmd_pos == NULL ){
		if ( eol_pos == NULL )
			cmd_pos = (const char*) (str + str_len);
		else
			cmd_pos = (const char*) (eol_pos - str);
	}

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

	return (eol_pos == NULL)? SMTP_E_CONTINUE:SMTP_E_OK;
}

