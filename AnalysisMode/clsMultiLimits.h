#ifndef CLSMULTILIMITS_H
#define CLSMULTILIMITS_H
#include "clsMeterLimit.h"
#include <QJsonDocument>
#include <QVariantMap>
/*
    用于多个Limit判断的类
*/
class clsMultiLimits
{
public:
    clsMultiLimits();
    ~clsMultiLimits();

    void setJsonString(QString json);
    QString toJsonString();

    void setItems(const QString &item1, const QString &item2);
    //用于表格显示
    QString getLimit1Show();
    QString getLimit2Show();
    QString getFreqRange();
    void resetStatus();

    void compareValue(double &freq, double item1, double item2);

    clsMeterLimit getFreq() const;
    void setFreq(const clsMeterLimit &value);

    clsMeterLimit getLimit1() const;
    void setLimit1(const clsMeterLimit &value);

    clsMeterLimit getLimit2() const;
    void setLimit2(const clsMeterLimit &value);

    bool getBlLimit2() const;
    void setBlLimit2(bool value);

    bool getBlLimit1() const;
    void setBlLimit1(bool value);

    bool getStatus(){return this->status;}

    bool hasEnaleLimit()
    {
        return blLimit1 || blLimit2;
    }

private:
    bool status;
    bool blLimit1;
    bool blLimit2;

    clsMeterLimit freq;
    clsMeterLimit limit1;
    clsMeterLimit limit2;


    QString item1,item2;
};

#endif // CLSMULTILIMITS_H
