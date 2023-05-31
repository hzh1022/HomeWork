#ifndef DATA_H
#define DATA_H

#include "shape.h"

#include <QVector>

class MyData
{
public:
    MyData();
    virtual  ~MyData(){};

public:
    virtual int save(QVector<Shape*> &shapes, QString name) = 0;
    virtual int read(QVector<Shape*> &shapes, QString name) = 0;
    int import(QVector<Shape*> &shapes, QString name);
    int create(QVector<Shape*> &shapes, QString name);
};

class FileData:public MyData
{

public:
    int save(QVector<Shape*> &shapes, QString name);
    int read(QVector<Shape*> &shapes, QString name);
    int import(QVector<Shape*> &shapes, QString name);
    int create(QVector<Shape*> &shapes, QString name);
};


class DatabaseData:public MyData
{

public:

    int save(QVector<Shape*> &shapes, QString name) ;
    int read(QVector<Shape*> &shapes, QString name) ;
};

class GeometryData:public MyData
{
public:
    int read(QVector<Shape*> &shapes, QString name);
    int save(QVector<Shape*> &shapes, QString name);
};


#endif // DATA_H
