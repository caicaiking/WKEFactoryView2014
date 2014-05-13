#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#define EMPTYSTRING ""

#include <QtGui>
#include <QMap>

/*

    doubleType db;
    db.setData(-0.100,"m");

    QString showxx = db.formateToString(5);

    QMessageBox::information(0,0,QString::number(db.Data()));
    QMessageBox::information(0,0,showxx);
  */

class doubleType
{
public:

    doubleType(double exValue=0, QString exUnit=EMPTYSTRING);


    void setData(const double orgValue, const QString orgUnit);

    void getDataAndUnit(double &exValue, QString &exUnit);
    QString formateToString(int length=7);

    QString formateWithUnit(QString unit, int length=7);
    double Data();
private:
    void initUnitMap();
    int Abs(double &value);
    double getData(const double orgValue, const QString orgUnit);
    QMap <QString,int> unitMap;

    double value;
    QString unit;
};

#endif // DATAPROCESS_H
