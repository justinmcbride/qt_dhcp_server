#include <QNetworkInterface>
#include <QPushButton>
#include <QTableView>

#include "window_main.h"
#include "ui_window_main.h"

#include "window_fake_client.h"
#include "form_dhcp_structure.h"

WindowMain::WindowMain( QWidget* parent ) :
  QMainWindow( parent ),
  ui( new Ui::WindowMain )
{
  ui->setupUi( this );
  connect( m_server, &dhcp_server_t::signal_receivedRequest, this, &WindowMain::slot_receivedRequest );
  connect( ui->cb_interface, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WindowMain::slot_interface_changed );

  ui->table_log->setModel( m_log );

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

  connect( ui->table_log, &QTableView::doubleClicked, this, &WindowMain::slot_dclicked_item );
}

void WindowMain::slot_dclicked_item( const QModelIndex& item )
{
//  auto true_index = m_log->mapToSource( item );
  auto request = m_log->GetItem( item.row() );
  auto window = new FormDhcpStructure( request, this );
  window->setModal( true );
  window->setAttribute( Qt::WA_DeleteOnClose, true );
  window->exec();
}

WindowMain::~WindowMain()
{
  delete ui;
}

void WindowMain::slot_receivedRequest( dhcp_message_t dhcp_message )
{
  m_log->AddRequest( dhcp_message );
}

void WindowMain::slot_interface_changed( int index )
{
  m_server->SetInterface( ui->cb_interface->itemData(index).toInt() );
}
