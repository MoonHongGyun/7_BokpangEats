#include "appuser.h"

#include <QSqlRecord>
AppUser::AppUser(QWidget *parent)
    : QWidget{parent}
{

}
AppUser::~AppUser()
{

}

void AppUser::div_Func(QTcpSocket* socket,QList<QString> xmsgList,QString id)
{
    client = socket;
    msgList = xmsgList;
    email = id;
    qDebug() << msgList[1];
    if (msgList[1] == "Login")
        Login();
    else if (msgList[1] == "Membership")
        Membership();
    else if (msgList[1] == "Search")
        Search();
    else if (msgList[1] == "Category")
        Category();
    else if (msgList[1] == "Shop")
        ShopSelect();
    else if (msgList[1] == "Menu")
        MenuSelect();
    else if (msgList[1] == "Order")
        Order();
    else if (msgList[1] == "OrderList")
        OrderList();
}

void AppUser::Login()
{
    QSqlQueryModel qryM;
    QSqlQuery qry;

    qry.prepare(tr("SELECT EMAIL FROM USER WHERE EMAIL = '%1' AND PW = '%2';").arg(msgList[2],msgList[3]));
    qry.exec();
    if(qry.next())
    {
        qryM.setQuery(tr("SELECT EMAIL,NAME FROM USER WHERE EMAIL = '%1'").arg(msgList[2]));
        QString data1 = qryM.record(0).value(0).toString();
        QString data2 = qryM.record(0).value(1).toString();
        client->write((tr("L:OK:%1:%2").arg(data1,data2)).toUtf8());
        emit signal_Login(client,msgList[2]);
    }
    else
        client->write("L:NO");
}
//e , p , n , ph , l
void AppUser::Membership()
{
    QSqlQueryModel qryM;
    QSqlQuery qry;
    qryM.setQuery(tr("SELECT EMAIL FROM USER WHERE EMAIL = '%1';").arg(msgList[2]));
    if (!qryM.record(0).value(0).toString().isEmpty())
    {
        qryM.setQuery(tr("SELECT PHONE FROM USER WHERE PHONE = '%1';").arg(msgList[5]));
        if (!qryM.record(0).value(0).toString().isEmpty())
            client->write("M:ALL");
        else
            client->write("M:EMAIL");
    }
    else
    {
        qryM.setQuery(tr("SELECT PHONE FROM USER WHERE PHONE = '%1';").arg(msgList[5]));
        if (!qryM.record(0).value(0).toString().isEmpty())
            client->write("M:PHONE");
        else
        {
            client->write("M:OK");
            qry.exec(tr("INSERT INTO USER VALUES('%1','%2','%3','%4','%5','%6')").arg(msgList[2],msgList[3],msgList[4],msgList[5],msgList[6],"0"));
            emit signal_proceed(client,"님이 회원가입 하였습니다.");
        }
    }
}

void AppUser::Search()
{
    QSqlQueryModel qryM;
    qryM.setQuery("SELECT NAME,IMAGE,MIN,DTIME FROM SHOP WHERE NAME LIKE %'"+msgList[2]+"'%");
    QString message = "&#";
    for (int i = 0; i < 4; i++)
    {
        message += qryM.record(0).value(i).toString();
        if (i < 3)
            message += "#";
    }
    client->write(message.toUtf8());
}

void AppUser::Category()
{
    QSqlQueryModel qryM;
    QString message = "&#";
    if (msgList[2] == "전체")
    {
        qryM.setQuery("SELECT NAME,IMAGE,MIN,DTIME FROM SHOP");
        for (int i = 0; i < qryM.rowCount(); i++)
        {
            for(int j = 0; j < 4; j++)
            {
                message += qryM.record(i).value(j).toString();
                if (j < 3)
                    message += "#";
            }
            if(i < qryM.rowCount()-1)
                message += "#";
        }
    }
    else
    {
        qryM.setQuery(tr("SELECT NAME,IMAGE,MIN,DTIME FROM SHOP WHERE KATEGORIE = '%1'").arg(msgList[2]));
        for (int i = 0; i < 4; i++)
        {
            message += qryM.record(0).value(i).toString();
            if (i < 3)
                message += "#";
        }
    }
    client->write(message.toUtf8());
}

void AppUser::ShopSelect()
{
    QSqlQueryModel qryM;
    QString message = "@#";

    qryM.setQuery(tr("SELECT NAME, IMAGE, DPRICE, MIN, DTIME, PTIME, LOCAL FROM SHOP WHERE IMAGE = '%1'").arg(msgList[2]));
    for (int i = 0; i < 7; i++)
    {
        message += qryM.record(0).value(i).toString();
        message += "#";
    }
    qryM.setQuery(tr("SELECT SHOPMENU.IMAGE, SHOPMENU.MENU, SHOPMENU.PRICE ,SHOPMENU.CAT FROM SHOP INNER JOIN SHOPMENU ON SHOP.NAME = SHOPMENU.SHOP WHERE SHOP.IMAGE = '%1'").arg(msgList[2]));
    for (int j = 0; j < qryM.rowCount();j++)
        for (int i = 0; i < 4; i++)
        {
            message += qryM.record(j).value(i).toString();
            if (j < qryM.rowCount() - 1)
                message += "#";
            else
            {
                if(i < 3)
                    message += "#";
            }
        }
    qDebug() << message;
    client->write(message.toUtf8());
}

void AppUser::MenuSelect()
{
    QSqlQueryModel qryM;
    QString message = "^#";
    QString shopname;
    QList<QString> typenum;

    qryM.setQuery(tr("SELECT SHOP,TYPE FROM SHOPMENU WHERE IMAGE = '%1'").arg(msgList[2]));
    shopname = qryM.record(0).value(0).toString();
    typenum = (qryM.record(0).value(1).toString()).split(":");
    qDebug() << typenum << shopname;
    for (int i = 0 ; i < typenum.size(); i++)
    {
        qryM.setQuery(tr("SELECT PMENU,PRICE,TITLE,BTN FROM PLUSMENU WHERE SHOP = '%1' AND TYPE = '%2'").arg(shopname,typenum[i]));
        for (int j = 0; j < qryM.rowCount(); j++)
        {
            for (int k = 0; k < 4;k++)
            {
                message += qryM.record(0).value(k).toString();
                if (i == typenum.size()-1 && j == qryM.rowCount() - 1)
                {
                    if(k < 3)
                        message += "#";
                }
                else
                    message += "#";
            }
        }
    }
    qDebug() << message;
    client->write(message.toUtf8());
}

void AppUser::Order()
{
    qDebug() << "결제하기" << msgList;
    QSqlQueryModel qryM;
    QSqlQuery qry;
    QString textUpdate;
    qryM.setQuery(tr("SELECT PHONE,LOCAL FROM USER WHERE EMAIL = '%1'").arg(email));
    QString phone = qryM.record(0).value(0).toString();
    QString local = qryM.record(0).value(1).toString();
    qryM.setQuery(tr("SELECT NAME FROM SHOP WHERE IMAGE = '%1'").arg(msgList[2]));
    QString shopname = qryM.record(0).value(0).toString();
    qryM.setQuery(tr("SELECT RECEIPT FROM USER WHERE EMAIL = '%1'").arg(email));
    textUpdate = qryM.record(0).value(0).toString();
    QString plus = shopname +":" + msgList[2] + ":" + msgList[3] + ":" + msgList[4] + ":" + msgList[5] + ":" + msgList[6] + ":" + msgList[7] + ":" + msgList[8]+ ":" + phone + ":" + local;
    if (textUpdate == "0")
    {
        qry.prepare(tr("UPDATE USER SET RECEIPT = '%1' WHERE EMAIL = '%2'").arg(plus,email));
        qry.exec();
    }
    else
    {
        qry.prepare(tr("UPDATE USER SET RECEIPT = '%1' WHERE EMAIL = '%2'").arg(textUpdate +  "#" + plus,email));
        qry.exec();
    }
    emit signal_proceed(client,"님이 주문을 완료하였습니다.");
}


void AppUser::OrderList()
{
    QSqlQueryModel qryM;
    qryM.setQuery(tr("SELECT RECEIPT FROM USER WHERE EMAIL = '%1'").arg(email));
    QString message = "O#" + qryM.record(0).value(0).toString();
    qDebug() << "sendMessage:" << message;
    client->write(message.toUtf8());
}


