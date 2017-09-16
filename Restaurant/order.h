#ifndef ORDER_H
#define ORDER_H

#include <QMainWindow>
#include <QtWidgets>
#include "keybutton.h"

namespace Ui {
class Order;
}

class Order : public QMainWindow
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = 0);
    ~Order();
    static int getOrderCount() {return orderCount;}  //总的菜品个数（每种可不止一次）
    static int getTotalCharge() {return totalCharge;}
    int currentTable;

private slots:
    void mySortByColumn(int col);
    void MouseTrackItem(int row, int column);
    void MouseTrackItem2(int row, int column);
    void rowSelect();
    void rowSelect2();
    void delRow(int row, int col);
    void addOrder(int row);
    void changeType(int);
    void makeRequest(int);
    void on_submitBtn_clicked();

private:
    Ui::Order *ui;
    KeyButton* button[3];
    static int orderCount;
    static double totalCharge;
    QStringList header = QStringList()<<"编号"<<"种类"<<"名字"<<"单价"<<"推荐度";
    QStringList header1 = QStringList()<<"编号"<<"名字"<<"单价"<<"推荐度"<<"份数";
    void setbox1();
    void setTypeBox();
    void setbox3();
    void setbox4();
    QHBoxLayout* setButtons();
    void showDishes();
    void setTableAppearance();
    void showCharge();
};

#endif // ORDER_H
