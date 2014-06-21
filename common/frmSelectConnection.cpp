#include "frmSelectConnection.h"
#include "clsRuningSettings.h"
#include "clsSettings.h"
#include <QRegExp>

frmSelectConnection::frmSelectConnection(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->btnAnalysis->setEnabled(false);
    this->btnMeter->setEnabled(false);
    this->btnMeter->setVisible(false);
    intSelect=0;

    readSettings();
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

    clsRS::getInst().setAddress(txtAddress->text());
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
        if(!ls.at(0).contains("WAYNE"))
            return;

        qDebug()<<"Instrument id " <<ls.at(1);
        if(!ls.at(1).trimmed().isEmpty())
        {
            clsRS::getInst().instrumentModel=ls.at(1).trimmed();

            QRegExp rx("[PMIA]*([0-9]+)([BPA]?Q{0,})");
            if(rx.exactMatch(clsRS::getInst().instrumentModel))
            {
                QRegExp rx4100 ("^4[13][0-9]*");
                if(rx4100.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterMode="4300";
                    goto ToHere;
                }

                QRegExp rx6500 ("^65[0-9]*[BPA]");
                if(rx6500.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterMode="6500";
                    goto ToHere;
                }

                QRegExp rx3260 ("^PMA3260[BA]");
                if(rx3260.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterMode="3260";
                    goto ToHere;
                }

                QRegExp rx3255 ("^IA3255[BA]Q{0,}");
                if(rx3255.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterMode="3255";
                    goto ToHere;
                }

                QRegExp rx6440 ("^64[43]0");
                if(rx6440.exactMatch(clsRS::getInst().instrumentModel))
                {
                    clsRS::getInst().meterMode="6440";
                    goto ToHere;
                }


            }
        }
        else
            clsRS::getInst().instrumentModel="No Instrument!";


        qDebug()<< "Instrument mode: "<<clsRS::getInst().meterMode;

    }

ToHere:
    qDebug()<< "Instrument mode:\t\t"<<clsRS::getInst().instrumentModel;
    qDebug()<< "Instrument category:\t"<<clsRS::getInst().meterMode;
    buttonOp(isInit);
    writeSttings();
}

void frmSelectConnection::buttonOp(bool value)
{
    grpConnection->setEnabled(!value);
    btnAnalysis->setEnabled(value);
    btnMeter->setEnabled(value);
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
        settings.writeSetting(strNode+"LANAddress",txtAddress->text());
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
        break;
    case 1:
        rbLan->setChecked(true);
        settings.readSetting(strNode+"LANAddress",this->strAddress);
        break;
    case 2:
        rbUsb->setChecked(true);
        settings.readSetting(strNode+"USBAddress",this->strAddress);
        break;
    default:
        break;
    }

    txtAddress->setText(strAddress);

}


void frmSelectConnection::on_rbUsb_toggled(bool checked)
{
    lblAddress->setVisible(!checked);
    txtAddress->setVisible(!checked);
}

void frmSelectConnection::on_rbGpib_clicked()
{
    intSelect =0;

    clsSettings settings;
    QString strNode ="Connection/";
    settings.readSetting(strNode+"GPIBAddress",this->strAddress);
    this->txtAddress->setText(strAddress);
}

void frmSelectConnection::on_rbLan_clicked()
{
    intSelect =1;

    clsSettings settings;
    QString strNode ="Connection/";
    settings.readSetting(strNode+"LANAddress",this->strAddress);
    this->txtAddress->setText(strAddress);
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
