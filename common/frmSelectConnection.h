#ifndef FRMSELECTCONNECTION_H
#define FRMSELECTCONNECTION_H

#include "ui_frmSelectConnection.h"
#include "frmWKEAnalysisMode.h"

enum SelectMode
{
    Analysis=1,Meter=2,Resonace=3,MultiChannel=4,AppMode=5
};

class frmSelectConnection : public QDialog, private Ui::frmSelectConnection
{
    Q_OBJECT


public:
    explicit frmSelectConnection(QWidget *parent = 0);

    SelectMode getMode()
    {
        return this->mode;
    }

    int getIsReboot() const;

    void setIsReboot(int value);

private slots:
    void on_btnTest_clicked();
    void on_rbUsb_toggled(bool checked);
    void on_rbGpib_clicked();
    void on_rbLan_clicked();
    void on_rbUsb_clicked();

    void on_rbLan_toggled(bool checked);

    void on_btnAnalysis_clicked();

    void on_btnMeter_clicked();

    void on_btnRansonace_clicked();

    void on_btnMulitChannel_clicked();


    void on_btnSwitch_clicked();

//    void on_btnAppMode_clicked();

protected:
    void closeEvent(QCloseEvent *);

    void keyPressEvent(QKeyEvent *e);
private:
    int intSelect;
    QString strAddress;

    void writeSttings();
    void readSettings();
    void buttonOp(bool value);

    SelectMode mode;

    int isReboot;
   // frmWKEAnalysisMode  analysis;
};

#endif // FRMSELECTCONNECTION_H
