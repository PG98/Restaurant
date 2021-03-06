#include "order.h"
#include "ui_order.h"
#include <QDebug>
#include "data.h"
#include "orderquery.h"
#include "finishdialog.h"
#include <QHeaderView>

double Order::totalCharge = 0;

Order::Order(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("点菜"));
    //this->setFixedWidth(this->width());
    setTypeBox();
    setbox1();
    setbox4();
    setbox3();
    QHBoxLayout* buttons = setButtons();
    showDishes();
    setTableAppearance();
    connect(ui->table1->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(mySortByColumn(int)));//单击表头排序
    connect(ui->table1, SIGNAL(cellEntered(int,int)), this, SLOT(MouseTrackItem(int, int)));//鼠标移动效果
    connect(ui->table2, SIGNAL(cellEntered(int,int)), this, SLOT(MouseTrackItem2(int, int)));//鼠标移动效果
    connect(ui->table1, SIGNAL(cellClicked(int,int)),this, SLOT(rowSelect()));//单击选中行
    connect(ui->table2, SIGNAL(cellClicked(int,int)),this, SLOT(rowSelect2()));//单击选中行
    connect(ui->table2, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(delRow(int, int)));
    connect(ui->table1, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(addOrder(int)));
    for(int i=0;i<3;i++){
        connect(button[i], SIGNAL(keyClicked(int)),this, SLOT(makeRequest(int)));
    }
    addOrder(0);
    totalCharge = 0;
    showCharge();
    //orderCount = 0;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ui->box1, 0, 0);
    layout->addWidget(ui->typeBox, 0, 1);
    layout->addWidget(ui->box3, 1, 0);
    layout->addWidget(ui->box4, 1, 1);
    layout->addLayout(buttons, 2, 0, 1, 2);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(0, 6);
    layout->setRowStretch(1, 25);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    //外观设置
    QImage im;
    im.load(":/images/backgnd1.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(im.scaled(this->width(),this->height())));
    this->setPalette(palette);
}

Order::~Order()
{
    delete ui;
}

void Order::setbox1(){
    ui->box1->setTitle(tr("点击图标以获得帮助信息"));
    QHBoxLayout* layout = new QHBoxLayout;
    ui->iconBtn->setFixedSize(64, 64);
    ui->label->setFixedWidth(105);
    layout->addWidget(ui->iconBtn);
    layout->addWidget(ui->label);
    layout->addWidget(ui->priceLabel);
    ui->priceLabel->setText("0");
    ui->box1->setLayout(layout);
}

void Order::setTypeBox(){
    ui->typeBox->setTitle("菜品种类");
    //combobox index  connect()
    ui->typeview->resize(100,20);
    ui->typeview->addItem("<所有>");
    ui->typeview->addItem("主食");
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
    ui->typeBox->setLayout(layout);
}

void Order::setbox4(){
    ui->box4->setTitle(tr("菜单"));
    ui->table1->verticalHeader()->setVisible(false);
    ui->table1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table1->setColumnCount(5);
    ui->table1->setRowCount(Dish::count);
    for(int i=0;i<5;i++){
        ui->table1->setColumnWidth(i, 80);
    }
    //showDishes();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->table1, 0, 0);
    ui->box4->setLayout(layout);
    ui->table1->setHorizontalHeaderLabels(header);  //表头设置放在函数最后否则出错
    ui->table1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
}

void Order::setbox3(){
    ui->box3->setTitle(tr("已点菜品"));
    ui->table2->verticalHeader()->setVisible(false);
    ui->table2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table2->setColumnCount(5);
    for(int i=0;i<5;i++){
        ui->table2->setColumnWidth(i, 80);
    }
    ui->table2->setRowCount(0);
    ui->table2->setShowGrid(false);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->table2, 0, 0);
    ui->box3->setLayout(layout);
    ui->table2->setHorizontalHeaderLabels(header1);
    ui->table2->setAlternatingRowColors(true);
    //ui->table2->resizeColumnToContents(4);
    ui->table2->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightcoral;}"); //设置表头背景色
}

QHBoxLayout* Order::setButtons(){
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(ui->queryBtn);
    for(int i=0;i<3;i++){
        button[i] = new KeyButton;
        button[i]->setIndex(i);
        button[i]->setEnabled(false);
        button[i]->setStyleSheet("QPushButton{background-color:lightslategray;\
                                    color: black;   border-radius: 10px;  border: 2px groove gray;\
                                    border-style: outset;}"
                                   "QPushButton:hover{background-color:lightsteelblue; color:black;}"
                                  "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                   border-style: inset; }"
                                   );
        button[i]->setFixedHeight(40);
        layout->addWidget(button[i]);
    }
    ui->submitBtn->setFixedHeight(30);
    ui->queryBtn->setFixedHeight(30);
    layout->addWidget(ui->submitBtn);
    button[0]->setText(tr("加水"));
    button[1]->setText(tr("催单"));
    button[2]->setText(tr("买单"));
    ui->queryBtn->setStyleSheet("QPushButton{background-color:lemonchiffon;\
                                color: black;   border-radius: 8px;  border: 2px groove gray;\
                                border-style: outset;}"
                               "QPushButton:hover{background-color:lightsteelblue; color:black;}"
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                               border-style: inset; }"
                               );
    ui->submitBtn->setStyleSheet("QPushButton{background-color:lemonchiffon;\
                                color: black;   border-radius: 8px;  border: 2px groove gray;\
                                border-style: outset;}"
                               "QPushButton:hover{background-color:lightsteelblue; color:black;}"
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                               border-style: inset; }"
                               );

    return layout;
}

void Order::showDishes(){
    int k=0;
    for(int j=1;j<=8;j++){
        QHashIterator<int, Dish*> i(Data::hash1);
        while(i.hasNext()){
            i.next();
            if(i.key()>=j*100 && i.key()<(j+1)*100 && i.value()->demand>-3){
                ui->table1->setItem(k, 0, new QTableWidgetItem(QString("%1").arg(i.value()->id)));
                ui->table1->setItem(k, 1, new QTableWidgetItem(i.value()->getType()));
                ui->table1->setItem(k, 2, new QTableWidgetItem(i.value()->name));
                ui->table1->setItem(k, 3, new QTableWidgetItem(QString("%1").arg(i.value()->price)));
                if(i.value()->special)
                    ui->table1->setItem(k, 4, new QTableWidgetItem(QIcon(":/buttons/star.png"), "特别推荐"));
                else
                    ui->table1->setItem(k, 4, new QTableWidgetItem(QIcon(":/buttons/graystar.png"), "普通"));
                k++;
            }
        }
    }
    ui->table1->setRowCount(Dish::count);
    mySortByColumn(0);
    mySortByColumn(0);
}
//下拉选项改变 显示特定菜品种类
void Order::changeType(int type){
    //传入当前下拉菜单项的下标，对应的dish类是type(enum的首项定义为1)
    if(type==0){
        //qDebug()<<"show all";    //按照类别顺序显示表格
        showDishes();
    }
    else{
        int k=0;
        QHashIterator<int, Dish*> i(Data::hash1);
        while(i.hasNext()){
            i.next();
            if(i.value()->id>=type*100 && i.value()->id<(type+1)*100 && i.value()->demand>-3){
                ui->table1->setItem(k, 0, new QTableWidgetItem(QString("%1").arg(i.value()->id)));
                ui->table1->setItem(k, 1, new QTableWidgetItem(i.value()->getType()));
                ui->table1->setItem(k, 2, new QTableWidgetItem(i.value()->name));
                ui->table1->setItem(k, 3, new QTableWidgetItem(QString("%1").arg(i.value()->price)));
                if(i.value()->special)
                    ui->table1->setItem(k, 4, new QTableWidgetItem(QIcon(":/buttons/star.png"), "特别推荐"));
                else
                    ui->table1->setItem(k, 4, new QTableWidgetItem(QIcon(":/buttons/graystar.png"), "普通"));
                k++;
            }
        }
        ui->table1->setRowCount(k);
        mySortByColumn(0);
        mySortByColumn(0);
    }
}

void Order::setTableAppearance(){
    ui->table1->horizontalHeader()->setSortIndicatorShown(true);
    QFont font = ui->table1->horizontalHeader()->font();
    font.setBold(true);
    ui->table1->horizontalHeader()->setFont(font);//表头文字样式
    ui->table1->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑

    ui->table2->horizontalHeader()->setSortIndicatorShown(true);
    ui->table2->horizontalHeader()->setFont(font);//表头文字样式
    ui->table2->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
}

void Order::mySortByColumn(int column)
{
    static bool f = true;
    qDebug()<<"sort";
    ui->table1->sortByColumn(column, f ? Qt::AscendingOrder : Qt::DescendingOrder);
    f = !f;
}

void Order::MouseTrackItem(int row, int column){
    Q_UNUSED(column);
    ui->table1->setStyleSheet("selection-background-color:cornflowerblue;"); //选中项的颜色
    ui->table1->setCurrentCell(row, QItemSelectionModel::Select); //设置该行为选中项。
}

void Order::MouseTrackItem2(int row, int column){
    Q_UNUSED(column);
    ui->table2->setStyleSheet("selection-background-color:slategray;"); //悬浮项的颜色
    ui->table2->setCurrentCell(row, QItemSelectionModel::Select); //设置该行为选中项。
}

void Order::rowSelect(){   //鼠标点击表格效果
    static bool track = true;
    //qDebug()<<track;
    if(track){
        ui->table1->setStyleSheet("selection-background-color:khaki;"); //选中项的颜色
        //qDebug()<<ui->table1->currentRow();
        ui->table1->setMouseTracking(false);
    }
    else
        ui->table1->setMouseTracking(true);
    track = !track;
}
void Order::rowSelect2(){   //鼠标点击
    static bool track = true;
    if(track){
        ui->table2->setStyleSheet("selection-background-color:wheat;"); //选中项的颜色
        ui->table2->setMouseTracking(false);
    }
    else
        ui->table2->setMouseTracking(true);
    track = !track;
}

void Order::delRow(int row, int col){
    Q_UNUSED(col)
    if(row >= stage){
        int sub = ui->table2->item(row, 4)->text().toInt();
        if(sub == 1){
                int curr = ui->table2->rowCount();
                if(curr == row + 1){            //删除表格上最后一行的情况
                    qDebug()<<"test ";
                    ui->table2->setRowCount(curr+1);
                    ui->table2->setItem(curr, 0, new QTableWidgetItem("======"));
                    ui->table2->setItem(curr, 1, new QTableWidgetItem("======"));
                    ui->table2->setItem(curr, 2, new QTableWidgetItem("======"));
                    ui->table2->setItem(curr, 3, new QTableWidgetItem("======"));
                    ui->table2->setItem(curr, 4, new QTableWidgetItem(QString("=====")));
                    ui->table2->removeRow(row);
                    //不在下面垫一行会崩溃。。。
                }else{
                    ui->table2->removeRow(row);
                }
        }
        else
            ui->table2->setItem(row, 4, new QTableWidgetItem(QString("%1").arg(--sub)));
        //Data::orderCount--;   //总数统一在提交时计算
        totalCharge -= ui->table2->item(row, 2)->text().toInt();
        showCharge();
    }else{
        QMessageBox::warning(this, tr("警告"), tr("不能删除已经提交过的菜品"));
    }
}

void Order::addOrder(int row){
    //static bool connect = false;
    //if(!connect) connect = true;
    int id = ui->table1->item(row, 0)->text().toInt();
    qDebug()<<"add dish(id) to order: "<<id;
    for(int i=0;i<ui->table2->rowCount();i++){
        if(ui->table2->item(i, 0)->text().toInt() == id && i>=stage){
            int newcount = ui->table2->item(i, 4)->text().toInt() + 1;
            ui->table2->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(newcount)));
            //以下是静态变量的变更
            //orderCount++;
            totalCharge += ui->table2->item(i, 2)->text().toInt();
            showCharge();
            //qDebug()<<Data::hash1[id]->name<<", newCount: "<<newcount;
            return;
        }
    }
    int newRow = ui->table2->rowCount();
    ui->table2->setRowCount(newRow + 1); //newRow为总行数，使用时下标从0开始
    if(Data::hash1.contains(id)){
        ui->table2->setItem(newRow-1, 0, new QTableWidgetItem(QString("%1").arg(Data::hash1[id]->id)));
        ui->table2->setItem(newRow-1, 1, new QTableWidgetItem(Data::hash1[id]->name));
        ui->table2->setItem(newRow-1, 2, new QTableWidgetItem(QString("%1").arg(Data::hash1[id]->price)));
        //ui->table2->setItem(newRow-1, 3, new QTableWidgetItem(QString("%1").arg(Data::hash1[id]->special)));
        if(Data::hash1[id]->special)
            ui->table2->setItem(newRow-1, 3, new QTableWidgetItem(QIcon(":/buttons/star.png"), "特别推荐"));
        else
            ui->table2->setItem(newRow-1, 3, new QTableWidgetItem(QIcon(":/buttons/graystar.png"), "普通"));
        ui->table2->setItem(newRow-1, 4, new QTableWidgetItem(QString("1")));
        //
        ui->table2->setItem(newRow, 0, new QTableWidgetItem("           "));
        ui->table2->setItem(newRow, 1, new QTableWidgetItem("           "));
        ui->table2->setItem(newRow, 2, new QTableWidgetItem("           "));
        ui->table2->setItem(newRow, 3, new QTableWidgetItem("           "));
        ui->table2->setItem(newRow, 4, new QTableWidgetItem(QString("           ")));
        totalCharge += Data::hash1[id]->price;
        showCharge();
        //qDebug()<<"rowCount: "<<newRow-1<<"  orderCount: "<<orderCount;
    }
    else{
        qDebug()<<"index fault using hash1";
    }
}

void Order::showCharge(){
    ui->priceLabel->setText(QString("%1 元").arg(totalCharge));
}

void Order::on_submitBtn_clicked()
{
    qDebug()<<"submit, customerID: "<<customerID;
    //第一次提交后才设置按钮可用
    if(!firstCommit){
        firstCommit = true;
        for(int i=0;i<3;i++){
            button[i]->setEnabled(true);
            button[i]->setStyleSheet("QPushButton{background-color:skyblue;\
                                        color: black;   border-radius: 10px;  border: 2px groove gray;\
                                        border-style: outset;}"
                                       "QPushButton:hover{background-color:lightsteelblue; color:black;}"
                                      "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                       border-style: inset; }"
                                       );
        }
    }
    //设置stage，表示上次提交最后一项的下标。不能在左边的表格中删除已经提交过的菜名。
     //提交新增的项目
    for(int i=stage;i<=ui->table2->rowCount()-2;i++){
        orderInfo* info = new orderInfo(Data::orderCount++, currentTable, ui->table2->item(i, 0)->text().toInt(), -1, ui->table2->item(i,4)->text().toInt());
        //每项分别：编号，当前桌号（0起），菜品id，份数
        Data::hash1[ui->table2->item(i, 0)->text().toInt()]->history += ui->table2->item(i,4)->text().toInt();
        //更新对应菜品历史次数
        Data::list<<info;
    }
    /*
    QListIterator<orderInfo> i(Data::list);
    while(i.hasNext()){
        qDebug()<<"table: "<<i.next().tableid<<"  dish: "<<i.next().dishid<<"  status:"<<i.next().status<<"  count:"<<i.next().count;
    }
    */
    //遍历 输出内存中整个list
    for(orderInfo* info : Data::list){
        qDebug()<<"orderID: "<<info->id<<" "<<info->tableid<<" "<<info->dishid<<"  "<<info->count<<" status: "<<info->status;
    }
    stage = ui->table2->rowCount()-1;//本次提交
}

void Order::makeRequest(int n){
    qDebug()<<"currentTable: "<<currentTable;
    //根据按钮下标判断操作
    if(n == 0){
        QMessageBox::information(NULL, tr("提示"), tr("消息成功送达服务员"));
        Data::table[currentTable].water = true;
    }
    else if(n == 1){
        int row = ui->table2->currentRow();
        //通过桌号、菜id找出需要加急的菜品订单号
        for(orderInfo* info : Data::list){
            if(info->tableid == currentTable && info->dishid == ui->table2->item(row, 0)->text().toInt()){
                Data::table[currentTable].remind = info->id;
                QMessageBox::information(NULL, tr("提示"), QString("成功要求服务员对 %1 加快进度").arg(ui->table2->item(row, 1)->text()));
            }
        }
        qDebug()<<"remind chefs on order id: "<<Data::table[currentTable].remind;
    }
    else{
        Data::table[currentTable].pay = true;
        QMessageBox box;
        QPixmap icon(":/images/QRcode.jpg");
        box.setIconPixmap(icon);
        if(!Data::hash0[customerID]->isMember){
            box.setText(QString("\n你需要支付\n%1 元").arg(totalCharge));
        }
        else
            box.setText(QString("尊敬的会员，九五折后你需要支付\n %1 元").arg(totalCharge*0.95));
        QPushButton* b = box.addButton(tr("成功转账(&Y)"), QMessageBox::YesRole);
        box.exec();
        if(box.clickedButton() == b)
            box.close();
        //打开评价服务员，菜品窗口
        FinishDialog* f = new FinishDialog(this, currentTable);
        //f->tableid = currentTable;
        f->show();
        this->close();
    }
}

void Order::on_queryBtn_clicked()
{
    orderQuery* q = new orderQuery;
    q->tableid = currentTable;
    q->show();
    q->exec();
}

void Order::on_iconBtn_clicked()
{
    QString helpString = "双击添加当前项到左边托盘（可反复点击）\n 双击托盘中菜品可以取消订单\n点击下方按钮向工作人员提出需求";
    QMessageBox::information(this, tr("帮助"), helpString);
}

void Order::on_actionChoose_triggered()
{
    addOrder(ui->table1->currentRow());
}

void Order::on_actionDelete_D_triggered()
{
    delRow(ui->table2->currentRow(), 0);
}
