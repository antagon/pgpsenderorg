#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

all:
	$(MAKE) -C include/

install:
	rsync -r --exclude "*.in" --exclude "Makefile" ./* $(INSTALL_WWW_PATH)

clean:
	$(MAKE) -C include/ clean

