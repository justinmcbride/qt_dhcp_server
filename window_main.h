#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>

#include "dhcp_server.h"
#include "model_dhcp_log.h"

namespace Ui {
  class WindowMain;
}

class WindowMain : public QMainWindow
{
  Q_OBJECT

  public:
    WindowMain( QWidget* parent = nullptr );
    ~WindowMain();

  private:
    Ui::WindowMain* ui{ nullptr };
    dhcp_server_t* m_server{ new dhcp_server_t( this ) };

    model_dhcp_log_t* m_log{ new model_dhcp_log_t( this ) };

    const QRegExp re_ip_address{ "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$" };

  private slots:
    void slot_receivedRequest( dhcp_message_t dhcp_message );
    void slot_interface_changed( int index );
};
