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
#include <QTranslator>
#include "testLimits.h"
#include "wkResonaceMode.h"
#include "clsMeterMode.h"
#include "clsDog.h"
#include "clsMeterMode.h"
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
//    QTextCodec::setCodecForTr(code);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //这儿是加载英文的翻译，如果要是实用界面为英文，请去掉此处的注释
    /*   QTranslator translator;
    bool ok=   translator.load(":/Translation/WKEFV.qm");
    a.installTranslator(&translator);
    qDebug()<< ok; */


    //test begin

//        clsMeterMode meter;
//        meter.showMaximized();
//       return a.exec();

    //for test

    QPixmap pixmap(":/Icons/splashScreen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.setFont(QFont("楷体",14, QFont::Bold));
    splash.showMessage(QObject::tr("正在初始化测试窗口，请稍等..."),Qt::AlignBottom |Qt::AlignRight,Qt::red);
    a.processEvents();

    UserfulFunctions::sleepMs(500);
    frmSelectConnection w;
    splash.finish(&w);




    QString strProductName;
    if((!clsDog::getName(strProductName))|| (strProductName !="WKE FactoryView 2014"))
    {
        QMessageBox::warning(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("请插入加密狗！"));
        return 0;
    }

    if( w.exec()==QDialog::Accepted)
    {
        if(w.getMode()==Analysis)
        {
            frmWKEAnalysisMode analysis;
            analysis.showMaximized();
            return a.exec();
        }
        else if(w.getMode()==Meter)
        {
            clsMeterMode meter;
            meter.show();
            return a.exec();
        }
        else if(w.getMode()==Resonace)
        {
            wkResonaceMode resonace;
            resonace.show();
            return a.exec();
        }
    }
    else
        return 0;
}
