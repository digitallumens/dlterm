#ifndef SOLARIZED_H
#define SOLARIZED_H

#include <QObject>
#include <QHash>

typedef enum {
  // background tones
  SOLAR_BASE_03, SOLAR_BASE_02, SOLAR_BASE_01, SOLAR_BASE_00,
  // content tones
  SOLAR_BASE_0, SOLAR_BASE_1,
  // background tones
  SOLAR_BASE_2, SOLAR_BASE_3,
  // accent colors
  SOLAR_YELLOW, SOLAR_ORANGE, SOLAR_RED, SOLAR_MAGENTA, SOLAR_VIOLET, SOLAR_BLUE, SOLAR_CYAN, SOLAR_GREEN
} solarizedColor_t;

class solarized : public QObject
{
  Q_OBJECT
public:
  explicit solarized(QObject *parent = 0);
  void setColor(QString *text, solarizedColor_t color);

signals:

public slots:

private:
  QHash <solarizedColor_t, QString> m_colorTable;
};

#endif // SOLARIZED_H
