#include "clsGenerateSteps.h"
#include "NumberInput.h"
#include "doubleType.h"
#include "math.h"
#include "UserfulFunctions.h"
#include <QFileDialog>
#include "clsSettings.h"

clsGenerateSteps::clsGenerateSteps(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    this->changeValue = Frequency;
    start =0;
    stop = 1;
    log = false;
    point =2;

    points.clear();

    readSettings();

    updateButtons();

}

SweepType clsGenerateSteps::getType()
{
    return this->changeValue;
}

QList<double> clsGenerateSteps::getPoints() const
{
   return this->points;
}


void clsGenerateSteps::on_btnFrequency_clicked()
{
    changeValue = Frequency;

    updateButtons();
}

void clsGenerateSteps::on_btnBiasA_clicked()
{
    changeValue = BiasA;
    updateButtons();
}

void clsGenerateSteps::on_btnLevelA_clicked()
{
    changeValue = levelA;
    updateButtons();
}

void clsGenerateSteps::on_btnLevelV_clicked()
{
    changeValue = levelV;
    updateButtons();
}

void clsGenerateSteps::on_btnCancel_clicked()
{
    this->reject();
}

void clsGenerateSteps::on_btnOk_clicked()
{
    this->writeSettings();
    this->accept();
}

void clsGenerateSteps::on_btnStart_clicked()
{
    NumberInput *dlg = new NumberInput(this) ;
    dlg->setWindowTitle(tr("输入开始值"));

    dlg->setValueAndSuffix(start, "");
    if(dlg->exec() == QDialog::Accepted)
    {
        start = dlg->getNumber();
    }
    updateButtons();
}

void clsGenerateSteps::on_btnStop_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入终止值"));

    dlg->setValueAndSuffix(stop, "");
    if(dlg->exec()== QDialog::Accepted)
    {
        stop = dlg->getNumber();
    }
    updateButtons();
}

void clsGenerateSteps::on_btnPoint_clicked()
{
    NumberInput *dlg = new NumberInput(this);
    dlg->setWindowTitle(tr("输入点数"));

    dlg->setValueAndSuffix(point, "");
    if(dlg->exec()== QDialog::Accepted)
    {
        point =(int)dlg->getNumber();
        if(point <2)
        {
            point =2;
        }
    }
    updateButtons();
}

void clsGenerateSteps::on_btnGenerate_clicked()
{
    if(log == false)
    {
        double distance = stop - start;
        double averageSpace = distance / (point-1);

        points.clear();
        points.append(start);

        for(int i =0; i< point-1; i++)
        {
            points.append(start + averageSpace*(i+1));
        }
    }
    else
    {
        if(start <= 0 || stop <=0 )
        {
            log = false;
            updateButtons();
            return;
        }

        double logStart =log10(start);
        double logStop = log10(stop);
        double logDistance = logStop - logStart;
        double logAverageSpace = logDistance /(point-1);
        points.clear();
        points.append(start);

        for(int i=0; i<point-1; i++)
        {
            points.append(pow(10.00, logStart +(i+1)* logAverageSpace));
        }


    }
    updateButtons();

    qDebug()<<points;
}

void clsGenerateSteps::on_btnOpenDataFile_clicked()
{
    fileName=QFileDialog::getOpenFileName(this,tr("打开数据文件"),fileName,tr("文本文件(*.txt)"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    points.clear();
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList lineList = line.split(",");
        if(!lineList.isEmpty())
        {
            QString strFreq = lineList.at(0);
            bool ok;

            double freq = strFreq.toDouble(&ok);
            if(ok)
                points.append(freq);
        }
    }

    file.close();

    this->showPoints();
}

void clsGenerateSteps::updateButtons()
{
    QString suffix = UserfulFunctions::getSweepTypeUnit(changeValue);

    doubleType dt;
    dt.setData(start);
    btnStart->setText(dt.formateToString(5)+suffix);

    dt.setData(stop);
    btnStop->setText(dt.formateToString(5)+suffix);

    if(start <=0 || stop <=0)
    {
        log = false;
    }
    chkLog->setChecked(log);

    btnPoint->setText(QString::number(point));

    switch (changeValue) {
    case Frequency:
        this->btnFrequency->setChecked(true);
        break;
    case levelA:
        this->btnLevelA->setChecked(true);
        break;
    case levelV:
        this->btnLevelV->setChecked(true);
        break;
    case BiasA:
        this->btnBiasA->setChecked(true);
        break;
    default:
        break;
    }

    this->showPoints();
}

void clsGenerateSteps::showPoints()
{
    tabDataList->clear();
    tabDataList->setRowCount(2);
    tabDataList->horizontalHeader()->setVisible(false);
    tabDataList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabDataList->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabDataList->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList headers;
    headers << tr("序号")<<tr("值");

    for(int i=0; i< tabDataList->rowCount(); i++)
    {
        QTableWidgetItem * item = new QTableWidgetItem();
        item->setText(headers[i]);
        item->setFont(QFont("楷体", 12, QFont::Bold));
        item->setBackgroundColor(QColor(Qt::gray));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tabDataList->setVerticalHeaderItem(i,item);
    }

    tabDataList->setColumnCount(points.length());

    for(int i=0; i<points.length(); i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(QString::number(i+1));
        item1->setFont(QFont("Tahoma", 12));
        item1->setTextAlignment(Qt::AlignCenter);
        tabDataList->setItem(0,i, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem();
        doubleType dt;
        dt.setData(points.at(i));
        item2->setText(dt.formateToString(5)+UserfulFunctions::getSweepTypeUnit(changeValue));

        item2->setFont(QFont("Tahoma", 12));
        item2->setTextAlignment(Qt::AlignCenter);
        tabDataList->setItem(1,i, item2);
    }

}

void clsGenerateSteps::readSettings()
{
    clsSettings settings;
    QString strNode = "GenerateSteps/";

    int tmp;
    settings.readSetting(strNode + "changeItem",tmp);
    this->changeValue = (SweepType) tmp;

    if(changeValue < (SweepType)1)
        changeValue =(SweepType)1;


    settings.readSetting(strNode + "start", this->start);
    settings.readSetting(strNode + "stop", this->stop);
    settings.readSetting(strNode + "point", this->point);
    if(point <2)
        point =2;
    settings.readSetting(strNode + "log", this->log);
    settings.readSetting(strNode + "file", this->fileName);

    settings.readSetting(strNode + "points", this->points);
}

void clsGenerateSteps::writeSettings()
{

    clsSettings settings;
    QString strNode = "GenerateSteps/";

    settings.writeSetting(strNode + "changeItem", this->changeValue);
    settings.writeSetting(strNode + "start", this->start);
    settings.writeSetting(strNode + "stop", this->stop);
    settings.writeSetting(strNode + "point", this->point);
    settings.writeSetting(strNode + "log", this->log);
    settings.writeSetting(strNode + "file", this->fileName);
    settings.writeSetting(strNode + "points", this->points);
}

void clsGenerateSteps::on_chkLog_toggled(bool checked)
{

    log = checked;
    updateButtons();
}
