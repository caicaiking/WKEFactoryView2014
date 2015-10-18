#ifndef CLS3260MINORFUNCTION_H
#define CLS3260MINORFUNCTION_H

#include "ui_cls3260minorfunction.h"

class cls3260MinorFunction : public QDialog, private Ui::cls3260MinorFunction
{
    Q_OBJECT

public:
    explicit cls3260MinorFunction(QWidget *parent = 0);
    QString getItem() const;
private slots:
    void on_btnQ_clicked();

    void on_btnD_clicked();

    void on_btnR_clicked();

private:
    QString item;
};

#endif // CLS3260MINORFUNCTION_H
