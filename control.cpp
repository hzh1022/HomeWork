#include "control.h"

QVector<Shape*> Control::shape_list = QVector<Shape*>();
QString Control::filename = "";




Shape *GuiControl::create_shape(Config &config,int is_add_list)
{
    Shape *shape = NULL;
    int n = 1;
    std::string str;
    if(config.shapeName == "点")
    {
        shape = new MyPoint();
        shape->setAtta(config,config.shapeName);
    }
    else if(config.shapeName == "折线")
    {
        shape = new MyrokenLine();
        shape->setAtta(config,config.shapeName);
    }
    else if(config.shapeName == "多边形")
    {
        shape = new MyPolygonal();
        shape->setAtta(config,config.shapeName);
    }
    else if(config.shapeName == "矩形")
    {
        shape = new MyRect();
        shape->setAtta(config,config.shapeName);
    }
    else if(config.shapeName == "圆")
    {
        shape = new MyCircle();
        shape->setAtta(config,config.shapeName);
    }
    else if(config.shapeName == "扇形")
    {
        shape = new MyArc();
        shape->setAtta(config,config.shapeName);
    }
    else
    {
        return NULL;
    }
    memset(shape->info.context,0,MAX_CHAR_LEN);
    str = config.shapeName.toStdString();
    strcpy_s(shape->info.type,str.c_str());
    while (1)
    {
        QString name = config.shapeName + "_" + QString::number(n);
        int flag = 1;
        for(Shape *p:shape_list)
        {
            if(p->info.name == name)
            {
                flag = 0;
                break;
            }
        }

        if(flag)
        {
            str = name.toStdString();
            strcpy_s(shape->info.name,str.c_str());
            break;
        }

        n++;
    }
    str = filename.toStdString();
    strcpy(shape->info.file_id,str.c_str());
    if(is_add_list)
        shape_list.push_back(shape);
    return shape;
}

Shape *GuiControl::move_shape(QString name, int m_x, int m_y)
{
    Shape *shape = findShapeByName(name);
    int n = 1;
    if(shape == NULL)
        return NULL;

    for(int i=0;i<shape->info.len;i++)
    {
        shape->info.points[i].setX(shape->info.points[i].x() + m_x);
        shape->info.points[i].setY(shape->info.points[i].y() + m_y);
    }

    return shape;
}

Shape *GuiControl::rotate_shape(QString name, int r)
{

}

Shape *GuiControl::resize_shape(QString name, double x)
{

}

int GuiControl::delete_shape(QString name)
{
    for(int i=0;i<shape_list.size();i++)
    {
        if(name == shape_list[i]->info.name)
        {
            shape_list.erase(shape_list.begin()+i);
            return 0;
        }
    }
    return 0;
}

Shape *Control::findShapeByName(QString name)
{
    for(int i=0;i<shape_list.size();i++)
    {
        if(name == shape_list[i]->info.name)
            return shape_list[i];
    }
    return NULL;
}

void Control::clear()
{
    for(int i=0;i<shape_list.size();i++)
    {
        Shape *p = shape_list[i];
        delete p;
    }
    shape_list.clear();
}
