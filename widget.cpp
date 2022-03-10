#include "widget.h"
#include "ui_widget.h"
#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>
#include <QColorDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
Widget::Widget(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);
    uName = name;
    this->port = 9999;

    udpSocket->bind(this->port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    sndMsg(UsrEnter);

    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        sndMsg(Msg);
    });

    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);


    connect(ui->exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });

    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->msgTxtEdit->setCurrentFont(font);
        ui->msgTxtEdit->setFocus();
    });

    void(QComboBox:: *cbxsingal)(const QString &text) = &QComboBox::currentIndexChanged;
    connect(ui->sizeCbx,cbxsingal,[=](const QString &text ){

        ui->msgTxtEdit->setFontPointSize(text.toDouble());
        ui->msgTxtEdit->setFocus();

    });

    connect(ui->boldTBtn,&QToolButton::clicked,[=](bool isCheck){
        if(isCheck)
        {
            ui->msgTxtEdit->setFontWeight(QFont::Bold);
        }
        else
        {
            ui->msgTxtEdit->setFontWeight(QFont::Normal);
        }
    });

    connect(ui->italicTBtn,&QToolButton::clicked,[=](bool check){
        ui->msgTxtEdit->setFontItalic(check);
    });

    connect(ui->underlineTBtn,&QToolButton::clicked,[=](bool check){
        ui->msgTxtEdit->setFontUnderline(check);
    });

    connect(ui->colorTBtn,&QToolButton::clicked,[=](){
       QColor color = QColorDialog::getColor(Qt::red);
       ui->msgTxtEdit->setTextColor(color);
    });

    connect(ui->clearTBtn,&QToolButton::clicked,[=](){
        ui->msgBrowser->clear();
    });

    connect(ui->saveTBtn,&QToolButton::clicked,[=](){
        if(ui->msgBrowser->document()->isEmpty())
        {
            QMessageBox::warning(this,"warning","file path is empty");
            return ;
        }
        else
        {
            QString path = QFileDialog::getSaveFileName(this,"save contents","*.txt");
            QFile file(path);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
              QTextStream stream(&file);
              stream << ui->msgBrowser->toPlainText();
              file.close();
        }
    });
}

void Widget::ReceiveMessage()
{
    qint64 size = udpSocket->pendingDatagramSize();
    QByteArray array = QByteArray(size,0);
    udpSocket->readDatagram(array.data(),size);

    QDataStream stream (&array,QIODevice::ReadOnly);

    int MsgType;
    QString usrName;
    QString msg;

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


    stream >>MsgType;

    switch (MsgType) {
    case Msg:
        stream >>usrName >> msg;

        ui->msgBrowser->setTextColor(Qt::blue);
        ui->msgBrowser->append("[" +usrName + "]" + time);
        ui->msgBrowser->append(msg);


        break;
    case UsrEnter:

        stream >> usrName;
        usrEnter(usrName);

        break;
    case UsrLeft:

        stream >> usrName;
        usrLeft(usrName,time);

        break;
    default:
        break;
    }
}

void Widget::usrLeft(QString usrname,QString time)
{
    bool isempty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(!isempty)
    {
        int row = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).first()->row();
        ui->usrTblWidget->removeRow(row);

        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(QString("%1 yu %2 leave ").arg(usrname).arg(time));

        ui->usrNumLbl->setText(QString("online number ,%1 ").arg(ui->usrTblWidget->rowCount()));

    }

}

void Widget::usrEnter(QString usrname)
{

    bool isEmpty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();

    if(isEmpty)
    {
        QTableWidgetItem * usr = new QTableWidgetItem(usrname);

        ui->usrTblWidget->insertRow(0);
        ui->usrTblWidget->setItem(0,0,usr);

        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(QString("%1 has loaded").arg(usrname));

        ui->usrNumLbl->setText(QString("online users, %1").arg(ui->usrTblWidget->rowCount()));

        sndMsg(UsrEnter);

    }
}

void Widget::sndMsg(MsgType type)
{
    QByteArray array;

    QDataStream stream(&array,QIODevice::WriteOnly);

    stream << type << getUsr();

    switch (type) {
    case Msg:

        if(ui->msgTxtEdit->toPlainText() == "")
        {
            QMessageBox::warning(this,"waring","write is blank");
            return ;
        }
        stream <<getMsg();

        break;
    case UsrEnter:

        break;
    case UsrLeft:
        break;
    default:
        break;
    }
    udpSocket->writeDatagram(array,QHostAddress::Broadcast,port);

}
QString Widget::getUsr()
{
    return this->uName;
}

QString Widget::getMsg()
{
    QString str = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();

    return str;
}

void Widget::closeEvent(QCloseEvent *e)
{
    emit this->closeWidget();
    sndMsg(UsrLeft);
    udpSocket->close();
    udpSocket->destroyed();

    QWidget::closeEvent(e);
}

Widget::~Widget()
{
    delete ui;
}
