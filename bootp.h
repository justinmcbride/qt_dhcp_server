#pragma once

#include <QByteArray>
#include <QDebug>

#include "mac_address.h"

enum class BootpOpType : quint8 {
  BOOT_REQUEST = 1,
  BOOT_REPLY = 2
};

static inline QString toString( BootpOpType op )
{
  if( op == BootpOpType::BOOT_REQUEST )    return "BOOT_REQUEST";
  else if( op == BootpOpType::BOOT_REPLY ) return "BOOT_REPLY";
  else return QString("UNKNOWN %1").arg(static_cast<int>(op));
}


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
    quint8 hardware_address_length{ 0 };
    quint8 hops{ 0 };
    quint32 transaction_id{ 0 };
    quint16 secs{ 0 };
    quint16 flags{ 0 };

    quint32 ciaddr{ 0 };
    quint32 yiaddr{ 0 };
    quint32 siaddr{ 0 };
    quint32 giaddr{ 0 };

    mac_address_t hardware_address_client;

    bool deserialize( QByteArray data );
    QByteArray serialize() const;

    const static QByteArray COOKIE;
};
