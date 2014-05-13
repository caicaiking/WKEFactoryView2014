#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QDebug>
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "frmSelectConnection.h"
#include "frmWKEAnalysisMode.h"
#include "frmPointEditor.h"
#include "Globle.h"
//#include "frmWKEMeterMode.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QTextCodec::setCodecForTr(code);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QPixmap pixmap(":/Icons/splashScreen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.setFont(QFont("楷体",14, QFont::Bold));
    splash.showMessage(QObject::tr("正在初始化测试窗口，请稍等..."),Qt::AlignBottom |Qt::AlignRight,Qt::red);
    a.processEvents();

    UserfulFunctions::sleepMs(500);
    frmSelectConnection w;
    splash.finish(&w);


    if( w.exec()==QDialog::Accepted)
    {
        if(w.getMode()==Analysis)
        {
            frmWKEAnalysisMode analysis;
            analysis.showMaximized();
            return a.exec();
        }
        else if(w.getMode()==Meter)
        {/*
            frmWKEMeterMode meter;
            meter.show();*/
            return a.exec();
        }

    }
    else
        return 0;

}
