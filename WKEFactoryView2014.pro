#-------------------------------------------------
#
# Project created by QtCreator 2013-02-20T17:30:00
#
#-------------------------------------------------

QT       += core gui network serialport xml sql multimedia
RC_FILE  =Icon.rc

CODECFORTR = UTF-8

INCLUDEPATH +=connections \
             common \
             AnalysisMode \
             Public \
             MeterMode \
             saftyDog \
             ResonaceMode \
             MulitChannalBox \
             QwtInclude \
             quazip\Include \
             AnalysisMode/Qextserial \
             AnalysisMode/EaPs8000 \
             MeterMode\AddtionOperation


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WKEFactoryView2014
DESTDIR =$$PWD/libs/
TEMPLATE = app

LIBS    += $$PWD/libs/gpib488.obj \
           $$PWD/libs/visa.lib \
           $$PWD/libs/AduHid.lib \
           $$PWD/libs/AduHid.dll \
           $$PWD/libs/dog_windows_3149756.dll


SOURCES +=\
    connections/clsGpib.cpp \
    connections/clsLan.cpp \
    connections/clsUsb.cpp \
    connections/Cnnt.cpp \
    common/frmSelectConnection.cpp \
    common/main.cpp \
    AnalysisMode/ClickedLabel.cpp \
    AnalysisMode/clsBiasAMeasument.cpp \
    AnalysisMode/clsBiasMeasument.cpp \
    AnalysisMode/clsFit.cpp \
    AnalysisMode/clsFreqMeasument.cpp \
    AnalysisMode/clsInputLimitCheck.cpp \
    AnalysisMode/clsRefTrace.cpp \
    AnalysisMode/clsSelectedRef.cpp \
    AnalysisMode/clsSelectedRefProperty.cpp \
    AnalysisMode/clsTimeMeasument.cpp \
    AnalysisMode/clsViewResult.cpp \
    AnalysisMode/dlgFunction.cpp \
    AnalysisMode/dlgSort.cpp \
    AnalysisMode/frmPointEditor.cpp \
    AnalysisMode/frmTraceSetup.cpp \
    AnalysisMode/frmWKEAnalysisMode.cpp \
    AnalysisMode/MessageDialog.cpp \
    AnalysisMode/wk4300AnalysisMeter.cpp \
    AnalysisMode/wk4300Calibration.cpp \
    AnalysisMode/wk6500AnalysisMeter.cpp \
    AnalysisMode/wk6500Calibration.cpp \
    AnalysisMode/wk6500LevelInputBox.cpp \
    AnalysisMode/wk6500Range.cpp \
    AnalysisMode/wk6500Speed.cpp \
    AnalysisMode/WKEInstrument.cpp \
    Public/clsComplexOp.cpp \
    Public/clsRuningSettings.cpp \
    Public/clsSettings.cpp \
    Public/doubleType.cpp \
    Public/UserfulFunctions.cpp \
    connections/clsConnection.cpp \
    AnalysisMode/Plot.cpp \
    MeterMode/clsMeterLimit.cpp \
    MeterMode/frmSetLimit.cpp \
    Public/dlgLevelInputBox.cpp \
    Public/dlgSpeed.cpp \
    Public/NumberInput.cpp \
    Public/qexcel.cpp \
    AnalysisMode/frmWk3260.cpp \
    AnalysisMode/frmWK3260Calibration.cpp \
    AnalysisMode/frmParameter.cpp \
    common/frmAbout.cpp \
    saftyDog/DogControl.cpp \
    saftyDog/clsDog.cpp \
    AnalysisMode/frmPeakSearch.cpp \
    MeterMode/testLimits.cpp \
    ResonaceMode/wkResonaceMode.cpp \
    ResonaceMode/frmResoType.cpp \
    ResonaceMode/cls6500ResonaceMode.cpp \
    ResonaceMode/StatusLabel.cpp \
    ResonaceMode/clsControlBox.cpp \
    ResonaceMode/clsSignalThread.cpp \
    AnalysisMode/wk6440AnalysisMeter.cpp \
    AnalysisMode/dlgWk6440Function1.cpp \
    AnalysisMode/dlgWk6440Function2.cpp \
    AnalysisMode/wk6440Calibration.cpp \
    MeterMode/clsMeterMode.cpp \
    MeterMode/clsMyMessageLabel.cpp \
    MeterMode/singleStepTestDisplay.cpp \
    MeterMode/PassFailLabel.cpp \
    MeterMode/WKEMeterMode.cpp \
    MeterMode/cls6440MeterMode.cpp \
    MeterMode/clsMeterModeFactory.cpp \
    MulitChannalBox/clsConnectSWBox.cpp \
    MulitChannalBox/clsPortListener.cpp \
    MulitChannalBox/clsMultiChannaeBox.cpp \
    MulitChannalBox/clsSwitchBoxTest.cpp \
    MeterMode/dlg4300FuctionMajor.cpp \
    MeterMode/dlg4300FunctionMinor.cpp \
    AnalysisMode/dlgLimitSetup.cpp \
    AnalysisMode/dlgSetupOp.cpp \
    AnalysisMode/clsMultiLimitSetting.cpp \
    AnalysisMode/clsMultiLimits.cpp \
    MulitChannalBox/clsChennalSelect.cpp \
    MulitChannalBox/clsMRBDisplayPannel.cpp \
    AnalysisMode/clsMultiCurveLimt.cpp \
    AnalysisMode/clsMultiLimitStatusShow.cpp \
    AnalysisMode/clsCurveLimit.cpp \
    MeterMode/clsSetTestStep.cpp \
    MeterMode/clsMeterUnit.cpp \
    MeterMode/clsMeterModeSettings.cpp \
    MeterMode/cls6440Calibration.cpp \
    MeterMode/cls4100MeterMode.cpp \
    MeterMode/cls4100MeterModeCalibration.cpp \
    MeterMode/cls6500MeterMode.cpp \
    AnalysisMode/clsMarkerItem.cpp \
    MulitChannalBox/clsDataProcess.cpp \
    MulitChannalBox/clsCalibration.cpp \
    MulitChannalBox/complexType.cpp \
    MulitChannalBox/clsCalibrationDbOp.cpp \
    MulitChannalBox/clsMultiModeMeter.cpp \
    MulitChannalBox/cls6440MultiMeterMode.cpp \
    MulitChannalBox/clsMultModeMeterUi.cpp \
    MulitChannalBox/clsMultiChannelSettings.cpp \
    MulitChannalBox/multiModePlot.cpp \
    MulitChannalBox/clsWriteDataToFile.cpp \
    MulitChannalBox/clsWriteFileThread.cpp \
    MulitChannalBox/cls6500MultiMeterMode.cpp \
    MeterMode/clsTestResult.cpp \
    MeterMode/clsDataStore.cpp \
    MeterMode/clsShowReport.cpp \
    MeterMode/cls3260Calibration.cpp \
    MeterMode/cls3260MeterMode.cpp \
    MeterMode/cls3260majorfunction.cpp \
    MeterMode/cls3260minorfunction.cpp \
    MeterMode/clsStatistics.cpp \
    MeterMode/clsMyBarChartPlot.cpp \
    MeterMode/PlotDataAnlysis.cpp \
    MeterMode/PlotSettings.cpp \
    MeterMode/clsLog.cpp \
    AnalysisMode/clsBiasExtMeasument.cpp \
    AnalysisMode/clsGwPsw800.cpp \
    AnalysisMode/clsMyserialPort.cpp \
    AnalysisMode/clsSelectComport.cpp \
    AnalysisMode/clsMaterialSettings.cpp \
    AnalysisMode/clsBiasAOp.cpp \
    AnalysisMode/clsBiasAOperation.cpp \
    AnalysisMode/clsSampleTest.cpp \
    MulitChannalBox/clsStandardValueInput.cpp \
    AnalysisMode/clsRetryDialog.cpp \
    MulitChannalBox/clsSingleTrig.cpp \
    AnalysisMode/EaPs8000/clsEaPs8000.cpp \
    AnalysisMode/EaPs8000/clsEaSerialPort.cpp \
    AnalysisMode/Qextserial/qextserialport.cpp \
    AnalysisMode/Qextserial/qextserialport_win.cpp \
    AnalysisMode/clsPowerSupply.cpp \
    MeterMode/AddtionOperation/cls6500TurnOffScreen.cpp \
    MeterMode/clsGenerateSteps.cpp \
    MeterMode/AddtionOperation/cls6440TurnOffScreen.cpp

RESOURCES += \
    recFile/myRes.qrc

OTHER_FILES += \
    connections/ReadMe.txt \
    AnalysisMode/ProgramDescription.txt


DEFINES    += QT_DLL QWT_DLL

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QwtLib -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QwtLib  -lqwtd

LIBS += -L$$PWD/quazip/ -lquazip


QT+=axcontainer

HEADERS += \
    connections/clsGpib.h \
    connections/clsLan.h \
    connections/clsUsb.h \
    connections/Cnnt.h \
    connections/GPIB.h \
    connections/gpib488.h \
    connections/visa.h \
    connections/visatype.h \
    AnalysisMode/ClickedLabel.h \
    AnalysisMode/clsBiasAMeasument.h \
    AnalysisMode/clsBiasMeasument.h \
    AnalysisMode/clsFit.h \
    AnalysisMode/clsFreqMeasument.h \
    AnalysisMode/clsInputLimitCheck.h \
    AnalysisMode/clsMeas.h \
    AnalysisMode/clsMeasFactory.h \
    AnalysisMode/clsMeasmentSetup.h \
    AnalysisMode/clsMeterFacotry.h \
    AnalysisMode/clsRefTrace.h \
    AnalysisMode/clsRefTraceProperty.h \
    AnalysisMode/clsSelectedRef.h \
    AnalysisMode/clsSelectedRefProperty.h \
    AnalysisMode/clsTimeMeasument.h \
    AnalysisMode/clsViewResult.h \
    AnalysisMode/dlgFunction.h \
    AnalysisMode/dlgSort.h \
    AnalysisMode/dlgSpeed.h \
    AnalysisMode/frmPointEditor.h \
    AnalysisMode/frmTraceSetup.h \
    AnalysisMode/frmWKEAnalysisMode.h \
    AnalysisMode/MessageDialog.h \
    AnalysisMode/wk4300AnalysisMeter.h \
    AnalysisMode/wk4300Calibration.h \
    AnalysisMode/wk6500AnalysisMeter.h \
    AnalysisMode/wk6500Calibration.h \
    AnalysisMode/wk6500LevelInputBox.h \
    AnalysisMode/wk6500MeasSetup.h \
    AnalysisMode/wk6500Range.h \
    AnalysisMode/wk6500Speed.h \
    AnalysisMode/WKEInstrument.h \
    AnalysisMode/wkPlots.h \
    AnalysisMode/Zoomer.h \
    Public/clsComplexOp.h \
    Public/clsRuningSettings.h \
    Public/clsSettings.h \
    Public/doubleType.h \
    Public/Globle.h \
    Public/UserfulFunctions.h \
    common/frmSelectConnection.h \
    connections/clsConnection.h \
    AnalysisMode/Plot.h \
    MeterMode/clsMeterLimit.h \
    MeterMode/frmSetLimit.h \
    Public/dlgLevelInputBox.h \
    Public/dlgSpeed.h \
    Public/NumberInput.h \
    Public/qexcel.h \
    AnalysisMode/frmWk3260.h \
    AnalysisMode/WK3260.h \
    AnalysisMode/frmWK3260Calibration.h \
    AnalysisMode/frmParameter.h \
    common/frmAbout.h \
    saftyDog/DogControl.h \
    saftyDog/clsDog.h \
    saftyDog/dog_api.h \
    saftyDog/dog_vcode.h \
    AnalysisMode/frmPeakSearch.h \
    MeterMode/testLimits.h \
    ResonaceMode/wkResonaceMode.h \
    ResonaceMode/frmResoType.h \
    ResonaceMode/clsResonaceMode.h \
    ResonaceMode/cls6500ResonaceMode.h \
    ResonaceMode/StatusLabel.h \
    ResonaceMode/AduHid.h \
    ResonaceMode/clsControlBox.h \
    ResonaceMode/clsSignalThread.h \
    AnalysisMode/wk6440AnalysisMeter.h \
    AnalysisMode/dlgWk6440Function1.h \
    AnalysisMode/dlgWk6440Function2.h \
    AnalysisMode/wk6440Calibration.h \
    MeterMode/clsMeterMode.h \
    MeterMode/clsMyMessageLabel.h \
    MeterMode/singleStepTestDisplay.h \
    MeterMode/PassFailLabel.h \
    MeterMode/WKEMeterMode.h \
    MeterMode/cls6440MeterMode.h \
    MeterMode/clsMeterModeFactory.h \
    MulitChannalBox/clsConnectSWBox.h \
    MulitChannalBox/clsPortListener.h \
    MulitChannalBox/clsMultiChannaeBox.h \
    MulitChannalBox/clsSwitchBoxTest.h \
    MeterMode/dlg4300FuctionMajor.h \
    MeterMode/dlg4300FunctionMinor.h \
    AnalysisMode/dlgLimitSetup.h \
    AnalysisMode/dlgSetupOp.h \
    AnalysisMode/clsMultiLimitSetting.h \
    AnalysisMode/clsMultiLimits.h \
    MulitChannalBox/clsChennalSelect.h \
    MulitChannalBox/clsMRBDisplayPannel.h \
    AnalysisMode/clsMultiCurveLimt.h \
    AnalysisMode/clsMultiLimitStatusShow.h \
    AnalysisMode/clsCurveLimit.h \
    MeterMode/clsSetTestStep.h \
    MeterMode/clsMeterUnit.h \
    MeterMode/clsMeterModeSettings.h \
    MeterMode/cls6440Calibration.h \
    MeterMode/cls4100MeterMode.h \
    MeterMode/cls4100MeterModeCalibration.h \
    MeterMode/cls6500MeterMode.h \
    AnalysisMode/clsMarkerItem.h \
    MulitChannalBox/clsDataProcess.h \
    MulitChannalBox/clsCalibration.h \
    MulitChannalBox/complexType.h \
    MulitChannalBox/clsCalibrationDbOp.h \
    MulitChannalBox/clsMultiModeMeter.h \
    MulitChannalBox/cls6440MultiMeterMode.h \
    MulitChannalBox/clsMultModeMeterUi.h \
    MulitChannalBox/clsMultiChannelSettings.h \
    MulitChannalBox/multiModePlot.h \
    MulitChannalBox/clsWriteDataToFile.h \
    MulitChannalBox/clsWriteFileThread.h \
    MulitChannalBox/clsMultiChannelMeterFactory.h \
    MulitChannalBox/cls6500MultiMeterMode.h \
    MeterMode/clsTestResult.h \
    MeterMode/clsShowReport.h \
    MeterMode/clsDataStore.h \
    Public/singleton.h \
    MeterMode/cls3260Calibration.h \
    MeterMode/cls3260MeterMode.h \
    MeterMode/cls3260majorfunction.h \
    MeterMode/cls3260minorfunction.h \
    MeterMode/clsStatistics.h \
    MeterMode/clsMyBarChartPlot.h \
    MeterMode/PlotDataAnlysis.h \
    MeterMode/PlotSettings.h \
    MeterMode/clsLog.h \
    AnalysisMode/clsBiasExtMeasument.h \
    AnalysisMode/clsGwPsw800.h \
    AnalysisMode/clsMyserialPort.h \
    AnalysisMode/clsPowerSupply.h \
    AnalysisMode/clsSelectComport.h \
    AnalysisMode/clsMaterialSettings.h \
    AnalysisMode/clsBiasAOp.h \
    AnalysisMode/clsBiasAOperation.h \
    AnalysisMode/clsSampleTest.h \
    MulitChannalBox/clsStandardValueInput.h \
    AnalysisMode/clsRetryDialog.h \
    MulitChannalBox/clsSingleTrig.h \
    AnalysisMode/EaPs8000/clsEaPs8000.h \
    AnalysisMode/EaPs8000/clsEaSerialPort.h \
    AnalysisMode/Qextserial/qextserialport.h \
    AnalysisMode/Qextserial/qextserialport_global.h \
    AnalysisMode/Qextserial/qextserialport_p.h \
    MeterMode/AddtionOperation/cls6500TurnOffScreen.h \
    MeterMode/clsGenerateSteps.h \
    MeterMode/AddtionOperation/cls6440TurnOffScreen.h \
    MeterMode/AddtionOperation/clsAdditionOpFactory.h
FORMS += \
    common/frmSelectConnection.ui \
    AnalysisMode/clsFit.ui \
    AnalysisMode/clsSelectedRef.ui \
    AnalysisMode/clsSelectedRefProperty.ui \
    AnalysisMode/clsViewResult.ui \
    AnalysisMode/dlgFunction.ui \
    AnalysisMode/dlgSort.ui \
    AnalysisMode/frmPointEditor.ui \
    AnalysisMode/frmTraceSetup.ui \
    AnalysisMode/frmWKEAnalysisMode.ui \
    AnalysisMode/MessageDialog.ui \
    AnalysisMode/wk4300AnalysisMeter.ui \
    AnalysisMode/wk4300Calibration.ui \
    AnalysisMode/wk6500AnalysisMeter.ui \
    AnalysisMode/wk6500Calibration.ui \
    AnalysisMode/wk6500LevelInputBox.ui \
    AnalysisMode/wk6500Range.ui \
    AnalysisMode/wk6500Speed.ui \
    MeterMode/frmSetLimit.ui \
    Public/dlgLevelInputBox.ui \
    Public/dlgSpeed.ui \
    Public/NumberInput.ui \
    AnalysisMode/frmWk3260.ui \
    AnalysisMode/frmWK3260Calibration.ui \
    AnalysisMode/frmParameter.ui \
    common/frmAbout.ui \
    AnalysisMode/frmPeakSearch.ui \
    MeterMode/testLimits.ui \
    ResonaceMode/wkResonaceMode.ui \
    ResonaceMode/frmResoType.ui \
    AnalysisMode/wk6440AnalysisMeter.ui \
    AnalysisMode/dlgWk6440Function1.ui \
    AnalysisMode/dlgWk6440Function2.ui \
    AnalysisMode/wk6440Calibration.ui \
    MeterMode/clsMeterMode.ui \
    MeterMode/singleStepTestDisplay.ui \
    MeterMode/cls6440MeterMode.ui \
    MulitChannalBox/clsMultiChannaeBox.ui \
    MulitChannalBox/clsSwitchBoxTest.ui \
    MeterMode/dlg4300FuctionMajor.ui \
    MeterMode/dlg4300FunctionMinor.ui \
    AnalysisMode/dlgLimitSetup.ui \
    AnalysisMode/dlgSetupOp.ui \
    AnalysisMode/clsMultiLimitSetting.ui \
    MulitChannalBox/clsChennalSelect.ui \
    MulitChannalBox/clsMRBDisplayPannel.ui \
    AnalysisMode/clsMultiLimitStatusShow.ui \
    MeterMode/clsSetTestStep.ui \
    MeterMode/clsMeterUnit.ui \
    MeterMode/clsMeterModeSettings.ui \
    MeterMode/cls6440Calibration.ui \
    MeterMode/cls4100MeterMode.ui \
    MeterMode/cls4100MeterModeCalibration.ui \
    MeterMode/cls6500MeterMode.ui \
    AnalysisMode/clsMarkerItem.ui \
    MulitChannalBox/clsCalibration.ui \
    MulitChannalBox/clsMultModeMeterUi.ui \
    MulitChannalBox/clsMultiChannelSettings.ui \
    MeterMode/clsShowReport.ui \
    MeterMode/cls3260Calibration.ui \
    MeterMode/cls3260MeterMode.ui \
    MeterMode/cls3260majorfunction.ui \
    MeterMode/cls3260minorfunction.ui \
    MeterMode/clsStatistics.ui \
    AnalysisMode/clsSelectComport.ui \
    AnalysisMode/clsMaterialSettings.ui \
    AnalysisMode/clsBiasAOperation.ui \
    AnalysisMode/clsSampleTest.ui \
    MulitChannalBox/clsStandardValueInput.ui \
    AnalysisMode/clsRetryDialog.ui \
    MeterMode/clsGenerateSteps.ui


TRANSLATIONS = recFile/Translation/WKEFV_EN.ts \
               recFile/Translation/WKEFV_Zh.ts \
               recFile/Translation/WKEFV_Tw.ts \
               recFile/Translation/WKEFV_GM.ts \
               recFile/Translation/WKEFV_JP.ts
