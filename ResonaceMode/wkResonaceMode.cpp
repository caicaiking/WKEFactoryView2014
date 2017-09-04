#include "wkResonaceMode.h"
#include "frmResoType.h"
#include "dlgSpeed.h"
#include "NumberInput.h"
#include "doubleType.h"
#include "cls6500ResonaceMode.h"
#include <QDebug>
#include "UserfulFunctions.h"
#include "Globle.h"
#include "clsDog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include "frmAbout.h"
wkResonaceMode::wkResonaceMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    chkCp->setChecked(false);
    connect(this->lblLimit,SIGNAL(Clicked()),this,SLOT(setLimit()));
    connect(this->lblLimitFafr,SIGNAL(Clicked()),this,SLOT(setLimitFafr()));
    resMode = new cls6500ResonaceMode();
    resEqucct = resMode->getEqucct();
    resStart = resMode->getStart();
    resStop = resMode->getStop();
    resDepth = resMode->getDepth();
    resSpeed = resMode->getSpeed();
    cpChecked = resMode->getCheckCp();
    this->updateBtn();

    m1.setString("Norm,0,0,0,0,0");
    lblLimit->setText(m1.showLimits("Hz"));
    lblLimitFafr->setText(m2.showLimits("Hz"));
    lblStatus->setStatus(IDEL);

    thread = new clsSignalThread(this);
    connect(thread,SIGNAL(initOk()),this,SLOT(threadInfo()));
    connect(thread,SIGNAL(trigCaptured()),this,SLOT(trig()));
    thread->start(QThread::HighPriority);

    btnInfo->setVisible(false);
}


void wkResonaceMode::threadInfo()
{
    this->lblControlBox->setText(tr("消息：控制盒已连接！"));
}

void wkResonaceMode::trig()
{
    if(btnSearch->isEnabled())
    {
        btnSearch->click();
    }
}

void wkResonaceMode::updateBtn()
{
    btnEqucct->setText(resEqucct);
    btnSpeed->setText(resSpeed);
    btnDepth->setText(QString::number(resDepth));
    doubleType dt;
    dt.setData(resStart,"");
    btnStart->setText(dt.formateToString()+"Hz");
    dt.setData(resStop,"");
    btnStop->setText(dt.formateToString()+"Hz");
    chkCp->setChecked(cpChecked);
    resTypeChanged(resEqucct);
}

void wkResonaceMode::closeEvent(QCloseEvent *e)
{
    thread->stop();
    UserfulFunctions::sleepMs(10);

    e->accept();
}


void wkResonaceMode::on_chkCp_toggled(bool checked)
{
    this->lblCp1->setVisible(checked);
    this->lblCp->setVisible(checked);
}

void wkResonaceMode::on_btnEqucct_clicked()
{
    frmResoType *dlg = new frmResoType(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        resEqucct = dlg->getResonanceType();
        resMode->setEqucct(resEqucct);
        resEqucct=resMode->getEqucct();
        btnEqucct->setText(resEqucct);
        resTypeChanged(resEqucct);
    }
}

void wkResonaceMode::on_btnSpeed_clicked()
{
    dlgSpeed *dlg = new dlgSpeed(this);
    dlg->setWindowTitle(tr("设置谐振点搜索速度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        resSpeed=dlg->getSpeed();
        resMode->setSpeed(resSpeed);
        resSpeed =resMode->getSpeed();
        this->btnSpeed->setText(resSpeed);
    }
}

void wkResonaceMode::resTypeChanged(const QString &value)
{
    if(value ==tr("晶体谐振器"))
    {
        btnPreSearch->setEnabled(true);
        chkCp->setEnabled(true);
        grpFafr->setEnabled(true);
        btnDepth->setEnabled(false);
        btnDepth->setText("--");
        freqStackWindow->setCurrentIndex(1);
        lblKeff->setVisible(true);
        lblKeff1->setVisible(true);
        this->lblC0->setVisible(true);
        lblC01->setVisible(true);

        lblCp->setVisible(cpChecked && chkCp->isChecked());
        lblCp1->setVisible(cpChecked && chkCp->isChecked());
    }
    else
    {
        btnPreSearch->setEnabled(false);
        chkCp->setEnabled(false);
        grpFafr->setEnabled(false);
        btnDepth->setEnabled(true);
        btnDepth->setText(QString::number(resDepth));
        freqStackWindow->setCurrentIndex(0);
        grpFafr->setChecked(false);
        lblKeff->setVisible(false);
        lblKeff1->setVisible(false);
        this->lblC0->setVisible(false);
        lblC01->setVisible(false);

        lblCp->setVisible(false);
        lblCp1->setVisible(false);
    }
}

void wkResonaceMode::on_btnDepth_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("设置搜索深度"));

    if(dlg->exec()==QDialog::Accepted)
    {
        int data = (int)dlg->getNumber();
        data = (data>16?16:data);
        data = (data<0?0:data);
        resMode->setDepth(data);
        data= resMode->getDepth();
        btnDepth->setText(QString::number(data));
        resDepth = data;
    }
}

//设置搜索的开始频率
void wkResonaceMode::on_btnStart_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("设置开始频率"));

    if(dlg->exec()==QDialog::Accepted)
    {
        resStart = dlg->getNumber();
        resMode->setStart(resStart);

        resStart =resMode->getStart();

        doubleType dt;
        dt.setData(resStart,"");
        btnStart->setText(dt.formateToString()+"Hz");

    }
}

//设置搜索的终止频率
void wkResonaceMode::on_btnStop_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("设置终止频率"));

    if(dlg->exec()==QDialog::Accepted)
    {
        resStop = dlg->getNumber();
        resMode->setStop(resStop);
        resStop = resMode->getStop();
        doubleType dt;
        dt.setData(resStop,"");
        btnStop->setText(dt.formateToString()+"Hz");
    }
}

//预搜索按钮
void wkResonaceMode::on_btnPreSearch_clicked()
{

    this->btnPreSearch->setEnabled(false);

    int res =resMode->training().toInt();

    qDebug()<<"Pre training :" << res;

    if(res==1)
        QMessageBox::information(this,tr("预搜索结果"),tr("搜索范围有效"),QMessageBox::Ok);
    else
        QMessageBox::warning(this,tr("预搜索结果"),tr("搜索范围无效"),QMessageBox::Ok);

    this->btnPreSearch->setEnabled(true);
}

//开始搜索谐振点
void wkResonaceMode::on_btnSearch_clicked()
{
    QString strProductName;
    if((! SingletonDog::Instance()->getName(strProductName))|| (strProductName !="WKE FactoryView 2014"))
    {
        QMessageBox::warning(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("请插入加密狗！"));
        return;
    }
    QTime t1 = QTime::currentTime();
    btnSearch->setEnabled(false);
    lblStatus->setStatus(BUSY);
    qApp->processEvents();
    QString strRes =resMode->trig()+",,,,,,,,";
   // qDebug()<<strRes;
    QList<double>lstRes=UserfulFunctions::resultPro(strRes);

    status =true;
    doubleType dt;
    if(resEqucct==tr("串联") || resEqucct==tr("并联"))
    {
        dt.setData(lstRes.at(0),"");
        lblFreq->setText(dt.formateToString(9)+"Hz");
        dt.setData(lstRes.at(3),"");
        lblC->setText(dt.formateToString()+"F");
        dt.setData(lstRes.at(4),"");
        lblL->setText(dt.formateToString()+"H");
        dt.setData(lstRes.at(5),"");
        lblR->setText(dt.formateToString()+UserfulFunctions::getSuffix("R"));
        dt.setData(lstRes.at(6),"");
        lblQ->setText(dt.formateWithUnit("")+"");
        if(grpLimit->isChecked())
        {
            QString tmp;
            bool ok= m1.comparaValue(lstRes.at(0),tmp);
            lblFreqPassFail->setText((ok?tr("Pass"):tmp));

            status = status && ok;
            lblStatus->setStatus(status);
            writeData(lstRes.at(0),lstRes.at(3),lstRes.at(4),lstRes.at(5),lstRes.at(6),ok);
        }
        else
        {
            lblFreqPassFail->setText("");
            lblStatus->setStatus(IDEL);
            writeData(lstRes.at(0),lstRes.at(3),lstRes.at(4),lstRes.at(5),lstRes.at(6));
        }
    }
    else //Crystal 的结果显示
    {
        double fr, fa;
        fr = lstRes.at(0);
        fa = lstRes.at(1);

        dt.setData(fr);
        lblfr->setText(dt.formateToString(8)+"Hz");
        dt.setData(fa);
        lblfa->setText(dt.formateToString(8)+"Hz");

        dt.setData(lstRes.at(5));
        lblR->setText(dt.formateToString()+UserfulFunctions::getSuffix("R"));
        dt.setData(lstRes.at(3));
        lblC->setText(dt.formateToString()+"F");
        dt.setData(lstRes.at(4));
        lblL->setText(dt.formateToString()+"H");
        dt.setData(lstRes.at(2));
        lblC01->setText(dt.formateToString()+"F");
        dt.setData(lstRes.at(6));
        lblQ->setText(dt.formateWithUnit("")+"");
        double keff = resMode->getKeff();
        dt.setData(keff);
        lblKeff1->setText(dt.formateToString());

        double cp1=0;
        if(chkCp->isChecked())
        {
            cp1=resMode->getCp1K();
            dt.setData(cp1);
            lblCp1->setText(dt.formateToString()+"F");
        }

        QString strFmFn = resMode->getFmFn()+",,,,,";
        QList<double> dblFmFn = UserfulFunctions::resultPro(strFmFn);
        dt.setData(dblFmFn.at(0));
        lblfm->setText(dt.formateToString(8)+"Hz");
        dt.setData(dblFmFn.at(1));
        lblfn->setText(dt.formateToString(8)+"Hz");

        QString strFsFp = resMode->getFsFp()+",,,,,";
        QList<double> dblFsFp = UserfulFunctions::resultPro(strFsFp);
        dt.setData(dblFsFp.at(0));
        lblfs->setText(dt.formateToString(8)+"Hz");
        dt.setData(dblFsFp.at(1));
        lblfp->setText(dt.formateToString(8)+"Hz");

        dt.setData(fa-fr);
        lblfafr->setText(dt.formateToString(8)+"Hz");

        if(grpLimit->isChecked() || grpFafr->isChecked())
        {
            bool ok=true;
            QString tmp;
            if(grpLimit->isChecked())
            {
                bool ok1 = m1.comparaValue(fr,tmp);
                lblFrPassFail->setText(ok1?tr("Pass"):tmp);
                ok=ok && ok1;
            }
            else
                lblFrPassFail->setText("");

            if(grpFafr->isChecked())
            {
                bool ok2 = m2.comparaValue(fa-fr,tmp);
                lblFafrPassFail->setText(ok2?tr("Pass"):tmp);
                ok= ok && ok2;
            }
            else
                lblFafrPassFail->setText("");

            status = status && ok;
            lblStatus->setStatus(status);
            writeData(lstRes.at(5),lstRes.at(3),lstRes.at(4),lstRes.at(2),lstRes.at(6),keff,
                      cp1,fr,fa,dblFmFn.at(0),dblFmFn.at(1),dblFsFp.at(0),dblFsFp.at(1),
                      fa-fr,status);
        }
        else
        {
            lblStatus->setStatus(IDEL);
            lblFrPassFail->setText("");
            lblFafrPassFail->setText("");
            writeData(lstRes.at(5),lstRes.at(3),lstRes.at(4),lstRes.at(2),lstRes.at(6),keff,
                      cp1,fr,fa,dblFmFn.at(0),dblFmFn.at(1),dblFsFp.at(0),dblFsFp.at(1),
                      fa-fr,status);
        }
    }


    btnSearch->setEnabled(true);
    QTime t2 = QTime::currentTime();

    QString info=QString("%1: %2 ms").arg(tr("本次测试用时"))
            .arg(QString::number(t1.msecsTo(t2)));
    lblTime->setText(info);
}

void wkResonaceMode::setLimit()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定谐振频率上下限"));
    dlg->setLimits(m1);
    dlg->setItem("Hz");

    if(dlg->exec()==QDialog::Accepted)
    {
        m1 = dlg->getMeterLimit();
        lblLimit->setText(m1.showLimits("Hz"));

    }
}

void wkResonaceMode::setLimitFafr()
{
    frmSetLimit *dlg = new frmSetLimit(this);
    dlg->setWindowTitle(tr("设定fa-fr上下限"));
    dlg->setLimits(m2);
    dlg->setItem("Hz");

    if(dlg->exec()==QDialog::Accepted)
    {
        m2 = dlg->getMeterLimit();
        lblLimitFafr->setText(m2.showLimits("Hz"));

    }
}

void wkResonaceMode::on_btnShowSetup_clicked()
{
    if(btnShowSetup->isChecked())
    {
        grpSetup->setVisible(false);
        btnShowSetup->setText(tr("显示\n设置"));
    }
    else
    {
        grpSetup->setVisible(true);
        btnShowSetup->setText(tr("隐藏\n设置"));
    }
}

void wkResonaceMode::on_btnExit_clicked()
{
    this->close();
}


//打开数据文件按钮
void wkResonaceMode::on_btnSaveDataFile_clicked()
{
    //在这里保存数据文件

    strFilePath = QFileDialog::getSaveFileName(this,
                                               tr("打开数据文件"), "./", tr("CSV Files (*.csv)")
                                               ,0,QFileDialog::DontConfirmOverwrite);

    if(!strFilePath.isEmpty())
    {
        this->myStatusBar->showMessage(tr("数据文件：%1").arg(strFilePath));
        writeFileHead();
    }
}

//写入文件头
void wkResonaceMode::writeFileHead()
{
    if(strFilePath.isEmpty())
        return;

    QFile file(strFilePath);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine().trimmed();

    if(line.isEmpty())
    {
        if(resEqucct==tr("串联") || resEqucct==tr("并联"))
        {
            in<<tr("Time")<<","<<tr("Frequency") <<","
             <<tr("C")<<","<<tr("L")<<","
            <<tr("R")<<","<<tr("Q")<<","<<tr("Status\n");
        }
        else
        {
            in<<tr("Time")<<","<<tr("R")<<"," <<tr("C")<<","<<tr("L")<< "," <<tr("C0") <<","
             <<tr("Q") <<","<<tr("keff") <<","<<tr("Cp@1kHz") <<","
            <<"fr"<<","<<"fa"<<","<<"fm"<<","<<"fn"<<","<<"fs"<<","<<"fp"<<","<<"fa-fr"<<","<<tr("Status\n");
        }
    }

    in.flush();
    file.close();


}

//写入数据
void wkResonaceMode::writeData(double freq,double c, double l, double r, double q, bool status)
{
    if(strFilePath.isEmpty())
        return;
    QString strStatus;

    strStatus =(status? tr("Pass"):tr("Fail"));


    QFile file(strFilePath);
    if(!file.open(QIODevice::Append|QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    QString strDate = QString("%1 %2")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"))
            .arg(QTime::currentTime().toString("hh:mm:ss"));
    out<<strDate<<","<<QString::number(freq)<<","
      <<QString::number(c)<<","
     <<QString::number(l)<<","
    <<QString::number(r)<<","
    <<QString::number(q)<<","
    <<strStatus<<"\n";

    out.flush();
    file.close();

}

void wkResonaceMode::writeData(double r, double c, double l, double C0,
                               double q, double keff, double cp1,
                               double fr, double fa, double fm, double fn, double fs, double fp,double fafr, bool status)
{
    if(strFilePath.isEmpty())
        return;
    QString strStatus;

    strStatus =(status? tr("Pass"):tr("Fail"));


    QFile file(strFilePath);
    if(!file.open(QIODevice::Append|QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    QString strDate = QString("%1 %2")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"))
            .arg(QTime::currentTime().toString("hh:mm:ss"));
    out<<strDate<<","
      <<QString::number(r)<<","
     <<QString::number(c)<<","
    <<QString::number(l)<<","
    <<QString::number(C0)<<","
    <<QString::number(q)<<","
    <<QString::number(keff)<<","
    <<QString::number(cp1)<<","
    <<QString::number(fr)<<","
    <<QString::number(fa)<<","
    <<QString::number(fm)<<","
    <<QString::number(fn)<<","
    <<QString::number(fs)<<","
    <<QString::number(fp)<<","
    <<QString::number(fafr)<<","
    <<strStatus<<"\n";

    out.flush();
    file.close();

}

void wkResonaceMode::on_btnInfo_clicked()
{
    frmAbout dlg;
    dlg.setWindowTitle(tr("关于本软件"));
    dlg.exec();
}

void wkResonaceMode::on_chkCp_clicked()
{
    resMode->setCheckCp(chkCp->isChecked());
}
