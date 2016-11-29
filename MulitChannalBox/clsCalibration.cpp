#include "clsCalibration.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include "doubleType.h"
#include "clsDataProcess.h"
#include "clsCalibrationDbOp.h"
#include "NumberInput.h"
#include "clsStandardValueInput.h"
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

    for(int i=0; i< channels.length(); i++)
    {
        if(!channels.at(i).isEmpty())
            cmbChannel->addItem(channels.at(i));
    }
    clsConnectSWBox::Instance()->selectChannel(cmbChannel->currentText().toInt());
    clsConnectSWBox::Instance()->turnOffAllLight();
    clsConnectSWBox::Instance()->setOnlyOneOrangeLEDON(cmbChannel->currentText().toInt());
    getAllDataFromDb(freq,cmbChannel->currentText().toInt());

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
