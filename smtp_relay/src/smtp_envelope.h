#ifndef _SMTP_ENVELOPE_H
#define _SMTP_ENVELOPE_H

#define SMTP_ADDR_MAX 256

struct smtp_env_addr
{
	char addr[SMTP_ADDR_MAX];
	struct smtp_env_addr *next;
};

struct smtp_env
{
	struct smtp_env_addr *to_head;
	struct smtp_env_addr *to_tail;
	struct smtp_env_addr *from_head;
	struct smtp_env_addr *from_tail;
	char hostname[SMTP_ADDR_MAX];
	int codeno;
	FILE *file_data;
};

extern void smtp_env_init (struct smtp_env *envelope);

extern struct smtp_env_addr* smtp_env_add_recipient (struct smtp_env *envelope, const char *address);

extern struct smtp_env_addr* smtp_env_add_sender (struct smtp_env *envelope, const char *address);

extern void smtp_env_free (struct smtp_env *envelope);

#endif

