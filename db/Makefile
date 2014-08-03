#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

OUTPUT=pgpsenderorg.db

all: alias.sql user.sql email.sql
	sqlite3 $(OUTPUT) < alias.sql
	sqlite3 $(OUTPUT) < user.sql
	sqlite3 $(OUTPUT) < email.sql

install:


clean:
	rm -f $(OUTPUT)

