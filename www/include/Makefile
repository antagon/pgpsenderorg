#
# Copyright (c) 2014, PGPSender.org
# 

include ../../Makefile.conf

all: Config.class.php

Config.class.php: Config.class.php.in
	sed -e 's|@VERSION@|$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)|g' \
		-e 's|@SQLITE3_DB_FILE@|$(INSTALL_DATA_PATH)/$(INSTALL_DB_FILE)|g' \
		-e 's|@MAIL_QUEUE_PATH@|$(MAIL_QUEUE_PATH)|g' \
			$^ > Config.class.php

clean:
	rm -f Config.class.php

