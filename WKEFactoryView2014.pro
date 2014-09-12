#-------------------------------------------------
#
# Project created by QtCreator 2013-02-20T17:30:00
#
#-------------------------------------------------

QT       += core gui network
RC_FILE  =Icon.rc

 LIBS += dog_windows_3147311.lib
#//翻译文件使用的代码
CODECFORTR = UTF-8

INCLUDEPATH +=connections \
             common \
             AnalysisMode \
             Public \
             MeterMode \
             saftyDog \
             ResonaceMode

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WKEFactoryView2014
TEMPLATE = app

LIBS    += gpib488.obj \
           visa.lib \
           AduHid.lib


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
    MeterMode/clsUserFunctionMeterMode.cpp \
    MeterMode/clsMeterModeFactory.cpp


RESOURCES += \
    recFile/myRes.qrc

OTHER_FILES += \
    connections/ReadMe.txt \
    AnalysisMode/ProgramDescription.txt

# QWT 所需要的定义  ###################################################
DEFINES    += QT_DLL QWT_DLL

CONFIG(debug,debug|release):LIBS += -L $(QTDIR)/bin -lqwtd
else:LIBS += -L $(QTDIR)/bin  -lqwt

#######################################################################

RESOURCES +=

CONFIG += qaxcontainer

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
    MeterMode/clsUserFunctionMeterMode.h \
    MeterMode/clsMeterModeFactory.h


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
    MeterMode/clsUserFunctionMeterMode.ui

TRANSLATIONS = recFile/Translation/WKEFV.ts
