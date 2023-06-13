#include "dao.h"
#include "data.h"

#include<stdio.h>
#include <QFile>
#include "control.h"
#include "shapefile.h"
#include "locationvalues.h"
#include<qdebug.h>
#include<QPainter>
#include <regex>

MyData::MyData()
{

}

int MyData::import(QVector<Shape *> &shapes, QString name)
{
    Control::shape_list.clear();
    Control::filename = name;
    load(name);
}

int MyData::create(QVector<Shape *> &shapes, QString name)
{
    Control::shape_list.clear();
    Control::filename = name;
    LocationValues::board->update();
}



int FileData::save(QVector<Shape*> &shapes, QString name)
{
    std::string str = name.toStdString();
    FILE* fp = fopen(str.c_str(),"w");

    if(fp == NULL)
        return 0;
    int len = shapes.size();
    fwrite(&len,sizeof(int),1,fp);
    for(int i=0;i<shapes.size();i++)
    {
        fwrite(shapes[i]->info.file_id,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fwrite(shapes[i]->info.name,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fwrite(shapes[i]->info.type,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fwrite(shapes[i]->info.context,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fwrite(&shapes[i]->info.len,sizeof(int),1,fp);
        fwrite(&shapes[i]->info.font_size,sizeof(int),1,fp);
        fwrite(&shapes[i]->info.line_size,sizeof(int),1,fp);
        fwrite(&shapes[i]->info.line_color,sizeof(QColor),1,fp);
        fwrite(&shapes[i]->info.fill_color,sizeof(QColor),1,fp);
        for(int j=0;j<shapes[i]->info.points.size();j++)
        {
            fwrite(&shapes[i]->info.points[j],sizeof(QPointF),1,fp);
        }
    }

    fclose(fp);
    Control::filename = name;
    return 1;
}

int FileData::read(QVector<Shape*> &shapes, QString name)
{
    std::string str = name.toStdString();
    FILE* fp = fopen(str.c_str(),"r");
    if(fp == NULL)
        return 0;
    int len = 0;
    qDebug()<<str.c_str();
    fread(&len,sizeof(int),1,fp);
    Control::clear();
    qDebug()<<"FileData";
    qDebug()<<"len: "<<len;
    for(int i=0;i<len;i++)
    {
        Info info;
        fread(info.file_id,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fread(info.name,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fread(info.type,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fread(info.context,sizeof(char)*MAX_CHAR_LEN,1,fp);
        fread(&info.len,sizeof(int),1,fp);
        fread(&info.font_size,sizeof(int),1,fp);
        fread(&info.line_size,sizeof(int),1,fp);
        fread(&info.line_color,sizeof(QColor),1,fp);
        fread(&info.fill_color,sizeof(QColor),1,fp);

        for(int j=0;j<info.len;j++)
        {
            QPointF p;
            fread(&p,sizeof(QPointF),1,fp);
            info.points.push_back(p);
            qDebug()<<p.x()<<" "<<p.y();
        }
        qDebug()<<info.type<<"  "<<len;
        QString t = info.type;
        Shape* shape = NULL;
        if(t== "点")
        {
            shape = new MyPoint();
        }
        else  if(t== "折线")
        {
            shape = new MyrokenLine();
        }
        else  if(t == "多边形")
        {
            shape = new MyPolygonal();
        }
        else  if(t == "矩形")
        {
            shape = new MyRect();
        }
        else  if(t == "圆")
        {
            shape = new MyCircle();
        }
        else  if(t == "扇形")
        {
            shape = new MyArc();
        }
        if(shape == NULL)
            continue;
        shape->info = info;
        shapes.push_back(shape);
    }
    fclose(fp);
    Control::filename = name;
    return 1;
}

int DatabaseData::save(QVector<Shape *> &shapes, QString name)
{
    Dao dao;
    std::string str = name.toStdString();
    for(int i=0;i<shapes.size();i++)
    {
        strcpy(shapes[i]->info.file_id,str.c_str());
    }
    dao.save(shapes,name);

    return 0;
}

int DatabaseData::read(QVector<Shape *> &shapes, QString name)
{
    Dao dao;
    auto list = dao.read(name);
    Control::clear();
    qDebug()<<"读取到:"<<list.size();
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
        else if(t == "矩形"){
            shape = new MyRect();
        }
        else if(t == "圆"){
            shape = new MyCircle();
        }
        else if(t == "扇形")
        {
            shape = new MyArc();
        }
        if(shape == NULL)
            continue;
        shape->info = list[i];
        shapes.push_back(shape);
    }
}


int GeometryData::save(QVector<Shape *> &shapes, QString name)
{

}

int GeometryData::read(QVector<Shape *> &shapes, QString name)
{
    std::string str = name.toStdString();
    QFile fp(name);
    if(!fp.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"file is null";
        return 0;
    }
    Control::clear();
    QTextStream stream(&fp);
    stream.setCodec("UTF-8");
    QString line = stream.readLine();
    QStringList items;
    int rectCount=0,circleCount=0,pointCount=0,arcCount=0,lineCount=0,polygonCount=0,count;
    while(!stream.atEnd())
    {

        QVector<QPointF> points;
        Info info;
        line = stream.readLine();
        items = line.split(" ");
        QString type = items[0];
        int len = 0; //点数
        int steps = (type == "扇形" ? 7:5); //数组长度差值
        qDebug()<<type;

        for(int i=1;i<items.size()-steps;i+=2)
        {
            QPoint point(items[i].toInt(),items[i+1].toInt());
            points.append(point);
            len++;
        }
        int context_index = len*2+1;
        int fontSize_index = len*2+2;
        int lineSize_index = len*2+3;
        int lineColor_index = len*2+4;
        int fillColor_index = len*2+5;

        if(type == "矩形")
        {
            count = ++rectCount;
        }
        else if(type == "圆")
        {
            count = ++circleCount;
        }
        else if(type == "点")
        {
            count = ++pointCount;
        }
        else if(type == "扇形")
        {
            count = ++arcCount;
            int starAngel_index = len*2+6;
            int spanAngle_index = len*2+7;
            std::string startAngel = items[starAngel_index].toStdString();
            info.startAngel = std::stoi(startAngel)*16;
            std::string spanAngel = items[spanAngle_index].toStdString();
            info.spanAngel = std::stoi(spanAngel)*16;
        }
        else if(type == "折线")
        {
            count = ++lineCount;
        }
        else if(type == "多边形")
        {
            count = ++polygonCount;
        }



        const char* strfile_id = str.c_str();
        strcpy(info.file_id,strfile_id);

        std::string strName = type.toStdString()+"_"+std::to_string(count);
        const char* strname = strName.c_str();
        strcpy(info.name,strname);


        const char* strtype = type.toStdString().c_str();
        strcpy(info.type,strtype);

        std::string strContext = items[context_index].toStdString();
        const char* strcontext = strContext.c_str();
        strcpy(info.context,strcontext);

        info.len=len;

        std::string strfont_Size = items[fontSize_index].toStdString();
        int font_Size = std::stoi(strfont_Size);
        info.font_size = font_Size;

        std::string strline_Size = items[lineSize_index].toStdString();
        int line_Size = std::stoi(strline_Size);
        info.line_size = line_Size;

        std::regex pattern(R"(QColor\s*\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))");
        std::smatch match;

        std::string strline_Color = items[lineColor_index].toStdString();
        if(std::regex_match(strline_Color,match,pattern)&&match.size()==5)
        {
            int r = std::stoi(match[1]);
            int g = std::stoi(match[2]);
            int b = std::stoi(match[3]);
            int a = std::stoi(match[4]);
            info.line_color = QColor(r,g,b,a);
        }


        std::string strfill_Color = items[fillColor_index].toStdString();
        if(std::regex_match(strfill_Color, match, pattern)&&match.size() == 5)
        {
           int r = std::stoi(match[1]);
           int g = std::stoi(match[2]);
           int b = std::stoi(match[3]);
           int a = std::stoi(match[4]);
           info.fill_color = QColor(r,g,b,a);
        }

        info.points = points;

        Shape* shape = NULL;
        if(type== "矩形")
        {
           shape = new MyRect();
        }
        else if(type == "圆")
        {
            shape = new MyCircle();
        }
        else if(type == "点")
        {
            shape = new MyPoint();
        }
        else if(type == "扇形")
        {
            shape = new MyArc();
        }
        else if(type == "折线")
        {
            shape = new MyrokenLine();
        }
        else if(type == "多边形")
        {
            shape = new MyPolygonal();
        }
        shape->info = info;

        shapes.push_back(shape);

    }
    fp.close();
    Control::filename = name;
    return 1;

}
