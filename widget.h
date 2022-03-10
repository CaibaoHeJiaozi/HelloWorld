#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    enum MsgType {Msg,UsrEnter,UsrLeft};

public:
    explicit Widget(QWidget *parent,QString name);
    ~Widget();

private:
    Ui::Widget *ui;

signals:

    void closeWidget();

public:
    void closeEvent(QCloseEvent *);

public:
    void sndMsg (MsgType type);
    void usrEnter(QString usrname);
    void usrLeft(QString usrname,QString time);
    QString getUsr();
    QString getMsg();
private:
    QUdpSocket * udpSocket;
    qint16 port;
    QString uName;

    void ReceiveMessage();

};

#endif // WIDGET_H
