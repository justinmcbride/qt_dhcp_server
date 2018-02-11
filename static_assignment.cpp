#include "static_assignment.h"

static_assignment_t::static_assignment_t( mac_address_t address, QHostAddress ip ) :
  m_address( address ),
  m_ip( ip )
{

}

QHostAddress static_assignment_t::ip() const
{
  return m_ip;
}

void static_assignment_t::setIp(const QHostAddress& ip)
{
  m_ip = ip;
}
