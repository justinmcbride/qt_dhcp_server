#include "form_dhcp_structure.h"
#include "ui_form_dhcp_structure.h"

form_dhcp_structure::form_dhcp_structure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_dhcp_structure)
{
    ui->setupUi(this);
}

form_dhcp_structure::~form_dhcp_structure()
{
    delete ui;
}
