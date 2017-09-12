#include "menumanage.h"
#include "ui_menumanage.h"
#include <QDebug>
#include "data.h"
#include <QHeaderView>

MenuManage::MenuManage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuManage)
{
    ui->setupUi(this);
    qDebug()<<"dish count:"<<Dish::count<<endl;

    setTypeGroupBox();
    setDetailGroupBox();
    setMenuGroupBox();

    QHBoxLayout* bottomLayout = setButtons();
    //connect 按钮..槽..

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(ui->dishtype, 0, 0);
    layout->addWidget(ui->menu, 1, 0);
    layout->addWidget(ui->detail, 0, 1, 2, 1);
    layout->addLayout(bottomLayout, 2, 0, 1, 2);
    layout->setColumnStretch(1, 1);
    layout->setColumnMinimumWidth(0, 500);
    layout->setRowMinimumHeight(0,100);


    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MenuManage::~MenuManage()
{
    delete ui;
}

void MenuManage::setTypeGroupBox(){
    ui->dishtype->setTitle("菜品种类");
    //combobox index  connect()
    ui->typeview->resize(100,20);
    QIcon icon1(":/buttons/eject.png");
    ui->typeview->addItem("<所有>");
    ui->typeview->addItem(icon1, "主食");
    ui->typeview->addItem("凉菜");
    ui->typeview->addItem("海鲜");
    ui->typeview->addItem("荤菜");
    ui->typeview->addItem("素菜");
    ui->typeview->addItem("汤类");
    ui->typeview->addItem("甜点");
    ui->typeview->addItem("饮料");
    connect(ui->typeview, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(ui->typeview, 0, 0);
    ui->dishtype->setLayout(layout);
}

void MenuManage::setMenuGroupBox(){
    ui->menu->setTitle("菜单");
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(Dish::count);
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(Data::dish[i].id)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(Data::dish[i].getType()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(Data::dish[i].name));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(Data::dish[i].price)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(Data::dish[i].notes));
        //ui->tableWidget->horizontalHeaderItem(i)->setBackgroundColor(QColor(100, 149, 237));//会崩溃
    }
    //connect  表格项目点击，激活等
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->tableWidget, 0, 0);
    ui->menu->setLayout(layout);
    ui->tableWidget->setHorizontalHeaderLabels(header);//设置表头
    //ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(134, 245, 99, 255),stop:0.5 rgba(134, 148, 99, 255),stop:1 rgba(115, 87, 128, 255));color:rgb(25, 70, 100);padding-left: 1px;border: 1px solid #FFFF00;}");
}

void MenuManage::setDetailGroupBox(){
    ui->detail->setTitle("详情");
    //菜品图片
    ui->imageLabel->setWordWrap(true);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    //ui->imageLabel->setPixmap(QPixmap(":/images/xxxx"));

    ui->notesLabel->setWordWrap(true);
    ui->notesLabel->setAlignment(Qt::AlignBottom);

    //其他label 可加入
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ui->imageLabel, 0, 0, 3, 2);
    layout->addWidget(ui->notesLabel, 0, 0);
    ui->detail->setLayout(layout);
}

QHBoxLayout* MenuManage::setButtons(){
    //设置按钮外观
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ui->backBtn);
    layout->addStretch(1);
    layout->addWidget(ui->OkBtn);
    return layout;
}
//以上各GroupBox初始化
void MenuManage::changeType(int type){
    //传入当前下拉菜单项的下标，对应的dish类是type-1
    qDebug()<<"clear table";
    ui->tableWidget->clear();
    int k=0;
    for(int i=0;i<Dish::count;i++){
        qDebug()<<QString("%1").arg((int)Data::dish[i].type);
        if((int)Data::dish[i].type==type-1){
            ui->tableWidget->setItem(k, 0, new QTableWidgetItem(QString("%1").arg(Data::dish[i].id)));
            ui->tableWidget->setItem(k, 1, new QTableWidgetItem(Data::dish[i].getType()));
            ui->tableWidget->setItem(k, 2, new QTableWidgetItem(Data::dish[i].name));
            ui->tableWidget->setItem(k, 3, new QTableWidgetItem(QString("%1").arg(Data::dish[i].price)));
            ui->tableWidget->setItem(k, 4, new QTableWidgetItem(Data::dish[i].notes));
            k++;
        }
    }
}




















