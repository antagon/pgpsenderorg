#
# Copyright (c) 2014, PGPSender.org
# 

all:
	$(MAKE) -C db/
	$(MAKE) -C gpgutils/
	$(MAKE) -C smtp_relay/
	$(MAKE) -C www/

install:
	$(MAKE) -C db/ install
	$(MAKE) -C gpgutils/ install
	$(MAKE) -C smtp_relay/ install
	$(MAKE) -C www/ install

clean:
	$(MAKE) -C db/ clean
	$(MAKE) -C gpgutils/ clean
	$(MAKE) -C smtp_relay/ clean
	$(MAKE) -C www/ clean

