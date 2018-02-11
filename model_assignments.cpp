#include <QDebug>

#include "model_assignments.h"

ModelAssignments::ModelAssignments( QObject* parent )
  : QAbstractTableModel( parent )
{
}

int ModelAssignments::rowCount( const QModelIndex& ) const
{
  return m_assignments->size();
}

int ModelAssignments::columnCount( const QModelIndex& ) const
{
  return 2;
}

QVariant ModelAssignments::data( const QModelIndex& index, int role ) const
{
  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int col = index.column();

  auto assignment = m_assignments->GetStaticAssignment( row );
  if( role == Qt::DisplayRole )
    {
      if( col == 0 )           return assignment.address().toString();
      else if( col == 1 )      return assignment.ip().toString();
    }
  return QVariant();
}

bool ModelAssignments::setData( const QModelIndex& index, const QVariant& value, int role )
{
  if (data(index, role) != value) {
    // FIXME: Implement me!
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags ModelAssignments::flags( const QModelIndex& index ) const
{
  return ( Qt::ItemIsEditable | QAbstractTableModel::flags(index) );
}

bool ModelAssignments::insertRows( int row, int count, const QModelIndex& parent )
{
  beginInsertRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endInsertRows();
  return true;
}

bool ModelAssignments::insertColumns( int column, int count, const QModelIndex& parent )
{
  beginInsertColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endInsertColumns();
  return true;
}

bool ModelAssignments::removeRows( int row, int count, const QModelIndex& parent )
{
  beginRemoveRows(parent, row, row + count - 1);
  // FIXME: Implement me!
  endRemoveRows();
  return true;
}

bool ModelAssignments::removeColumns( int column, int count, const QModelIndex& parent )
{
  beginRemoveColumns(parent, column, column + count - 1);
  // FIXME: Implement me!
  endRemoveColumns();
  return true;
}

QVariant ModelAssignments::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role == Qt::DisplayRole )
  {
    if( orientation == Qt::Horizontal )
    {
      if( section == 0 )      return "MAC Address";
      else if( section == 1 ) return "IP Address";
    }
  }
  return QVariant();
}
