#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

NAME=$(SQLITE3_DB_FILE)

all: $(NAME)

$(NAME): alias.sql user.sql email.sql
	sqlite3 $(NAME) < alias.sql
	sqlite3 $(NAME) < user.sql
	sqlite3 $(NAME) < email.sql

install: $(NAME)
	install -m 664 $(NAME) $(INSTALL_DB_PATH)

clean:
	rm -f $(NAME)

