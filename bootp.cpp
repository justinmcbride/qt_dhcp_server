#include "bootp.h"

#include <QDataStream>

const QByteArray bootp_t::COOKIE{ "\x63\x82\x53\x63" };

bootp_t::bootp_t()
{

}

bool bootp_t::deserialize( QByteArray data )
{
  QDataStream stream( data );

  quint8 op, htype;
  quint32 ciaddr, yiaddr, siaddr, giaddr;

  stream >>
    op >> htype >> hardware_address_length >> hops >>
    transaction_id >>
    secs >> flags >>
    ciaddr >>
    yiaddr >>
    siaddr >>
    giaddr
  ;

  QByteArray chaddr;
  chaddr.resize( 16 );
  stream.readRawData( chaddr.data(), chaddr.size() );

  QByteArray sname;
  sname.resize( 64 );
  stream.readRawData( sname.data(), sname.size() );

  QByteArray file;
  file.resize( 128 );
  stream.readRawData( file.data(), file.size() );

  QByteArray cookie;
  cookie.resize( 4 );
  stream.readRawData( cookie.data(), cookie.size() );
  if( cookie != COOKIE )
  {
    qDebug() << "bad cookie";
    qDebug() << cookie;
    qDebug() << COOKIE;
    return false;
  }

  hardware_address_type = static_cast<BootpHwType>( htype );
  operation = static_cast<BootpOpType>( op );

  if( hardware_address_type == BootpHwType::ETHERNET && hardware_address_length == 6 )
  {
    hardware_address_client = QByteArray( chaddr );
  }

  return true;
}

QByteArray bootp_t::serialize() const
{
  QByteArray output_array;

  QDataStream stream( &output_array, QIODevice::WriteOnly );
  stream <<
    static_cast<quint8>( operation ) << static_cast<quint8>( hardware_address_type ) << hardware_address_length << hops <<
    transaction_id <<
    secs << flags <<
    ciaddr <<
    yiaddr <<
    siaddr <<
    giaddr
  ;

  QByteArray chaddr;
  chaddr.fill( 0, 16 );
  stream.writeRawData( chaddr.data(), chaddr.size() );

  QByteArray sname;
  sname.fill( 0, 64 );
  stream.writeRawData( sname.data(), sname.size() );

  QByteArray file;
  file.fill( 0, 128 );
  stream.writeRawData( file.data(), file.size() );

  stream.writeRawData( COOKIE.data(), COOKIE.size() );

  return output_array;
}
