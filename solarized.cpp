#include "solarized.h"
#include <QHash>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollBar>
#include <QFrame>

solarized::solarized(QObject *parent) : QObject(parent){}

void solarized::setTextColor(QString *text, solarizedColor color) {
  QHash <solarizedColor, QString> colorTable;
  colorTable.insert(SOLAR_BASE_03, "#002b36");
  colorTable.insert(SOLAR_BASE_02, "#073642");
  colorTable.insert(SOLAR_BASE_01, "#586e75");
  colorTable.insert(SOLAR_BASE_00, "#657b83");
  colorTable.insert(SOLAR_BASE_0, "#839496");
  colorTable.insert(SOLAR_BASE_1, "#93a1a1");
  colorTable.insert(SOLAR_BASE_2, "#eee8d5");
  colorTable.insert(SOLAR_BASE_3, "#fdf6e3");
  colorTable.insert(SOLAR_YELLOW, "#b58900");
  colorTable.insert(SOLAR_ORANGE, "#cb4b16");
  colorTable.insert(SOLAR_RED, "#dc322f");
  colorTable.insert(SOLAR_MAGENTA, "#d33682");
  colorTable.insert(SOLAR_VIOLET, "#6c71c4");
  colorTable.insert(SOLAR_BLUE, "#268bd2");
  colorTable.insert(SOLAR_CYAN, "#2aa198");
  colorTable.insert(SOLAR_GREEN, "#859900");
  *text = QString("%1%2%3%4%5").arg("<font color=\"").arg(colorTable[color]).arg("\">").arg(*text).arg("<\\font>");
}

void solarized::setStyleSheetQLineEdit(QLineEdit *lineEdit) {
  QString lineEditQss = "QLineEdit {"
                        "font-family: Consolas;"
                        "font-style: normal;"
                        "font-size: 14pt;"
                        "font-weight: bold;"
                        "border: 0px;"
                        "border-radius: 0px;"
                        "padding: 0px;"
                        "background-color: #fdf6e3;"
                        "selection-background-color: #fdf6e3;"
                        "color: #586e75;"
                        "selection-color: #93a1a1;"
                        "}";
  lineEdit->setStyleSheet(lineEditQss);
}

void solarized::setStyleSheetQTextEdit(QTextEdit *textEdit) {
  QString textEditQss = "QTextEdit {"
                        "font-family: Consolas;"
                        "font-style: normal;"
                        "font-size: 14pt;"
                        "font-weight: bold;"
                        "border: 0px;"
                        "border-radius: 0px;"
                        "padding: 0px;"
                        "background-color: #fdf6e3;"
                        "color: #073642;"
                        "}";
  QString scrollBarQss = "QScrollBar {"
                         "background-color: #fdf6e3;"
                         "}"
                         "QScrollBar::handle {"
                         "background: #eee8d5;"
                         "}";
  textEdit->setStyleSheet(textEditQss);
  textEdit->verticalScrollBar()->setStyleSheet(scrollBarQss);
}

void solarized::setStyleSheetQFrame(QFrame *frame) {
  QString frameQss = "QFrame {"
                     "color: #93a1a1;"
                     "}";
  frame->setStyleSheet(frameQss);
}
