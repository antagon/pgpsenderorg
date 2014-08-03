all:
	$(MAKE) -C db/
	$(MAKE) -C smtp_relay/

clean:
	$(MAKE) -C db/ clean
	$(MAKE) -C smtp_relay/ clean

