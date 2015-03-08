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
    clearAll();
}

void clsMRBDisplayPannel::setTestResult(  QString json)
{

    lblResult->setText(json);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(),&error);
    if(error.error == QJsonParseError::NoError)
    {
        if(jsonDocument.isObject())
        {
            QVariantMap  result = jsonDocument.toVariant().toMap();
            processJson(result);
        }
    }
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

void clsMRBDisplayPannel::processJson(const QVariantMap &result  )
{
    //bool isOnTest2 = result["isOnTest2"].toBool();
    QString testReslt = result["Result"].toString();
    QVariantMap test1 = result["test1"].toMap();
    QVariantMap test2 = result["test2"].toMap();

    QString displayRes="";
    QStringList slResult = testReslt.split(',');

    double formatValue;
    bool status;
    TestItem item;
    if(slResult.length()>=1)
    {
        double testValue;
        bool ok = converDouble(slResult.at(0),testValue);
        if(ok)
        {
            displayRes= formatToString(test1["Item1"].toString(),
                    test1["Unit1"].toString(),
                    testValue,
                    test1["Limit1"].toString(),
                    formatValue,
                    status);
        }

        item.item=test1["Item1"].toString();
        item.status=status;
        item.value = formatValue;

        this->res.item.append(item);
    }

    if(slResult.length()>=2)
    {
        double testValue;
        bool ok = converDouble(slResult.at(1),testValue);
        if(ok)
        {
            displayRes=displayRes.append("\r\n");
            displayRes+= formatToString(test1["Item2"].toString(),
                    test1["Unit2"].toString(),
                    testValue,
                    test1["Limit2"].toString(),
                    formatValue,
                    status);
        }

        item.item=test1["Item2"].toString();
        item.status=status;
        item.value = formatValue;

        this->res.item.append(item);
    }

    if(slResult.length()>=3)
    {
        double testValue;
        bool ok = converDouble(slResult.at(2),testValue);
        if(ok)
        {
            displayRes=displayRes.append("\r\n");
            displayRes+= formatToString(test2["Item1"].toString(),
                    test2["Unit1"].toString(),
                    testValue,
                    test2["Limit1"].toString(),
                    formatValue,
                    status);
        }

        item.item=test2["Item1"].toString();
        item.status=status;
        item.value = formatValue;

        this->res.item.append(item);
    }

    if(slResult.length()>=4)
    {
        double testValue;
        bool ok = converDouble(slResult.at(3),testValue);
        if(ok)
        {
            displayRes=displayRes.append("\r\n");
            displayRes+= formatToString(test2["Item2"].toString(),
                    test2["Unit2"].toString(),
                    testValue,
                    test2["Limit2"].toString(),
                    formatValue,
                    status);
        }

        item.item=test2["Item2"].toString();
        item.status=status;
        item.value = formatValue;

        this->res.item.append(item);
    }


    lblResult->setText(displayRes);
    bool st=true;
    for(int i=0; i< res.item.length();i++)
    {
        st= st&& res.item.at(i).status;
        //qDebug()<< res.item.at(i).value <<" "<< res.item.at(i).item;
    }

    res.status= st;
    lblStatus->setStatus(st);
    setNumberLabel(st);

}



bool clsMRBDisplayPannel::converDouble(const QString &value, double &s)
{
    bool ok;
    s=value.toDouble(&ok);
    return ok;
}

QString clsMRBDisplayPannel::formatToString(const QString &item, const QString &unit,
                                            const double &testValue, const QString &limit,
                                            double &fromateValue, bool &status)
{
    QString retStr;
    doubleType dt;
    dt.setData(testValue,"");
    retStr=retStr.append("  ");
    retStr+= item +":\t";

    if(unit==tr("无"))
    {
        retStr+= dt.formateToString(7);
        fromateValue= testValue;
    }
    else
    {
        retStr+=dt.formateWithUnit(unit,7)+unit;
        fromateValue=dt.getDataWithUnit(unit);
    }

    retStr+=UserfulFunctions::getSuffix(item.toUpper())+"\t";

    clsMeterLimit clsLimit;
    clsLimit.setString(limit);
    status= clsLimit.comparaValue(testValue);

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
        this->lblNumber->setStyleSheet("background-color: rgb(183, 255, 172); border-radius: 20px");
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

