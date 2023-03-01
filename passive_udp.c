#include "passive_udp.h"
#include <sys/types.h>

static struct servent   *pse; // pointer to host information entry
static  struct protoent *ppe; // pointer to protocol information entry
static  struct sockaddr_in sin; // an Internet endpoint address

int passivesock(const char *service, const char *transport, int qlen);

int passiveUDP(const char *service) { return passivesock(service, "udp", 0); }

u_short portbase = 0;

bool map_service_port(const char *service, const char *transport) {
  if ((pse = getservbyname(service, transport))) { 
    sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);
    return true;
  }
  else if ((sin.sin_port = htons((u_short)atoi(service)) != 0))
    return true;
  
  return  false;
}

bool map_protname_protnum(const char *transport) {
  if ((ppe = getprotobyname(transport)) != 0)
    return true;

  return false;
}

bool map_prot_socktype(const char *transport, int *socktype) {
  if (strcmp(transport, "udp") == 0) {
    *socktype = SOCK_DGRAM;
    return true;
  }
  else {
    *socktype = SOCK_STREAM;
    return true;
  }
  return false;
}

int passivesock(const char *service, const char *transport, int qlen) {

  int _socket, socket_type;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;

  if (!map_service_port(service, transport)) {
    printf("error: can not get \"%s\" service entry\n", service);
    exit(1);
  }

  if (!map_protname_protnum(transport)) {
    printf("error: can not get \"%s\" protocol entry\n", transport);
    exit(1);
  }

  map_prot_socktype(transport, &socket_type);

  // allocate a socket
  _socket = socket(PF_INET, socket_type, ppe->p_proto);
  if (_socket < 0) {
   printf("error: can not create socket: %s\n", strerror(errno));
   exit(1);
  }

  // bind the socket
  if (bind(_socket, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    printf("error: can not bind to %s port: %s\n", service, strerror(errno));
    exit(1);
  }
  if (socket_type == SOCK_STREAM && listen(_socket, qlen) < 0) {
    printf("error: can not listen %s port: %s\n", service, strerror(errno));
    exit(1);
  }
      
  return _socket;
}
