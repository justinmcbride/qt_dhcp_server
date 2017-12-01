#include "static_assignment_t.h"

static_assignment_t::static_assignment_t( QString address, QHostAddress ip ) :
  m_address( address ),
  m_ip( ip )
{

}

QString static_assignment_t::address() const
{
  return m_address;
}

QHostAddress static_assignment_t::ip() const
{
  return m_ip;
}

void static_assignment_t::setIp(const QHostAddress& ip)
{
  m_ip = ip;
}
