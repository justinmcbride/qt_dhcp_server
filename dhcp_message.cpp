#include "dhcp_message.h"
#include "bootp.h"

#include <QString>
#include <QDataStream>

dhcp_message_t::dhcp_message_t( QByteArray message_data )
{
  auto preamble = message_data.left( 240 );
  message_data.remove( 0, 240 );

  bool good = header.deserialize( preamble );
  if( !good )
  {
    qDebug() << "couldn't deserialize header";
    throw std::invalid_argument( "something" );
  }

  parseOptions( message_data );
}

void dhcp_message_t::parseOptions( QByteArray data )
{
  qDebug() << "***********************************";
  qDebug() << "[PARSING OPTIONS]";
  while( data.size() )
  {
    auto option_type = getOptionType( data.at( 0 ) );
    if( option_type == DhcpOption::UNKNOWN )
    {
      qDebug() << "uh oh";
    }
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
    else if( option_type == DhcpOption::DHCP_MAX_SIZE )
    {
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );

      data.remove( 0, option_length );
    }
    else if( option_type == DhcpOption::PARAMETER_REQUEST_LIST )
    {
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );

      qDebug() << "requesting " << option_length << " items";
      for( int i = 0; i < option_length; i++ )
      {
        quint8 requested_parameter = data.at( 0 );
        data.remove( 0, 1 );
        qDebug() << "requested: " << DhcpOptionToString(requested_parameter);
      }
    }
    else if( option_type == DhcpOption::REQUESTED_IP )
    {
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );
      if( option_length != 4 )
      {
        qDebug() << "some problem with length: " << option_length;
      }
      quint8 byte_1 = data.at( 0 );
      quint8 byte_2 = data.at( 1 );
      quint8 byte_3 = data.at( 2 );
      quint8 byte_4 = data.at( 3 );

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
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );
      if( option_length != 1 )
      {
        qDebug() << "invalid length for opt";
      }
      quint8 type = data.at( 0 );
      request_type = static_cast<DhcpRequestType>( type );
      qDebug() << "type: " << ::toString( request_type );
      data.remove( 0, option_length );
    }
    else if( option_type == DhcpOption::HOSTNAME )
    {
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );

      m_client_hostname = data.left( option_length );
      qDebug() << "hostname " << m_client_hostname;
      data.remove( 0, option_length );
    }
    else if( option_type == DhcpOption::CLIENT_ID )
    {
      quint8 option_length = data.at( 0 );
      data.remove( 0, 1 );

      quint8 type = data.at( 0 );
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
      quint8 option_length = data.at( 0 );
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
  array.append( static_cast<quint8>(type) );
  SetOption( DhcpOption::DHCP_MESSAGE_TYPE, array );
}

void dhcp_message_t::SetClientMAC( mac_address_t client_mac )
{
  header.hardware_address_client = client_mac;
  header.hardware_address_length = 6;
  header.hardware_address_type = BootpHwType::ETHERNET;
}

void dhcp_message_t::SetOption( DhcpOption option, QByteArray options_data )
{
  if( option == DhcpOption::END ) return; // end option needs to at the end of every message... we'll do it manually
  m_options[option] = options_data;
}

void dhcp_message_t::SetRouter( QHostAddress address )
{
  QByteArray data;
  {
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream << address.toIPv4Address();
  }
  SetOption( DhcpOption::ROUTER, data );
}

void dhcp_message_t::SetServerIdentifier( QHostAddress address )
{
  QByteArray data;
  {
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream << address.toIPv4Address();
  }
  SetOption( DhcpOption::DHCP_SERVER_IDENTIFIER, data );
}

void dhcp_message_t::SetLeaseTime( int seconds )
{
  QByteArray lease_time;
  {
    QDataStream stream( &lease_time, QIODevice::WriteOnly );
    stream << quint32( seconds );
  }
  SetOption( DhcpOption::IP_LEASE_TIME, lease_time );
}

void dhcp_message_t::SetSubnet( QHostAddress address )
{
  QByteArray data;
  {
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream << address.toIPv4Address();
  }
  SetOption( DhcpOption::SUBNET_MASK, data );
}

void dhcp_message_t::SetDns( QHostAddress address )
{
  QByteArray data;
  {
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream << address.toIPv4Address();
  }
  SetOption( DhcpOption::DNS, data );
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

QByteArray dhcp_message_t::serialize() const
{
  QByteArray array;

  array.append( header.serialize() );

  auto i = m_options.constBegin();
  while( i != m_options.constEnd() )
  {
    array.append( static_cast<quint8>( i.key() ) );
    array.append( (quint8)i.value().length() );
    array.append( i.value() );

    ++i;
  }

  array.append( static_cast<quint8>( DhcpOption::END ) ); // always the last option
  return array;
}

dhcp_message_t dhcp_message_t::CreateOffer( QHostAddress offered_address )
{
  dhcp_message_t dhcp;
  dhcp.header.operation = BootpOpType::BOOT_REPLY;
  dhcp.SetRequestType( DhcpRequestType::DHCPOFFER );
  dhcp.m_address_yours = offered_address;

  return dhcp;
}

dhcp_message_t dhcp_message_t::CreateACK( uint32_t transaction_id, QHostAddress client_addresss, mac_address_t client_mac )
{
  dhcp_message_t dhcp;
  dhcp.SetRequestType( DhcpRequestType::DHCPACK );
  dhcp.header.operation = BootpOpType::BOOT_REPLY;

  dhcp.m_address_yours = client_addresss;
  dhcp.SetClientMAC( client_mac );

  return dhcp;
}

dhcp_message_t dhcp_message_t::CreateNAK()
{
  dhcp_message_t dhcp;

  dhcp.SetRequestType( DhcpRequestType::DHCPNAK );
  return dhcp;
}
