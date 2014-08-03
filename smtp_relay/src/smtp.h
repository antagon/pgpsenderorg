#ifndef _SMTP_H
#define _SMTP_H

#include <stdio.h>

enum
{
	SMTP_E_OK = 0x00,
	SMTP_E_CONTINUE = 0x01,
	SMTP_E_UNKNWN
};

enum
{
	SMTP_C_HELO = 0x01,
	SMTP_C_EHLO,
	SMTP_C_MAILFROM,
	SMTP_C_RCPTTO,
	SMTP_C_DATA,
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
554	Transaction failed*/

struct smtp_request
{
	int type;
};

struct smtp_response
{
	int type;
	char *message;
};

extern struct smtp_response smtp_responses[];

extern int smtp_parse_request (const char *str, size_t str_len, struct smtp_request *req);

#endif

