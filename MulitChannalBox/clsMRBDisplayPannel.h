#ifndef CLSMRBDISPLAYPANNEL_H
#define CLSMRBDISPLAYPANNEL_H

#include "ui_clsMRBDisplayPannel.h"
#include <QList>
#include "clsMeterLimit.h"
struct TestItem
{
    QString item;
    double value;
    QString suffix;
    clsMeterLimit limit;
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
    void setTestResult(TestResult value);
    void setNumber(int chennal);
    int number();
    TestResult getTestResult()
    {
        return this->res;
    }
    bool getStatus()
    {
        return this->res.status;
    }

    void clearAll();
    void setResType(int value);

private slots:
    void statusLabelClick();
    void resultLableClick();

private:

    TestResult res;
    bool converDouble(const QString &value, double &s);
    QString formatToString(const QString &item, const QString &unit, const double &testValue, const bool status);
    void setNumberLabel(bool value);
    void setNumberLabel(Status value);
};

#endif // CLSMRBDISPLAYPANNEL_H
