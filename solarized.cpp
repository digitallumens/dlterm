#include "solarized.h"
#include <QHash>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollBar>
#include <QFrame>

/*
" base03    #002b36  8/4 brblack  234 #1c1c1c
" base02    #073642  0/4 black    235 #262626
" base01    #586e75 10/7 brgreen  240 #4e4e4e
" base00    #657b83 11/7 bryellow 241 #585858
" base0     #839496 12/6 brblue   244 #808080
" base1     #93a1a1 14/4 brcyan   245 #8a8a8a
" base2     #eee8d5  7/7 white    254 #d7d7af
" base3     #fdf6e3 15/7 brwhite  230 #ffffd7
" yellow    #b58900  3/3 yellow   136 #af8700
" orange    #cb4b16  9/3 brred    166 #d75f00
" red       #dc322f  1/1 red      160 #d70000
" magenta   #d33682  5/5 magenta  125 #af005f
" violet    #6c71c4 13/5 brmagenta 61 #5f5faf
" blue      #268bd2  4/4 blue      33 #0087ff
" cyan      #2aa198  6/6 cyan      37 #00afaf
" green     #859900  2/2 green     64 #5f8700
*/
solarized::solarized(QObject *parent) : QObject(parent)
{
  m_colorTable.insert(SOLAR_BASE_03, "#002b36");
  m_colorTable.insert(SOLAR_BASE_02, "#073642");
  m_colorTable.insert(SOLAR_BASE_01, "#586e75");
  m_colorTable.insert(SOLAR_BASE_00, "#657b83");
  m_colorTable.insert(SOLAR_BASE_0, "#839496");
  m_colorTable.insert(SOLAR_BASE_1, "#93a1a1");
  m_colorTable.insert(SOLAR_BASE_2, "#eee8d5");
  m_colorTable.insert(SOLAR_BASE_3, "#fdf6e3");
  m_colorTable.insert(SOLAR_YELLOW, "#b58900");
  m_colorTable.insert(SOLAR_ORANGE, "#cb4b16");
  m_colorTable.insert(SOLAR_RED, "#dc322f");
  m_colorTable.insert(SOLAR_MAGENTA, "#d33682");
  m_colorTable.insert(SOLAR_VIOLET, "#6c71c4");
  m_colorTable.insert(SOLAR_BLUE, "#268bd2");
  m_colorTable.insert(SOLAR_CYAN, "#2aa198");
  m_colorTable.insert(SOLAR_GREEN, "#859900");
}

void solarized::setTextColor(QString *text, solarizedColor_t color) {
  *text = QString("%1%2%3%4%5").arg("<font color=\"").arg(m_colorTable[color]).arg("\">").arg(*text).arg("<\\font>");
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
