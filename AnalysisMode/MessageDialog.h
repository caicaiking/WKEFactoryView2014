#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include "ui_MessageDialog.h"

class MessageDialog : public QDialog, private Ui::MessageDialog
{
    Q_OBJECT
    
public:
    explicit MessageDialog(QWidget *parent = 0);
    void setMessage(QString strMessage, QString strTitle);
private slots:
    void on_btnOk_clicked();
private:
    QString strMessge;
    QString strTitle;
};

#endif // MESSAGEDIALOG_H
