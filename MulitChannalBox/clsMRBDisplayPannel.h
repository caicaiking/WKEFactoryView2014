#ifndef CLSMRBDISPLAYPANNEL_H
#define CLSMRBDISPLAYPANNEL_H

#include "ui_clsMRBDisplayPannel.h"
#include <QList>

struct TestItem
{
    double value;
    QString item;
    bool status;
};

struct TestResult
{
    QList<TestItem> item;
    bool status;
};

class clsMRBDisplayPannel : public QWidget, private Ui::clsMRBDisplayPannel
{
    Q_OBJECT


public:
    explicit clsMRBDisplayPannel(QWidget *parent = 0);
    void setTestResult(QString json);
    void setNumber(int chennal);
    int number();
    const  TestResult getTestResult()
    {
        return this->res;
    }
    const bool getStatus()
    {
        return this->res.status;
    }



    void clearAll();
    void setResType(int value);

private slots:
    void statusLabelClick();
    void resultLableClick();

private:
    void processJson(const QVariantMap &result);
    TestResult res;
    bool converDouble(const QString &value, double &s);
    QString formatToString(const QString &item, const QString &unit, const double &testValue, const QString &limit, double &fromateValue, bool &status);
    void setNumberLabel(bool value);
    void setNumberLabel(Status value);
};

#endif // CLSMRBDISPLAYPANNEL_H
