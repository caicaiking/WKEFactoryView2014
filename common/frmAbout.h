#ifndef FRMABOUT_H
#define FRMABOUT_H

#include "ui_frmAbout.h"

class frmAbout : public QDialog, private Ui::frmAbout
{
    Q_OBJECT

    void readVersion();
public:
    explicit frmAbout(QWidget *parent = 0);
private slots:
    void on_btnClose_clicked();
    void on_btnAboutQT_clicked();
};

#endif // FRMABOUT_H
