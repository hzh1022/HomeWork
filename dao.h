#ifndef DAO_H
#define DAO_H

#include "shape.h"

class Dao
{
public:
    Dao();
    QVector<Info> read(QString name);
    void save(QVector<Shape*> l,QString name);

};

#endif // DAO_H
