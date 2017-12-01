#include "dhcp_message.h"

#include <QString>

dhcp_message_t::dhcp_message_t( QByteArray message_data )
{
  if( message_data.size() < MIN_DHCP_SIZE )
  {
    qDebug() << "packet too small: " << message_data.size();
    throw std::invalid_argument( "something" );
  }
  dhcp_header_t* hdr = reinterpret_cast<dhcp_header_t*>( message_data.data() );
  m_operation = static_cast<OpType>( hdr->op );
  m_hardware_type = hdr->htype;
  m_hardware_length = hdr->hlen;
  m_hops = hdr->hops;
  m_transaction_id = hdr->xid;
  m_seconds = hdr->secs;
  m_is_broadcast = ( hdr->flags & 0x80 );

  m_address_client = QHostAddress( hdr->ciaddr );
  m_address_yours = QHostAddress( hdr->yiaddr );
  m_address_nextServer = QHostAddress( hdr->siaddr );
  m_address_relay = QHostAddress( hdr->giaddr );

  if( m_hardware_length == 6 )
  {
    for( int i = 0; i < m_hardware_length; i++ )
    {
      m_hardware_address_client[i] = hdr->chaddr[i];
    }
  }

  if( hdr->cookie[0] == 99 && hdr->cookie[1] == 130 && hdr->cookie[2] == 83 && hdr->cookie[3] == 99 )
  {
    qDebug() << "cookie valid";
  }
  else
  {
    qDebug() << "bad cookie!!! " << hdr->cookie[0] << ":" << hdr->cookie[1] << ":" << hdr->cookie[2] << ":" << hdr->cookie[3];
    throw std::invalid_argument( "something" );
  }

  auto remaining_size = message_data.size() - MIN_DHCP_SIZE;
  m_options = message_data.right( remaining_size );
  parseOptions( m_options );
}

void dhcp_message_t::parseOptions( QByteArray data )
{
  while( data.size() )
  {
    try
    {
      auto option_type = getOptionType( data.at( 0 ) );
      data.remove( 0, 1 );
      qDebug() << "option type: " << ::toString( option_type );
      if( option_type == DhcpOption::PAD )
      {
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
          qDebug() << "requested: " << requested_parameter;
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

        auto client_id = data.left( option_length );
        data.remove( 0, option_length );
        qDebug() << "client id: " << client_id;
        if( type != 1 )
        {
          qDebug() << "unknown type of identifier: " << type;
        }
      }
      else
      {
        uint8_t option_length = data.at( 0 );
        data.remove( 0, 1 );

        qDebug() << "don't know this so just removing its stuff = " << int(option_length);
        data.remove( 0, option_length );
      }
    }
    catch( std::out_of_range& ex )
    {
      uint8_t option_type = data.at( 0 );
      data.remove( 0, 1 );
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );
      data.remove( 0, option_length );
      qDebug() << "other: type = " << int(option_type) << " size=" << int(option_length);
    }
  }
}

QString dhcp_message_t::toString() const
{
  return
    QString( "[%1] [%2] [%3]" )
      .arg( ::toString(request_type) )
      .arg( m_client_hostname )
      .arg( ::toString(m_hardware_address_client) )
  ;
}

QByteArray dhcp_message_t::serialize() const
{
  QByteArray array;
  array.reserve( MIN_DHCP_SIZE );

  dhcp_header_t header;
  header.op = static_cast<uint8_t>( m_operation );
  header.htype = static_cast<uint8_t>( m_hardware_type );

  header.hlen = 6; //mac length
  header.hops = 0;
  header.xid = m_transaction_id;
  header.secs = 0;
  header.flags = ( m_is_broadcast ? 0x01 : 0x00 );
  header.ciaddr = m_address_client.toIPv4Address();
  header.yiaddr = m_address_yours.toIPv4Address();
  header.siaddr = m_address_nextServer.toIPv4Address();
  header.giaddr = m_address_relay.toIPv4Address();
  // ::strncpy( &header.chaddr, "", 1 );
  // ::strncpy( &header.sname, "", 1 );
  // ::strncpy( &header.file, "", 1 );
  header.cookie[0] = 99;
  header.cookie[1] = 130;
  header.cookie[2] = 83;
  header.cookie[3] = 99;

  ::memcpy( array.data(), &header, MIN_DHCP_SIZE );
  return array;
}
