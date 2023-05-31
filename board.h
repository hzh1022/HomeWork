#ifndef BOARD_H
#define BOARD_H

#include "control.h"

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class Board;
}

class Board : public QWidget
{
    Q_OBJECT

    int model;
    QVector<QPointF> list;
    QVector<Shape*> selected;
    Control *control;
    Shape* temp;

public:
    explicit Board(QWidget *parent = nullptr);
    ~Board();
protected:
    //鼠标点击事件
    void mousePressEvent(QMouseEvent* event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);


private:
    Ui::Board *ui;
};

#endif // BOARD_H
