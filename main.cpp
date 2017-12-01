#include "window_main.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  WindowMain w;
  w.show();

  return a.exec();
}
