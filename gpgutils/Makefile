#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

OBJECTS = gpgimport.sh gpggetkey.sh gpgencrypt.sh gpgdeletepubkey.sh

GPG_KEYRING = pubkeys.dat
GPG_SEC_KEYRING = seckeys.dat

all: $(OBJECTS)

install: $(OBJECTS)
	install -m 755 gpgimport.sh $(INSTALL_BIN_PATH)
	install -m 755 gpggetkey.sh $(INSTALL_BIN_PATH)
	install -m 755 gpgencrypt.sh $(INSTALL_BIN_PATH)
	install -m 755 gpgdeletepubkey.sh $(INSTALL_BIN_PATH)

uninstall:
	rm -f $(INSTALL_BIN_PATH)/gpgimport.sh
	rm -f $(INSTALL_BIN_PATH)/gpggetkey.sh
	rm -f $(INSTALL_BIN_PATH)/gpgencrypt.sh
	rm -f $(INSTALL_BIN_PATH)/gpgdeletepubkey.sh


gpgimport.sh: gpgimport.sh.in
	sed -e 's|@GPG_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_KEYRING)|g'\
		-e 's|@GPG_SEC_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_SEC_KEYRING)|g'\
		$^ > gpgimport.sh

gpggetkey.sh: gpggetkey.sh.in
	sed -e 's|@GPG_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_KEYRING)|g'\
		-e 's|@GPG_SEC_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_SEC_KEYRING)|g'\
		$^ > gpggetkey.sh

gpgencrypt.sh: gpgencrypt.sh.in
	sed -e 's|@GPG_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_KEYRING)|g'\
		-e 's|@GPG_SEC_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_SEC_KEYRING)|g'\
		$^ > gpgencrypt.sh

gpgdeletepubkey.sh: gpgdeletepubkey.sh.in
	sed -e 's|@GPG_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_KEYRING)|g'\
		-e 's|@GPG_SEC_KEYRING@|$(INSTALL_DATA_PATH)/$(GPG_SEC_KEYRING)|g'\
		$^ > gpgdeletepubkey.sh

clean:
	rm -f *.sh

