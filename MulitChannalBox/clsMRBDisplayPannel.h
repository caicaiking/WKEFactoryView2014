#ifndef CLSMRBDISPLAYPANNEL_H
#define CLSMRBDISPLAYPANNEL_H

#include "ui_clsMRBDisplayPannel.h"
#include <QList>
class clsMRBDisplayPannel : public QWidget, private Ui::clsMRBDisplayPannel
{
    Q_OBJECT


public:
    explicit clsMRBDisplayPannel(QWidget *parent = 0);
    void setTestResult(int chennal, QString json);

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

private slots:
    void statusLabelClick();
    void resultLableClick();

private:
    void processJson(const QVariantMap &result);
    TestResult res;
    bool converDouble(const QString &value, double &s);
    QString formatToString(const QString &item, const QString &unit, const double &testValue, const QString &limit, double &fromateValue, bool &status);
};

#endif // CLSMRBDISPLAYPANNEL_H
