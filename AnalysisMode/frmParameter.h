#ifndef FRMPARAMETER_H
#define FRMPARAMETER_H

#include "ui_frmParameter.h"

class frmParameter : public QDialog, private Ui::frmParameter
{
    Q_OBJECT

public:
    explicit frmParameter(QWidget *parent = 0);
    QStringList getValues() const;
    void setValues(const QStringList &value);

    void readSettings();
private slots:
    void on_btnOk_clicked();
private:

    QStringList values;
    void writeSettings();
};

#endif // FRMPARAMETER_H
