#include "dhcp_assignments.h"

dhcp_assignments_t::dhcp_assignments_t()
{
  m_assignments.push_back( static_assignment_t( "First Address", QHostAddress( QHostAddress::LocalHost ) ) );
  m_assignments.push_back( static_assignment_t( "Second Address", QHostAddress( "192.126.44.44" ) ) );
}

void dhcp_assignments_t::AddAssignment(QString target, QString address)
{

}

void dhcp_assignments_t::RemoveAssignment(QString target)
{

}

const static_assignment_t& dhcp_assignments_t::GetAssignment( int index ) const
{
  return m_assignments.at(index);
}

QVector<static_assignment_t> dhcp_assignments_t::GetAssignments() const
{
  return m_assignments;
}
