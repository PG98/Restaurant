#include "stafflogin.h"
#include "ui_stafflogin.h"
#include "data.h"
#include "waiterwindow.h"
#include "chefwindow.h"
#include "managerdialog.h"
#include <QtSql>
#include "QtWidgets"

staffLogin::staffLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::staffLogin)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText(tr("服务员'2'开头，厨师'3'开头"));
    ui->waiterBtn->setFocusPolicy(Qt::NoFocus);
    ui->waiterBtn_2->setFocusPolicy(Qt::NoFocus);
    ui->managerBtn->setFocusPolicy(Qt::NoFocus);
    QImage img;
    img.load(":/images/backgnd.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(img.scaled(this->width(),this->height())));
    this->setPalette(palette);
    setButtonStyle();
}

staffLogin::~staffLogin()
{
    delete ui;
}

void staffLogin::setButtonStyle(){
    ui->managerBtn->setStyleSheet("QPushButton{background-color:white;\
                                      color: black;   border-radius: 12px;  border: 3px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:silver; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }"
                                     );
    ui->waiterBtn->setStyleSheet("QPushButton{background-color:white;\
                                      color: black;   border-radius: 12px;  border: 3px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:silver; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }"
                                     );
    ui->waiterBtn_2->setStyleSheet("QPushButton{background-color:white;\
                                      color: black;   border-radius: 12px;  border: 3px groove gray;\
                                      border-style: outset;}"
                                     "QPushButton:hover{background-color:silver; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                     border-style: inset; }"
                                     );
}

void staffLogin::on_waiterBtn_clicked()
{
    if(ui->lineEdit->text() ==""){
        QMessageBox::warning(this,tr("警告"), tr("员工号为空！"));
        ui->lineEdit->setFocus();
        return;
    }
    int id = ui->lineEdit->text().toInt(), IDcheck;
    bool flag = false;
    QSqlQuery query;
    QString waiterstring = QString("select * from waiter where id = %1").arg(id);
    query.exec(waiterstring);
    if(!query.exec(waiterstring)){    //查找数据库中对应的号码
            qDebug()<<query.lastError();
            flag = false;
    }
    else{
        while(query.next()){
            IDcheck = query.value(0).toInt();
            qDebug()<<"waiter id: "<<IDcheck;
        }
        if(id == IDcheck)
            flag =true;
        else
            flag = false;
    }
    if(flag){
        waiterWindow* w = new waiterWindow;
        w->id = id;
        for(int i=0;i<5;i++){
            if(Data::waiter[i].id == id){
                qDebug()<<"waiter index:"<<i<<", waiterID: "<<id;
                w->index = i;
                w->table1 = Data::waiter[i].table1;
                w->table2 = Data::waiter[i].table2;
            }
        }
        w->on_refreshBtn_clicked();
        w->show();
        this->close();
    }
    else{
        QMessageBox::warning(this, tr("警告"), tr("员工号不正确"));
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
}

void staffLogin::on_waiterBtn_2_clicked()
{
    if(ui->lineEdit->text() ==""){
        QMessageBox::warning(this,tr("警告"), tr("员工号为空！"));
        ui->lineEdit->setFocus();
        return;
    }
    int id = ui->lineEdit->text().toInt(), IDcheck;
    QString chefstring = QString("select * from chef where id = %1").arg(id);
    bool flag = false;
    QSqlQuery query;
    query.exec(chefstring);
    if(!query.exec(chefstring)){    //查找数据库中对应的号码
            qDebug()<<query.lastError();
            flag = false;
    }
    else{
        while(query.next()){
            IDcheck = query.value(0).toInt();
            qDebug()<<"chef id: "<<IDcheck;
        }
        if(id == IDcheck)
            flag = true;
        else flag = false;
    }
    if(flag){
        chefWindow* w = new chefWindow;
        w->id = id;
        for(int i=0;i<5;i++){
            if(Data::chef[i].id == id)
                w->index = i;
        }
        w->on_action_R_triggered();
        w->show();
        this->close();
    }
    else{
        QMessageBox::warning(this, tr("警告"), tr("员工号不正确"));
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
}

void staffLogin::on_managerBtn_clicked()
{
    if(ui->lineEdit->text() ==""){
        QMessageBox::warning(this,tr("警告"), tr("员工号为空！"));
        ui->lineEdit->setFocus();
    }
    if(ui->lineEdit->text() == "101"){
        managerDialog* w = new managerDialog;
        w->show();
        this->close();
    }
}
