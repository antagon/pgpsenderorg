#ifndef _SMTP_PARSER_H
#define _SMTP_PARSER_H

#include <stddef.h>

#include "parser.h"

enum
{
	SMTP_C_HELO = 1,
	SMTP_C_EHLO,
	SMTP_C_MAILFROM,
	SMTP_C_RCPTTO,
	SMTP_C_DATA,
	SMTP_C_EOF,
	SMTP_C_RSET,
	SMTP_C_VRFY,
	SMTP_C_NOOP,
	SMTP_C_QUIT
};

/*
211	System status, or system help reply
214	Help message
220	<domain> Service ready
221	<domain> Service closing transmission channel
250	Requested mail action okay, completed
251	User not local; will forward to <forward-path>
252	Cannot VRFY user, but will accept message and attempt delivery
354	Start mail input; end with <CRLF>.<CRLF>
421	<domain> Service not available, closing transmission channel
450	Requested mail action not taken: mailbox unavailable
451	Requested action aborted: local error in processing
452	Requested action not taken: insufficient system storage
500	Syntax error, command unrecognised
501	Syntax error in parameters or arguments
502	Command not implemented
503	Bad sequence of commands
504	Command parameter not implemented
521	<domain> does not accept mail (see rfc1846)
530	Access denied (???a Sendmailism)
550	Requested action not taken: mailbox unavailable
551	User not local; please try <forward-path>
552	Requested mail action aborted: exceeded storage allocation
553	Requested action not taken: mailbox name not allowed
554	Transaction failed
*/

#define SMTP_ARGS_MAX 4

struct smtp_req_arg
{
	const char *val;
	size_t len;
};

struct smtp_req
{
	int type;
	int complete;
	size_t argc;
	struct smtp_req_arg argv[SMTP_ARGS_MAX];
};

struct smtp_parser
{
	struct parser parser;
	void *user_data;
	int (*on_helo) (struct smtp_req_arg *argv, size_t argc, void *user_data);
	int (*on_ehlo) (struct smtp_req_arg *argv, size_t argc, void *user_data);
	int (*on_mail) (struct smtp_req_arg *argv, size_t argc, void *user_data);
	int (*on_rcpt) (struct smtp_req_arg *argv, size_t argc, void *user_data);
	int (*on_data) (void *user_data);
	int (*on_eof) (void *user_data);
	int (*on_rset) (void *user_data);
	int (*on_vrfy) (struct smtp_req_arg *argv, size_t argc, void *user_data);
	int (*on_noop) (void *user_data);
	int (*on_quit) (void *user_data);
	struct smtp_req request;
};

extern void smtp_parser_init (struct smtp_parser *smtp_parser);

extern size_t smtp_parser_exec (struct smtp_parser *smtp_parser, const char *buff, size_t len);

#endif

