#ifndef SIGNIN_H
#define SIGNIN_H

#include "appuser.h"
#include "shopuser.h"
#include "rideruser.h"

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>


class Signin : public QTcpServer
{
    Q_OBJECT
public:
    explicit Signin(QObject *parent = nullptr);
    ~Signin();
    void Chatting(QString,QString);

signals:
    void signal_ListCount(int,int,int);
    void signal_UserListShow(QMap<QTcpSocket*,QString>);
    void signal_Proceed(QString);
    void signal_ChatListUp(QString,int);
    void signal_Chatting(QString,QString);
    void signal_ShopListShow(QMap<QTcpSocket*,QString>);
    void signal_RiderListShow(QMap<QTcpSocket*,QString>);

private:
    QSet<QTcpSocket*> qset_userList;
    QSet<QTcpSocket*> qset_shopList;
    QSet<QTcpSocket*> qset_riderList;
    QSet<QTcpSocket*> qset_chatList;
    QMap<QTcpSocket*,QString> qmap_userList;
    QMap<QTcpSocket*,QString> qmap_shopList;
    QMap<QTcpSocket*,QString> qmap_riderList;
    QMap<QTcpSocket*,QString> qmap_chatList;
    void incomingConnection(qintptr socketfd);
    AppUser * appuser;
    ShopUser * shopuser;
    RiderUser * rideruser;
    void Chatting(QTcpSocket*,QString);
    QTcpSocket * now_chat;
    void ChatListDisconnect(QTcpSocket*);

private slots:
    void ReadyRead();
    void slot_Login(QTcpSocket*,QString);
    void slot_Proceed(QTcpSocket* clnt,QString);
    void slot_Disconnect();
    void slot_Delivery(QString,QString);
    // void slot_RidertoShop(QString,QString);

};

#endif // SIGNIN_H
