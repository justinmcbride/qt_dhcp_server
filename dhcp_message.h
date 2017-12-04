#pragma once

#include <cstdint>
#include <array>

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QHostAddress>

#include "dhcp_option.h"

struct dhcp_header_t {
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
auto constexpr MIN_DHCP_SIZE = sizeof( dhcp_header_t );

enum class OpType : uint8_t {
  BOOT_REQUEST = 1,
  BOOT_REPLY = 2
};

enum class DhcpRequestType : uint8_t {
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

typedef std::array< uint8_t, 6 > mac_address_t;
static inline QString toString( mac_address_t address )
{
  QString s;
  for( int i = 0; i < address.size(); i++ )
  {
    s += QString::number( address[i], 16 );
    if( i < ( address.size() - 1) ) s += ":";
  }
  return s;
}


class dhcp_message_t
{
  private:
    OpType m_operation;
    int m_hardware_type{ 1 };
    int m_hardware_length{ 6 };
    int m_hops{ 0 };

    uint32_t m_transaction_id;

    int m_seconds{ 0 };
    bool m_is_broadcast;

    DhcpRequestType request_type;

    QHostAddress m_address_client{ "0.0.0.0" };
    QHostAddress m_address_yours{ "0.0.0.0" };
    QHostAddress m_address_nextServer{ "0.0.0.0" };
    QHostAddress m_address_relay{ "0.0.0.0" };

    QMap<DhcpOption, QByteArray> m_options;

    mac_address_t m_hardware_address_client;
    QString m_client_hostname;

    void parseOptions( QByteArray data );

  public:

    void SetRequestType( DhcpRequestType type );
    void SetOperationType( OpType type ) { m_operation = type; }
    void SetHardwareType( int type ) { m_hardware_type = type; }
    void SetHops( int hops ) { m_hops = hops; }
    void SetTransactionId( uint32_t transaction_id ) { m_transaction_id = transaction_id; }
    void SetClientAddress( QHostAddress client_addresss ) { m_address_client = client_addresss; }
    void SetClientMAC( mac_address_t client_mac ) { m_hardware_address_client = client_mac; }
    void SetOption( DhcpOption option, QByteArray options_data ) { m_options[option] = options_data; }
    void SetRouter( QHostAddress router_address );

    DhcpRequestType GetRequestType() const { return request_type; }
    
    dhcp_message_t() { }
    dhcp_message_t( QByteArray message_data );

    QString toString() const;
    QByteArray serialize() const;
};

static dhcp_message_t CreateDiscover()
{
  dhcp_message_t dhcp;
  return dhcp;
}
static dhcp_message_t CreateOffer()
{
  dhcp_message_t dhcp;
  dhcp.SetOperationType( OpType::BOOT_REPLY );
  dhcp.SetRequestType( DhcpRequestType::DHCPOFFER );
  dhcp.SetRouter( QHostAddress("192.168.1.1") );

  return dhcp;
}

static dhcp_message_t CreateRequest()
{
  dhcp_message_t dhcp;
  return dhcp;
}
static dhcp_message_t CreateDecline()
{
  dhcp_message_t dhcp;
  return dhcp;
}
static dhcp_message_t CreateACK( uint32_t transaction_id, QHostAddress client_addresss, mac_address_t client_mac )
{
  dhcp_message_t dhcp;
  dhcp.SetRequestType( DhcpRequestType::DHCPACK );
  dhcp.SetOperationType( OpType::BOOT_REPLY );
  dhcp.SetHardwareType( 1 );
  dhcp.SetTransactionId( transaction_id );
  dhcp.SetClientAddress( client_addresss );
  dhcp.SetClientMAC( client_mac );

  return dhcp;
}

static dhcp_message_t CreateNAK()
{
  dhcp_message_t dhcp;
  return dhcp;
}
static dhcp_message_t CreateRelease()
{
  dhcp_message_t dhcp;
  return dhcp;
}
static dhcp_message_t CreateInform()
{
  dhcp_message_t dhcp;
  return dhcp;
}
