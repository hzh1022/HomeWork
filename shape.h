    #ifndef SHAPE_H
#define SHAPE_H

#include "config.h"

#include<QPainter>
#include<QPoint>

#include<iostream>
#include<vector>


#define MAX_CHAR_LEN 100

class Info{

public:
    char file_id[MAX_CHAR_LEN];    //属于那个文件
    char name[MAX_CHAR_LEN];       //这个图形的id
    char type[MAX_CHAR_LEN];       //图形类型
    char context[MAX_CHAR_LEN];       //图形包含的字符串
    QVector<QPointF> points;    //对应点的列表
    int len;            //点的数量
    int font_size;      //里面的文字大小
    int line_size;      //里面的文字大小
    QColor line_color;      //线条颜色
    QColor fill_color;      //填充颜色
    int startAngel; //扇形起始角度
    int spanAngel;  //扇形扫描角度

    Info()
    {
        font_size = Config::fontSize;
        line_size = Config::lineSize;
        fill_color = Config::fillColor;
        line_color = Config::lineColor;
        memset(file_id,0,MAX_CHAR_LEN);
        memset(name,0,MAX_CHAR_LEN);
        memset(type,0,MAX_CHAR_LEN);
        memset(context,0,MAX_CHAR_LEN);
    }
};

class Shape{
public:
    Info info;
public:
    virtual ~Shape(){};
    virtual void draw(QPainter &board)=0;
    void setAtta(Config& config, QString type);

    friend std::ostream &operator<<(std::ostream &output,const Shape &D )
    {
       output<< D.info.file_id << " "<< D.info.name << " "<< D.info.type << " "<< D.info.context << " "<< D.info.len << " "
             << D.info.font_size << " "<< D.info.line_color.value() << " "<< D.info.fill_color.value() << "\n";
       for(int i=0;i<D.info.len;i++)
           output<<D.info.points[i].x()<<" "<<D.info.points[i].y()<<"\n";
       return output;
    }

protected:
    void setAtta(QPainter &board);
};

//点、折线、多边形、矩形、圆、扇形

class MyPoint:public Shape{
public:
    void draw(QPainter &board);
};


class MyrokenLine:public Shape{
public:
    void draw(QPainter &board);
};


class MyPolygonal:public Shape{
public:
    void draw(QPainter &board);
};

class MyRect:public Shape{
public:
    void draw(QPainter &board);
};

class MyCircle:public Shape{
public:
    void draw(QPainter &board);
};

class MyArc:public Shape{
public:
    void draw(QPainter &board);
};


#endif // SHAPE_H
