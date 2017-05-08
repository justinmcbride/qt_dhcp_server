#include "dhcp_message.h"

#include <QString>

dhcp_message_t::dhcp_message_t( QByteArray message_data )
{
  qDebug() << __FUNCTION__;
  op = message_data.at( 0 );
  htype = message_data.at( 1 );
  hlen = message_data.at( 2 );
  hops = message_data.at( 3 );
  xid = ( message_data.at( 7 ) << 24 )
        | ( message_data.at( 6 ) << 16 )
        | ( message_data.at( 5 ) << 8 )
        | ( message_data.at( 4 ) )
  ;

  secs = ( message_data.at( 9 ) << 8 )
         | ( message_data.at( 8 ) )
  ;

  qDebug() << "hello there";
  qDebug() << toString();
}

QString dhcp_message_t::toString() const
{
  return QString( "emit: %1 %2" )
      .arg( xid )
      .arg( secs );
}
