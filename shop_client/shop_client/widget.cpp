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
    QHostAddress serveradr("10.10.20.123");
    tcpSocket->connectToHost(serveradr,20000);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(Messageread()));
    connect(ui->pushButton_2, SIGNAL(pressed()),this,SLOT(slot_riderconnect()));
    connect(ui->pushButton_3, SIGNAL(pressed()),this, SLOT(slot_deliverystart()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::shop_division() // 서버에게 매장 구분
{
    shopname = "S:Login:"+ui->lineEdit->text().trimmed();
    tcpSocket->write(shopname.toUtf8());
}

void Widget::Messageread() // 서버가 보내는 데이터 수신
{
    QString readData = tcpSocket->readAll();
    QList<QString> msglist = readData.split(":");

    qDebug() << readData;
    if(msglist[0] == "U")
    {
        QList<QString> list = shopname.split(":");
        shop = msglist[2];

        QString message;
        message = "주문자: "+msglist[9]+"\n"+"메뉴: "+msglist[3]+"\n"+"가격: "+msglist[5]+"\n"+"가게요청사항: "+msglist[6]+"\n"+"라이더 요청사항: "+msglist[7]+"\n"+"결제방법: "+msglist[8]+"\n";
        ui->textEdit->append(message);
        ui->tableWidget_2->insertRow(row);
        ui->tableWidget_2->setItem(row,0,new QTableWidgetItem(msglist[3]+","+msglist[7]));
        ui->tableWidget_2->setItem(row,1,new QTableWidgetItem(msglist[9]));
        row++;
    }
    else
    {
        qDebug() << "awegrae";
        int i;
        for(i=0; i < ui->tableWidget_2->rowCount();i++)
        {
            qDebug() << ui->tableWidget_2->item(i,0)->text() << msglist[0];
            if(shop+","+ui->tableWidget_2->item(i,0)->text() == msglist[0])
            {
                break;
            }
        }
        qDebug() << i;
        ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(msglist[1]));
        ui->textEdit->append(tr("-라이더주문-\n라이더 : %1\n주문메뉴 : %2\n").arg(msglist[1],msglist[0]));
    }

}

void Widget::slot_riderconnect()
{
    QString message = shop+","+ui->tableWidget_2->currentItem()->text();
    tcpSocket->write(("S:CallR:" + message).toUtf8());
}

void Widget::slot_deliverystart()
{
    int num = ui->tableWidget_2->currentRow();
    QString client = ui->tableWidget_2->item(num,1)->text();
    QString menu = ui->tableWidget_2->item(num,0)->text();
    qDebug() << client << menu;
    tcpSocket->write(("S:D:"+client+":"+menu).toUtf8());
}
