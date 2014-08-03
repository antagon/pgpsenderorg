#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

all: alias.sql user.sql email.sql
	sqlite3 $(SQLITE3_DB_FILE) < alias.sql
	sqlite3 $(SQLITE3_DB_FILE) < user.sql
	sqlite3 $(SQLITE3_DB_FILE) < email.sql

install:


clean:
	rm -f $(SQLITE3_DB_FILE)

