#
# Copyright (c) 2014, PGPSender.org
# 

include ../Makefile.conf

OBJECTS = gpgimport.sh gpggetkey.sh gpgencrypt.sh gpgdeletepubkey.sh

all: $(OBJECTS)

install: $(OBJECTS)
	install -m 755 gpgimport.sh $(INSTALL_GPGUTILS_PATH)
	install -m 755 gpggetkey.sh $(INSTALL_GPGUTILS_PATH)
	install -m 755 gpgencrypt.sh $(INSTALL_GPGUTILS_PATH)
	install -m 755 gpgdeletepubkey.sh $(INSTALL_GPGUTILS_PATH)

gpgimport.sh: gpgimport.sh.in
	cp gpgimport.sh.in gpgimport.sh

gpggetkey.sh: gpggetkey.sh.in
	cp gpggetkey.sh.in gpggetkey.sh

gpgencrypt.sh: gpgencrypt.sh.in
	cp gpgencrypt.sh.in gpgencrypt.sh

gpgdeletepubkey.sh: gpgdeletepubkey.sh.in
	cp gpgdeletepubkey.sh.in gpgdeletepubkey.sh

clean:
	rm -f *.sh

