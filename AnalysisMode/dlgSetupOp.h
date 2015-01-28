#ifndef DLGSETUPOP_H
#define DLGSETUPOP_H

#include "ui_dlgSetupOp.h"

class dlgSetupOp : public QDialog, private Ui::dlgSetupOp
{
    Q_OBJECT

public:
    explicit dlgSetupOp(QWidget *parent = 0);

    int getSelect()
    {
        return intSelect;
    }

    QString getSelectFilePath()
    {
        return this->strSelectFilePath;
    }

private slots:
    void on_btnSaveSettingsFile_clicked();
    void on_btnOpenSettingsFile_clicked();
    void on_btnClose_clicked();

private:
    int intSelect;
    QString strSelectFilePath;

};

#endif // DLGSETUPOP_H
