#ifndef FORM_DHCP_STRUCTURE_H
#define FORM_DHCP_STRUCTURE_H

#include <QWidget>

namespace Ui {
class form_dhcp_structure;
}

class form_dhcp_structure : public QWidget
{
    Q_OBJECT

public:
    explicit form_dhcp_structure(QWidget *parent = nullptr);
    ~form_dhcp_structure();

private:
    Ui::form_dhcp_structure *ui;
};

#endif // FORM_DHCP_STRUCTURE_H
