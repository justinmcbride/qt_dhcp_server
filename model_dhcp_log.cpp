#include "model_dhcp_log.h"

model_dhcp_log_t::model_dhcp_log_t( QObject* parent ) :
  QAbstractTableModel( parent )
{
}

QVariant model_dhcp_log_t::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role == Qt::DisplayRole )
  {
    if( orientation == Qt::Horizontal )
    {
      return toString( static_cast<model_dhcp_headers_e>( section ) );
    }
  }
  return QVariant();
}

int model_dhcp_log_t::rowCount( const QModelIndex& ) const
{
  return m_requests.count();
}

int model_dhcp_log_t::columnCount( const QModelIndex& ) const
{
  return static_cast<int>( model_dhcp_headers_e::SENTINEL );
}

QVariant model_dhcp_log_t::data( const QModelIndex& index, int role ) const
{
  int row = index.row();
  int col = index.column();

  auto column = static_cast<model_dhcp_headers_e>( col );

  if( !index.isValid() )
  {
    return QVariant();
  }
  
  auto dhcp = m_requests.at( row );
  if( role == Qt::DisplayRole )
  {
    if( column == model_dhcp_headers_e::OPERATION_TYPE )            return toString( dhcp.request_type );
    else if( column == model_dhcp_headers_e::CLIENT_MAC )           return dhcp.m_client_id.toString();
    else if( column == model_dhcp_headers_e::CLIENT_GIVEN_ADDRESS ) return dhcp.m_address_client.toString();
  }
  // else if( role == Qt::ForegroundRole )
  // {
  //   return QColor::fromRgb( 255, 10, 10 );
  // }
  // else if( role == Qt::BackgroundRole )
  // {
  //   return QColor::fromRgb( 168, 30, 30 );
  // }

  return QVariant();
}

void model_dhcp_log_t::AddRequest( dhcp_message_t dhcp_request )
{
  beginInsertRows( QModelIndex(), m_requests.size(), m_requests.size() );
  m_requests.push_back( dhcp_request );
  endInsertRows();
}

dhcp_message_t model_dhcp_log_t::GetItem( int index )
{
  return m_requests.at( index );
}
