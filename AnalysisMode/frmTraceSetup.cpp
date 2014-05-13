#include "frmTraceSetup.h"
#include "clsSettings.h"
#include "NumberInput.h"
#include <QInputDialog>
#include "frmPointEditor.h"
frmTraceSetup::frmTraceSetup(WKEInstrument *ms, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->meter=ms;

   getSupportFunction();

    readSettings(this->gsetup);
    updateSweepTitile(this->gsetup.sweepType);
}

void frmTraceSetup::getSupportFunction()
{
    QString strSup = meter->getSuportFunction();

    QStringList list= strSup.split(",");

    if(list.length()>0)
        btnFrequency->setVisible(list.at(0).toInt()==1?true:false);

    if(list.length()>1)
        btnBias->setVisible(list.at(1).toInt()==1?true:false);

    if(list.length()>2)
        btnBiasA->setVisible(list.at(2).toInt()==1?true:false);

    if(list.length()>3)
        btnTime->setVisible(list.at(3).toInt()==1?true:false);


}

void frmTraceSetup::on_btnTime_clicked()
{
    updateSweepTitile(time);
}

void frmTraceSetup::on_btnFrequency_clicked()
{
    updateSweepTitile(frequency);
}

void frmTraceSetup::on_btnBias_clicked()
{
    updateSweepTitile(biasV);
}

void frmTraceSetup::updateSweepTitile(SweepType t)
{

    switch (t) {
    case time:
        grpSweepItem->setTitle(tr("时间范围（s）"));
        btnTime->setChecked(true);
        break;
    case frequency:
        grpSweepItem->setTitle(tr("频率范围（Hz）"));
        btnFrequency->setChecked(true);
        break;
    case biasV:
        grpSweepItem->setTitle(tr("偏置范围（V）"));
        btnBias->setChecked(true);

        break;
    case biasA:
        grpSweepItem->setTitle(tr("偏置范围（A）"));
        btnBiasA->setChecked(true);
        break;
    default:
        break;
    }

    this->gsetup.sweepType = t;
    checkLimit = LimitCheckFactory::getLimitCheckType(t,meter);
    //checkLimit->setMeter(this->meter);
    readSettings(this->gsetup,false);
    updateButtons();
}

void frmTraceSetup::on_btnCancel_clicked()
{
    this->reject();
}

void frmTraceSetup::on_btnOk_clicked()
{

    graphSetup tmp;
    readSettings(tmp,true);


    //跟原来的点数进行比对
    if(this->gsetup.points == tmp.points)
    {
        if(tmp.logX!= gsetup.logX || tmp.xmin!=gsetup.xmin || tmp.xmax != gsetup.xmax)
        {

            this->gsetup.points =frmPointEditor::getStaticPoints(gsetup.xmin,
                                                                 gsetup.xmax,
                                                                 gsetup.logX,
                                                                 tmp.points.length());
        }
    }


    this->gsetup.points = frmPointEditor::rmvPP(this->gsetup.points);
    writeSettings(this->gsetup);
    this->accept();
}


bool  frmTraceSetup::readSettings(graphSetup &gsetup, bool incldSwty)
{
    clsSettings settings;

    if(! settings.FileExit())
        return false;
    const QString strNode="GraphSetup/";
    if(incldSwty)
    {
        int tmp;
        settings.readSetting(strNode+"SweepType",tmp);
        gsetup.sweepType=(SweepType)tmp;
    }

    switch (gsetup.sweepType) {
    case time:
        settings.readSetting(strNode+"TimeXmin",gsetup.xmin);
        settings.readSetting(strNode+"TimeXmax",gsetup.xmax);
        settings.readSetting(strNode+"TimeLog",gsetup.logX);
        settings.readSetting(strNode+"TimePoints",gsetup.points);
        break;
    case frequency:
        settings.readSetting(strNode+"FreqXmin",gsetup.xmin);
        settings.readSetting(strNode+"FreqXmax",gsetup.xmax);
        settings.readSetting(strNode+"FreqLog",gsetup.logX);
        settings.readSetting(strNode+"FreqPoints",gsetup.points);
        break;

    case biasV:
        settings.readSetting(strNode+"BiasXmin",gsetup.xmin);
        settings.readSetting(strNode+"BiasXmax",gsetup.xmax);
        settings.readSetting(strNode+"BiasLog",gsetup.logX);
        settings.readSetting(strNode+"BiasPoints",gsetup.points);
        settings.readSetting(strNode+"BiasVRate",gsetup.biasVRate);
        break;
    case biasA:
        settings.readSetting(strNode+"BiasAXmin",gsetup.xmin);
        settings.readSetting(strNode+"BiasAXmax",gsetup.xmax);
        settings.readSetting(strNode+"BiasALog",gsetup.logX);
        settings.readSetting(strNode+"BiasAPoints",gsetup.points);
        break;
    default:
        break;
    }

    settings.readSetting(strNode+"TraceAMax",gsetup.yLeftMax);
    settings.readSetting(strNode+"TraceAMin",gsetup.yLeftMin);
    settings.readSetting(strNode+"TraceALog",gsetup.logYleft);
    settings.readSetting(strNode+"TraceBMax",gsetup.yRightMax);
    settings.readSetting(strNode+"TraceBMin",gsetup.yRightMin);
    settings.readSetting(strNode+"Title",gsetup.title);

    return true;

}

bool  frmTraceSetup::writeSettings(graphSetup gsetup)
{
    clsSettings settings;

    if(! settings.FileExit())
        return false;
    const QString strNode="GraphSetup/";

    settings.writeSetting(strNode+"SweepType",gsetup.sweepType);
    switch (gsetup.sweepType) {
    case time:
        settings.writeSetting(strNode+"TimeXmin",gsetup.xmin);
        settings.writeSetting(strNode+"TimeXmax",gsetup.xmax);
        settings.writeSetting(strNode+"TimeLog",gsetup.logX);
        settings.writeSetting(strNode +"TimePoints",gsetup.points);
        break;
    case frequency:
        settings.writeSetting(strNode+"FreqXmin",gsetup.xmin);
        settings.writeSetting(strNode+"FreqXmax",gsetup.xmax);
        settings.writeSetting(strNode+"FreqLog",gsetup.logX);
        settings.writeSetting(strNode +"FreqPoints",gsetup.points);
        break;

    case biasV:
        settings.writeSetting(strNode+"BiasXmin",gsetup.xmin);
        settings.writeSetting(strNode+"BiasXmax",gsetup.xmax);
        settings.writeSetting(strNode+"BiasLog",gsetup.logX);
        settings.writeSetting(strNode+"BiasPoints",gsetup.points);
        settings.writeSetting(strNode+"BiasVRate",gsetup.biasVRate);
        break;
    case biasA:
        settings.writeSetting(strNode+"BiasAXmin",gsetup.xmin);
        settings.writeSetting(strNode+"BiasAXmax",gsetup.xmax);
        settings.writeSetting(strNode+"BiasALog",gsetup.logX);
        settings.writeSetting(strNode+"BiasAPoints",gsetup.points);
        break;
    default:
        break;
    }

    settings.writeSetting(strNode+"TraceAMax",gsetup.yLeftMax);
    settings.writeSetting(strNode+"TraceAMin",gsetup.yLeftMin);
    settings.writeSetting(strNode+"TraceALog",gsetup.logYleft);
    settings.writeSetting(strNode+"TraceBMax",gsetup.yRightMax);
    settings.writeSetting(strNode+"TraceBMin",gsetup.yRightMin);
    settings.writeSetting(strNode+"Title",gsetup.title);


    return true;

}


void frmTraceSetup::updateButtons()
{
    doubleType dt;

    dt.setData(this->gsetup.xmin,"");
    this->btnXmin->setText(dt.formateToString());

    dt.setData(this->gsetup.xmax,"");
    this->btnXmax->setText(dt.formateToString());

    if(this->gsetup.sweepType ==frequency && this->gsetup.logX)
    {
        if(gsetup.xmax>0 && gsetup.xmin >0)
        {
            this->chkXlog->setChecked(true);
            this->gsetup.logX=true;
        }
        else
        { this->chkXlog->setChecked(false);
            this->gsetup.logX=false;
        }
    }
    else
    {
        this->chkXlog->setChecked(false);
        this->gsetup.logX=false;
    }


    dt.setData(this->gsetup.yLeftMax,"");
    this->btnYLeftMax->setText(dt.formateToString());

    dt.setData(this->gsetup.yLeftMin,"");
    this->btnYLeftMin->setText(dt.formateToString());

    if(meter->getItem1() =="Z" || meter->getItem1()=="Y")
    {
        if(gsetup.yLeftMax >0 && gsetup.yLeftMin>0 && gsetup.logYleft)
        {
            this->chkYleftLog->setChecked(true);
            this->gsetup.logYleft=true;
        }
        else
        {
            this->chkYleftLog->setChecked(false);
            this->gsetup.logYleft=false;
        }
    }
    else
    {
        this->chkYleftLog->setChecked(false);
        this->gsetup.logYleft=false;
    }

    dt.setData(gsetup.yRightMax,"");
    btnYRightMax->setText(dt.formateToString());

    dt.setData(gsetup.yRightMin,"");
    btnYRightMin->setText(dt.formateToString());

    btnPoints->setText(QString::number(gsetup.points.length())+tr("点"));

    if(gsetup.sweepType==biasV)
         btnRate->setVisible(true);
    else
        btnRate->setVisible(false);

    dt.setData(gsetup.biasVRate,"");
    btnRate->setText(tr("速率：")+dt.formateToString(6)+"V/s");
}

void frmTraceSetup::on_btnXmin_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最小值"));

    numberInput->setValueAndSuffix(gsetup.xmin,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        int value = numberInput->getNumber();

        gsetup.xmin = checkLimit->compareMin(value);
    }

    updateButtons();
}

void frmTraceSetup::on_chkXlog_clicked()
{

    this->gsetup.logX = chkXlog->isChecked();
    updateButtons();
}

void frmTraceSetup::on_chkYleftLog_clicked()
{
    this->gsetup.logYleft = chkYleftLog->isChecked();
    updateButtons();
}

void frmTraceSetup::on_btnXmax_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最大值"));

    numberInput->setValueAndSuffix(gsetup.xmax,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        double value = numberInput->getNumber();

        gsetup.xmax = checkLimit->compareMax(value);
    }

    updateButtons();
}

void frmTraceSetup::on_btnYLeftMin_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最小值"));

    numberInput->setValueAndSuffix(gsetup.yLeftMin,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        gsetup.yLeftMin  = numberInput->getNumber();
    }

    updateButtons();
}

void frmTraceSetup::on_btnYRightMin_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最小值"));

    numberInput->setValueAndSuffix(gsetup.yRightMin,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        gsetup.yRightMin  = numberInput->getNumber();
    }

    updateButtons();
}

void frmTraceSetup::on_btnYLeftMax_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最大值"));

    numberInput->setValueAndSuffix(gsetup.yLeftMax,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        gsetup.yLeftMax  = numberInput->getNumber();
    }

    updateButtons();
}

void frmTraceSetup::on_btnYRightMax_clicked()
{
    NumberInput *numberInput = new NumberInput(this);
    numberInput->setWindowTitle(tr("输入最大值"));

    numberInput->setValueAndSuffix(gsetup.yRightMax,"");


    if(numberInput->exec()==QDialog::Accepted)
    {
        gsetup.yRightMax  = numberInput->getNumber();
    }

    updateButtons();
}

void frmTraceSetup::on_btnTitle_clicked()
{

    bool ok;
    QString text = QInputDialog::getText(this, tr("输入标题"),
                                         tr("标题:"), QLineEdit::Normal,
                                         this->gsetup.title, &ok);
    if (ok && !text.isEmpty())
    {
        this->gsetup.title = text;
    }
}

void frmTraceSetup::on_btnPoints_clicked()
{
    frmPointEditor dlg(gsetup.xmin,gsetup.xmax,gsetup.logX,gsetup.sweepType);

    graphSetup tmp;
    readSettings(tmp,true);

    //跟原来的点数进行比对

    if(tmp.points == gsetup.points)
    {
        if(tmp.logX!= gsetup.logX || tmp.xmin!=gsetup.xmin || tmp.xmax != gsetup.xmax)
        {
            int point =(this->gsetup.points.length()==0?10:this->gsetup.points.length());
            dlg.setTestPoint(point);
        }
        else
        {
            dlg.setPoints(this->gsetup.points);
        }
    }
    else
    {
        int point =(this->gsetup.points.length()==0?10:this->gsetup.points.length());
        dlg.setTestPoint(point);
    }



    if(dlg.exec()==QDialog::Accepted)
    {
        gsetup.points=dlg.getPoints();
    }

    updateButtons();
}
graphSetup frmTraceSetup::getGsetup() const
{
    return gsetup;
}



void frmTraceSetup::on_btnBiasA_clicked()
{
    updateSweepTitile(biasA);
}

void frmTraceSetup::on_btnRate_clicked()
{
    NumberInput *dlg = new NumberInput(gsetup.biasVRate,"", this);
    dlg->setWindowTitle(tr("设置偏置电压速率"));


    if(dlg->exec()==QDialog::Accepted)
    {
        gsetup.biasVRate = dlg->getNumber();
        updateButtons();
    }


}
