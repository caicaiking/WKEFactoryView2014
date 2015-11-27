#ifndef CLSMYPUSHBUTTON_H
#define CLSMYPUSHBUTTON_H

#include "ui_clsMyPushButton.h"

class clsMyPushButton : public QWidget, private Ui::clsMyPushButton
{
    Q_OBJECT

public:
    explicit clsMyPushButton(QWidget *parent = 0);

    void setText(QString value);
signals:
    void btnClick(QString value);
private slots:
    void on_btnApp_clicked();
};

#endif // CLSMYPUSHBUTTON_H
