#ifndef PASSIVE_UDP
#define PASSIVE_UDP

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>

int passiveUDP(const char *service);

#endif
