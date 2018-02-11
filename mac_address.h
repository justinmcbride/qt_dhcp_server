#pragma once

#include <QByteArray>
#include <QString>

struct mac_address_t
{
  public:
    mac_address_t();
    mac_address_t( QByteArray data );
    QString toString() const;

    friend inline bool operator==( const mac_address_t& one, const mac_address_t& two ) { return one.raw_address == two.raw_address; }
  private:
    QByteArray raw_address;
};
