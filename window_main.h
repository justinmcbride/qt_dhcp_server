#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>

#include "dhcp_server.h"

namespace Ui {
class MainWindow;
}

class WindowMain : public QMainWindow
{
  Q_OBJECT

public:
  explicit WindowMain(QWidget *parent = 0);
  ~WindowMain();

private:
  Ui::MainWindow* ui{ nullptr };
  dhcp_server_t* m_server{ new dhcp_server_t( this ) };
  QLabel* m_label_count{ new QLabel(this) };

  const QRegExp re_ip_address{ "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$" };

private slots:
  void LogMessage( QString message );
  void slot_interface_changed( int index );
};
