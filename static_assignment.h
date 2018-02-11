#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

#include "mac_address.h"

class static_assignment_t
{
  public:
    static_assignment_t( mac_address_t address, QHostAddress ip );
    static_assignment_t() {}

    mac_address_t address() const { return m_address; }

    QHostAddress ip() const;
    void setIp( const QHostAddress& ip );

  private:
    mac_address_t m_address;
    QHostAddress m_ip;
};
