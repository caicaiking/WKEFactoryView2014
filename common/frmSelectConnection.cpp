#include "frmSelectConnection.h"
#include "clsRuningSettings.h"
#include "clsSettings.h"
#include <QRegExp>

frmSelectConnection::frmSelectConnection(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->btnAnalysis->setEnabled(false);
    this->btnRansonace->setVisible(false);
    this->btnMeter->setEnabled(false);
    //    this->btnAppMode->setVisible(false);
    //    this->btnAppMode->setEnabled(false);
    //this->btnMeter->setVisible(false);
    this->btnMulitChannel->setEnabled(false);
    // this->btnMulitChannel->setVisible(false);
    this->btnAppMode->setVisible(false);

    rbTwLan->setVisible(false);
    intSelect=0;
    isReboot=0;
    readSettings();

    int selectLan = UserfulFunctions::getLanguage();

    switch (selectLan) {
    case 0:
        rbChinese->setChecked(true);
        break;
    case 1:
        rbEnglish->setChecked(true);
        break;
    case 2:
        rbTwLan->setChecked(true);
        break;
    default:
        break;
    }
}

void frmSelectConnection::on_btnTest_clicked()
{
    lblInformation->setText("");
    switch (intSelect) {
    case 0:
        clsRS::getInst().setConnectType("GPIB");
        break;
    case 1:
        clsRS::getInst().setConnectType("LAN");
        break;
    case 2:
        clsRS::getInst().setConnectType("USB");
        break;
    default:
        break;
    }

    QString newAddress;
    if(intSelect==1)
        newAddress =QString("%1,%2").arg(txtAddress->text()).arg(cmbModel->currentText());
    else
        newAddress = txtAddress->text();

    clsRS::getInst().setAddress(newAddress);
    bool isInit =clsRS::getInst().initConnection();

    if(!isInit)
    {
        this->lblInformation->setText(tr("初始化连接失败！"));
        return;
    }
    QString strReturn = clsRS::getInst().sendCommand("*IDN?",true);

    lblInformation->setText(tr("仪器ID：%1").arg(strReturn));

    QStringList ls = strReturn.split(",");

    //qDebug()<<ls;
    if(ls.length()>=2)
    {
        if(!(ls.at(0).contains("WAYNE")|| ls.at(0).contains("KUWAKI")))
            return;

        qDebug()<<"Instrument id " <<ls.at(1);
        if(!ls.at(1).trimmed().isEmpty())
        {
            clsRS::getInst().instrumentModel=ls.at(1).trimmed();

            QRegExp rx("[PMIA]*([0-9]+)([BPA]?[QL]{0,})");
            if(rx.exactMatch(clsRS::getInst().instrumentModel))
            {
                QRegExp rx4100 ("^4[13][0-9]*");
                if(rx4100.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterSeries="4300";
                    goto ToHere;
                }

                QRegExp rx6500 ("^65[0-9]*[BPA]");
                if(rx6500.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterSeries="6500";
                    goto ToHere;
                }

                QRegExp rx3260 ("^PMA3260[BA]");
                if(rx3260.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterSeries="3260";
                    goto ToHere;
                }

                QRegExp rx3255 ("^IA3255[BA][QL]{0,}");
                if(rx3255.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterSeries="3255";
                    goto ToHere;
                }

                QRegExp rx6440 ("^64[43]0");
                if(rx6440.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterSeries="6440";
                    goto ToHere;
                }


            }
        }
        else
            clsRS::getInst().instrumentModel="No Instrument!";


        qDebug()<< "Instrument mode: "<<clsRS::getInst().meterSeries;



    }

ToHere:
    qDebug()<< "clsRS::getInst().instrumentModel:\t"<<clsRS::getInst().instrumentModel;
    qDebug()<< "clsRS::getInst().meterSeries:\t\t"<<clsRS::getInst().meterSeries;
    buttonOp(isInit);
    writeSttings();
}

void frmSelectConnection::buttonOp(bool value)
{
    grpConnection->setEnabled(!value);
    btnAnalysis->setEnabled(value);
    btnMeter->setEnabled(value);
    btnMulitChannel->setEnabled(value);

    if(clsRS::getInst().meterSeries=="6500")
    {
        btnRansonace->setVisible(true);

    }

}
void frmSelectConnection::setIsReboot(int value)
{
    isReboot = value;
}

int frmSelectConnection::getIsReboot() const
{
    return isReboot;
}


void frmSelectConnection::writeSttings()
{
    clsSettings settings;

    QString strNode ="Connection/";
    settings.writeSetting(strNode + "Type",this->intSelect);

    switch (intSelect) {
    case 0:
        settings.writeSetting(strNode+"GPIBAddress",txtAddress->text());
        break;
    case 1:
        settings.writeSetting(strNode+"LANAddress",txtAddress->text()+","+cmbModel->currentText());
        break;
    case 2:
        settings.writeSetting(strNode+"USBAddress",txtAddress->text());
        break;
    default:
        break;
    }

}

void frmSelectConnection::readSettings()
{
    clsSettings settings;

    QString strNode ="Connection/";
    settings.readSetting(strNode + "Type",this->intSelect);


    switch (intSelect) {
    case 0:
        rbGpib->setChecked(true);
        settings.readSetting(strNode+"GPIBAddress",this->strAddress);
        cmbModel->setVisible(false);
        break;
    case 1:
        rbLan->setChecked(true);
        settings.readSetting(strNode+"LANAddress",this->strAddress);
        cmbModel->setVisible(true);
        break;
    case 2:
        rbUsb->setChecked(true);
        settings.readSetting(strNode+"USBAddress",this->strAddress);
        cmbModel->setVisible(false);
        break;
    default:
        break;
    }

    if(intSelect==1)
    {
        QStringList st= strAddress.split(",");
        if(st.length()>1 && (!st.at(1).isEmpty()))
            cmbModel->setCurrentText(st.at(1));

        this->txtAddress->setText(st.at(0));
    }
    else
    {
        txtAddress->setText(strAddress);
    }



    if(UserfulFunctions::getLanguage()==1)
    {
        rbEnglish->setChecked(true);
    }
}


void frmSelectConnection::on_rbUsb_toggled(bool checked)
{
    lblAddress->setVisible(!checked);
    txtAddress->setVisible(!checked);
    cmbModel->setVisible(false);
}

void frmSelectConnection::on_rbGpib_clicked()
{
    intSelect =0;

    clsSettings settings;
    QString strNode ="Connection/";
    settings.readSetting(strNode+"GPIBAddress",this->strAddress);
    this->txtAddress->setText(strAddress);
    cmbModel->setVisible(false);
}

void frmSelectConnection::on_rbLan_clicked()
{
    intSelect =1;

    clsSettings settings;
    QString strNode ="Connection/";
    settings.readSetting(strNode+"LANAddress",this->strAddress);

    QStringList st= strAddress.split(",");
    if(st.length()>1 && (!st.at(1).isEmpty()))
        cmbModel->setCurrentText(st.at(1));

    this->txtAddress->setText(st.at(0));

    cmbModel->setVisible(true);
}

void frmSelectConnection::on_rbUsb_clicked()
{
    intSelect =2;

    clsSettings settings;
    QString strNode ="Connection/";
    settings.readSetting(strNode+"USBAddress",this->strAddress);
    this->txtAddress->setText(strAddress);

}

void frmSelectConnection::closeEvent(QCloseEvent *)
{
}

void frmSelectConnection::on_rbLan_toggled(bool /*checked*/)
{
    // lblInformation->setVisible(checked);
}

void frmSelectConnection::on_btnAnalysis_clicked()
{
    this->mode = Analysis;
    this->accept();
}

void frmSelectConnection::on_btnMeter_clicked()
{
    this->mode =Meter;
    this->accept();
}

void frmSelectConnection::on_btnRansonace_clicked()
{
    this->mode = Resonace;
    this->accept();
}

void frmSelectConnection::on_btnMulitChannel_clicked()
{
    this->mode =MultiChannel;
    this->accept();
}



void frmSelectConnection::on_btnSwitch_clicked()
{
    if(rbChinese->isChecked())
        UserfulFunctions::setLanguage(0);
    else if(rbEnglish->isChecked())
        UserfulFunctions::setLanguage(1);
    else if(rbTwLan->isChecked())
        UserfulFunctions::setLanguage(2);
    else
    {
        //Do nonthing here
    }

    qApp->processEvents();
    UserfulFunctions::sleepMs(15);

    isReboot = 100;

    this->reject();
}

//void frmSelectConnection::on_btnAppMode_clicked()
//{
//    this->mode = AppMode;
//    this->accept();
//}
