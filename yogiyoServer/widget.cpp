#include "widget.h"
#include "ui_widget.h"
#include "signin.h"

#define PORT 20000

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Initialise();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Initialise()
{
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipList.size(); i++)
    {
        if (ipList.at(i) != QHostAddress::LocalHost && ipList.at(i).toIPv4Address())
        {
            m_hostadr = ipList.at(i);
            break;
        }
    }

    if (m_hostadr.toString().isEmpty())
    {
        m_hostadr = QHostAddress(QHostAddress::LocalHost);
    }

    server = new Signin(this);
    server->listen(m_hostadr,PORT);
    connect(server, SIGNAL(signal_UserListShow(QMap<QTcpSocket*,QString>)),this,SLOT(slot_UserListShow(QMap<QTcpSocket*,QString>)));
    connect(server, SIGNAL(signal_Proceed(QString)), this, SLOT(slot_Proceed(QString)));
    connect(server, SIGNAL(signal_ListCount(int,int,int)), this, SLOT(slot_ListCount(int,int,int)));
    connect(server, SIGNAL(signal_Chatting(QString,QString)),this,SLOT(slot_Chatting(QString,QString)));
    connect(server, SIGNAL(signal_ChatListUp(QString,int)),this,SLOT(slot_ChatListUp(QString,int)));
    connect(ui->chat_send, SIGNAL(pressed()),this,SLOT(slot_MyChatting()));
    connect(server, SIGNAL(signal_ShopListShow(QMap<QTcpSocket*,QString>)), this, SLOT(slot_ShopListShow(QMap<QTcpSocket*,QString>)));
    connect(server, SIGNAL(signal_RiderListShow(QMap<QTcpSocket*,QString>)), this, SLOT(slot_RiderListShow(QMap<QTcpSocket*,QString>)));


}

void Widget::slot_UserListShow(QMap<QTcpSocket*,QString> UserList)
{
    ui->user_list->clear();
    QMap<QTcpSocket*,QString>:: iterator i;
    int row = 0;
    for (i = UserList.begin(); i != UserList.end(); i++,row++)
    {
        QString check = i.value();
        ui->user_list->addItem(check);
        if (check.contains("#",Qt::CaseInsensitive) == true)
            ui->user_list->item(row)->setForeground(Qt::red);
        else
            ui->user_list->item(row)->setForeground(Qt::blue);
    }
}

void Widget::slot_ShopListShow(QMap<QTcpSocket*,QString> ShopList)
{
    ui->shop_list->clear();
    QMap<QTcpSocket*,QString>:: Iterator i;
    for (i = ShopList.begin(); i != ShopList.end();i++)
    {
        QString id = i.value();
        qDebug() << id;
        ui->shop_list->addItem(id);
    }
}

void Widget::slot_RiderListShow(QMap<QTcpSocket*,QString> RiderList)
{
    ui->rider_list->clear();
    QMap<QTcpSocket*,QString>:: Iterator i;
    for (i = RiderList.begin(); i != RiderList.end();i++)
    {
        QString id = i.value();
        qDebug() << id;
        ui->rider_list->addItem(id);
    }
}

void Widget::slot_Proceed(QString message)
{
    ui->all_te->append(message);
}

void Widget::slot_ListCount(int check,int people,int all)
{
    if (check == 0)
        ui->user_lb->setText(tr("유저 %1명").arg(QString::number(people)));
    else if (check == 1)
        ui->shop_lb->setText(tr("유저 %1명").arg(QString::number(people)));
    else
        ui->rider_lb->setText(tr("유저 %1명").arg(QString::number(people)));
    ui->all_lb->setText(tr("총 %1명").arg(QString::number(all)));
}

void Widget::slot_Chatting(QString id,QString msg)
{
    ui->chat_te->append(id+"->"+msg);
    ui->chat_te->setAlignment(Qt::AlignLeft);
}

void Widget::slot_ChatListUp(QString id,int num)
{
    int check = 0;
    if (num == 0)
    {
        if(ui->chat_list->count() == 0)
            ui->chat_list->addItem(id);
        else
        {
            for(int i = 0; i < ui->chat_list->count(); i++)
            {
                if(ui->chat_list->item(i)->text() == id)
                    check = 1;
            }
            if(check == 0)
                ui->chat_list->addItem(id);
        }
    }
    else
    {
        int j;
        for (j = 0; j < ui->chat_list->count(); j++)
        {
            if(ui->chat_list->item(j)->text() == id)
                break;
        }
        ui->chat_list->takeItem(j);
    }
}

void Widget::slot_MyChatting()
{
    ui->chat_te->append(ui->chat_list->currentItem()->text() +"<-"+ ui->chat_le->text());
    ui->chat_te->setAlignment(Qt::AlignRight);
    server->Chatting(ui->chat_list->currentItem()->text(),"C:" + ui->chat_le->text());
}


