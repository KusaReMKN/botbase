CC=	cc
CFLAGS=	-O -Wall
RM=	rm -f

all: pingbot

pingbot: pingbot.c
	$(CC) $(CFLAGS) -o pingbot pingbot.c

clean:
	$(RM) pingbot

distclean: clean
