#include "dhcp_assignments.h"

std::shared_ptr<dhcp_assignments_t> dhcp_assignments_t::get()
{
  static std::shared_ptr<dhcp_assignments_t> a( new dhcp_assignments_t );
  return a;
}

dhcp_assignments_t::dhcp_assignments_t()
{
}

void dhcp_assignments_t::SetBaseAddress( QHostAddress new_base )
{
  m_address_base = new_base;
  m_next_inc = 1;
}

void dhcp_assignments_t::AddAssignment( QString target, QString address )
{

}

void dhcp_assignments_t::RemoveAssignment( QString target )
{

}

QHostAddress dhcp_assignments_t::GetAssignment( mac_address_t client_address )
{
  // search all static reservations first
  for( static_assignment_t assignment : m_assignments_static )
  {
    if( assignment.address() == client_address ) return assignment.ip();
  }

  // there is no static reservation for this client.. generate new one
  // FIXME: range check

  // qDebug() << __FUNCTION__ << " ";
  return QHostAddress( m_address_base.toIPv4Address() + m_next_inc++ );
}

const static_assignment_t& dhcp_assignments_t::GetStaticAssignment( int index ) const
{
  return m_assignments_static.at( index );
}

QVector<static_assignment_t> dhcp_assignments_t::GetAllStaticAssignments() const
{
  return m_assignments_static;
}
