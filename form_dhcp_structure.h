#pragma once

#include <QDialog>

#include "dhcp_message.h"

namespace Ui {
 class form_dhcp_structure;
}

class FormDhcpStructure : public QDialog
{
  Q_OBJECT

  public:
    explicit FormDhcpStructure( dhcp_message_t request, QWidget* parent = nullptr );
    ~FormDhcpStructure();

  private:
    Ui::form_dhcp_structure* ui{ nullptr };
};
