#ifndef CLSRETRYDIALOG_H
#define CLSRETRYDIALOG_H

#include "ui_clsRetryDialog.h"

class clsRetryDialog : public QDialog, private Ui::clsRetryDialog
{
    Q_OBJECT

public:
    explicit clsRetryDialog(QWidget *parent = 0);
    void setMessage(QString value);
private slots:
    void on_btnGiveUp_clicked();
    void on_btnRetry_clicked();
};

#endif // CLSRETRYDIALOG_H
