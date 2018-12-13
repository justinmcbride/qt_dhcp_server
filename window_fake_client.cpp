#include "window_fake_client.h"
#include "ui_window_fake_client.h"

WindowFakeClient::WindowFakeClient( QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::WindowFakeClient )
{
  ui->setupUi( this );
}

WindowFakeClient::~WindowFakeClient()
{
  delete ui;
}
