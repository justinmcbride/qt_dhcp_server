#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "dhcp_message.h"

enum class model_dhcp_headers_e {
  OPERATION_TYPE,
  CLIENT_MAC,
  CLIENT_GIVEN_ADDRESS,
  SENTINEL
};

const QVector<model_dhcp_headers_e> MODEL_DHCP_HEADERS {
  model_dhcp_headers_e::OPERATION_TYPE,
  model_dhcp_headers_e::CLIENT_MAC,
  model_dhcp_headers_e::CLIENT_GIVEN_ADDRESS,
};

inline QString toString( model_dhcp_headers_e val )
{
  switch( val )
  {
    case model_dhcp_headers_e::OPERATION_TYPE: return "Operation";
    case model_dhcp_headers_e::CLIENT_MAC: return "Client MAC";
    case model_dhcp_headers_e::CLIENT_GIVEN_ADDRESS: return "IP Address";
    case model_dhcp_headers_e::SENTINEL: return "SENTINEL";
    default: return "UNKNOWN";
  }
}

class model_dhcp_log_t : public QAbstractTableModel
{
  Q_OBJECT

  public:
    explicit model_dhcp_log_t( QObject *parent = nullptr );

    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

    int rowCount( const QModelIndex& = QModelIndex() ) const override;
    int columnCount( const QModelIndex& = QModelIndex() ) const override;

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

    void AddRequest( dhcp_message_t dhcp_request );

    dhcp_message_t GetItem( int index );
private:
    QVector< dhcp_message_t > m_requests;
};
