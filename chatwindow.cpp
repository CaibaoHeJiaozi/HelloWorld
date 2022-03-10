#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QToolButton>
#include "widget.h"
#include <QMessageBox>

chatwindow::chatwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatwindow)
{
    ui->setupUi(this);
    setWindowTitle("MyselfQQ 2021");

    setWindowIcon(QPixmap(":/images/qq.png"));

    QList<QString>nameList;
    nameList << "waibiwaibi"<< "abaaba" << "1" << "2" << "3"
             <<"4" <<"5" <<"6" <<"7";

    QStringList iconNameList;
    iconNameList << "sqpy"<< "sqpy"<< "sqpy"<< "sqpy"<< "sqpy"<< "sqpy"
                    << "sqpy"<< "sqpy"<< "sqpy"<< "sqpy";


    QVector <QToolButton *> vToolBtn;


    for(int i = 0;i < 9;i++)
    {

        QToolButton *btn = new QToolButton;

        btn->setText(nameList[i]);

        //QString str = QString(":/images/spqy.png").arg(iconNameList.at(i));
        QString str = QString(":/images/spqy.png");
        btn->setIcon(QPixmap(str));

        btn->setIconSize(QPixmap(str).size());

        btn->setAutoRaise(true);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        ui->vLayout->addWidget(btn);
        vToolBtn.push_back(btn);

        isShow.push_back(false);
    }

        for(int i = 0;i < vToolBtn.size();i++)
        {
            connect(vToolBtn[i],&QToolButton::clicked,[=](){
                if(isShow[i])
                {
                    QString str = QString("%1 window is opening").arg(vToolBtn[i]->text());
                    QMessageBox::warning(this,"warning",str);
                    return;
                }

                isShow[i] = true;

                Widget *widget = new Widget(0,vToolBtn[i]->text());
                widget->setWindowTitle(vToolBtn[i]->text());
                widget->setWindowIcon(vToolBtn[i]->icon());
                widget->show();

                connect(widget,&Widget::closeWidget,[=](){

                    isShow[i] = false;

                });

            });
        }
}

chatwindow::~chatwindow()
{
    delete ui;
}
