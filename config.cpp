#include "config.h"
#include "control.h"

#include <QVector>

int Config::fileModel = 1;
int Config::model = 1;
int Config::lineSize = 1;
int Config::fontSize = 1;
QColor Config::lineColor = Qt::black;
QColor Config::fillColor = Qt::white;
QString Config::shapeName = "矩形";
QVector<QPointF> Config::points = QVector<QPointF>();
int Config::startAngel = 0;
int Config::spanAngel = 180*16;

int Config::isClear = 0;
int Config::isShp = 0;

