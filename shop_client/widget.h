#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>

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
    QTcpSocket *tcpSocket;

private slots:
    void shop_division(); // 매장이름 구분
    void Messageread(); //서버 데이터 수신
};
#endif // WIDGET_H
