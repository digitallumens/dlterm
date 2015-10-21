#ifndef SOLARIZED_H
#define SOLARIZED_H

#include <QObject>
#include <QHash>

class QTextEdit;
class QLineEdit;
class QFrame;

class solarized : public QObject
{
  Q_OBJECT
public:
  enum solarizedColor {
    // background tones
    SOLAR_BASE_03, SOLAR_BASE_02, SOLAR_BASE_01, SOLAR_BASE_00,
    // content tones
    SOLAR_BASE_0, SOLAR_BASE_1,
    // background tones
    SOLAR_BASE_2, SOLAR_BASE_3,
    // accent colors
    SOLAR_YELLOW, SOLAR_ORANGE, SOLAR_RED, SOLAR_MAGENTA, SOLAR_VIOLET, SOLAR_BLUE, SOLAR_CYAN, SOLAR_GREEN
  };
  explicit solarized(QObject *parent = 0);
  static void setStyleSheetQLineEdit(QLineEdit *lineEdit);
  static void setStyleSheetQTextEdit(QTextEdit *textEdit);
  static void setStyleSheetQFrame(QFrame *frame);
  static void setTextColor(QString *text, solarizedColor color);

signals:
public slots: 
};

#endif // SOLARIZED_H
