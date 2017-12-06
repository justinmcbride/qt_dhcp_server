#pragma once

#include <QObject>
#include <QtNetwork/QUdpSocket>

#include "dhcp_message.h"

constexpr auto PORT_DHCP_SERVER = 67;
constexpr auto PORT_DHCP_CLIENT = 68;

class dhcp_server_t : public QObject
{
  Q_OBJECT

  public:
    explicit dhcp_server_t( QObject *parent = nullptr );

    void setup( int interface_index );
  signals:
    void LogMessage( QString message );
  public slots:

  private slots:
    void readPendingDatagrams();
  private:
    QUdpSocket* m_socket_listener{ nullptr };
    void performOffer( dhcp_message_t request );

    QHostAddress getAddress( mac_address_t client_id );
};
