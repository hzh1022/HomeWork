#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <qpoint.h>


class Config{
public:
    static int fileModel;   //文件模式 1 - 3
    static int model;       //1创建  2选择
    static int lineSize;
    static int fontSize;
    //static int r;
    static QColor lineColor;
    static QColor fillColor;    //绘制属性
    static QString shapeName;       //当前的图形
    static QVector<QPointF> points;
    static int isClear;
    static int startAngel;
    static int spanAngel;
};



#endif // CONFIG_H
