#ifndef DHCP_SERVER_H
#define DHCP_SERVER_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

constexpr auto PORT_DHCP_SERVER = 67;
constexpr auto PORT_DHCP_CLIENT = 68;

class dhcp_server_t : public QObject
{
  Q_OBJECT
public:
  explicit dhcp_server_t(QObject *parent = 0);

  void setup( int interface_index );
signals:
  void LogMessage( QString message );
public slots:

private slots:
  void readPendingDatagrams();
private:
  QUdpSocket* m_socket_listener{ new QUdpSocket(this) };
};

#endif // DHCP_SERVER_H
