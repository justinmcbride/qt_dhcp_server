#pragma once

#include <cstdint>
#include <exception>

enum class DhcpOption {
  PAD = 0,
  HOSTNAME = 12,
  REQUESTED_IP = 50,
  DHCP_MESSAGE_TYPE = 53,
  PARAMETER_REQUEST_LIST = 55,
  DHCP_MAX_SIZE = 57,
  CLIENT_ID = 61,
  END = 255
};

static inline QString toString( DhcpOption op )
{
  if( op == DhcpOption::PAD ) return "PAD";
  else if( op == DhcpOption::HOSTNAME ) return "HOSTNAME";
  else if( op == DhcpOption::REQUESTED_IP ) return "REQUESTED_IP";
  else if( op == DhcpOption::DHCP_MESSAGE_TYPE ) return "DHCP_MESSAGE_TYPE";
  else if( op == DhcpOption::PARAMETER_REQUEST_LIST ) return "PARAMETER_REQUEST_LIST";
  else if( op == DhcpOption::DHCP_MAX_SIZE ) return "DHCP_MAX_SIZE";
  else if( op == DhcpOption::CLIENT_ID ) return "CLIENT_ID";
  else if( op == DhcpOption::END ) return "END";
  else return "UNKNOWN";
}

struct option_abc
{
  virtual QString representation() const = 0;
};

//template< uint8_t code, uint8_t length, QString representation >
//class option_tt : public option_abc
//{
//  private:
//    constexpr uint8_t m_length = length;
//    constexpr uint8_t m_code = code;
//    const QString m_string = representation;
//    std::array<uint8_t, N> m_data;
//  public:
//    option_tt( QByteArray& data )
//    {
//      uint8_t option_code = data.at( 0 );
//      data.remove( 0, 1 );

//      if( option_code != m_code )
//      {
//        qDebug() << "some problem occured";
//        throw std::invalid_argument( "code is not same" );
//      }

//      uint8_t option_length = data.at( 0 );
//      data.remove( 0, 1 );

//      for( int i = 0; i < m_length; i++ )
//      {
//        m_data[i] = data.at( 0 );
//        data.remove( 0, 1 );
//      }
//    }

//    virtual QString representation() const override { return m_string; }
//};

//template< uint8_t code, 0, QString representation >
//class option_tt : public option_abc
//{
//  private:
//    constexpr uint8_t m_length = length;
//    constexpr uint8_t m_code = code;
//    const QString m_string = representation;
//  public:
//    option_tt( QByteArray& data )
//    {
//      uint8_t option_code = data.at( 0 );
//      data.remove( 0, 1 );

//      if( option_code != m_code )
//      {
//        qDebug() << "some problem occured";
//        throw std::invalid_argument( "code is not same" );
//      }
//    }

//    virtual QString representation() const override { return m_string; }
//};

//typedef option_tt<0, 0, "Padding"> option_padding;
//typedef option_tt<0, 0, "Padding"> option_hostname;

//option_abc* parseOption( QByteArray& data )
//{
//  uint8_t code = data.at( 0 );
//  if( code == 0 )   return option_padding( data );
//}

static DhcpOption getOptionType( uint8_t val )
{
  switch( val )
  {
    case 0:
      return DhcpOption::PAD;
    case 12:
      return DhcpOption::HOSTNAME;
    case 50:
      return DhcpOption::REQUESTED_IP;
    case 53:
      return DhcpOption::DHCP_MESSAGE_TYPE;
    case 55:
      return DhcpOption::PARAMETER_REQUEST_LIST;
    case 57:
      return DhcpOption::DHCP_MAX_SIZE;
    case 61:
      return DhcpOption::CLIENT_ID;
    case 255:
      return DhcpOption::END;
    default:
      throw std::out_of_range( "Bad option : " + std::to_string(val) );
  }
}
