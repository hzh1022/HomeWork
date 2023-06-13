#include "board.h"
#include "control.h"
#include "tools.h"
#include "ui_board.h"

#include<qdebug.h>
#include <mutex>
#include<QPainter>


std::mutex mutex;

Board::Board(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Board)
{
    ui->setupUi(this);
    model = 1;
    control = new GuiControl();
    temp = NULL;

}

Board::~Board()
{
    delete ui;
    delete control;
}

void Board::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    for(Shape *p:control->shape_list)
    {
        p->draw(painter);
    }
    mutex.lock();
    if(temp)
        temp->draw(painter);
    mutex.unlock();

    if(Config::isClear == 1)
    {
        control->clear();
        Config::isClear = 0;
    }


}

void Board::mousePressEvent(QMouseEvent *event)
{
    Config config;
    if(Config::model == 1)
    {
        if(Config::shapeName != "多边形" && Config::shapeName != "折线" )
        {
            list.clear();
            Config::points.clear();
            list.push_back(QPointF(event->x(),event->y()));
        }
        else
        {
            if(event->button() == Qt::LeftButton)
            {
                list.push_back(QPointF(event->x(),event->y()));
                setMouseTracking(true);
            }
            else
            {
                Config::points = list;
                if(list.size() > 2)
                {
                    control->create_shape(config,1);
                }
                list.clear();
                delete temp;
                temp = NULL;
                setMouseTracking(false);
            }
        }
    }
    else if(Config::model == 2)
    {

        if(selected.size() == 0)
        {
            selected.clear();
            list.clear();
        }
    }
    update();

}

void Board::mouseMoveEvent(QMouseEvent *event)
{
    Config config;
    if(config.model == 1)
    {
        mutex.lock();
        if(temp != NULL)
        {
            delete temp;
            temp = NULL;
        }
        Config::points = list;
        if(Config::shapeName != "多边形" && Config::shapeName != "折线"  )
        {
            Config::points.push_back(QPointF(event->x(),event->y()));
            temp = control->create_shape(config);
        }
        else
        {
            if(Config::points.size()>0)
            {
               Config::points.push_back(QPointF(event->x(),event->y()));
            }
            temp = control->create_shape(config);
            if(Config::points.size()>0)
            {
                Config::points.pop_back();
            }
        }
        mutex.unlock();
    }
    update();
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    Config config;
    if(event->button()==Qt::LeftButton)
    {
        if(Config::model == 1)
        {
            if(Config::shapeName != "多边形" &&Config::shapeName != "折线")
            {
                Config::points = list;
                if(Config::shapeName != "点")
                {
                    Config::points.push_back(QPointF(event->x(),event->y()));
                }
                control->create_shape(config,1);
                list.clear();
                Config::points.clear();
                delete temp;
                temp = NULL;
                qDebug()<<control->shape_list.size();
            }
            update();

        }
        else if(Config::model == 2)
        {
            if(selected.size() == 0)
            {

            }
        }
    }



}

