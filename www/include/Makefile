#
# Copyright (c) 2014, PGPSender.org
# 

include ../../Makefile.conf

all: Config.class.php

Config.class.php: Config.class.php.in
	sed 's|@SQLITE3_DB_FILE@|$(INSTALL_DB_PATH)/$(SQLITE3_DB_FILE)|g' $^ > Config.class.php

clean:
	rm -f Config.class.php

