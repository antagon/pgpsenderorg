#include <stddef.h>
#include <string.h>
#include <strings.h>

#include "smtp_parser.h"

#define CMD_MATCHES(str, cmd, str_len) (strncasecmp (str, cmd, str_len) == 0)

static int
_parser_on_word_cb (struct parser *parser, const char *buff, size_t len)
{
	struct smtp_req *request;

	request = (struct smtp_req*) parser->user_data;

	if ( request->type == 0 ){
		if ( CMD_MATCHES (buff, "HELO", len) )
			request->type = SMTP_C_HELO;
		else if ( CMD_MATCHES (buff, "EHLO", len) )
			request->type = SMTP_C_EHLO;
		else if ( CMD_MATCHES (buff, "MAIL", len) )
			request->type = SMTP_C_MAILFROM;
		else if ( CMD_MATCHES (buff, "RCPT", len) )
			request->type = SMTP_C_RCPTTO;
		else if ( CMD_MATCHES (buff, "DATA", len) )
			request->type = SMTP_C_DATA;
		else if ( CMD_MATCHES (buff, ".", len) )
			request->type = SMTP_C_EOF;
		else if ( CMD_MATCHES (buff, "RSET", len) )
			request->type = SMTP_C_RSET;
		else if ( CMD_MATCHES (buff, "VRFY", len) )
			request->type = SMTP_C_VRFY;
		else if ( CMD_MATCHES (buff, "NOOP", len) )
			request->type = SMTP_C_NOOP;
		else if ( CMD_MATCHES (buff, "QUIT", len) )
			request->type = SMTP_C_QUIT;

		return 1;
	}

	switch ( request->type ){
		case SMTP_C_HELO:
		case SMTP_C_EHLO:
			request->argv[request->argc].val = buff;
			request->argv[request->argc].len = len;
			request->argc++;
			break;
		case SMTP_C_MAILFROM:
			request->argv[request->argc].val = buff;
			request->argv[request->argc].len = len;
			request->argc++;
			break;
		case SMTP_C_RCPTTO:
			request->argv[request->argc].val = buff;
			request->argv[request->argc].len = len;
			request->argc++;
			break;
		case SMTP_C_VRFY:
			request->argv[request->argc].val = buff;
			request->argv[request->argc].len = len;
			request->argc++;
			break;
		case SMTP_C_DATA:
		case SMTP_C_RSET:
		case SMTP_C_NOOP:
		case SMTP_C_QUIT:
			// no additional parameters are expected for these commands
			break;
	}

	return 1;
}

static int
_parser_on_eol_cb (struct parser *parser)
{
	struct smtp_req *request;

	request = (struct smtp_req*) parser->user_data;

	request->complete = 1;

	return 0;
}

void
smtp_parser_init (struct smtp_parser *smtp_parser)
{
	smtp_parser->parser.word_delim = ' ';
	smtp_parser->parser.on_word = _parser_on_word_cb;
	smtp_parser->parser.on_eol = _parser_on_eol_cb;
}

size_t
smtp_parser_exec (struct smtp_parser *smtp_parser, const char *buff, size_t len)
{
	size_t parsed_len;
	int rval;

	memset (&(smtp_parser->request), 0, sizeof (struct smtp_req));
	smtp_parser->parser.user_data = &(smtp_parser->request);

	parsed_len = parser_exec (&(smtp_parser->parser), buff, len);

	if ( smtp_parser->request.complete ){
		switch ( smtp_parser->request.type ){
			case SMTP_C_HELO:
				rval = smtp_parser->on_helo (smtp_parser->request.argv, smtp_parser->request.argc,
												smtp_parser->user_data);
				break;
			case SMTP_C_EHLO:
				rval = smtp_parser->on_ehlo (smtp_parser->request.argv, smtp_parser->request.argc,
												smtp_parser->user_data);
				break;
			case SMTP_C_MAILFROM:
				rval = smtp_parser->on_mail (smtp_parser->request.argv, smtp_parser->request.argc,
												smtp_parser->user_data);
				break;
			case SMTP_C_RCPTTO:
				rval = smtp_parser->on_rcpt (smtp_parser->request.argv, smtp_parser->request.argc,
												smtp_parser->user_data);
				break;
			case SMTP_C_VRFY:
				rval = smtp_parser->on_vrfy (smtp_parser->request.argv, smtp_parser->request.argc,
												smtp_parser->user_data);
				break;
			case SMTP_C_DATA:
				rval = smtp_parser->on_data (smtp_parser->user_data);
				break;
			case SMTP_C_EOF:
				rval = smtp_parser->on_eof (smtp_parser->user_data);
				break;
			case SMTP_C_RSET:
				rval = smtp_parser->on_rset (smtp_parser->user_data);
				break;
			case SMTP_C_NOOP:
				rval = smtp_parser->on_noop (smtp_parser->user_data);
				break;
			case SMTP_C_QUIT:
				rval = smtp_parser->on_quit (smtp_parser->user_data);
				break;
		}
	}
	
	return parsed_len;
}

