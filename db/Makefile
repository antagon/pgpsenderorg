#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

all: alias.sql user.sql email.sql
	sqlite3 $(SQLITE3_DB_FILE) < alias.sql
	sqlite3 $(SQLITE3_DB_FILE) < user.sql
	sqlite3 $(SQLITE3_DB_FILE) < email.sql

install:
	install -m 664 $(SQLITE3_DB_FILE) $(INSTALL_DB_PATH)

clean:
	rm -f $(SQLITE3_DB_FILE)

