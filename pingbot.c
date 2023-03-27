#include <sys/socket.h>
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define LINELEN	2048
#define NICK	"PingBot"

static int	tcpConnect(const char *host, const char *serv);
static void	usage(void);

int
main(int argc, char *argv[])
{
	int sock;
	FILE *fp;
	char buf[LINELEN], command[LINELEN], dest[LINELEN], user[LINELEN];

	if (argc != 2 && argc != 3)
		usage();

	/* If it returns, it has succeeded */
	sock = tcpConnect(argv[1], argc == 3 ? argv[2] : "6667");

	fp = fdopen(sock, "r+");
	if (fp == NULL)
		err(EXIT_FAILURE, "fdopen");
	setbuf(fp, NULL);

	/* Registration */
	if (fprintf(fp, "NICK %s\r\n", NICK) < 0)
		err(EXIT_FAILURE, "fprintf(NICK)");
	(void)fprintf(stderr, "< NICK %s\r\n", NICK);
	if (fprintf(fp, "USER %s %s %s %s\r\n", NICK, NICK, NICK, NICK) < 0)
		err(EXIT_FAILURE, "fprintf(USER)");
	(void)fprintf(stderr, "< USER %s %s %s %s\r\n", NICK, NICK, NICK, NICK);

	/* Skip to end of MOTD */
	do {
		if (fgets(buf, sizeof(buf), fp) == NULL)
			err(EXIT_FAILURE, "fgets");
		(void)fprintf(stderr, "> %s", buf);

		if (sscanf(buf, "%s%s%s", user, command, dest) != 3)
			err(EXIT_FAILURE, "sscanf:\nbuf: %s\n", buf);
	} while (strcmp(command, "376") != 0);

	while (/* CONSTCOND */ 1) {
		if (fgets(buf, sizeof(buf), fp) == NULL)
			err(EXIT_FAILURE, "fgets");
		(void)fprintf(stderr, "> %s", buf);

		if (strstr(buf, "PING ") == buf) {
			if (sscanf(buf+5, ":%s", dest) != 1)
				err(EXIT_FAILURE, "sscanf:\nbuf: %s\n", buf);
			if (fprintf(fp, "PONG %s\r\n", dest) < 0)
				err(EXIT_FAILURE, "fprintf(PONG)");
			(void)fprintf(stderr, "< PONG %s\r\n", dest);
		}
	}

	/* NOTREACHED */
	fclose(fp);

	return EXIT_FAILURE;
}

static int
tcpConnect(const char *host, const char *serv)
{
	int error, sock;
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((error = getaddrinfo(host, serv, &hints, &result)) != 0)
		errx(EXIT_FAILURE, "getaddrinfo: %s", gai_strerror(error));
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sock == -1)
			continue;
		if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0)
			break;	/* success */
		(void)close(sock);
	}
	freeaddrinfo(result);
	if (rp == NULL)
		err(EXIT_FAILURE, "tcpConnect");

	return sock;
}

static void
usage(void)
{
	fprintf(stderr, "usage: pingbot host [port]\n");
	exit(EXIT_FAILURE);
}
