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
    this->updateBtn();

    m1.setString("Norm,150000,142000,0,0,0");
    lblLimit->setText(m1.showLimits("Hz"));
    lblLimitFafr->setText(m2.showLimits("Hz"));
    lblStatus->setStatus(IDEL);

    thread = new clsSignalThread(this);
    connect(thread,SIGNAL(initOk()),this,SLOT(threadInfo()));
    connect(thread,SIGNAL(trigCaptured()),this,SLOT(trig()));
    thread->start(QThread::HighPriority);
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

void wkResonaceMode::on_btnPreSearch_clicked()
{
    qDebug()<<"Tranning res: "<<resMode->training();
}

void wkResonaceMode::on_btnSearch_clicked()
{
    QString strProductName;
    if((!clsDog::getName(strProductName))|| (strProductName !="WKE FactoryView 2014"))
    {
        QMessageBox::warning(0,QObject::tr("WKE FactoryView 2014"),QObject::tr("请插入加密狗！"));
        return;
    }
    QTime t1 = QTime::currentTime();
    btnSearch->setEnabled(false);
    lblStatus->setStatus(BUSY);
    qApp->processEvents();
    QString strRes =resMode->trig()+",,,,,,,,";
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
        }
        else
        {
            lblFreqPassFail->setText("");
            lblStatus->setStatus(IDEL);
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
        lblLimitFafr->setText("fa-fr:"+m2.showLimits("Hz"));

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
