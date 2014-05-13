#ifndef WK6500RANGE_H
#define WK6500RANGE_H

#include "ui_wk6500Range.h"

class wk6500Range : public QDialog, private Ui::wk6500Range
{
    Q_OBJECT
    
public:
    explicit wk6500Range(QWidget *parent = 0);

    QString getRange() {return Range;}
private slots:
    void on_btn0_clicked();
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn7_clicked();

private:
    QString Range;
};

#endif // WK6500RANGE_H
