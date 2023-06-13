#include "dao.h"

#include <qmessagebox.h>
#include <qmessagebox.h>
#include <qsqldatabase.h>

#include<QSqlQuery>
#include <QString>
#include <QVariant>
#include <qstring.h>
#include <qdebug.h>

Dao::Dao()
{

}
/*
create database shapes charset=utf8;
use shapes;
create table shape(
    _file_id varchar(1000) not null,
    _name varchar(1000) not null,
    _type varchar(1000) not null,
    _context varchar(1000) not null,
    _points varchar(1000) not null,
    _len int not null,
    _font_size int not null,
    _line_size int not null,
    _line_color int not null,
    _fill_color int not null
);

*/

QString pointsToString(Info info)
{
    QString out = "";
    for(int i=0;i<info.len;i++)
    {
        QString x = QString::number(info.points[i].x());
        QString y = QString::number(info.points[i].y());
        out = out + x + ":" +y + ",";
    }
    return out;
}
void getPoints(Info &info,QString text)
{
    QStringList list = text.split(",");
    int n=0;
    for(QString s:list)
    {
        QStringList xy = s.split(":");
        if(xy.size() == 2)
        {
            QPointF p;
            p.setX( xy[0].toInt());
            p.setY( xy[1].toInt());
            info.points.push_back(p);
        }
    }
    info.len = n;
}

int getColorCode(QColor c)
{
    int l;
    l = c.red();
    l *= 0x100;
    l += c.green();
    l *= 0x100;
    l += c.blue();
    return l;
}

QColor getColor(int code)
{
    int blue = code%0x100;
    code /= 0x100;
    int green = code % 0x100;
    code /= 0x100;
    int red = code % 0x100;

    return QColor(red,green,blue);
}

QVector<Info> Dao::read(QString name)
{
    QVector<Info> list;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    std::string str;
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("shapes");
    db.setUserName("root");
    db.setPassword("hzh1022");
    bool ok = db.open();

    QSqlQuery query;
    query.prepare("select * from shape where _file_id = :file_id;");
    query.bindValue(0, name);

    if (ok)
    {
        query.exec();
        while (query.next())
        {
            Info info;
            int n = 0;
            strcpy(info.file_id, query.value(n++).toString().toStdString().c_str());
            strcpy(info.name, query.value(n++).toString().toStdString().c_str());
            strcpy(info.type, query.value(n++).toString().toStdString().c_str());
            strcpy(info.context, query.value(n++).toString().toStdString().c_str());
            getPoints(info, query.value(n++).toString());
            info.len =  query.value(n++).toInt();
            info.font_size =  query.value(n++).toInt();
            info.line_size =  query.value(n++).toInt();
            info.line_color = getColor(query.value(n++).toInt());
            info.fill_color = getColor(query.value(n++).toInt());
            list.push_back(info);
        }
    }
    else
    {
        QMessageBox::information(NULL, "infor", "数据库连接失败");
        return list;
    }
    db.close();
    return list;
}

void Dao::save(QVector<Shape *> l, QString name)
{
    QVector<Info> list;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("shapes");
    db.setUserName("root");
    db.setPassword("hzh1022");

    QSqlQuery query;


    bool ok = db.open();
    if (ok){
        query.prepare("delete from shape where _file_id = :name;");
        query.bindValue(0, name);
        query.exec();

        for(int i=0;i<l.size();i++)
        {
            Info info = l[i]->info;
            int n=0;
            query.prepare("INSERT INTO `shape` (`_file_id`, `_name`, `_type`, `_context`, `_points`, `_len`, `_font_size`, `_line_size`, `_line_color`, `_fill_color`) "
                          "VALUES (:file_id, :name, :type, :context, :points, :len, :font_size, :line_size, :line_color, :fill_color);");

            query.bindValue(n++, info.file_id);
            query.bindValue(n++, info.name);
            query.bindValue(n++, info.type);
            query.bindValue(n++, info.context);
            query.bindValue(n++, pointsToString(info));
            query.bindValue(n++, info.len);
            query.bindValue(n++, info.font_size);
            query.bindValue(n++, info.line_size);
            query.bindValue(n++, getColorCode(info.line_color));
            query.bindValue(n++, getColorCode(info.fill_color));
            query.exec();
            qDebug()<<info.file_id<<","<<info.name<<","<<info.type<<","<<info.context<<","<<pointsToString(info)<<","<<info.len<<","<<info.font_size<<","<<info.line_size<<","<<getColorCode(info.line_color)<<","<<getColorCode(info.fill_color);
        }

    }
    else {
        QMessageBox::information(NULL, "infor", "数据库连接失败");
        return;
    }
    db.close();
}

