#include "dhcp_server.h"
#include "dhcp_message.h"

#include <QNetworkDatagram>
#include <QDataStream>

dhcp_server_t::dhcp_server_t( QObject* parent ) :
  QObject( parent )
{
}

void dhcp_server_t::SetInterface( int interface_index )
{
  if( m_socket_listener )
  {
    emit LogMessage( "Deleting old server socket" );
    delete m_socket_listener;
    m_socket_listener = nullptr;
  }
  m_server_interface = QNetworkInterface::interfaceFromIndex( interface_index );
  m_server_address = m_server_interface.addressEntries().back().ip().toString();
}

bool dhcp_server_t::SetState( bool on )
{
  if( on )
  {
    m_socket_listener = new QUdpSocket( this );
    connect( m_socket_listener, &QUdpSocket::readyRead, this, &dhcp_server_t::readPendingDatagrams );
    bool is_bound = m_socket_listener->bind( PORT_DHCP_SERVER, QUdpSocket::ShareAddress );
    emit LogMessage( QString( "Server socket connected: %1" ).arg( is_bound ? "true" : "false" ) );
    return is_bound;
  }
  else
  {
    emit LogMessage( "Shutting down server" );
    if( m_socket_listener )
    {
      emit LogMessage( "Deleting old server socket" );
      delete m_socket_listener;
      m_socket_listener = nullptr;
    }
    return false;
  }
}

void dhcp_server_t::readPendingDatagrams()
{
  while( m_socket_listener->hasPendingDatagrams() )
  {
    QNetworkDatagram datagram = m_socket_listener->receiveDatagram();
    try
    {
      dhcp_message_t client_request( datagram.data() );
      emit LogMessage( client_request.toString() );
      const auto type = client_request.GetRequestType();
      switch( type )
      {
        case DhcpRequestType::DHCPDISCOVER:
        case DhcpRequestType::DHCPREQUEST:
          performOffer( client_request );
        default:
          ;
      }
    }
    catch( ... )
    {
      continue;
    }
  }
}

void dhcp_server_t::performOffer( dhcp_message_t request )
{
  /*
   If the 'giaddr' field is zero and the 'ciaddr' field is nonzero, then the server
   unicasts DHCPOFFER and DHCPACK messages to the address in 'ciaddr'.
   If 'giaddr' is zero and 'ciaddr' is zero, and the broadcast bit is
   set, then the server broadcasts DHCPOFFER and DHCPACK messages to
   0xffffffff. If the broadcast bit is not set and 'giaddr' is zero and
   'ciaddr' is zero, then the server unicasts DHCPOFFER and DHCPACK
   messages to the client's hardware address and 'yiaddr' address.  In
   all cases, when 'giaddr' is zero, the server broadcasts any DHCPNAK
   messages to 0xffffffff.
  */

  auto response = dhcp_message_t::CreateOffer( QHostAddress() );
  response.header.transaction_id = request.header.transaction_id;
  response.SetRouter( QHostAddress( "192.168.1.1" ) );
  response.SetClientAddress( getAddress( request.m_client_id ) );
  response.SetSubnet( m_address_subnet );
  response.SetDns( m_address_dns );
  response.SetClientMAC( request.header.hardware_address_client );
  response.SetLeaseTime( 60 * 5 );
  response.SetServerIdentifier( m_server_address );

  auto response_data = response.serialize();
  qDebug() << response_data;
  /*
   A server or relay agent sending or relaying a DHCP message directly
   to a DHCP client (i.e., not to a relay agent specified in the
   'giaddr' field) SHOULD examine the BROADCAST bit in the 'flags'
   field.  If this bit is set to 1, the DHCP message SHOULD be sent as
   an IP broadcast using an IP broadcast address (preferably 0xffffffff)
   as the IP destination address and the link-layer broadcast address as
   the link-layer destination address.  If the BROADCAST bit is cleared
   to 0, the message SHOULD be sent as an IP unicast to the IP address
   specified in the 'yiaddr' field and the link-layer address specified
   in the 'chaddr' field.  If unicasting is not possible, the message
   MAY be sent as an IP broadcast using an IP broadcast address
   (preferably 0xffffffff) as the IP destination address and the link-
   layer broadcast address as the link-layer destination address.
   */

  QHostAddress target( "192.168.86.51" );
  m_socket_listener->writeDatagram( response_data, target, PORT_DHCP_CLIENT );
}

QHostAddress dhcp_server_t::getAddress( mac_address_t client_id )
{
  return QHostAddress( "192.168.86.51" );
}
