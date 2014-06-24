#ifndef DLGWK6440FUNCTION2_H
#define DLGWK6440FUNCTION2_H

#include "ui_dlgWk6440Function2.h"

class dlgWk6440Function2 : public QDialog, private Ui::dlgWk6440Function2
{
    Q_OBJECT

public:
    explicit dlgWk6440Function2(QDialog *parent = 0);
    QString getItem()
    {
        return this->strItem;
    }

private slots:
    void on_btnQ_clicked();
    void on_btnD_clicked();
    void on_btnR_clicked();
    void on_btnG_clicked();
private:
    QString strItem;

};

#endif // DLGFUNCTION2_H
