#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this); // 매장 클라이언트 소켓 생성
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(shop_division()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(shop_division()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::shop_division() // 서버에게 매장 구분
{
    QString shopname = ui->lineEdit->text().trimmed();
    tcpSocket->write(shopname.toUtf8());
}

void Widget::Messageread() // 서버가 보내는 데이터 수신
{
    if(tcpSocket->bytesAvailable() >=0)
    {
        QString readData = tcpSocket->readAll();
        QList<QString> msglist = readData.split(":");
    }
}
