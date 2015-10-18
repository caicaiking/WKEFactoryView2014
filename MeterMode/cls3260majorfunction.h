#ifndef CLS3260MAJORFUNCTION_H
#define CLS3260MAJORFUNCTION_H

#include "ui_cls3260majorfunction.h"

class cls3260MajorFunction : public QDialog, private Ui::cls3260MajorFunction
{
    Q_OBJECT

public:
    explicit cls3260MajorFunction(QWidget *parent = 0);

    QString getItem() const;

private slots:
    void on_btnL_clicked();

    void on_btnC_clicked();

    void on_btnZ_clicked();

private:
    QString item;
};

#endif // CLS3260MAJORFUNCTION_H
