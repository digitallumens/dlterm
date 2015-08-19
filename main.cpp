#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  // load application stylesheet
  QFile styleFile("/Users/mshaffer/Source/dlterm/solarized-dark.qss");
  styleFile.open(QFile::ReadOnly);
  QString style(styleFile.readAll());
  a.setStyleSheet(style);
  // set application font
  QFont appFont("Consolas", 14, QFont::Bold, false);
  a.setFont(appFont);
  MainWindow w;
  w.show();
  return a.exec();
}
