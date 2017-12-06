#pragma once

#include <QByteArray>
#include <QString>

struct mac_address_t
{
  public:
    mac_address_t();
    mac_address_t( QByteArray data );
    QString toString() const;
  private:
    QByteArray raw_address;
};
