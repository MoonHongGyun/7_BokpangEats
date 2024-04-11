#ifndef RIDERUSER_H
#define RIDERUSER_H

#include <QWidget>
#include <QTcpSocket>
class RiderUser : public QWidget
{
    Q_OBJECT
public:
    explicit RiderUser(QWidget *parent = nullptr);

    void div_Func(QMap<QTcpSocket*,QString>,QSet<QTcpSocket*>,QTcpSocket*,QString);
signals:
    void siganl_RidertoShop(QString,QString);

private:
    QMap<QTcpSocket*,QString> qmap_shoplist;
    QTcpSocket* rider;
    QSet<QTcpSocket*> qset_riderlsit;
    QString Dmenu;

    void Delivery();
};

#endif // RIDERUSER_H
