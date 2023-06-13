#include "control.h"
#include "tools.h"
#include "ui_tools.h"
#include "locationvalues.h"
#include "widget.h"

#include <QColorDialog>
#include <QFileDialog>
#include <qdebug.h>
#include <QInputDialog>
#include<QMessageBox>

Tools::Tools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tools)
{
    ui->setupUi(this);
    QPalette pal = ui->pushButton->palette();
    pal.setColor(QPalette::Button,Qt::black);
    ui->pushButton->setPalette(pal);
    ui->pushButton->setAutoFillBackground(true);
    ui->pushButton->setFlat(true);

    pal = ui->pushButton_2->palette();
    pal.setColor(QPalette::Button,Qt::white);
    ui->pushButton_2->setPalette(pal);
    ui->pushButton_2->setAutoFillBackground(true);
    ui->pushButton_2->setFlat(true);

    ui->label_6->setVisible(0);
    ui->lineEdit_3->setVisible(0);
    ui->label_7->setVisible(0);
    ui->lineEdit_4->setVisible(0);

    data1 = new FileData();
    data2 = new DatabaseData();
    data3 = new GeometryData();
}

Tools::~Tools()
{
    delete ui;
}

//新建
void Tools::on_pushButton_3_clicked()
{
        QString fileName = QFileDialog::getSaveFileName(this,QStringLiteral("新建文件"),"",QStringLiteral(""));
        data1->create(Control::shape_list,fileName);
        Control::shape_list.clear();
        LocationValues::board->update();

}
//打开
void Tools::on_pushButton_4_clicked()
{
    Config::isShp = 0;
    if(Config::fileModel == 1)
    {
        QString fileName = QFileDialog::getOpenFileName(this,QStringLiteral("打开文件"),"",QStringLiteral(""));
        data1->read(Control::shape_list,fileName);
        LocationValues::board->update();
    }
    else if(Config::fileModel == 2)
    {
        bool ok;
        QLineEdit* nameLabel2 = new QLineEdit();
        QString text=QInputDialog::getText(this,tr("打开数据库文件"),tr("文件名:"),QLineEdit::Normal,nameLabel2->text(),&ok);
        if(ok && !text.isEmpty())
        {
            qDebug()<<"打开文件:"<<text;
            data2->read(Control::shape_list,text);
            LocationValues::board->update();
        }
        delete nameLabel2;
    }
}
//保存
void Tools::on_pushButton_5_clicked()
{
    if(Config::fileModel == 1)
    {
        QString fileName = Control::filename;
        if(fileName == "")
        {
            fileName = QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),"",QStringLiteral(""));
        }
        data1->save(Control::shape_list,fileName);
        LocationValues::board->update();
    }
    else if(Config::fileModel == 2)
    {
        bool ok;
        QLineEdit* nameLabel2 = new QLineEdit();
        QString text=QInputDialog::getText(this,tr("保存至数据库"),tr("文件名:"),QLineEdit::Normal,nameLabel2->text(),&ok);
        if(ok && !text.isEmpty())
        {
            qDebug()<<"保存文件:"<<text;
            data2->save(Control::shape_list,text);
        }
        delete nameLabel2;
    }
}
//导入
void Tools::on_pushButton_6_clicked()
{
    Config::isShp = 1;
    if(Config::fileModel==1)
    {
        QString fileName = QFileDialog::getOpenFileName(this,QStringLiteral("导入文件"),"",QStringLiteral(""));
        data1->import(Control::shape_list,fileName);
        LocationValues::board->update();
    }
}

//读取
void Tools::on_pushButton_8_clicked()
{
    Config::isShp = 0;
    QString fileName = QFileDialog::getOpenFileName(this,QStringLiteral("几何图形文件"),"",QStringLiteral(""));
    data3->read(Control::shape_list,fileName);
}



//线条颜色
void Tools::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    Config::lineColor = color;
    QPalette pal = ui->pushButton->palette();
    pal.setColor(QPalette::Button,color);
    ui->pushButton->setPalette(pal);
    ui->pushButton->setAutoFillBackground(true);
    ui->pushButton->setFlat(true);
}
//填充颜色
void Tools::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    Config::fillColor = color;
    QPalette pal = ui->pushButton_2->palette();
    pal.setColor(QPalette::Button,color);
    ui->pushButton_2->setPalette(pal);
    ui->pushButton_2->setAutoFillBackground(true);
    ui->pushButton_2->setFlat(true);
}
//线条粗细
void Tools::on_lineEdit_2_textChanged(const QString &arg1)
{
    try
    {
        int size = arg1.toInt();
        if(size>0)
            Config::lineSize = size;
    }
    catch (...)
    {

    }
}
//字体大小
void Tools::on_lineEdit_textChanged(const QString &arg1)
{
    try
    {
        int size = arg1.toInt();
        if(size>0)
            Config::fontSize = size;
    }
    catch (...)
    {

    }
}
//创建图形模式
void Tools::on_radioButton_clicked()
{
    Config::model = 1;
}
//选择图形模式
void Tools::on_radioButton_2_clicked()
{
    Config::model = 2;
}

void Tools::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
}

void Tools::on_comboBox_2_currentIndexChanged(int index)
{
    Config::fileModel = index + 1;
}

void Tools::on_comboBox_currentIndexChanged(const QString &arg1)
{
    Config::shapeName = arg1;

    Config::points.clear();

    if(arg1 == "扇形")
    {
        ui->label_6->setVisible(1);
        ui->lineEdit_3->setVisible(1);
        ui->label_7->setVisible(1);
        ui->lineEdit_4->setVisible(1);
    }
    else
    {
        ui->label_6->setVisible(0);
        ui->lineEdit_3->setVisible(0);
        ui->label_7->setVisible(0);
        ui->lineEdit_4->setVisible(0);
    }
}
//扫描角度
void Tools::on_lineEdit_3_textChanged(const QString &arg1)
{
    try
    {
        int size = arg1.toInt();
        if(size>=0)
        {
            Config::spanAngel = size*16;
        }
    }
    catch(...)
    {

    }
}
//起始角度
void Tools::on_lineEdit_4_textChanged(const QString &arg1)
{
    try
    {
        int size = arg1.toInt();
        if(size>=0)
        {
            Config::startAngel = size*16;
        }
    }
    catch(...)
    {

    }
}

void Tools::on_pushButton_7_clicked()
{
   qDebug()<<"清空";
   Config::isClear = 1;
}










