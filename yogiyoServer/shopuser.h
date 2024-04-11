#ifndef SHOPUSER_H
#define SHOPUSER_H

#include <QWidget>
#include <QTcpSocket>

class ShopUser : public QWidget
{
    Q_OBJECT
public:
    explicit ShopUser(QWidget *parent = nullptr);
    ~ShopUser();
    void div_Func(QTcpSocket*,QList<QString>,QString,QSet<QTcpSocket*>);
signals:
    void signal_delivery(QString,QString);

private:
    QTcpSocket * ShopClient;
    QList<QString> msgList;
    QString Shopname;
    QSet<QTcpSocket*> qset_riderList;
    void CallR();
    void Delivery();
};

#endif // SHOPUSER_H
