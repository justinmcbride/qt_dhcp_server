#include "dhcp_message.h"

#include <QString>

dhcp_message_t::dhcp_message_t( QByteArray message_data )
{
  if( message_data.size() < MIN_DHCP_SIZE )
  {
    qDebug() << "packet too small: " << message_data.size();
    return;
  }
  dhcp_header_t* hdr = reinterpret_cast<dhcp_header_t*>( message_data.data() );
  operation = static_cast<OpType>( hdr->op );
  hardware_type = hdr->htype;
  hardware_length = hdr->hlen;
  hops = hdr->hops;
  transaction_id = hdr->xid;
  seconds = hdr->secs;
  broadcast = ( hdr->flags & 0x80 );

  address_client = QHostAddress( hdr->ciaddr );
  address_yours = QHostAddress( hdr->yiaddr );
  address_nextServer = QHostAddress( hdr->siaddr );
  address_relay = QHostAddress( hdr->giaddr );

  for( int i = 0; i < hardware_length; i++ )
  {
    hardware_address_client += QString::number( hdr->chaddr[i], 16 );
    if( i < ( hardware_length - 1) ) hardware_address_client += ":";
  }

  if( hdr->cookie[0] == 99 && hdr->cookie[1] == 130 && hdr->cookie[2] == 83 && hdr->cookie[3] == 99 )
  {
    qDebug() << "cookie valid";
  }
  else
  {
    qDebug() << "bad cookie!!! " << hdr->cookie[0] << ":" << hdr->cookie[1] << ":" << hdr->cookie[2] << ":" << hdr->cookie[3];
  }

  auto remaining_size = message_data.size() - MIN_DHCP_SIZE;
  options = message_data.right( remaining_size );
  parseOptions( options );
}

void dhcp_message_t::parseOptions( QByteArray data )
{
  while( data.size() )
  {
    uint8_t option_type = data.at( 0 );
    data.remove( 0, 1 );
    qDebug() << "option type: " << option_type;
    if( option_type == 0 )
    {
      qDebug() << "pad option";
    }
    else if( option_type == 255 )
    {
      qDebug() << "end option-- padding=" << data.size();
      data.clear();
    }
    else if( option_type == 53 )
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
    else if( option_type == 12 )
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );

      client_hostname = data.left( option_length );
      qDebug() << "hostname " << client_hostname;
      data.remove( 0, option_length );
    }
    else
    {
      uint8_t option_length = data.at( 0 );
      data.remove( 0, 1 );
      data.remove( 0, option_length );
      qDebug() << "other: type = " << option_type << " size=" << option_length;
    }
  }
}

QString dhcp_message_t::toString() const
{
  return QString( "%1 %2 %3" )
      .arg( ::toString(request_type) )
      .arg( client_hostname )
      .arg( hardware_address_client );
}
