#pragma once

#include "mac_address.h"

#include <QByteArray>
#include <QDebug>

enum class BootpOpType : quint8 {
  BOOT_REQUEST = 1,
  BOOT_REPLY = 2
};

enum class BootpHwType : quint8 {
  ETHERNET = 1
};

struct bootp_t
{
/*
  struct bootp_header_t {
    uint8_t op; uint8_t htype; uint8_t hlen; uint8_t hops;
    uint32_t xid;
    uint16_t secs;  uint16_t flags;
    uint32_t ciaddr;
    uint32_t yiaddr;
    uint32_t siaddr;  // IP address of next server to use in bootstrap; returned in DHCPOFFER, DHCPACK by server.
    uint32_t giaddr;  // Relay agent IP address, used in booting via a relay agent.
    uint8_t chaddr[16]; // Client hardware address.
    uint8_t sname[64]; // Optional server host name, null terminated string.
    uint8_t file[128]; // Boot file name, null terminated string; "generic" name or null in DHCPDISCOVER, fully qualified directory-path name in DHCPOFFER
    uint8_t cookie[4];
  };
*/
  public:
    bootp_t();

    BootpOpType operation;
    BootpHwType hardware_address_type;
    quint8 hardware_address_length;
    quint8 hops;
    quint32 transaction_id;
    quint16 secs;
    quint16 flags;

    mac_address_t hardware_address_client;

    bool deserialize( QByteArray data );
    QByteArray serialize() const;

    const static QByteArray COOKIE;
};
