#ifndef WIDGET_H
#define WIDGET_H

#include "signin.h"

#include <QWidget>
#include <QtNetwork>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QHostAddress m_hostadr;
    void Initialise();
    Signin * server;

private slots:
    void slot_UserListShow(QMap<QTcpSocket*,QString>);
    void slot_ShopListShow(QMap<QTcpSocket*,QString>);
    void slot_RiderListShow(QMap<QTcpSocket*,QString>);
    void slot_Proceed(QString);
    void slot_ListCount(int,int,int);
    void slot_ChatListUp(QString,int);
    void slot_Chatting(QString,QString);
    void slot_MyChatting();

};
#endif // WIDGET_H
