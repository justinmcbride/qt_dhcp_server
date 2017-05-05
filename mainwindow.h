#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include "dhcp_server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  dhcp_server_t* m_server{ new dhcp_server_t( this ) };

private slots:
  void LogMessage( QString message );
  void slot_interface_changed( int index );
};

#endif // MAINWINDOW_H
