#include "form_dhcp_structure.h"
#include "ui_form_dhcp_structure.h"

FormDhcpStructure::FormDhcpStructure( dhcp_message_t request, QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::form_dhcp_structure )
{
  ui->setupUi( this );

  ui->le_operation->setText( toString( request.header.operation ) );
  ui->le_hw_type->setText( "ETHERNET" );
  ui->le_hw_address_length->setText( QString( "%0").arg( request.header.hardware_address_length ) );

}

FormDhcpStructure::~FormDhcpStructure()
{
  delete ui;
}
