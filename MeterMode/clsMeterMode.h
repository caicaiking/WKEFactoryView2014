#ifndef CLSMETERMODE_H
#define CLSMETERMODE_H

#include "ui_clsMeterMode.h"
#include "WKEMeterMode.h"
#include <QList>
#include <QPointer>


class clsMeterMode : public QMainWindow, private Ui::clsMeterMode
{
    Q_OBJECT

public:
    explicit clsMeterMode(QWidget *parent = 0);

private slots:


    void on_btnSetStep_clicked();

protected:

private:
    QList<WKEMeterMode *> steps;
};

#endif // CLSMETERMODE_H
