#-------------------------------------------------
#
# Project created by QtCreator 2013-02-20T17:30:00
#
#-------------------------------------------------

QT       += core gui network
RC_FILE  =Icon.rc
LIBS     += -lqextserialportd

INCLUDEPATH +=connections \
             common \
             AnalysisMode \
             Public \
             MeterMode

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WKEFactoryView2014
TEMPLATE = app

LIBS    += gpib488.obj \
        visa.lib


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
    AnalysisMode/dlgSpeed.cpp \
    AnalysisMode/frmPointEditor.cpp \
    AnalysisMode/frmTraceSetup.cpp \
    AnalysisMode/frmWKEAnalysisMode.cpp \
    AnalysisMode/MessageDialog.cpp \
    AnalysisMode/qexcel.cpp \
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
    AnalysisMode/dlgLevelInputBox.cpp \
    AnalysisMode/NumberInput.cpp \
    AnalysisMode/Plot.cpp \
    MeterMode/clsMeterLimit.cpp \
    MeterMode/frmSetLimit.cpp


RESOURCES += \
    recFile/myRes.qrc

OTHER_FILES += \
    connections/ReadMe.txt
DEFINES    += QT_DLL QWT_DLL

CONFIG(debug,debug|release):LIBS+=-L $(QTDIR) -lqwtd
else:LIBS+=-L $(QTDIR) -lqwt

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
    AnalysisMode/qexcel.h \
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
    AnalysisMode/dlgLevelInputBox.h \
    AnalysisMode/NumberInput.h \
    AnalysisMode/Plot.h \
    MeterMode/clsMeterLimit.h \
    MeterMode/frmSetLimit.h

FORMS += \
    common/frmSelectConnection.ui \
    AnalysisMode/clsFit.ui \
    AnalysisMode/clsSelectedRef.ui \
    AnalysisMode/clsSelectedRefProperty.ui \
    AnalysisMode/clsViewResult.ui \
    AnalysisMode/dlgFunction.ui \
    AnalysisMode/dlgSort.ui \
    AnalysisMode/dlgSpeed.ui \
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
    AnalysisMode/dlgLevelInputBox.ui \
    AnalysisMode/NumberInput.ui \
    MeterMode/frmSetLimit.ui

