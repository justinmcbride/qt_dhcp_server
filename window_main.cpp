#include "window_main.h"
#include "ui_window_main.h"

#include "window_fake_client.h"

#include <QNetworkInterface>
#include <QPushButton>

WindowMain::WindowMain( QWidget* parent ) :
  QMainWindow( parent ),
  ui( new Ui::MainWindow )
{
  ui->setupUi( this );
  connect( m_server, &dhcp_server_t::LogMessage, this, &WindowMain::LogMessage );
  connect( ui->cb_interface, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WindowMain::slot_interface_changed );

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
  }

  ui->le_router->setValidator( new QRegExpValidator( re_ip_address ) );

  connect( ui->menu_action_client, &QAction::triggered, [=] {
    auto window = new WindowFakeClient( this );
    window->setModal( true );
    window->setAttribute( Qt::WA_DeleteOnClose, true );
    window->exec();
  } );

  connect( ui->button_server_state, &QPushButton::toggled , [=] (bool checked) {
    auto router = ui->le_router->text();
    m_server->SetRouter( QHostAddress(router) );

    bool server_state = m_server->SetState( checked );
    if( server_state )
    {
      ui->button_server_state->setText( "Server is ON" );
    }
    else
    {
      ui->button_server_state->setText( "Server is OFF" );
    }
  } );

  ui->statusBar->addWidget( m_label_count );
}

WindowMain::~WindowMain()
{
  delete ui;
}

void WindowMain::LogMessage( QString message )
{
  static int count = 0;
  count++;
  ui->te_log->append( message );
  m_label_count->setText( QString("Messages: %1").arg(count) );
}

void WindowMain::slot_interface_changed( int index )
{
  m_server->SetInterface( ui->cb_interface->itemData(index).toInt() );
}
