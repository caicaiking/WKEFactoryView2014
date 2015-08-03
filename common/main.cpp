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
#include "clsMultiChannaeBox.h"
#include <QMessageBox>
#include "clsDataProcess.h"
#include "doubleType.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);


    QApplication::addLibraryPath("./plugins");
    //    QTextCodec::setCodecForTr(code);
    //    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //如果要是发布无锁版本，请使用clsDog::setProduct(false),有锁版本clsDog::setProduct(true)

    clsDog::setProduct(false);

    //这儿是加载英文的翻译，如果要是实用界面为英文，请去掉此处的注释
RELOAD:
    QTranslator translator;
    if(UserfulFunctions::getLanguage()==1)
        translator.load(":/Translation/WKEFV_EN.qm");
    else
        translator.load(":/Translation/WKEFV_ZH.qm");

    a.installTranslator(&translator);

    //    qDebug()<< "Tranlate file loaded " << ok;

    //设置Splash 屏幕
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

RESELECT:

    if( w.exec()==QDialog::Accepted)
    {
        if(w.getMode()==Analysis)
        {
            frmWKEAnalysisMode analysis;
            analysis.showMaximized();
            a.exec();
            goto RESELECT;
        }
        else if(w.getMode()==Meter)
        {
            clsMeterMode meter;
            meter.show();
            a.exec();
            goto RESELECT;
        }
        else if(w.getMode()==Resonace)
        {
            wkResonaceMode resonace;
            resonace.show();
            a.exec();
            goto RESELECT;
        }
        else if(w.getMode() == MultiChannel)
        {
            clsMultiChannaeBox multiChannel;
            multiChannel.show();
            a.exec();
            goto RESELECT;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(w.getIsReboot()==100)
        {
            w.setIsReboot(0);
            w.close();
            goto RELOAD;
        }
        else
            return 0;
    }
}
