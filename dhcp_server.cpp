#include "dhcp_server.h"
#include "dhcp_message.h"
#include <QNetworkDatagram>


dhcp_server_t::dhcp_server_t(QObject *parent) : QObject(parent)
{
}

void dhcp_server_t::setup( int interface_index )
{
  if( m_socket_listener )
  {
    emit LogMessage( "Deleting old server socket" );
    delete m_socket_listener;
    m_socket_listener = nullptr;
  }
  m_socket_listener = new QUdpSocket( this );
  connect( m_socket_listener, &QUdpSocket::readyRead, this, &dhcp_server_t::readPendingDatagrams );
  bool b = m_socket_listener->bind( PORT_DHCP_SERVER, QUdpSocket::ShareAddress );
  emit LogMessage( QString( "Bind success: %1" ).arg( b ) );
  qDebug() << "bind = " << b;
}

void dhcp_server_t::readPendingDatagrams()
{
  while( m_socket_listener->hasPendingDatagrams() )
  {
    QNetworkDatagram datagram = m_socket_listener->receiveDatagram();
    auto msg = reinterpret_cast<dhcp_message_t*>( datagram.data() );
    emit LogMessage( datagram.data().toHex() );
  }
}
