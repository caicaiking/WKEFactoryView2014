void on_btnRCLoadCalibration_clicked();
void on_btnDone_clicked();
void on_btnInputAllStdValue_clicked();
#ifndef CLSCALIBRATION_H
#define CLSCALIBRATION_H

#include "ui_clsCalibration.h"
#include "clsDataProcess.h"
#include "clsRuningSettings.h"
#include "clsConnectSWBox.h"
#include "clsMultiModeMeter.h"
class clsCalibration : public QDialog, private Ui::clsCalibration
{
    Q_OBJECT

public:
    explicit clsCalibration(QWidget *parent = 0);
    void setMeter(clsMultiModeMeter *meter);
    void setChannels(QStringList value);
    void insertRecord(double freq, int channal, double z, double a, QString type);
    void updataText();
    void saveSettings();
private slots:

    void on_btnTest_clicked();
    void on_btnMultiFreq_clicked();
    void on_btnOpen_clicked();
    void on_btnShort_clicked();
    void on_btnLoad_clicked();
    void on_btnTest1Freq_toggled(bool checked);
    void on_btnTest2Freq_toggled(bool checked);
    void on_btnNextChannel_clicked();
    void on_btnStdZ_clicked();
    void on_btnStdA_clicked();
    void on_btnTest2Freq_clicked();
    void on_btnTest1Freq_clicked();
    void on_btnClearOpen_clicked();
    void on_btnClearShort_clicked();
    void on_btnClearLoad_clicked();
    void on_btnClearStdLoad_clicked();
    void on_btnInputAllStdValue_clicked();
    void on_btnCancel_clicked();
    void on_btnDone_clicked();
    void  on_btnOpenTrim_clicked();
    void on_btnLoadTrim_clicked();
    void on_btnShortTrim_clicked();
    void on_btnNextChannel_2_clicked();
    void on_btnInstumentCal_clicked();
    void changeWidget();
    void on_btnRCLoadCalibration_clicked();
private:
    double ocZ,ocA,scZ,scA,loadZ,loadA, stdA,stdZ;
    double freq;
    QList<double> freqList;
    clsMultiModeMeter *meter;
    QStringList channels;
    QList<double> getCalDataFromDb(double freq, int channal, QString type);
    void getAllDataFromDb(double freq, int channel);
    QList<double> getCalFrequencys();
    QList<int> getCalChannels();
    int showCalMessage(QString calType, double calFreq, int channel);
    int showCalMessage(QString calType, int channel);
    void setCalLabelInfo(QLabel *lbl, QString calType);
    void readSettings();
};

#endif // CLSCALIBRATION_H
