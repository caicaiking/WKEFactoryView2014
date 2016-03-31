#ifndef CLSSELECTCOMPORT_H
#define CLSSELECTCOMPORT_H

#include "ui_clsSelectComport.h"

class clsSelectComport : public QDialog, private Ui::clsSelectComport
{
    Q_OBJECT

public:
    explicit clsSelectComport(QWidget *parent = 0);

    void setComports(QStringList ports);
    QString getPortSelected() const;

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    QString portSelected;
};

#endif // CLSSELECTCOMPORT_H
