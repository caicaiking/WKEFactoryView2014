#include "frmWKEAnalysisMode.h"
#include <QBoxLayout>
#include <QFileDialog>
#include <Qwt/qwt_plot_renderer.h>
#include <QImageWriter>
#include "Qwt/qwt_picker_machine.h"
#include "frmPeakSearch.h"
#include "clsRuningSettings.h"
#include "frmTraceSetup.h"
#include "clsMeasFactory.h"
#include "UserfulFunctions.h"
#include "clsFit.h"
#include "clsSelectedRef.h"
#include "clsSelectedRefProperty.h"
#include "clsViewResult.h"
#include "frmPointEditor.h"
#include "clsMeterFacotry.h"
#include "frmAbout.h"
#include "clsDog.h"
#include <QMessageBox>
#include <QtMultimedia/QSound>
#include "dlgLimitSetup.h"
#include <QtMultimedia/QSound>
#include "dlgSetupOp.h"
frmWKEAnalysisMode::frmWKEAnalysisMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    initPlot();

    this->statusBar()->setVisible(false);
    progressBar->setVisible(false);
    lblDisplayMsg->setText(tr("仪器：WK %1").arg(clsRS::getInst().instrumentModel));

    init();

}


void frmWKEAnalysisMode::init()
{
    statusLabel->setStatus(IDEL);
    meter =clsMeterFacotry::getMeter(clsRS::getInst().meterSeries);
    if(meter==0)
        return;
    connectSignals();
    meter->updateInstrument();
    this->curveLimit.readSettings();
    plot->setCurveLimit(this->curveLimit);
    frmTraceSetup::readSettings(gs,true);

    meas = MeasFactory::getMeas(gs.sweepType);
    meas->setPoint(&gs.points);
    connect(meas,SIGNAL(showProgress(int)),this->progressBar,SLOT(setValue(int)));
    updateGraph();
    updateButtons();
    initZoomer();

    btnRep->setVisible(false);

}

void frmWKEAnalysisMode::initZoomer()
{
    d_zoomer = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
                           plot->canvas());
    d_zoomer->setRubberBand(QwtPicker::RectRubberBand);
    d_zoomer->setRubberBandPen(QColor(Qt::green));
    d_zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    d_zoomer->setTrackerPen(QColor(Qt::white));

    //    d_zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yRight,
    //         plot->canvas());
    connect(d_zoomer,SIGNAL(zoomed(QRectF)),this,SLOT(zoomed(QRectF)));



    enableZoomMode(false);
}

//接收来自meter的信号。
void frmWKEAnalysisMode::setFrequency(QString value)
{
    this->btnFreq->setText(value);
}
//接收来自meter的信号。
void frmWKEAnalysisMode::setLevel(QString value)
{
    this->btnLevel->setText(value);
}
//接收来自meter的信号。
void frmWKEAnalysisMode::setBias(bool value)
{
    btnBias->setChecked(value);
    if(value)
    {
        btnBias->setIcon(QIcon(":/Icons/BiasOn.png"));
    }
    else
    {
        btnBias->setIcon(QIcon(":/Icons/BiasOff.png"));
    }
}
//接收来自meter的信号。
void frmWKEAnalysisMode::setSpeed(QString value)
{
    this->btnSpeed->setText(value);
}

void frmWKEAnalysisMode::setItems(QString value1, QString value2)
{
    plot->setTraceA(value1);
    plot->setTraceB(value2);
    updateButtons();
}

void frmWKEAnalysisMode::setBias(QString value)
{
    this->btnBias->setText(tr("Bias\n%1").arg(value));
}

void frmWKEAnalysisMode::connectSignals()
{
    connect(meter,SIGNAL(frequencySignal(QString)),this,SLOT(setFrequency(QString)));
    connect(meter,SIGNAL(levelSignal(QString)),this,SLOT(setLevel(QString)));
    connect(meter,SIGNAL(biasStatusSignal(bool)),this,SLOT(setBias(bool)));
    connect(meter,SIGNAL(speedSignal(QString)),this,SLOT(setSpeed(QString)));
    connect(meter,SIGNAL(sweepItemSignal(QString,QString)),this,SLOT(setItems(QString,QString)));
    connect(meter,SIGNAL(biasValueSignal(QString)),this,SLOT(setBias(QString)));
}

void frmWKEAnalysisMode::updateGraph()
{
    QString st;
    st = UserfulFunctions::getSweepTypeName(gs.sweepType);

    plot->setTraces(meter->getItem1(),meter->getItem2(),st);

    if(meter->getItem1() !="Z" && meter->getItem1() !="Y")
        gs.logYleft=false;

    if(gs.sweepType !=Frequency)
        gs.logX=false;

    plot->setScale(gs.xmin,gs.xmax,gs.logX,
                   gs.yLeftMin,gs.yLeftMax,gs.logYleft,
                   gs.yRightMin,gs.yRightMax);

    plot->getScale(gs.xmin,gs.xmax,gs.logX,
                   gs.yLeftMin,gs.yLeftMax,gs.logYleft,
                   gs.yRightMin,gs.yRightMax);


    meter->saveSettings();
    frmTraceSetup::writeSettings(this->gs);


}

void frmWKEAnalysisMode::updateButtons()
{

    if(gs.sweepType==Frequency)
    {
        btnFreq->setVisible(false);
        btnLevel->setVisible(true);
    }
    else
    {
        btnFreq->setVisible(true);
        btnLevel->setVisible(true);
    }

    plot->setTitle(gs.title);

    //设置要关闭曲线的显示名称
    if(btnTraceA->isChecked())
        btnTraceA->setText(meter->getItem1()+"\n"+"ON");
    else
        btnTraceA->setText(meter->getItem1()+"\n"+"OFF");

    if(btnTraceB->isChecked())
        btnTraceB->setText(meter->getItem2()+"\n"+"ON");
    else
        btnTraceB->setText(meter->getItem2()+"\n"+"OFF");
}

//初始化Plot的界面
void frmWKEAnalysisMode::initPlot()
{
    //生成Plot的实例
    plot = new Plot;
    QHBoxLayout *layout = new QHBoxLayout;
    //添加Plot到主窗口中
    layout->addWidget(plot);

    plotWidget->setLayout(layout);

    //对比路径，第0 条曲线是默认的不可更改
    refTraces<<curveProperty(0)<<curveProperty(1)<<curveProperty(2)<<curveProperty(3)
            <<curveProperty(4)<<curveProperty(5)<<curveProperty(6)<<curveProperty(7)
           <<curveProperty(8)<<curveProperty(9)<<curveProperty(10);
    //设置第0条去曲线的属性
    refTraces[0]= curveProperty(0,Qt::cyan,QColor::fromRgb(255,215,0),tr("默认"),true);
    //将第0条曲线添加到Plot
    plot->addNewCurve(refTraces[0]);
    //将第0条曲线设置为最上面。
    plot->setToTop(refTraces[0],true);

    //拾取测试点
    d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                 QwtPlotPicker::NoRubberBand  , QwtPicker::AlwaysOn,
                                 plot->canvas());
    d_picker->setTrackerMode(QwtPicker::AlwaysOff );
    d_picker->setStateMachine(new QwtPickerDragPointMachine());
    d_picker->setRubberBandPen(QColor(Qt::red));
    d_picker->setRubberBand(QwtPicker::NoRubberBand  );
    d_picker->setTrackerPen(QColor(Qt::white));
    connect(d_picker,SIGNAL(moved(QPoint)),this,SLOT(moved(QPoint)));

}
void frmWKEAnalysisMode::enableZoomMode(bool on)
{
    d_zoomer->setEnabled(on);
    d_picker->setEnabled(!on);

}

void frmWKEAnalysisMode::zoomed(QRectF value)
{
    btnZoom->setChecked(false);
    enableZoomMode(false);

    double xmax,xmin;
    xmax = value.right();
    xmax = (xmax> gs.xmax? gs.xmax:xmax);

    xmin=value.left();
    xmin=(xmin<gs.xmin?gs.xmin:xmin);

    QString strMax, strMin;
    doubleType dt;
    dt.setData(xmax,"");
    strMax = dt.formateToString()+UserfulFunctions::getSweepFunctionSuffix(gs.sweepType);

    dt.setData(xmin,"");
    strMin = dt.formateToString()+UserfulFunctions::getSweepFunctionSuffix(gs.sweepType);


    int returnValue = QMessageBox::question(this,tr("曲线放大"),tr("放大范围：\n从%1到%2").arg(strMin).arg(strMax),QMessageBox::Ok|QMessageBox::Cancel);

    if(returnValue ==QMessageBox::Ok)
    {
        gs.xmax =xmax;
        gs.xmin=xmin;
        gs.points = frmPointEditor::getStaticPoints(gs.xmin,gs.xmax,gs.logX,gs.points.length());
        frmTraceSetup::writeSettings(gs);

        updateGraph();
        plot->clearData();
    }
    else
    {
        updateGraph();
        plot->repaint();
    }
}


void frmWKEAnalysisMode::showInfo(QString text)
{
    myStatusBar->showMessage(text);
}
void frmWKEAnalysisMode::moved(const QPoint &pos)
{
    QString info;

    info=  plot->setMarker(plot->invTransform(QwtPlot::xBottom, pos.x()));
    showInfo(info);
}
void frmWKEAnalysisMode::on_btnSavePic_clicked()
{

    QPixmap originalPixmap = QPixmap::grabWidget(this);

    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存测试图像"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(format.toUpper())
                                                    .arg(format));
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, "png");
}

void frmWKEAnalysisMode::on_btnTraceB_clicked()
{
    QString myLabel = btnTraceB->text();

    if(myLabel.contains(tr("ON")))
    {
        plot->turnOffCurves(Plot::yRight,false);
        myLabel.replace(tr("ON"),tr("OFF"));
    }
    else
    {
        plot->turnOffCurves(Plot::yRight,true);
        myLabel.replace(tr("OFF"),tr("ON"));
    }

    btnTraceB->setText(myLabel);

}

void frmWKEAnalysisMode::on_btnTraceA_clicked()
{
    QString myLabel = btnTraceA->text();

    if(myLabel.contains(tr("ON")))
    {
        plot->turnOffCurves(Plot::yLeft,false);
        myLabel.replace(tr("ON"),tr("OFF"));
    }
    else
    {
        plot->turnOffCurves(Plot::yLeft,true);
        myLabel.replace(tr("OFF"),tr("ON"));
    }

    btnTraceA->setText(myLabel);
}

void frmWKEAnalysisMode::on_btnMeasSetup_clicked()
{
    meter->readSettings();
    if(meter->exec()==QDialog::Accepted)
    {
        plot->clearData();
        updateButtons();

    }
}

void frmWKEAnalysisMode::on_btnBias_clicked()
{
    if(btnBias->isChecked())
    {
        meter->turnOnBias();
        btnBias->setIcon(QIcon(":/Icons/BiasOn.png"));
    }
    else
    {
        meter->turnOffBias();
        btnBias->setIcon(QIcon(":/Icons/BiasOff.png"));
    }
}

void frmWKEAnalysisMode::on_btnSpeed_clicked()
{
    meter->setSpeed("");
}

void frmWKEAnalysisMode::on_btnLevel_clicked()
{
    meter->setLevel(0,"");
}

void frmWKEAnalysisMode::on_btnFreq_clicked()
{
    meter->setFrequency(0);
}

void frmWKEAnalysisMode::on_btnTraceSetup_clicked()
{
    frmTraceSetup * dlg  = new frmTraceSetup(this->meter);
    dlg->setWindowTitle(tr("扫描设定"));
    if(dlg->exec() ==QDialog::Accepted)
    {
        if(!this->gs.equal( dlg->getGsetup()))
            plot->clearData();

        if(this->gs.sweepType!= dlg->getGsetup().sweepType)
        {
            plot->turnOffRefTrace();
        }

        this->gs = dlg->getGsetup();

        meas = MeasFactory::getMeas(gs.sweepType);

        meas->setPoint(&gs.points);
        // qDebug()<< gs.points;
        connect(meas,SIGNAL(showProgress(int)),this->progressBar,SLOT(setValue(int)));
        updateGraph();
        updateButtons();
    }
}

bool frmWKEAnalysisMode::checkDog()
{
    QString strProductName;
    if((!clsDog::getName(strProductName))|| (strProductName !="WKE FactoryView 2014"))
    {
        QMessageBox::warning(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("请插入加密狗！"));
        return false;
    }
    return true;
}

void frmWKEAnalysisMode::closeEvent(QCloseEvent *e)
{
    meas->stop();
    meter->turnOffBias();

    qApp->processEvents();
    e->accept();
}

void frmWKEAnalysisMode::on_btnTrig_clicked()
{
    // qDebug()<< "Iam in trig mode";
    if(! checkDog())
    {
        btnTrig->setChecked(false);
        return;
    }

    btnZoom->setChecked(false);
    enableZoomMode(false);
    progressBar->setValue(0);
    progressBar->setVisible(true);
    if(btnTrig->isChecked())
    {
        statusLabel->setStatus(BUSY);
        btnRep->setEnabled(false);
        btnTrig->setText(tr("停止\n测试"));
        btnTrig->setIcon(QIcon(":/Icons/stop.png"));
        meas->setMax(gs.xmax);
        meas->setMin(gs.xmin);
        meas->setXLog(gs.logX);
        meas->setItemsAndPoints(meter->getItem1(),meter->getItem2(),&gs.points);
        meas->setMeter(this->meter);
        meas->setPlot(plot);
        meas->setGraphSettings(gs);
        meas->trig();
        btnTrig->setText(tr("开始\n测试"));
        btnTrig->setIcon(QIcon(":/Icons/single.png"));
        btnTrig->setChecked(false);
        updateButtons();
        btnRep->setEnabled(true);

        resPassFail();
    }
    else
    {
        btnTrig->setText(tr("开始\n测试"));
        btnTrig->setIcon(QIcon(":/Icons/single.png"));
        meas->stop();
        btnRep->setEnabled(true);
    }
    progressBar->setVisible(false);
    //安全起见，请关闭Bias。
    meter->turnOffBias();
    gs.points = frmPointEditor::rmvPP(gs.points);
    frmTraceSetup::writeSettings(gs);



}

void frmWKEAnalysisMode::resPassFail()
{
    bool traceAStatus =true;
    bool traceBStatus = true;

    QList<PlotCurves> curves= plot->getCurves();
    if(curves.length()<=0)
        return;


    QList<QPointF> traceA=  UserfulFunctions::getPlotCurveData(curves.at(0).cur1);
    QList<QPointF> traceB=  UserfulFunctions::getPlotCurveData(curves.at(0).cur2);


    if(this->curveLimit.blTraceALimit == false && this->curveLimit.blTraceBLimit==false)
    {
        statusLabel->setStatus(IDEL);

    }
    else
    {

        QString cmpString;
        if(this->curveLimit.blTraceALimit)
        {
            for(int i=0; i< traceA.length();i++)
            {
                traceAStatus = traceAStatus && this->curveLimit.cmlTraceALimit.comparaValue(traceA.at(i).y(),cmpString);
                if(traceAStatus==false)
                    break;
            }
        }

        if(this->curveLimit.blTraceBLimit)
        {
            for(int i=0; i<traceB.length();i++)
            {
                traceBStatus = traceBStatus && this->curveLimit.cmlTraceBLimit.comparaValue(traceB.at(i).y(),cmpString);
            }
        }

        if(traceAStatus && traceBStatus)
        {
            statusLabel->setStatus(PASS);
            QSound::play(":/Sounds/pass.wav");
        }
        else
        {
            statusLabel->setStatus(FAIL);
            QSound::play(":/Sounds/fail.wav");
        }
    }

    //保存数据到文件
    if(!strDataFilePath.isEmpty())
    {
        QFile file(this->strDataFilePath);
        if(!file.open(QIODevice::Append|QIODevice::Text))
        {
            return;
        }

        QTextStream out(&file);
        // out.setCodec("ANSI");

        QString strStatus =(traceAStatus && traceBStatus ?"通过":"失败");

        QString strDate = tr("日期:,%1,时间:,%2,状态:,%3")
                .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                .arg(QTime::currentTime().toString("hh:mm:ss"))
                .arg(strStatus);

        out<<strDate<<"\n";

        QString title = tr("%1,%2,%3").arg(UserfulFunctions::getSweepTypeName(gs.sweepType))
                .arg(UserfulFunctions::getName(meter->getItem1()))
                .arg(UserfulFunctions::getName(meter->getItem2()));
        out<<title<<"\n";

        for(int i=0; i< traceA.length();i++)
        {
            QString data =tr("%1,%2,%3").arg(QString::number(traceA.at(i).x()))
                    .arg(QString::number(traceA.at(i).y()))
                    .arg(QString::number(traceB.at(i).y()));
            out<< data<<"\n";
        }

        out.flush();
        file.close();
    }
    //保存数据到文件结束

}

void frmWKEAnalysisMode::on_btnRep_clicked()
{
    btnTrig->setEnabled(false);
    btnRep->setText(tr("停止\n测试"));
    btnRep->setIcon(QIcon(":/Icons/stop.png"));
    btnZoom->setChecked(false);
    enableZoomMode(false);
    progressBar->setValue(0);
    progressBar->setVisible(true);
    while(btnRep->isChecked())
    {

        meas->setMax(gs.xmax);
        meas->setMin(gs.xmin);
        meas->setXLog(gs.logX);
        meas->setItemsAndPoints(meter->getItem1(),meter->getItem2(),&gs.points);
        meas->setMeter(this->meter);
        meas->setGraphSettings(gs);
        meas->setPlot(plot);
        meas->trig();

        //去除重复的点，然后写入配置文件。
        gs.points = frmPointEditor::rmvPP(gs.points);
        frmTraceSetup::writeSettings(gs);

    }
    btnRep->setText(tr("重复\n测试"));
    btnRep->setIcon(QIcon(":/Icons/repeat.png"));
    meas->stop();
    btnTrig->setEnabled(true);
    progressBar->setVisible(false);
    //安全起见，请关闭Bias。
    //meter->turnOffBias();

}

void frmWKEAnalysisMode::on_btnAutoScale_clicked()
{
    clsFit *fit = new clsFit(this);
    fit->setTraceTitle(meter->getItem1(),meter->getItem2());
    if(fit->exec()==QDialog::Accepted)
    {
        plot->autoScale(fit->getChoice());
        plot->getScale(gs.xmin,gs.xmax,gs.logX,
                       gs.yLeftMin,gs.yLeftMax,gs.logYleft,
                       gs.yRightMin,gs.yRightMax);

        frmTraceSetup::writeSettings(this->gs);
    }

}



void frmWKEAnalysisMode::on_btnRefTrace_clicked()
{
    clsSelectedRef *selectRef= new clsSelectedRef(this);

    if(selectRef->exec()!=QDialog::Accepted)
        return;

    clsSelectedRefProperty *selectedRefProperty=
            new clsSelectedRefProperty(refTraces[selectRef->getSelected()],this);

    connect(selectedRefProperty,SIGNAL(refTraceChanged(curveProperty,bool)),
            plot,SLOT(addNewCurve(curveProperty,bool)));

    if( selectedRefProperty->exec()!= QDialog::Accepted)
        return;

    refTraces[selectRef->getSelected()]=selectedRefProperty->getCurveProperty();


}

void frmWKEAnalysisMode::on_btnSaveResults_clicked()
{
    QStringList headers;
    headers << UserfulFunctions::getSweepTypeName(gs.sweepType)+"("+UserfulFunctions::getSweepTypeUnit(gs.sweepType)+")"
            << UserfulFunctions::getName(meter->getItem1())+"("+UserfulFunctions::getSuffix(meter->getItem1())+")"
            << UserfulFunctions::getName(meter->getItem2())+"("+UserfulFunctions::getSuffix(meter->getItem2())+")";


    clsViewResult *viewResult= new clsViewResult(plot->getCurves(),headers,this);
    viewResult->setParameters(meter->getItem1(),meter->getItem2(),meter->getEqucct(),gs.sweepType);
    connect(viewResult,SIGNAL(setMark(double,int)),plot,SLOT(setMarker(double,int)));
    viewResult->exec();

}

void frmWKEAnalysisMode::on_btnCalibration_clicked()
{
    meter->calibration();
}

void frmWKEAnalysisMode::on_btnZoom_clicked()
{
    enableZoomMode(btnZoom->isChecked());
}

void frmWKEAnalysisMode::on_btnHelp_clicked()
{
    frmAbout *dlg = new frmAbout();
    dlg->setWindowTitle(tr("关于本软件"));
    dlg->exec();
}

void frmWKEAnalysisMode::on_btnPeak_clicked()
{
    frmPeakSearch *dlg = new frmPeakSearch(this->plot,this->gs,this);
    dlg->setGeometry(this->geometry().left()+ btnMeasSetup->frameGeometry().left(),
                     this->geometry().top()+btnMeasSetup->frameGeometry().top()/*+5+(btnPeak->height()/2)*/,
                     btnTraceB->geometry().right()-btnMeasSetup->geometry().left(),
                     dlg->geometry().height());

    dlg->show();
}



void frmWKEAnalysisMode::on_btnSetLimit_clicked()
{
    dlgLimitSetup *limit=new dlgLimitSetup(this->meter,this);
    limit->setCurveLimit(this->curveLimit);
    if(limit->exec())
    {
        this->curveLimit = limit->getCurveLimit();
        this->curveLimit.writeSettings();
        plot->setCurveLimit(this->curveLimit);
        //        plot
    }
    else
    {
        qDebug()<<"limit cancel";
    }
}

void frmWKEAnalysisMode::on_btnSaveRes_clicked()
{
    //在这里保存数据文件

    QString  strFilePath = QFileDialog::getSaveFileName(this,
                                                        tr("打开数据文件"), "./", tr("CSV Files (*.csv)")
                                                        ,0,QFileDialog::DontConfirmOverwrite);

    if(!strFilePath.isEmpty())
    {
        this->strDataFilePath = strFilePath;
    }
}

void frmWKEAnalysisMode::on_btnSettings_clicked()
{
    dlgSetupOp dlg;
    dlg.setWindowTitle(tr("配置文件操作"));
    if(dlg.exec()==QDialog::Accepted)
    {
        int select = dlg.getSelect();
        QString filePath = dlg.getSelectFilePath();

        if(select ==0)
        {
            QFile file("./Settings.ini");
            if(file.exists())
            {
                qDebug()<<"old setting file is removed: "<< file.remove();
            }

            QFile::copy(filePath,"./Settings.ini");
            init();
            plot->clearData();
        }
        else
        {
            //qDebug()<<QFile::exists("./Settings.ini");
            QFile::copy("./Settings.ini",filePath);

        }
    }
}
