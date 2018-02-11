#pragma once

#include <cstdint>
#include <array>

#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QHostAddress>

#include "dhcp_option.h"
#include "mac_address.h"
#include "bootp.h"


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

//static inline QString toString( OpType op )
//{
//  if( op == OpType::BOOT_REQUEST ) return "BOOT_REQUEST";
//  else if( op == OpType::BOOT_REPLY ) return "BOOT_REPLY";
//  else return QString("UNKNOWN %1").arg(static_cast<int>(op));
//}

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

class dhcp_message_t
{
  public:
    bootp_t header;

    mac_address_t m_client_id;
    bool m_is_broadcast;

    DhcpRequestType request_type;

    QHostAddress m_address_client{ "0.0.0.0" };
    QHostAddress m_address_yours{ "0.0.0.0" };
    QHostAddress m_address_nextServer{ "0.0.0.0" };
    QHostAddress m_address_relay{ "0.0.0.0" };

    QMap<DhcpOption, QByteArray> m_options;

    QString m_client_hostname;

    void parseOptions( QByteArray data );

  public:

    void SetRequestType( DhcpRequestType type );
    void SetClientMAC( mac_address_t client_mac );
    void SetOption( DhcpOption option, QByteArray options_data );
    void SetRouter( QHostAddress address );
    void SetServerIdentifier( QHostAddress address );
    void SetLeaseTime( int seconds );
    void SetDns( QHostAddress address );
    void SetSubnet( QHostAddress address );

    DhcpRequestType GetRequestType() const { return request_type; }
    
    dhcp_message_t() { }
    dhcp_message_t( QByteArray message_data );

    QString toString() const;
    QByteArray serialize() const;

    static dhcp_message_t CreateOffer( QHostAddress offered_address );
    static dhcp_message_t CreateACK( uint32_t transaction_id, QHostAddress client_addresss, mac_address_t client_mac );
    static dhcp_message_t CreateNAK();
};

