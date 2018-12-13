#pragma once

#include <QObject>
#include <QQueue>
#include <QtNetwork/QUdpSocket>
#include <QNetworkInterface>

#include "dhcp_message.h"
#include "dhcp_assignments.h"

constexpr auto PORT_DHCP_SERVER = 67;
constexpr auto PORT_DHCP_CLIENT = 68;

class dhcp_server_t : public QObject
{
  Q_OBJECT

  public:
    dhcp_server_t( QObject* parent = nullptr );

    void SetInterface( int interface_index );
    bool SetState( bool on );
    void SetRouter( QHostAddress address );
  signals:
    void signal_receivedRequest( dhcp_message_t );
  public slots:
  private slots:
    void readPendingDatagrams();
  private:
    QUdpSocket* m_socket_listener{ nullptr };
    void performOffer( dhcp_message_t request );

    std::shared_ptr<dhcp_assignments_t> m_assignments{ dhcp_assignments_t::get() };

    QHostAddress m_address_subnet{ "255.255.255.0" };
    QHostAddress m_address_dns{ "8.8.8.8" };
    QHostAddress m_address_router{ "192.168.1.1" };

    QHostAddress m_server_address;
    QNetworkInterface m_server_interface;
};
