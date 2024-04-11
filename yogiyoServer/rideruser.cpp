#include "rideruser.h"

RiderUser::RiderUser(QWidget *parent)
    : QWidget{parent}
{

}

void RiderUser::div_Func(QMap<QTcpSocket*,QString> ml,QSet<QTcpSocket*> rs,QTcpSocket* socket,QString data)
{
    qmap_shoplist = ml;
    rider = socket;
    Dmenu = data;
    qset_riderlsit = rs;
    // QList<QString> list = Dmenu.split(":");
    Delivery();

}

void RiderUser::Delivery()
{
    QList<QString> msgList =  Dmenu.split(":");
    QList<QString> menuList = msgList[0].split(",");
    QString shopname = menuList[0];
    QTcpSocket* shopclnt = qmap_shoplist.key(shopname);
    shopclnt->write(Dmenu.toUtf8());
    qDebug() << Dmenu;
    qDebug() << msgList[0];
    QSet<QTcpSocket*>::Iterator i;
    for(i = qset_riderlsit.begin(); i != qset_riderlsit.end();i++)
    {
        QTcpSocket*clnt = *i;
        clnt->write(("R:Select:"+msgList[0]).toUtf8());
    }
}
