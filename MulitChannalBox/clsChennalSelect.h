#ifndef CLSCHENNALSELECT_H
#define CLSCHENNALSELECT_H

#include "ui_clsChennalSelect.h"
#include <QCheckBox>
#include <QList>
const int CHENNAL_COUNT =20;
class clsChennalSelect : public QDialog, private Ui::clsChennalSelect
{
    Q_OBJECT

public:
    explicit clsChennalSelect(QWidget *parent = 0);
    void setChennal(QString value);
    QString getChennal();
private slots:
    void setText();

    void on_btnSelect10_clicked();

    void on_btnSelectAll_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    QList<QCheckBox *> chennals;

};

#endif // CLSCHENNALSELECT_H
