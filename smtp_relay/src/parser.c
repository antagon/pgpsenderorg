#include <stddef.h>

#include "parser.h"

int
parser_exec (struct parser *parser, const char *buff, size_t len)
{
	const char *buff_pos, *buff_evt_pos;
	int eol;

	eol = 0;

	for ( buff_pos = buff_evt_pos = buff; buff_pos < buff + len; buff_pos++ ){

		if ( *buff_pos == parser->word_delim ){

			if ( parser->on_word (parser, buff_evt_pos, (const char*) buff_evt_pos - buff) < 1 )
				return buff_pos - buff;

			buff_evt_pos = (const char*) (buff_pos - buff + 1);

		} else if ( *buff_pos == CR ){
			eol += 0x01;
		} else if ( *buff_pos == LF ){
			eol += 0x02;
		}

		if ( ((eol & 0x01) != 0) && ((eol & 0x02) != 0) ){

			if ( parser->on_eol (parser) < 1 )
				return buff_pos - buff;
		}
	}

	return buff_pos - buff;
}

