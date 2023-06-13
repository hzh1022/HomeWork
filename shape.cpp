#include "shape.h"

#include<iostream>
#include<string>
#include <QtMath>
#include<QDebug>
using namespace std;


void Shape::setAtta(Config& config, QString type)
{
    this->info.len = config.points.size();
    this->info.font_size = config.fontSize;
    this->info.line_size = config.lineSize;
    this->info.fill_color = config.fillColor;
    this->info.line_color = config.lineColor;
    for(int i=0;i<config.points.size();i++)
    {
        this->info.points.push_back(config.points[i]);
    }
    memset(this->info.context,0,MAX_CHAR_LEN);
    if(type == "矩形")
    {
        this->info.len = 4;
        this->info.points.clear();
        this->info.points.push_back(config.points[0]);
        this->info.points.push_back( QPointF(config.points[0].x(),config.points[1].y()));
        this->info.points.push_back( config.points[1]);
        this->info.points.push_back(QPointF(config.points[1].x(),config.points[0].y()));
    }
    if(type=="扇形")
    {
        this->info.points.push_back(QPointF());
        this->info.startAngel = Config::startAngel;
        this->info.spanAngel = Config::spanAngel;

        this->info.len = 2;
    }
}

void Shape::setAtta(QPainter &board)
{
    QFont font = board.font();
    font.setPointSize(info.font_size);
    board.setFont(font);
    board.setPen(QPen(info.line_color,info.line_size));
    board.setBrush(QBrush(info.fill_color));

    //绘制shp文件
    if(Config::isShp==1)
    {
        board.setViewport(0, 0, 500, 500);
        board.setWindow(-5000000,-5000000,10000000,10000000);
        board.setRenderHint(QPainter::Antialiasing);
        board.setPen(QPen(Qt::red, 10000));
    }

}

void MyPoint::draw(QPainter &board)
{
    setAtta(board);
    if(info.points.size()>=1)
    {
        board.drawPoint(info.points[0].x(),info.points[0].y());
    }
}


void MyrokenLine::draw(QPainter &board)
{
    setAtta(board);
    for(int i=1;i<info.len;i++)
    {
        board.drawLine(info.points[i].x(),info.points[i].y(),info.points[i-1].x(),info.points[i-1].y());
    }
}


void MyPolygonal::draw(QPainter &board)
{
    setAtta(board);
    if(info.points.size()>=1)
    {
        board.drawPolygon(&info.points[0],info.len);
        for(int i=1;i<info.points.size();i++)
        {
            board.drawLine(info.points[i].x(),info.points[i].y(),info.points[i-1].x(),info.points[i-1].y());
        }
    }
}

void MyRect::draw(QPainter &board)
{
    setAtta(board);
    if(info.points.size()>=1)
    {
        board.drawPolygon(&info.points[0],info.len);
        for(int i=1;i<info.len;i++){
            board.drawLine(info.points[i].x(),info.points[i].y(),info.points[i-1].x(),info.points[i-1].y());
        }
    }
}

void MyCircle::draw(QPainter &board)
{
    if(info.points.size()>=2)
    {
        setAtta(board);
        int dx = info.points[1].x()-info.points[0].x();
        int dy = info.points[1].y()-info.points[0].y();
        int r = qSqrt(dx*dx + dy*dy);
        QRectF circle(info.points[0].x(),info.points[0].y(),r,r);
        board.drawEllipse(circle);
    }
}

void MyArc::draw(QPainter &board)
{
    if(info.points.size()>=2)
    {
        setAtta(board);
        board.drawPie(info.points[0].x(),info.points[0].y(),abs(info.points[1].x()-info.points[0].x()),abs(info.points[0].y()-info.points[1].y()),info.startAngel,info.spanAngel);
    }
}
