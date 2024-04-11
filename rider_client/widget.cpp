#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <qrgba64.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(Messageread()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(login())); // 라이더 접속 누르면 서버에게 신호 전송
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_serverbtn_clicked()
{
    QString serverIP = "10.10.20.123";
    QString serverPort = "20000";
    QHostAddress serverAddress(serverIP);

    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
}

void Widget::login()
{
    QString name = ui->lineEdit->text();
    QString msg = "R:Login:";
    msg = msg+name;
    qDebug()<<msg;
    tcpSocket->write(msg.toUtf8());
}

void Widget::btn_clicked()
{
    riderbtn = (QPushButton*)sender();
    QModelIndex item = ui->tableWidget->indexAt(riderbtn->pos());
    QString order = ui->tableWidget->item(item.row(),0)->text();
    list.append(order);
    ui->label_2->setText(order);
    QString name = ui->lineEdit->text();
    QString msg = "R:OK:";
    msg = msg+order+":"+name;
    tcpSocket->write(msg.toUtf8());
    qDebug()<<msg;
}



void Widget::Messageread() // 서버가 보내는 데이터 수신
{
    QString readData = tcpSocket->readAll(); // 서버가 보내는 데이터 전부 읽음
    qDebug()<<readData;

    QList<QString> msglist = readData.split(":");

    qDebug()<<msglist;
    if(msglist[0]=="R")
    {
        qDebug()<<"여기 들어오긴함?";
        int i;
        for (i=0;i<ui->tableWidget->rowCount();i++)
        {
            if(ui->tableWidget->item(i,0)->text() == msglist[2])
                break;
        }
        ui->tableWidget->removeRow(i);
        row = 0;
    }
    else
    {
        ui->tableWidget->insertRow(row);

        riderbtn= new QPushButton;
        riderbtn->setText("배달 수락");

        ui->tableWidget->setItem(row,0,new QTableWidgetItem(msglist[0]));
        ui->tableWidget->setCellWidget(row,1,riderbtn);
        connect(riderbtn,&QPushButton::clicked, this, &Widget::btn_clicked);
        //배달 수락 버튼 클릭하면 함수 실행
        row++;
        qDebug()<<msglist;
    }
}




