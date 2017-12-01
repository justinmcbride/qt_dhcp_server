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
  Ui::MainWindow *ui;
  dhcp_server_t* m_server{ new dhcp_server_t( this ) };
  QLabel* m_label_count{ new QLabel(this) };

private slots:
  void LogMessage( QString message );
  void slot_interface_changed( int index );
};
