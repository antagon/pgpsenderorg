CC = gcc
CFLAGS = -std=c99 -pedantic -ggdb -Wall -D_GNU_SOURCE

NAME = smtpd

all: $(NAME)

clean:
	rm -f $(NAME) *.o

$(NAME): main.o smtpd.o smtp.o
	$(CC) $(CFLAGS) $^ -o $(NAME)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

smtpd.o: smtpd.c smtpd.h
	$(CC) $(CFLAGS) -c $<

smtp.o: smtp.c smtp.h
	$(CC) $(CFLAGS) -c $<

