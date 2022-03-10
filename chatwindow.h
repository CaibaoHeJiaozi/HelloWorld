#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

namespace Ui {
class chatwindow;
}

class chatwindow : public QWidget
{
    Q_OBJECT

public:
    explicit chatwindow(QWidget *parent = 0);
    ~chatwindow();

private:
    Ui::chatwindow *ui;
     QVector<bool> isShow;
};

#endif // CHATWINDOW_H
