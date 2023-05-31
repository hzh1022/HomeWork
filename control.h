#ifndef CONTROL_H
#define CONTROL_H

#include "shape.h"
#include "tools.h"

#include <QVector>



class Control
{
public:

    static QVector<Shape*> shape_list;
    static QString filename;

    virtual Shape* create_shape(Config &config,int is_add_list = 0) = 0;
    virtual Shape* move_shape(QString name,int m_x,int m_y) = 0;
    virtual Shape* rotate_shape(QString name,int r) = 0;
    virtual Shape* resize_shape(QString name,double x) = 0;
    virtual int delete_shape(QString namex) = 0;

    Shape* findShapeByName(QString name);
    static void clear();
};

class GuiControl:public Control
{
public:
    Shape* create_shape(Config &config,int is_add_list = 0);
    Shape* move_shape(QString name,int m_x,int m_y);
    Shape* rotate_shape(QString name,int r);
    Shape* resize_shape(QString name,double x);
    int delete_shape(QString namex);

};


#endif // CONTROL_H
