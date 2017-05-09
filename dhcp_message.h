#ifndef DHCP_MESSAGE_H
#define DHCP_MESSAGE_H

#include <cstdint>

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QHostAddress>

struct dhcp_header_t {
  uint8_t op; uint8_t htype; uint8_t hlen; uint8_t hops;
  uint32_t xid;
  uint16_t secs;  uint16_t flags;
  uint32_t ciaddr;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t giaddr;
  uint8_t chaddr[16];
  uint8_t sname[64];
  uint8_t file[128];
  uint8_t cookie[4];
};
auto constexpr MIN_DHCP_SIZE = sizeof( dhcp_header_t );

enum class OpType {
  BOOT_REQUEST,
  BOOT_REPLY
};

enum class DhcpRequestType {
  DHCPDISCOVER = 1,
  DHCPOFFER,
  DHCPREQUEST,
  DHCPDECLINE,
  DHCPACK,
  DHCPNAK,
  DHCPRELEASE,
  DHCPINFORM
};

static inline QString toString( OpType op )
{
  if( op == OpType::BOOT_REQUEST ) return "BOOT_REQUEST";
  else if( op == OpType::BOOT_REPLY ) return "BOOT_REPLY";
  else return QString("UNKNOWN %1").arg(static_cast<int>(op));
}

static inline QString toString( DhcpRequestType type )
{
  if( type == DhcpRequestType::DHCPDISCOVER ) return "DHCPDISCOVER";
  else if( type == DhcpRequestType::DHCPOFFER ) return "DHCPOFFER";
  else if( type == DhcpRequestType::DHCPREQUEST ) return "DHCPREQUEST";
  else if( type == DhcpRequestType::DHCPDECLINE ) return "DHCPDECLINE";
  else if( type == DhcpRequestType::DHCPACK ) return "DHCPACK";
  else if( type == DhcpRequestType::DHCPNAK ) return "DHCPNAK";
  else if( type == DhcpRequestType::DHCPRELEASE ) return "DHCPRELEASE";
  else if( type == DhcpRequestType::DHCPINFORM ) return "DHCPINFORM";
  else return QString("UNKNOWN %1").arg(static_cast<int>(type));
}

struct dhcp_message_t {
  OpType operation;
  int hardware_type;
  int hardware_length;
  int hops;

  int transaction_id;

  int seconds;
  bool broadcast;

  DhcpRequestType request_type;

  QHostAddress address_client;
  QHostAddress address_yours;
  QHostAddress address_nextServer;
  QHostAddress address_relay;

  QByteArray options;

  QString hardware_address_client;
  QString client_hostname;

  dhcp_message_t( QByteArray message_data );

  void parseOptions( QByteArray data );

  QString toString() const;
};

#endif // DHCP_MESSAGE_H
