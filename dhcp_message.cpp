#include "dhcp_message.h"
#include "bootp.h"

#include <QString>

dhcp_message_t::dhcp_message_t( QByteArray message_data )
{
  auto preamble = message_data.left( 240 );
  message_data.remove( 0, 240 );

  bool good = header.deserialize( preamble );
  if( !good )
  {
    qDebug() << "couldn't serialize header";
    throw std::invalid_argument( "something" );
  }

//  m_operation = static_cast<OpType>( hdr->op );
//  m_hardware_type = hdr->htype;
//  m_hardware_length = hdr->hlen;
//  m_hops = hdr->hops;
//  m_transaction_id = hdr->xid;
//  m_seconds = hdr->secs;
//  m_is_broadcast = ( hdr->flags & 0x80 );

//  m_address_client = QHostAddress( hdr->ciaddr );
//  m_address_yours = QHostAddress( hdr->yiaddr );
//  m_address_nextServer = QHostAddress( hdr->siaddr );
//  m_address_relay = QHostAddress( hdr->giaddr );

  parseOptions( message_data );
}

void dhcp_message_t::parseOptions( QByteArray data )
{
  qDebug() << "***********************************";
  qDebug() << "[PARSING OPTIONS]";
  while( data.size() )
  {
    auto option_type = getOptionType( data.at( 0 ) );
    qDebug() << "--->[" << DhcpOptionToString( data.at( 0 ) ) << "]";
    data.remove( 0, 1 );
    if( option_type == DhcpOption::PAD )
    {
      continue;
    }
    else if( option_type == DhcpOption::END )
    {
      qDebug() << "padding=" << data.size();
      data.clear();
    }
    else if( option_type == DhcpOption::PARAMETER_REQUEST_LIST )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );

      qDebug() << "requesting " << option_length << " items";
      for( int i = 0; i < option_length; i++ )
      {
        uint8_t requested_parameter = data.at( 0 );
        data.remove( 0, 1 );
        qDebug() << "requested: " << DhcpOptionToString(requested_parameter);
      }
    }
    else if( option_type == DhcpOption::REQUESTED_IP )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );
      if( option_length != 4 )
      {
        qDebug() << "some problem with length: " << option_length;
      }
      uint8_t byte_1 = data.at( 0 );
      uint8_t byte_2 = data.at( 1 );
      uint8_t byte_3 = data.at( 2 );
      uint8_t byte_4 = data.at( 3 );

      auto ip =
        QString( "%0.%1.%2.%3" )
          .arg( byte_1 )
          .arg( byte_2 )
          .arg( byte_3 )
          .arg( byte_4 )
      ;
      QHostAddress address( ip );
      qDebug() << "requesting address " << address.toString();
      data.remove( 0, 4 );
    }
    else if( option_type == DhcpOption::DHCP_MESSAGE_TYPE )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );
      if( option_length != 1 )
      {
        qDebug() << "invalid length for opt";
      }
      uint8_t type = data.at( 0 );
      request_type = static_cast<DhcpRequestType>( type );
      qDebug() << "type: " << ::toString( request_type );
      data.remove( 0, option_length );
    }
    else if( option_type == DhcpOption::HOSTNAME )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );

      m_client_hostname = data.left( option_length );
      qDebug() << "hostname " << m_client_hostname;
      data.remove( 0, option_length );
    }
    else if( option_type == DhcpOption::CLIENT_ID )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );

      uint8_t type = data.at( 0 );
      data.remove( 0, 1 );
      option_length--;

      QByteArray client_id = data.left( option_length );
      data.remove( 0, option_length );
      if( static_cast<BootpHwType>(type) == BootpHwType::ETHERNET )
      {
        m_client_id = mac_address_t( client_id );
        qDebug() << "Client ID: " << m_client_id.toString();
      }
      else
      {
        qDebug() << "Client ID type unknown: " << type;
      }
    }
    else
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );

      qDebug() << "Removing " << int(option_length);
      data.remove( 0, option_length );
    }
  }
  qDebug() << "***********************************";
}

void dhcp_message_t::SetRequestType(DhcpRequestType type)
{
  request_type = type;
  QByteArray array;
  array.append( 0x1 );
  array.append( static_cast<uint8_t>(type) );
  SetOption( DhcpOption::DHCP_MESSAGE_TYPE, array );
}

void dhcp_message_t::SetOperationType( BootpOpType type )
{
  header.operation = type;
}

void dhcp_message_t::SetHardwareType( int type )
{
//  header.hardware_address_length = type;
}

void dhcp_message_t::SetHops( int hops )
{
  header.hops = hops;
}

void dhcp_message_t::SetClientMAC(mac_address_t client_mac)
{
  header.hardware_address_client = client_mac;
}

void dhcp_message_t::SetOption(DhcpOption option, QByteArray options_data)
{
  if( option == DhcpOption::END ) return; // end option needs to at the end of every message... we'll do it manually
  m_options[option] = options_data;
}

void dhcp_message_t::SetRouter( QHostAddress router_address )
{
  QByteArray array;
  array.append( 0x4 );
  array.append( router_address.toIPv4Address() );
  SetOption( DhcpOption::ROUTER, array );
}

void dhcp_message_t::SetClientAddress(QHostAddress client_addresss)
{
  m_address_client = client_addresss;
}

QString dhcp_message_t::toString() const
{
  return
    QString( "[%1] [%2] [%3]" )
      .arg( ::toString(request_type) )
      .arg( m_client_hostname )
      .arg( header.hardware_address_client.toString() )
  ;
}

//QByteArray dhcp_message_t::serialize() const
//{
//  QByteArray array;
//  array.reserve( MIN_DHCP_SIZE );

//  dhcp_header_t header;
//  header.op = static_cast<uint8_t>( m_operation );
//  header.htype = static_cast<uint8_t>( m_hardware_type );

//  header.hlen = 6; //mac length
//  header.hops = 0;
//  header.xid = m_transaction_id;
//  header.secs = 0;
//  header.flags = ( m_is_broadcast ? 0x01 : 0x00 );
//  header.ciaddr = m_address_client.toIPv4Address();
//  header.yiaddr = m_address_yours.toIPv4Address();
//  header.siaddr = m_address_nextServer.toIPv4Address();
//  header.giaddr = m_address_relay.toIPv4Address();
//  // ::strncpy( &header.chaddr, "", 1 );
//  // ::strncpy( &header.sname, "", 1 );
//  // ::strncpy( &header.file, "", 1 );
//  header.cookie[0] = 0x63;
//  header.cookie[1] = 0x82;
//  header.cookie[2] = 0x53;
//  header.cookie[3] = 0x63;
//  ::memcpy( array.data(), &header, MIN_DHCP_SIZE );
////  array.append( static_cast<const char*>( &header ), sizeof(dhcp_header_t) );

//  auto i = m_options.constBegin();
//  while( i != m_options.constEnd() )
//  {
//    array.append( static_cast<uint8_t>( i.key() ) );
//    array.append( i.value() );

//    ++i;
//  }

//  array.append( static_cast<uint8_t>( DhcpOption::END ) ); // always the last option
//  return array;
//}
