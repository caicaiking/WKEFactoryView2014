#ifndef FRMWKEANALYSISMODE_H
#define FRMWKEANALYSISMODE_H

#include "ui_frmWKEAnalysisMode.h"
#include "Plot.h"
#include "Qwt/qwt_plot_picker.h"
#include "WKEInstrument.h"
#include "Globle.h"
#include "clsMeas.h"
#include "Zoomer.h"
#include "Qwt/qwt_plot_panner.h"
#include "clsCurveLimit.h"
#include "clsMultiCurveLimt.h"
#include <QTimer>

#include "clsSignalThread.h"
#include "clsMultiLimitStatusShow.h"
#include "clsMarkerItem.h"
class frmWKEAnalysisMode : public QMainWindow, private Ui::frmWKEAnalysisMode
{
    Q_OBJECT

public:
    explicit frmWKEAnalysisMode(QWidget *parent = 0);

private slots:
    void on_btnSavePic_clicked();
    void on_btnTraceB_clicked();
    void moved(const QPoint &pos);
    void showInfo(QString text);
    void on_btnTraceA_clicked();
    void on_btnMeasSetup_clicked();
    void on_btnBias_clicked();
    void on_btnSpeed_clicked();
    void on_btnLevel_clicked();
    void on_btnFreq_clicked();
    void setFrequency(QString value);
    void setLevel(QString value);
    void setBias(bool value);
    void setSpeed(QString value);
    void setItems(QString value1, QString value2);
    void on_btnTraceSetup_clicked();
    void updateGraph();
    void updateButtons();
    void on_btnTrig_clicked();
    void on_btnRep_clicked();
    void on_btnAutoScale_clicked();
    void setBias(QString value);
    void on_btnRefTrace_clicked();
    void on_btnSaveResults_clicked();
    void on_btnCalibration_clicked();
    void on_btnZoom_clicked();
    void enableZoomMode(bool on);
    void zoomed(QRectF value);
    void on_btnHelp_clicked();
    void on_btnPeak_clicked();
    void on_btnSetLimit_clicked();
    void on_btnSaveRes_clicked();
    void on_btnSettings_clicked();
    void testConnection();
    void captureTrig();
    void showMulitLimit();
    void on_btnMarker_toggled(bool checked);
    void setCurrentMarker(int value);
    void showCurrentMarkerMsg(int, QString);
    void on_btnMaterialSettings_clicked();
    void on_btnBiasSettings_clicked();

    void on_btnContactTest_clicked();

private:
    Plot *plot;
    void initPlot();
    void initZoomer();
    QList<curveProperty> refTraces;
    QwtPlotPicker *d_picker;
    WKEInstrument *meter;
    void connectSignals();
    graphSetup gs;
    clsMeas *meas;
    QwtPlotZoomer *d_zoomer;
    bool checkDog();

    //用于曲线判断
    clsCurveLimit curveLimit;
    clsMultiCurveLimt multiCureLimit;

    clsMultiLimitStatusShow showLimit;

    void resPassFail();
    QString strDataFilePath;
    void init();
    QTimer *timer;
    clsSignalThread *controlBox;

   QList< clsMarkerItem *>markers ;
   void finishTest();
   void resetMarker();
   void setDemoVersion(bool value);
   bool getMaterialOption();
protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *event);
signals:

};

#endif // FRMWKEANALYSISMODE_H
