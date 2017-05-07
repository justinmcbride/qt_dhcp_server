#ifndef DHCP_MESSAGE_H
#define DHCP_MESSAGE_H

#include <cstdint>

struct dhcp_message_t {
  uint8_t op; uint8_t htype; uint8_t hlen; uint8_t hops;
  uint32_t xid;
  uint16_t secs;  uint16_t flags;
  uint32_t ciaddr;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t giaddr;
  uint32_t chaddr[4];
  uint8_t sname[64];
  uint8_t file[128];
};

#endif // DHCP_MESSAGE_H
