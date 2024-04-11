#include "signin.h"
#include "appuser.h"
#include "shopuser.h"
#include "rideruser.h"

#include <QTcpSocket>
#include <QRandomGenerator>
Signin::Signin(QObject *parent)
    : QTcpServer(parent)
{
    appuser = new AppUser;
    shopuser = new ShopUser;
    rideruser = new RiderUser;
    connect(appuser, SIGNAL(signal_Login(QTcpSocket*,QString)),this,SLOT(slot_Login(QTcpSocket*,QString)));
    connect(shopuser, SIGNAL(signal_delivery(QString,QString)),this, SLOT(slot_Delivery(QString,QString)));
    connect(appuser, SIGNAL(signal_proceed(QTcpSocket*,QString)),this,SLOT(slot_Proceed(QTcpSocket*,QString)));
}

Signin::~Signin()
{

}

void Signin::incomingConnection(qintptr socketfd)
{
    QTcpSocket* client = new QTcpSocket;
    client->setSocketDescriptor(socketfd);
    qset_userList.insert(client);
    int randnum = QRandomGenerator::global()->bounded(1000,2000);
    while(qmap_userList.key("#"+QString::number(randnum)) != NULL)
    {
        randnum = QRandomGenerator::global()->bounded(1000,2000);
    }
    qmap_userList.insert(client,"#"+QString::number(randnum));
    emit signal_Proceed(tr("비회원 %1님이 접속하였습니다.").arg(qmap_userList.value(client)));
    emit signal_ListCount(0,qset_userList.count(),qset_userList.count());
    emit signal_UserListShow(qmap_userList);
    connect(client,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
    connect(client,SIGNAL(disconnected()), this, SLOT(slot_Disconnect()));
}

void Signin::ReadyRead()
{
    QTcpSocket* client = (QTcpSocket*)sender();
    QString message = client->readAll();
    QList<QString> msgList = message.split(":");
    int allpeople = qset_userList.count() + qset_shopList.count() + qset_riderList.count();
    if (msgList[0] == "U")
    {
        if (msgList[1] == "Order")
        {
            QTcpSocket*SendShop = qmap_shopList.key(msgList[2]);
            QString order = qmap_userList.value(client);
            message += ":" + order;
            SendShop->write(message.toUtf8());
            appuser->div_Func(client,msgList,qmap_userList.value(client));
            emit signal_Proceed(tr("%1님이 %2메뉴를 주문하였습니다.").arg(qmap_userList.value(client),msgList[4]));
        }
        else
        {
            appuser->div_Func(client,msgList,qmap_userList.value(client));
            emit signal_ListCount(0,qset_userList.count(),qset_userList.count());
        }
    }
    else if (msgList[0] == "S")
    {
        if (msgList[1] == "Login")
        {
            qmap_userList.remove(client);
            qset_userList.remove(client);
            qset_shopList.insert(client);
            qmap_shopList.insert(client,msgList[2]);
            emit signal_ListCount(1,qset_shopList.count(),allpeople);
            emit signal_ListCount(0,qset_userList.count(),allpeople);
            emit signal_ShopListShow(qmap_shopList);
            emit signal_UserListShow(qmap_userList);
            emit signal_Proceed(tr("%1님이 로그인 하였습니다.").arg(qmap_shopList.value(client)));
        }
        else
        {
            shopuser->div_Func(client,msgList,qmap_shopList.value(client),qset_riderList);
        }
    }
    else if (msgList[0] == "R")
    {
        qDebug() << msgList;
        if (msgList[1] == "Login")
        {
            qmap_userList.remove(client);
            qset_userList.remove(client);
            qset_riderList.insert(client);
            qmap_riderList.insert(client,msgList[2]);
            emit signal_ListCount(2,qset_riderList.count(),allpeople);
            emit signal_ListCount(0,qset_userList.count(),allpeople);
            emit signal_RiderListShow(qmap_riderList);
            emit signal_UserListShow(qmap_userList);
            emit signal_Proceed(tr("%1님이 로그인 하였습니다.").arg(qmap_riderList.value(client)));
        }
        else
        {
            rideruser->div_Func(qmap_shopList,qset_riderList,client,msgList[2]+":"+msgList[3]);
        }
    }
    else if (msgList[0] == "C")
    {
        Chatting(client,msgList[1]);
    }
}

void Signin::slot_Login(QTcpSocket* clnt,QString name)
{
    qmap_userList[clnt] = name;
    emit signal_UserListShow(qmap_userList);
    emit signal_Proceed(tr("%1님이 로그인 하였습니다.").arg(name));
}

void Signin::slot_Proceed(QTcpSocket* clnt,QString msg)
{
    QString sendM = qmap_userList.value(clnt)+msg;
    emit signal_Proceed(sendM);
}

void Signin::slot_Disconnect()
{
    QTcpSocket * delClient = (QTcpSocket*)sender();
    qDebug() << qmap_userList.value(delClient) << qmap_shopList.value(delClient) << qmap_riderList.value(delClient);
    QString name;
    if (qmap_userList.value(delClient) != "0")
    {
        name = qmap_userList.value(delClient);
        qset_userList.remove(delClient);
        qmap_userList.remove(delClient);
        emit signal_ListCount(0,qset_userList.count(),qset_userList.count());
        emit signal_UserListShow(qmap_userList);
        emit signal_Proceed(tr("%1님이 접속해제 하였습니다.").arg(name));
    }
    else if (qmap_shopList.value(delClient) != "0")
    {
        name = qmap_shopList.value(delClient);
        qset_shopList.remove(delClient);
        qmap_shopList.remove(delClient);
        emit signal_ListCount(1,qset_shopList.count(),qset_userList.count());
        emit signal_ShopListShow(qmap_shopList);
        emit signal_Proceed(tr("%1님이 접속해제 하였습니다.").arg(name));
    }
    else if (qmap_riderList.value(delClient) != "0")
    {
        name = qmap_riderList.value(delClient);
        qset_riderList.remove(delClient);
        qmap_riderList.remove(delClient);
        emit signal_ListCount(2,qset_riderList.count(),qset_userList.count());
        emit signal_RiderListShow(qmap_riderList);
        emit signal_Proceed(tr("%1님이 접속해제 하였습니다.").arg(name));
    }
    delClient->deleteLater();

}

void Signin::Chatting(QTcpSocket* clnt,QString data)
{
    emit signal_ChatListUp(qmap_userList.value(clnt),0);
    emit signal_Chatting(qmap_userList.value(clnt),data);
}

void Signin::slot_Delivery(QString username,QString msg)
{
    QTcpSocket * clnt = qmap_userList.key(username);
    clnt->write(("S:"+msg).toUtf8());
}

void Signin::Chatting(QString id,QString data)
{
    qmap_userList.key(id)->write(data.toUtf8());
}

void Signin::ChatListDisconnect(QTcpSocket* clnt)
{
    qmap_chatList.remove(clnt);
    emit signal_ChatListUp(qmap_userList.value(clnt),1);
}
