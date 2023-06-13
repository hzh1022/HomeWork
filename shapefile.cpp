#include "control.h"
#include "shape.h"
#include "shapefile.h"
#include<qdebug.h>

ostream &operator<<(ostream &out, BBox &box)
{
    out << "(" << box.xmin << ",";
    out << box.xmax << ",";
    out << box.ymin << ",";
    out << box.ymax << ")";

    return out;
}

bool Shapefile::Open(const char *fname)
{
    ifs.open(fname, ios::binary);
    if (!ifs)
        return false;

    ifs.seekg(0);
    if (GetBigInt() != 9994)
        return false;

    ifs.seekg(24);
    int size = GetBigInt() * 2;

    ifs.seekg(0, ios::end);
    int size0 = ifs.tellg();
    if (size != size0)
        return false;

    return true;
}

int Shapefile::GetShapeType()
{
    ifs.seekg(32);
    this->shapeType = GetLittleInt();
    return this->shapeType;
}

string Shapefile::GetShapeTypeString(int shapeType)
{
    string shapeTypeString;
    switch (shapeType)
    {
    case 1:
        shapeTypeString = "Point";
        break;
    case 2:
        shapeTypeString = "PolyLine";
        break;
    case 3:
        shapeTypeString = "Polygon";
        break;
    }
    return shapeTypeString;
}

bool Shapefile::GetBoundingBox(BBox &box)
{
    ifs.seekg(36);
    box.xmin = GetDouble();
    ifs.seekg(44);
    box.ymin = GetDouble();

    ifs.seekg(52);
    box.xmax = GetDouble();
    ifs.seekg(60);
    box.ymax = GetDouble();
    return true;
}

int load(QString name)
{
    Shapefile shp;
    name.replace("/","\\");
    if (!shp.Open(name.toStdString().c_str()))
    {
        qDebug()<<"打开失败"<<name;
        return 0;
    }

    BBox box;
    shp.GetBoundingBox(box);

    qDebug() << "xmin"<<QString::number(box.xmin,'f',10) << "xmax" << QString::number(box.xmax,'f',10)
             << "ymin"<<QString::number(box.ymin,'f',10) << "ymax" << QString::number(box.ymax,'f',10);

    int count = 0;
    QVector<Info> list;

    for (Shapefile::iterator it = shp.begin(); it < shp.end(); it++)
    {
        count++;
        Geometry *pt = *it;
        QVector<Info> info =  pt->wkt();
        list.append(info);
        delete pt;
    }
    qDebug()<<"count:"<<count;
    qDebug()<<"list.size:"<<list.size();

    Control::clear();
    for(int i=0;i<list.size();i++)
    {
        QString t = list[i].type;
        Shape* shape = NULL;
        if(t== "点")
        {
            shape = new MyPoint();
        }
        else if(t== "折线")
        {
            shape = new MyrokenLine();
        }
        else if(t == "多边形")
        {
            shape = new MyPolygonal();
        }
        else if(t == "矩形")
        {
            shape = new MyRect();
        }
        else  if(t == "圆")
        {
            shape = new MyCircle();
        }
        else  if(t == "弧")
        {
            shape = new MyArc();
        }
        if(shape == NULL)
            continue;
        shape->info = list[i];
        Control::shape_list.push_back(shape);
    }

}
