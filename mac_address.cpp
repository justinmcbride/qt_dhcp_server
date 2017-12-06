#include "mac_address.h"

#include <QDebug>

mac_address_t::mac_address_t()
{
  raw_address.resize(6);
  for( int i = 0; i < raw_address.size(); i++ )
  {
    raw_address[i] = 0;
  }
}

mac_address_t::mac_address_t( QByteArray data )
{
  raw_address.resize( 6 );
  for( int i = 0; i < raw_address.size(); i++ )
  {
    qDebug() << "data[" << i << "] = " << quint8(data[i]);
    raw_address[i] = quint8(data[i]);
  }
}

QString mac_address_t::toString() const
{
  // change this to be generated at time it's created
  return
    QString( "%0:%1:%2:%3:%4:%5" )
      .arg( QString::number( quint8( raw_address[0] ), 16 ), 2, '0' )
      .arg( QString::number( quint8( raw_address[1] ), 16 ), 2, '0' )
      .arg( QString::number( quint8( raw_address[2] ), 16 ), 2, '0' )
      .arg( QString::number( quint8( raw_address[3] ), 16 ), 2, '0' )
      .arg( QString::number( quint8( raw_address[4] ), 16 ), 2, '0' )
      .arg( QString::number( quint8( raw_address[5] ), 16 ), 2, '0' )
  ;
}
