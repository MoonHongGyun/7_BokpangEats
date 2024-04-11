#ifndef APPUSER_H
#define APPUSER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QTcpSocket>
class AppUser : public QWidget
{
    Q_OBJECT
public:
    explicit AppUser(QWidget *parent = nullptr);
    ~AppUser();

    void div_Func(QTcpSocket*,QList<QString>,QString);
signals:
    void signal_Login(QTcpSocket*,QString);
    void signal_proceed(QTcpSocket*,QString);

private:
    QTcpSocket* client;
    QString email;
    QList<QString> msgList;
    void Login();
    void Membership();
    //검색시, 카테고리 메뉴 선택시, 매장선택시
    void Search();
    void Category();
    void ShopSelect();
    void MenuSelect();
    void Order();
    void OrderList();

};

#endif // APPUSER_H
