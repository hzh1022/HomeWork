#include "widget.h"
#include "ui_widget.h"
#include "tools.h"
#include "board.h"

#include <QColorDialog>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

#include "locationvalues.h"

Widget::Widget(QWidget *parent): QWidget(parent),ui(new Ui::Widget)
{
   ui->setupUi(this);

   tools =  new Tools(this);
   tools->move(0,0);
   board = new Board(this);
   board->move(0,80);

   LocationValues::board = board;

}

Widget::~Widget()
{
    delete ui;
}



