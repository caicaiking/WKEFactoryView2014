#ifndef DLG4300FUNCTIONMINOR_H
#define DLG4300FUNCTIONMINOR_H

#include "ui_dlg4300FunctionMinor.h"

class dlg4300FunctionMinor : public QDialog, private Ui::dlg4300FunctionMinor
{
    Q_OBJECT

public:
    explicit dlg4300FunctionMinor(QWidget *parent = 0);
    QString getItem()
    {
        return this->strItem;
    }

private slots:
    void on_btnOFF_clicked();
    void on_btnY_clicked();
    void on_btnA_clicked();
    void on_btnZ_clicked();
    void on_btnB_clicked();
    void on_btnG_clicked();
    void on_btnX_clicked();
    void on_btnR_clicked();
    void on_btnQ_clicked();
    void on_btnL_clicked();
    void on_btnD_clicked();
    void on_btnC_clicked();

private:
    QString strItem;
};

#endif // DLG4300FUNCTIONMINOR_H
