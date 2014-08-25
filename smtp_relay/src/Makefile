#
# Copyright (c) 2014, PGPSender.org
# 

include ../../Makefile.conf

CC = gcc
CFLAGS = -std=c99 -pedantic -O2 -ggdb -Wall -D_GNU_SOURCE

NAME = smtp_relay

all: $(NAME)

install: $(NAME)
	install -m 755 $(NAME) $(INSTALL_BIN_PATH)

uninstall:
	rm -f $(INSTALL_BIN_PATH)/$(NAME)

$(NAME): main.o smtpd.o parser.o smtp_parser.o smtp_envelope.o
	$(CC) $(CFLAGS) $^ -o $(NAME)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

smtpd.o: smtpd.c smtpd.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c $<

smtp_parser.o: smtp_parser.c smtp_parser.h
	$(CC) $(CFLAGS) -c $<

smtp_envelope.o: smtp_envelope.c smtp_envelope.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(NAME) *.o

