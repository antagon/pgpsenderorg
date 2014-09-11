#ifndef _SMTP_PARSER_H
#define _SMTP_PARSER_H

#include <stddef.h>

#include "parser.h"

enum
{
	SMTP_C_HELO = 0x01,
	SMTP_C_EHLO = 0x02,
	SMTP_C_MAILFROM = 0x04,
	SMTP_C_RCPTTO = 0x08,
	SMTP_C_DATA = 0x10,
	SMTP_C_EOF = 0x20,
	SMTP_C_RSET = 0x40,
	SMTP_C_VRFY = 0x80,
	SMTP_C_NOOP = 0x100,
	SMTP_C_QUIT = 0x200
};

enum
{
	SMTP_READY = 220,
	SMTP_BYE = 221,
	SMTP_MAILOK = 250,
	SMTP_STARTMAIL = 354,
	SMTP_ENOTAVAIL = 421,
	SMTP_ELOCAL = 451,
	SMTP_ESTORAGE = 452,
	SMTP_ESYNTAX = 500,
	SMTP_EARGSYNTAX = 501,
	SMTP_ECMDNIMPL = 502,
	SMTP_EBADSEQ = 503,
	SMTP_EARGNIMPL = 504
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

struct smtp_req_arg
{
	const char *val;
	size_t len;
};

#define SMTP_ARGS_MAX 4

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

extern int smtp_strresponse (int code, char *buff, size_t len);

#endif

