#include <time.h>

#include "passive_udp.h"

int main(int argc, char *argv[]) {
  struct sockaddr_in sin;
  char *service = "time";
  char buffer[1];
  int _socket;
  time_t now;
  unsigned int length;
  _socket = passiveUDP(service);
  while (1) {
    length = sizeof(sin);

    if (recvfrom(_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&sin, &length) < 0) {
      printf("error: recvfrom: %s\n", strerror(errno));
      exit(1);
    }

    time(&now);
    now = htonl((u_long)now);
    sendto(_socket, (char *)&now, sizeof(now), 0, (struct sockaddr *)&sin, sizeof(sin));
  }
  return 0;
}
