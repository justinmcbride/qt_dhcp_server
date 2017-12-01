#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

class static_assignment_t
{
  public:
    static_assignment_t( QString address, QHostAddress ip );
    static_assignment_t() {}

    QString address() const;

    QHostAddress ip() const;
    void setIp( const QHostAddress& ip );

  private:
    QString m_address;
    QHostAddress m_ip;
};
