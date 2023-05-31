#ifndef TOOLS_H
#define TOOLS_H

#include "data.h"

#include <QWidget>

namespace Ui {
class Tools;
}


class Tools : public QWidget
{
    Q_OBJECT

public:
    explicit Tools(QWidget *parent = nullptr);
    ~Tools();

private:
    Ui::Tools *ui;

    MyData *data1;
    MyData *data2;
    MyData *data3;

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked(); 
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_pushButton_8_clicked();

    void on_lineEdit_4_textChanged(const QString &arg1);

public slots:
    void on_pushButton_7_clicked();
};

#endif // TOOLS_H
