#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QPushButton>

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
    int row = 0;
    QPushButton *riderbtn;
    QList<QString>list;

private slots:
    void Messageread();
    void login();
    void on_serverbtn_clicked();
    void btn_clicked();
};
#endif // WIDGET_H
