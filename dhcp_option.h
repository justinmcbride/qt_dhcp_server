#pragma once

#include <cstdint>
#include <exception>

enum class DhcpOption {
  PAD = 0,
  SUBNET_MASK = 1,
  ROUTER = 3,
  DNS = 6,
  HOSTNAME = 12,
  REQUESTED_IP = 50,
  IP_LEASE_TIME = 51,
  DHCP_MESSAGE_TYPE = 53,
  DHCP_SERVER_IDENTIFIER = 54,
  PARAMETER_REQUEST_LIST = 55,
  DHCP_MAX_SIZE = 57,
  CLIENT_ID = 61,
  END = 255,
  UNKNOWN = 256 // out of range intentionally
};

static inline QString DhcpOptionToString( int option )
{
  switch( option )
  {
    case 0: return "PAD";
    case 1: return "SUBNET_MASK";
    case 3: return "ROUTER";
    case 6: return "DNS";
    case 12: return "HOSTNAME";
    case 50: return "REQUESTED_IP";
    case 51: return "IP_LEASE_TIME";
    case 53: return "DHCP_MESSAGE_TYPE";
    case 54: return "DHCP_SERVER_IDENTIFIER";
    case 55: return "PARAMETER_REQUEST_LIST";
    case 57: return "DHCP_MAX_SIZE";
    case 61: return "CLIENT_ID";
    case 255: return "END";
    default: return QString( "UNKNOWN -- %0" ).arg( option );
  }
}

static inline QString toString( const DhcpOption option )
{
  return DhcpOptionToString( static_cast<int>( option ) );
}

static DhcpOption getOptionType( uint8_t val )
{
  switch( val )
  {
    case 0: return DhcpOption::PAD;
    case 12: return DhcpOption::HOSTNAME;
    case 50: return DhcpOption::REQUESTED_IP;
    case 53: return DhcpOption::DHCP_MESSAGE_TYPE;
    case 54: return DhcpOption::DHCP_SERVER_IDENTIFIER;
    case 55: return DhcpOption::PARAMETER_REQUEST_LIST;
    case 57: return DhcpOption::DHCP_MAX_SIZE;
    case 61: return DhcpOption::CLIENT_ID;
    case 255: return DhcpOption::END;
    default: return DhcpOption::UNKNOWN;
  }
}
