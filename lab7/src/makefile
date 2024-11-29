
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -O2

TARGETS=out-server-tcp out-client-tcp \
	out-server-udp out-client-udp

.PHONY: all clean

all: $(TARGETS)

out-server-tcp: tcpserver.c
	env $(CC) $^ $(CFLAGS) -o $@

out-client-tcp: tcpclient.c
	env $(CC) $^ $(CFLAGS) -o $@

out-server-udp: udpserver.c
	env $(CC) $^ $(CFLAGS) -o $@

out-client-udp: udpclient.c
	env $(CC) $^ $(CFLAGS) -o $@

clean:
	env rm -f $(TARGETS)
