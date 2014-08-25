#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "smtp_envelope.h"

void
smtp_env_init (struct smtp_env *envelope)
{
	memset (envelope, 0, sizeof (struct smtp_env));
}

struct smtp_env_addr*
smtp_env_add_recipient (struct smtp_env *envelope, const char *address)
{
	struct smtp_env_addr *to_new;

	to_new = (struct smtp_env_addr*) malloc (sizeof (struct smtp_env_addr));

	if ( to_new == NULL )
		return NULL;
	
	memset (to_new, 0, sizeof (struct smtp_env_addr));

	strncpy (to_new->addr, address, sizeof (to_new->addr));
	to_new->addr[SMTP_ADDR_MAX - 1] = '\0';

	if ( envelope->to_tail == NULL ){
		envelope->to_head = to_new;
		envelope->to_tail = to_new;
	} else {
		envelope->to_tail->next = to_new;
		envelope->to_tail = to_new;
	}

	return envelope->to_tail;
}

struct smtp_env_addr*
smtp_env_add_sender (struct smtp_env *envelope, const char *address)
{
	struct smtp_env_addr *from_new;

	from_new = (struct smtp_env_addr*) malloc (sizeof (struct smtp_env_addr));

	if ( from_new == NULL )
		return NULL;
	
	memset (from_new, 0, sizeof (struct smtp_env_addr));

	strncpy (from_new->addr, address, sizeof (from_new->addr));
	from_new->addr[SMTP_ADDR_MAX - 1] = '\0';

	if ( envelope->from_tail == NULL ){
		envelope->from_head = from_new;
		envelope->from_tail = from_new;
	} else {
		envelope->from_tail->next = from_new;
		envelope->from_tail = from_new;
	}

	return envelope->from_tail;
}

void
smtp_env_free (struct smtp_env *envelope)
{
	struct smtp_env_addr *env_addr, *env_addr_next;

	for ( env_addr = envelope->to_head; env_addr != NULL; ){
		env_addr_next = env_addr->next;
		free (env_addr);
		env_addr = env_addr_next;
	}

	for ( env_addr = envelope->from_head; env_addr != NULL; ){
		env_addr_next = env_addr->next;
		free (env_addr);
		env_addr = env_addr_next;
	}

	fclose (envelope->file_data);
}

