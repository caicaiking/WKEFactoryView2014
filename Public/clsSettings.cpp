#include "clsSettings.h"
#include <QDebug>
#include <QList>
#include <QStringList>
#include <QColor>
clsSettings::clsSettings(QObject *parent) :
    QObject(parent)
{
    configIniWrite = new QSettings(settingsFilePath, QSettings::IniFormat);
}

bool clsSettings::FileExit()
{
    fileExit=QFile::exists("./Settings.ini");
    return fileExit;
}

void clsSettings::writeSetting(QString strPath, QList<double> points)
{
    QStringList tmpString;

    foreach (double value, points) {
        tmpString<<QString::number(value);
    }

    writeSetting(strPath, tmpString);


}

void clsSettings::writeSetting(QString strPath, QVariant value)
{
    configIniWrite->setValue(strPath,value);
}

void clsSettings::readSetting(QString strPath, QVariant & value)
{
    value =configIniWrite->value(strPath);
}

void clsSettings::readSetting(QString strPath, int & value)
{
    value =configIniWrite->value(strPath).toInt();
}


void clsSettings::readSetting(QString strPath, double & value)
{
    value =configIniWrite->value(strPath).toDouble();
}

void clsSettings::readSetting(QString strPath, QColor &value)
{
    value =  configIniWrite->value(strPath).value<QColor>();
}

void clsSettings::readSetting(QString strPath, QString & value)
{
    value =configIniWrite->value(strPath).toString();
}

void clsSettings::readSetting(QString strPath, bool & value)
{
    value =configIniWrite->value(strPath).toBool();
}

void clsSettings::readSetting(QString strPath, QStringList &value)
{
    value =configIniWrite->value(strPath).toStringList();
}

void clsSettings::readSetting(QString strPath, QList<double> &value)
{
    QStringList tmp;
    readSetting(strPath,tmp);
    value.clear();

    foreach (QString my, tmp) {
        value<< my.toDouble();
    }
}

