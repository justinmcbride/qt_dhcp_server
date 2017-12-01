#pragma once

#include <QObject>
#include <QHostAddress>

class client_t
{
  private:
    QString m_identifier;
    QString m_mac;
    QString m_hostname;

    QHostAddress m_address_current;
  public:
    client_t();
};
