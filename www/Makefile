#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

all: include/config.php

install: include/config.php
	rsync -r --exclude "*.in" --exclude "Makefile" ./* $(INSTALL_WWW_PATH)

include/config.php: include/config.php.in
	sed 's|@SQLITE3_DB_FILE@|$(INSTALL_DB_PATH)/$(SQLITE3_DB_FILE)|g' include/config.php.in > include/config.php

clean:
	rm -f include/config.php

