#include "clsCalibration.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include "MessageDialog.h"
#include "doubleType.h"
#include "clsDataProcess.h"
#include "clsCalibrationDbOp.h"
#include "NumberInput.h"
#include "clsStandardValueInput.h"
#include "clsSettings.h"
clsCalibration::clsCalibration(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    ocZ=OR;
    ocA=OR;
    scZ=OR;
    scA=OR;
    loadZ=OR;
    loadA=OR;
    stdA=OR;
    stdZ=OR;

    this->stkCalibration->setCurrentIndex(0);
    connect(this->lblswtich,SIGNAL(Clicked()),this,SLOT(changeWidget()));
}

void clsCalibration::setMeter(clsMultiModeMeter *meter)
{
    this->meter = meter;
    btnTest2Freq->setVisible(meter->getTest2On());
    doubleType dt;
    dt.setData(meter->getFreqency(0));
    btnTest1Freq->setText(dt.formateToString()+"Hz");
    dt.setData(meter->getFreqency(1));
    btnTest2Freq->setText(dt.formateToString()+"Hz");
    btnTest1Freq->setChecked(true);
    meter->setConditionForCalibration(0);
    meter->setFreqencyForCal(0);
    freq = meter->getFreqency(0);
}

void clsCalibration::setChannels(QStringList value)
{
    this->channels = value;
    cmbChannel->clear();
    this->cmbChannel_2->clear();

    for(int i=0; i< channels.length(); i++)
    {
        if(!channels.at(i).isEmpty())
        {
            cmbChannel->addItem(channels.at(i));
            cmbChannel_2->addItem(channels.at(i));
        }
    }
    clsConnectSWBox::Instance()->selectChannel(cmbChannel->currentText().toInt());
    clsConnectSWBox::Instance()->turnOffAllLight();
    clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(cmbChannel->currentText().toInt());
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());

    readSettings();

}



void clsCalibration::on_btnTest_clicked()
{


    QList<double> resValue =meter->getOriginZA();
    if(resValue.length()<2)
        return;

    double z,a;
    z = resValue.at(0);
    a = resValue.at(1);

    QList<double> openData = clsCalDb::getInst()->getCalData(freq,cmbChannel->currentText().toInt(),"O");
    QList<double> shortData = clsCalDb::getInst()->getCalData(freq,cmbChannel->currentText().toInt(),"S");
    QList<double> loadData = clsCalDb::getInst()->getCalData(freq,cmbChannel->currentText().toInt(),"Lm");
    QList<double> stdData = clsCalDb::getInst()->getCalData(freq,cmbChannel->currentText().toInt(),"Ls");

    clsDataProcess d(z,a,freq);

    if((openData.length() ==2) && (shortData.length() ==2))
    {
        d.applyOpenData(openData.at(0),openData.at(1));
        d.applyShortData(shortData.at(0),shortData.at(1));

        if((loadData.length()==2) &&
                (stdData.length()==2) &&
                chkUseLoadvalue->isChecked())
        {
            d.applyLoadData(loadData.at(0),loadData.at(1));
            d.applyStdData(stdData.at(0),stdData.at(1));
            d.useLoadData(true);
            d.doCalibration();
        }
        else
        {
            d.useLoadData(false);
            d.doCalibration();
        }
    }

    double c= d.getItem("C",QObject::tr("串联"));
    double dx= d.getItem("D",QObject::tr("串联"));

    doubleType dt;
    dt.setData(c);
    txtC->setText(dt.formateToString());
    dt.setData(dx);
    txtD->setText(dt.formateWithUnit(""));

    double l= d.getItem("L",QObject::tr("串联"));
    double r= d.getItem("R",QObject::tr("串联"));


    dt.setData(l);
    txtL->setText(dt.formateToString());
    dt.setData(r);
    txtR->setText(dt.formateToString());

    double z1 = d.getItem("Z",QObject::tr("串联"));
    dt.setData(z1);
    txtZ->setText(dt.formateToString());
    double a1= d.getItem("A",QObject::tr("串联"));
    dt.setData(a1);
    txtA->setText(dt.formateToString());
}

void clsCalibration::on_btnOpen_clicked()
{


    QList<double> resValue = meter->getOriginZA();

    doubleType dt;
    dt.setData(resValue.at(0));

    txtOpenZ->setText(dt.formateToString());
    ocZ = dt.Data();
    dt.setData(resValue.at(1));
    txtOpenA->setText(dt.formateToString());
    ocA = dt.Data();

    clsCalDb::getInst()->insertRecord(freq,cmbChannel->currentText().toInt(),
                                      ocZ,ocA,"O");
}

void clsCalibration::on_btnShort_clicked()
{


    QList<double> resValue = meter->getOriginZA();

    doubleType dt;
    dt.setData(resValue.at(0));

    txtShortZ->setText(dt.formateToString());
    scZ = dt.Data();
    dt.setData(resValue.at(1));
    txtShortA->setText(dt.formateToString());
    scA = dt.Data();
    clsCalDb::getInst()->insertRecord(freq,cmbChannel->currentText().toInt(),
                                      scZ,scA,"S");
}



void clsCalibration::on_btnLoad_clicked()
{
    QList<double> resValue = meter->getOriginZA();

    doubleType dt;
    dt.setData(resValue.at(0));

    txtLoadZ->setText(dt.formateToString());
    loadZ = dt.Data();
    dt.setData(resValue.at(1));
    txtLoadA->setText(dt.formateToString());
    loadA = dt.Data();

    clsCalDb::getInst()->insertRecord(freq,cmbChannel->currentText().toInt(),
                                      loadZ,loadA,"Lm");

}

void clsCalibration::on_btnTest1Freq_toggled(bool checked)
{
    if(!checked)
        return;
    meter->setConditionForCalibration(0);
    meter->setFreqencyForCal(0);
}

void clsCalibration::on_btnTest2Freq_toggled(bool checked)
{
    if(!checked)
        return;

    meter->setConditionForCalibration(1);
    meter->setFreqencyForCal(1);
}

void clsCalibration::on_btnNextChannel_clicked()
{
    int index = cmbChannel->currentIndex();
    cmbChannel->setCurrentIndex((index+1)%cmbChannel->count());
    clsConnectSWBox::Instance()->selectChannel(
                cmbChannel->currentText().toInt());

    clsConnectSWBox::Instance()->turnOffAllLight();
    clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(cmbChannel->currentText().toInt());

    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

/*!
 * \brief 查询数据库中的记录
 * \param freq 频率
 * \param channal 通道
 * \param type 类型
 */
QList<double> clsCalibration::getCalDataFromDb(
        double freq, int channal,QString type)
{
    return clsCalDb::getInst()->getCalData(freq,channal,type);
}

/*!
 * \brief 将校准数据写入数据库
 * \param freq 频率
 * \param channal 通道
 * \param z 阻抗
 * \param a 相位
 * \param type 类型 O 开路 S 短路 Lm负载量测值 Ls负载标准值
 */
void clsCalibration::insertRecord(
        double freq, int channal,
        double z,double a, QString type)
{
    clsCalDb::getInst()->insertRecord(freq,channal,z,a,type);
}

void clsCalibration::updataText()
{
    doubleType dt;

    dt.setData(ocZ);
    if(ocZ==OR)
        txtOpenZ->setText(tr("没有数据"));
    else
        txtOpenZ->setText(dt.formateToString());

    dt.setData(ocA);
    if(ocA==OR)
        txtOpenA->setText(tr("没有数据"));
    else
        txtOpenA->setText(dt.formateToString());

    dt.setData(scZ);
    if(scZ==OR)
        txtShortZ->setText(tr("没有数据"));
    else
        txtShortZ->setText(dt.formateToString());

    dt.setData(scA);
    if(scA==OR)
        txtShortA->setText(tr("没有数据"));
    else
        txtShortA->setText(dt.formateToString());

    dt.setData(loadZ);
    if(loadZ==OR)
        txtLoadZ->setText(tr("没有数据"));
    else
        txtLoadZ->setText(dt.formateToString());

    dt.setData(loadA);
    if(loadA==OR)
        txtLoadA->setText(tr("没有数据"));
    else
        txtLoadA->setText(dt.formateToString());

    dt.setData(stdZ);
    if(stdZ==OR)
        btnStdZ->setText(tr("点击输入"));
    else
        btnStdZ->setText(dt.formateToString());

    dt.setData(stdA);
    if(stdA==OR)
        btnStdA->setText(tr("点击输入"));
    else
        btnStdA->setText(dt.formateToString());
}

void clsCalibration::getAllDataFromDb(double freq, int channel)
{
    QList<double> tmp;
    tmp = clsCalDb::getInst()->getCalData(freq,channel,"O"); //开路值

    if(tmp.length() ==2)
    {
        ocZ = tmp.at(0);
        ocA = tmp.at(1);
    }
    else
    {
        ocZ=OR;
        ocA=OR;
    }

    tmp = clsCalDb::getInst()->getCalData(freq,channel,"S"); //短路值

    if(tmp.length() ==2)
    {
        scZ = tmp.at(0);
        scA = tmp.at(1);
    }
    else
    {
        scZ=OR;
        scA=OR;
    }

    tmp = clsCalDb::getInst()->getCalData(freq,channel,"Lm"); //短路值

    if(tmp.length() ==2)
    {
        loadZ = tmp.at(0);
        loadA = tmp.at(1);
    }
    else
    {
        loadZ=OR;
        loadA=OR;
    }

    tmp = clsCalDb::getInst()->getCalData(freq,channel,"Ls"); //短路值

    if(tmp.length() ==2)
    {
        stdZ = tmp.at(0);
        stdA = tmp.at(1);
    }
    else
    {
        stdZ=OR;
        stdA=OR;
    }

    updataText();

}

void clsCalibration::on_btnStdZ_clicked()
{
    NumberInput *dlg = new NumberInput();
    dlg->setWindowTitle(tr("输入标准阻抗"));

    if(dlg->exec()== QDialog::Accepted)
    {
        stdZ= dlg->getNumber();
        this->updataText();
        clsCalDb::getInst()->insertRecord(freq,cmbChannel->currentText().toInt(),
                                          stdZ,stdA,"Ls");

    }
}

void clsCalibration::on_btnStdA_clicked()
{
    NumberInput *dlg = new NumberInput();
    dlg->setWindowTitle(tr("输入标准相位角"));

    if(dlg->exec()== QDialog::Accepted)
    {
        stdA= dlg->getNumber();

        //qDebug()<< stdA;
        clsCalDb::getInst()->insertRecord(freq,cmbChannel->currentText().toInt(),
                                          stdZ,stdA,"Ls");
        this->updataText();
    }
}

void clsCalibration::on_btnTest2Freq_clicked()
{
    meter->setFreqencyForCal(1);
    freq = meter->getFreqency(1);
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnTest1Freq_clicked()
{
    meter->setFreqencyForCal(0);
    freq = meter->getFreqency(0);
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnClearOpen_clicked()
{
    clsCalDb::getInst()->deleteRecord(freq,cmbChannel->currentText().toInt(),"O");
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnClearShort_clicked()
{
    clsCalDb::getInst()->deleteRecord(freq,cmbChannel->currentText().toInt(),"S");
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnClearLoad_clicked()
{
    clsCalDb::getInst()->deleteRecord(freq,cmbChannel->currentText().toInt(),"Lm");
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnClearStdLoad_clicked()
{
    clsCalDb::getInst()->deleteRecord(freq,cmbChannel->currentText().toInt(),"Ls");
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());
}

void clsCalibration::on_btnInputAllStdValue_clicked()
{
    doubleType dt;
    dt.setData(freq);
    clsStandardValueInput *dlg = new clsStandardValueInput(this);
    dlg->setWindowTitle(tr("输入%1的标准负载值").arg(dt.formateToString()+"Hz"));
    dlg->setFrequency(freq);

    if(dlg->exec() == QDialog::Accepted)
    {
        double z = dlg->getZ();
        double a = dlg->getA();

        for(int i =0; i< cmbChannel->count(); i++)
            clsCalDb::getInst()->insertRecord(freq,cmbChannel->itemText(i).toInt(),
                                              z,a,"Ls");
        getAllDataFromDb(freq,cmbChannel->currentText().toInt());
    }
}

void clsCalibration::on_btnDone_clicked()
{
    this->close();
}

void clsCalibration::on_btnCancel_clicked()
{
    this->close();
}

void clsCalibration::on_btnLoadTrim_clicked()
{
    btnNextChannel_2->clicked();
    QList<double> calFrequencys = getCalFrequencys();
    QList<int> calChannels = this->getCalChannels();

    MessageDialog* dlg = new MessageDialog(this);
    dlg->setMessage(tr("将继电器控制盒调到仪器校准的通道。"),tr("负载校准"));
    dlg->layout()->setSizeConstraint(QLayout::SetFixedSize);
    if(dlg->exec()!=QDialog::Accepted)
        return;

    dlg->setMessage(tr("请放入标准负载"),tr("负载校准"));
    if(dlg->exec()!=QDialog::Accepted)
        return;


    for(int x =0; x<calFrequencys.length(); x++)
    {
        meter->setConditionForCalibration(x);
        meter->setFreqencyForCal(x);

        clsStandardValueInput * dlg = new clsStandardValueInput(this);
        doubleType dt;
        dt.setData(calFrequencys.at(x));
        dlg->setWindowTitle(tr("输入%1的标准负载值").arg(dt.formateToString()+"Hz"));
        dlg->setFrequency(calFrequencys.at(x));

        QList<double> resValue=meter->getOriginZA();
        stdZ =  resValue.at(0);
        stdA = resValue.at(1);
        dlg->setZ(stdZ);
        dlg->setA(stdA);
        if(dlg->exec()!=QDialog::Accepted)
            return;

        for(int j=0; j< calChannels.length(); j++)
        {
            clsCalDb::getInst()->insertRecord(calFrequencys.at(x),calChannels.at(j),
                                              stdZ,stdA,"Ls");
        }

    }

    for(int c=0; c< calChannels.length(); c++)
    {
        int currentChannel = calChannels.at(c);
        clsConnectSWBox::Instance()->selectChannel(currentChannel);
        clsConnectSWBox::Instance()->turnOffAllLight();
        clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(currentChannel);

        showCalMessage(tr("负载校准"),currentChannel);

        for(int f=0; f<calFrequencys.length(); f++)
        {
            showCalMessage(tr("负载校准"),calFrequencys.at(f),currentChannel);

            meter->setFreqencyForCal(f);
            meter->setConditionForCalibration(f);
            QList<double> resValue = meter->getOriginZA();
            loadZ=resValue.at(0);
            loadA=resValue.at(1);
            clsCalDb::getInst()->insertRecord(calFrequencys.at(f),currentChannel,
                                              loadZ,loadA,"Lm");

        }
    }
    setCalLabelInfo(lblHFTrim,tr("负载校准"));
}

void clsCalibration::on_btnOpenTrim_clicked()
{
    QList<double> calFrequencys = getCalFrequencys();
    QList<int> calChannels = this->getCalChannels();

    for(int c=0; c< calChannels.length(); c++)
    {
        int currentChannel = calChannels.at(c);
        clsConnectSWBox::Instance()->selectChannel(currentChannel);
        clsConnectSWBox::Instance()->turnOffAllLight();
        clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(currentChannel);

        showCalMessage(tr("开路校准"),currentChannel);

        for(int f=0; f<calFrequencys.length(); f++)
        {
            showCalMessage(tr("开路校准"),calFrequencys.at(f),currentChannel);
            meter->setFreqencyForCal(f);
            meter->setConditionForCalibration(f);
            QList<double> resValue = meter->getOriginZA();

            doubleType dt;
            dt.setData(resValue.at(0));
            ocZ = dt.Data();

            dt.setData(resValue.at(1));
            ocA = dt.Data();

            clsCalDb::getInst()->insertRecord(calFrequencys.at(f),currentChannel,
                                              ocZ,ocA,"O");

        }
        // 10kHz 为了高频校准
        meter->set10KHz();
        QList<double> resValue = meter->getOriginZA();

        clsCalDb::getInst()->insertRecord(10000.0,currentChannel,
                                          resValue.at(0),resValue.at(1),"OC_10k");

    }
    setCalLabelInfo(lblOpenTrim,tr("开路校准"));
}

void clsCalibration::on_btnShortTrim_clicked()
{
    QList<double> calFrequencys = getCalFrequencys();
    QList<int> calChannels = this->getCalChannels();

    for(int c=0; c< calChannels.length(); c++)
    {
        int currentChannel = calChannels.at(c);
        clsConnectSWBox::Instance()->selectChannel(currentChannel);
        clsConnectSWBox::Instance()->turnOffAllLight();
        clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(currentChannel);

        showCalMessage(tr("短路校准"),currentChannel);

        for(int f=0; f<calFrequencys.length(); f++)
        {
            showCalMessage(tr("短路校准"),calFrequencys.at(f),currentChannel);

            meter->setFreqencyForCal(f);
            meter->setConditionForCalibration(f);
            QList<double> resValue = meter->getOriginZA();

            doubleType dt;
            dt.setData(resValue.at(0));
            scZ = dt.Data();

            dt.setData(resValue.at(1));
            scA = dt.Data();

            clsCalDb::getInst()->insertRecord(calFrequencys.at(f),currentChannel,
                                              scZ,scA,"S");

        }

        // 10kHz 为了高频校准
        meter->set10KHz();
        QList<double> resValue = meter->getOriginZA();

        clsCalDb::getInst()->insertRecord(10000.0,currentChannel,
                                          resValue.at(0),resValue.at(1),"SC_10k");
    }
    setCalLabelInfo(lblShortTrim,tr("短路校准"));
}

QList<double> clsCalibration::getCalFrequencys()
{
    QList<double> tmp;
    tmp.append(meter->getFreqency(0));
    if(meter->getTest2On())
        tmp.append(meter->getFreqency(1));
    return tmp;

}
QList<int> clsCalibration::getCalChannels()
{
    QList<int> tmp;
    for(int i=0; i< channels.length(); i++)
    {
        if(!channels.at(i).isEmpty())
            tmp.append(channels.at(i).toInt());
    }

    return tmp;
}

int clsCalibration::showCalMessage(QString calType, double calFreq, int channel)
{
    this->lblCalbrationType->setText(calType);
    doubleType dt;
    dt.setData(calFreq,"");
    this->lblCalFrequency->setText(dt.formateToString()+"Hz");
    lblCalChannel->setText(QString::number(channel));
    qApp->processEvents();
    return 0;
}

int clsCalibration::showCalMessage(QString calType, int channel)
{
    MessageDialog *ms = new MessageDialog(this);

    ms->layout()->setSizeConstraint(QLayout::SetFixedSize);
    QString strMessage = tr("<p>现在第<strong><span style=\"font-size:16px;color:#E53333;\">%1</span></strong>通道</p>").arg(channel);
    ms->setMessage(strMessage,calType);
    return ms->exec();
}

void clsCalibration::setCalLabelInfo(QLabel *lbl, QString calType)
{
    lbl->setText(tr("%3时间为：%1 %2").arg(QDate::currentDate().toString("yyyy-M-dd"))
                 .arg(QTime::currentTime().toString("hh:mm:ss"))
                 .arg(calType));
    this->saveSettings();
}

void clsCalibration::on_btnNextChannel_2_clicked()
{
    clsConnectSWBox::Instance()->selectChannel(cmbChannel_2->currentText().toInt());
    clsConnectSWBox::Instance()->turnOffAllLight();
    clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(cmbChannel_2->currentText().toInt());
    saveSettings();
}

void clsCalibration::on_btnInstumentCal_clicked()
{
    meter->calibration();
}

void clsCalibration::readSettings()
{

    clsSettings settings;
    QString strNode ="MulitChannel/";
    QString tmp;
    settings.readSetting(strNode +"Cal-Channel",tmp);
    this->cmbChannel_2->setCurrentText(tmp);
    settings.readSetting(strNode +"OpenTrim", tmp);
    lblOpenTrim->setText(tmp);
    settings.readSetting(strNode + "ShortTrim",tmp);
    lblShortTrim->setText(tmp);
    settings.readSetting(strNode +"LoadTrim",tmp);
    lblHFTrim->setText(tmp);
    btnNextChannel_2->click();
}

void clsCalibration::saveSettings()
{
    clsSettings settings;
    QString strNode ="MulitChannel/";
    settings.writeSetting(strNode +"Cal-Channel",this->cmbChannel_2->currentText());
    settings.writeSetting(strNode +"OpenTrim", lblOpenTrim->text());
    settings.writeSetting(strNode +"ShortTrim",lblShortTrim->text());
    settings.writeSetting(strNode +"LoadTrim",lblHFTrim->text());
}

void clsCalibration::changeWidget()
{
    stkCalibration->setCurrentIndex(1);
}



void clsCalibration::on_btnRCLoadCalibration_clicked()
{
    MessageDialog *dlg = new MessageDialog(this);
    dlg->setMessage(tr("确定是否已经做完了开路和短路校准"),tr("阻容校准"));

    if(dlg->exec()!=QDialog::Accepted)
        return;

    dlg->setMessage(tr("放入负载100R"),tr("阻容校准"));
    dlg->exec();
    if(dlg->result()!=QDialog::Accepted)
        return;


    QList<double> calFrequencys = getCalFrequencys();
    QList<int> calChannels = this->getCalChannels();

    for(int c=0; c< calChannels.length(); c++)
    {
        int currentChannel = calChannels.at(c);
        clsConnectSWBox::Instance()->selectChannel(currentChannel);
        clsConnectSWBox::Instance()->turnOffAllLight();
        clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(currentChannel);

        showCalMessage(tr("100R校准"),currentChannel);

        meter->set10KHz();
        meter->setConditionForCalibration(0);
        QList<double> res = meter->getOriginZA();

        double Zm_100R_10K = res.at(0);
        double A_100R_10k = res.at(1);

        //获取开路和短路值
        //获取开路值
        QList<double> oc_10k = clsCalDb::getInst()->getCalData(10000.0,currentChannel,"OC_10k");
        //获取短路值
        QList<double> sc_10k = clsCalDb::getInst()->getCalData(10000.0,currentChannel,"SC_10k");

        clsDataProcess c100R(Zm_100R_10K,A_100R_10k,10000);
        if((oc_10k.length()==2)&&(sc_10k.length()==2))
        {
            c100R.applyOpenData(oc_10k.at(0),oc_10k.at(1));
            c100R.applyShortData(sc_10k.at(0),sc_10k.at(1));
            c100R.useLoadData(false);
            //c100R.doCalibration();
        }

        double z100R = c100R.getItem("Z",tr("串联"));
        double a100R = c100R.getItem("A",tr("串联"));

        clsCalDb::getInst()->insertRecord(10000,currentChannel,z100R,a100R,"HF_100RRef");


        for(int f=0; f<calFrequencys.length(); f++)
        {
            showCalMessage(tr("100R校准"),calFrequencys.at(f),currentChannel);

            meter->setFreqencyForCal(f);
            meter->setConditionForCalibration(f);
            QList<double> resValue = meter->getOriginZA();


            clsCalDb::getInst()->insertRecord(calFrequencys.at(f),currentChannel,
                                              resValue.at(0),resValue.at(1),"HF_C100R");

        }
    }
    //100pF校准
    dlg->setMessage(tr("放入负载100pF"),tr("阻容校准"));
    dlg->exec();
    if(dlg->result()!=QDialog::Accepted)
        return;




    for(int c=0; c< calChannels.length(); c++)
    {
        int currentChannel = calChannels.at(c);
        clsConnectSWBox::Instance()->selectChannel(currentChannel);
        clsConnectSWBox::Instance()->turnOffAllLight();
        clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(currentChannel);

        showCalMessage(tr("100pF校准"),currentChannel);

        meter->set10KHz();
        meter->setConditionForCalibration(0);
        QList<double> res = meter->getOriginZA();

        double Zm_100P_10K = res.at(0);
        double A_100P_10k = res.at(1);

        //获取开路和短路值
        //获取开路值
        QList<double> oc_10k = clsCalDb::getInst()->getCalData(10000.0,currentChannel,"OC_10k");
        //获取短路值
        QList<double> sc_10k = clsCalDb::getInst()->getCalData(10000.0,currentChannel,"SC_10k");

        clsDataProcess c100P(Zm_100P_10K,A_100P_10k,10000);
        if((oc_10k.length()==2)&&(sc_10k.length()==2))
        {
            c100P.applyOpenData(oc_10k.at(0),oc_10k.at(1));
            c100P.applyShortData(sc_10k.at(0),sc_10k.at(1));
            c100P.useLoadData(false);
            //c100P.doCalibration();
        }

        double z100P = c100P.getItem("Z",tr("串联"));
        double a100P = c100P.getItem("A",tr("串联"));

        clsCalDb::getInst()->insertRecord(10000,currentChannel,z100P,a100P,"HF_100PRef");


        for(int f=0; f<calFrequencys.length(); f++)
        {
            showCalMessage(tr("100P校准"),calFrequencys.at(f),currentChannel);

            meter->setFreqencyForCal(f);
            meter->setConditionForCalibration(f);
            QList<double> resValue = meter->getOriginZA();


            clsCalDb::getInst()->insertRecord(calFrequencys.at(f),currentChannel,
                                              resValue.at(0),resValue.at(1),"HF_C100P");

        }

    }
    setCalLabelInfo(lblRCTrime,tr("阻容校准"));

}
