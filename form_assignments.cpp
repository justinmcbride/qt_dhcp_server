#include "form_assignments.h"
#include "ui_form_assignments.h"

FormAssignments::FormAssignments( QWidget* parent ) :
  QWidget( parent ),
  ui( new Ui::FormAssignments )
{
  ui->setupUi( this );

  m_model_proxy->setSourceModel( m_model_assignments );
  ui->table->setModel( m_model_proxy );
}

FormAssignments::~FormAssignments()
{
  delete ui;
}
