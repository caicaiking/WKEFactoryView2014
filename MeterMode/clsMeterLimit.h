#ifndef CLSMETERLIMIT_H
#define CLSMETERLIMIT_H

#include <QString>

class clsMeterLimit
{
public:
    clsMeterLimit();

    QString toString();
    void setString(QString value);
    double getNorminal() const;
    void setNorminal(double value);

    double getPerHi() const;
    void setPerHi(double value);

    double getPerLo() const;
    void setPerLo(double value);

    double getAbsHi() const;
    void setAbsHi(double value);

    double getAbsLo() const;
    void setAbsLo(double value);

    QString getLimitType() const;
    void setLimitType(const QString &value);

    double getAbsLimitLow();
    double getAbsLimitHigh();
    bool operator ==(clsMeterLimit value);
    bool comparaValue(double value, QString &type);
    bool comparaValue(double value);
    QString showLimits(QString suffix="");
private:
    QString limitType;
    double norminal, perHi, perLo, absHi, absLo;
};

#endif // CLSMETERLIMIT_H
