all:
	$(MAKE) -C db/
	$(MAKE) -C smtp_relay/
	$(MAKE) -C www/

install:
	$(MAKE) -C db/ install
	$(MAKE) -C smtp_relay/ install
	$(MAKE) -C www/ install

clean:
	$(MAKE) -C db/ clean
	$(MAKE) -C smtp_relay/ clean

