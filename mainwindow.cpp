#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect( m_server, &dhcp_server_t::LogMessage, this, &MainWindow::LogMessage );
  connect( ui->cb_interface, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::slot_interface_changed );

  auto interfaces = QNetworkInterface::allInterfaces();
  for( auto interface : interfaces )
  {
    auto flags = interface.flags();
    bool use_this = ( flags.testFlag(QNetworkInterface::IsUp) && flags.testFlag(QNetworkInterface::CanBroadcast) && !flags.testFlag(QNetworkInterface::IsLoopBack) );
    if( use_this && !interface.addressEntries().isEmpty() )
    {
      auto name = QString( "%1 (%2)" ).arg(interface.humanReadableName()).arg(interface.addressEntries().back().ip().toString() );
      ui->cb_interface->addItem( name, interface.index() );
    }
  } //m_server->setup();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::LogMessage(QString message)
{
  ui->te_log->append( message );
}

void MainWindow::slot_interface_changed(int index)
{
  m_server->setup( ui->cb_interface->itemData(index).toInt() );
}
