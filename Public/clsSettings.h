#ifndef CLSSETTINGS_H
#define CLSSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QFile>

const QString settingsFilePath ="./Settings.ini";

class clsSettings : public QObject
{
    Q_OBJECT
public:
    explicit clsSettings(QObject *parent = 0);

    bool FileExit();
    void writeSetting(QString strPath,QVariant value);
    void writeSetting(QString strPath, QList<double> value);
    void readSetting(QString strPath, QString &value);
    void readSetting(QString strPath, bool &value);
    void readSetting(QString strPath, QVariant &value);
    void readSetting(QString strPath, int &value);
    void readSetting(QString strPath, double &value);
    void readSetting(QString strPath, QList<double> &value);
    void readSetting(QString strPath, QStringList &value);

signals:
    
public slots:

private:
    bool fileExit;

    QSettings *configIniWrite;



    
};

#endif // CLSSETTINGS_H
