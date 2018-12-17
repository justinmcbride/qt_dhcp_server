#pragma once

#include <QDialog>

namespace Ui {
  class WindowFakeClient;
}

class WindowFakeClient : public QDialog
{
  Q_OBJECT

  public:
    explicit WindowFakeClient( QWidget* parent = nullptr );
    ~WindowFakeClient();

  private:
    Ui::WindowFakeClient* ui{ nullptr };
};
