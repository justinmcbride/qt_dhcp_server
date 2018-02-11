#pragma once

#include <QWidget>
#include <QSortFilterProxyModel>

#include "model_assignments.h"

namespace Ui {
  class FormAssignments;
}

class FormAssignments : public QWidget
{
  Q_OBJECT

  public:
    explicit FormAssignments( QWidget* parent = nullptr );
    ~FormAssignments();

  private:
    Ui::FormAssignments* ui{ nullptr };
    QSortFilterProxyModel* m_model_proxy{ new QSortFilterProxyModel( this ) };
    ModelAssignments* m_model_assignments{ new ModelAssignments( this ) };
};
