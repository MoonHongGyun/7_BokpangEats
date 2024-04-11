#include "shopuser.h"

ShopUser::ShopUser(QWidget *parent)
    : QWidget{parent}
{

}
ShopUser::~ShopUser()
{

}

void ShopUser::div_Func(QTcpSocket* user,QList<QString> msg,QString name,QSet<QTcpSocket*> rdlist)
{
    ShopClient = user;
    msgList = msg;
    Shopname = name;
    qset_riderList = rdlist;
    if (msgList[1] == "CallR")
        CallR();
    if (msgList[1] == "D")
        Delivery();
}

void ShopUser::CallR()
{
    foreach(QTcpSocket* sendrider,qset_riderList)
    {
        qDebug() << msgList[2];
        sendrider->write(msgList[2].toUtf8());
    }
}

void ShopUser::Delivery()
{
    emit signal_delivery(msgList[2],msgList[3]);
}
