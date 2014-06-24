#ifndef DLGWK6440FUNCTION1_H
#define DLGWK6440FUNCTION1_H

#include "ui_dlgWk6440Function1.h"
#include "WKEInstrument.h"
class dlgWk6440Function1 : public QDialog, private Ui::dlgWk6440Function1
{
    Q_OBJECT

public:
    explicit dlgWk6440Function1(QDialog *parent = 0);
    QString getItem()
    {
        return this->strItem;
    }

private slots:
    void on_btnC_clicked();
    void on_btnL_clicked();
    void on_btnB_clicked();
    void on_btnZ_clicked();
    void on_btnY_clicked();
    void on_btnX_clicked();
private:
    QString strItem;

};

#endif // DLGFUNCTION_H
