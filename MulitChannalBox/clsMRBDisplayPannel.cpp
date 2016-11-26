#include "clsMRBDisplayPannel.h"
#include <QJsonDocument>
#include <QStringList>
#include "UserfulFunctions.h"
#include "doubleType.h"
#include "clsMeterLimit.h"
clsMRBDisplayPannel::clsMRBDisplayPannel(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    res.status=true;
    lblStatus->setStatus(IDEL);

    connect(this->lblStatus,SIGNAL(Clicked()),this,SLOT(statusLabelClick()));
    connect(this->lblResult,SIGNAL(Clicked()),this,SLOT(resultLableClick()));
    connect(this->lblNumber,SIGNAL(Clicked()),this,SLOT(numberClick1()));
    clearAll();
}

void clsMRBDisplayPannel::setTestResult( TestResult value)
{

    QString strToDisplay;
    for(int i=0; i< value.item.length();i++)
    {
        if(i> 0 && i<= value.item.length()-1)
        {
            strToDisplay.append("\r\n");
        }

        strToDisplay += formatToString(value.item.at(i).item,
                                       value.item.at(i).suffix,
                                       value.item.at(i).value,
                                       value.item.at(i).status);

    }

    //qDebug()<< strToDisplay;



    lblStatus->setStatus(value.status);
    lblResult->setText(strToDisplay);

    setNumberLabel(value.status);

    this->res = value;


}

void clsMRBDisplayPannel::setNumber(int chennal)
{
    this->lblNumber->setText(QString::number(chennal));
}

int clsMRBDisplayPannel::number()
{
    return this->lblNumber->text().toInt();
}


void clsMRBDisplayPannel::clearAll()
{
    setNumberLabel(IDEL);
    res.item.clear();
    this->lblStatus->setStatus(IDEL);
    this->lblResult->setText(tr("   没有测试结果"));

}

void clsMRBDisplayPannel::setResType(int value)
{
    if(value<2)
        this->stackedWidget->setCurrentIndex(value);
}

bool clsMRBDisplayPannel::converDouble(const QString &value, double &s)
{
    bool ok;
    s=value.toDouble(&ok);
    return ok;
}

QString clsMRBDisplayPannel::formatToString(const QString &item, const QString &unit,
                                            const double &testValue, const bool status)
{
    QString retStr;
    doubleType dt;
    dt.setData(testValue,"");
    retStr=retStr.append("  ");
    retStr+= item +":\t";

    if(unit==tr("OFF"))
    {
        retStr+= dt.formateToString(7);

    }
    else
    {
        retStr+=dt.formateWithUnit(unit,7)+unit;

    }

    retStr+=UserfulFunctions::getSuffix(item.toUpper())+"\t";

    if(status)
    {
        retStr+="√" ;
    }
    else
        retStr+="×";
    return retStr;
}

void clsMRBDisplayPannel::setNumberLabel(bool value)
{
    if(value)
        this->lblNumber->setStyleSheet("background-color: rgb(0, 255, 0); border-radius: 20px");
    else
        this->lblNumber->setStyleSheet("background-color: rgb(255, 0, 0); border-radius: 20px");
}

void clsMRBDisplayPannel::setNumberLabel(Status value)
{
    if(value ==IDEL)
    {
        this->lblNumber->setStyleSheet("background-color: rgb(255, 104, 32); border-radius: 20px");
    }
}



void clsMRBDisplayPannel::statusLabelClick()
{
    this->stackedWidget->setCurrentIndex(1);
}

void clsMRBDisplayPannel::resultLableClick()
{
    this->stackedWidget->setCurrentIndex(0);
}

void clsMRBDisplayPannel::numberClick1()
{
    emit numberClick(this);
}

