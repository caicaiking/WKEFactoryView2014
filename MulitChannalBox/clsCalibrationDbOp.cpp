#include "clsCalibrationDbOp.h"
#include <QtSql>
#include <QMutex>
#include <QDebug>
#include "clsRuningSettings.h"
clsCalDb * clsCalDb::_instance =0;

clsCalDb *clsCalDb::getInst()
{
    static QMutex mutex;
    if (!_instance) {
        QMutexLocker locker(&mutex);
        if (!_instance) {
            _instance = new clsCalDb();
        }
    }
    return _instance;
}

clsCalDb::clsCalDb(QObject *parent) : QObject(parent)
{
    isDataBaseIsOpen = false;
    hasTable= false;
}
QString clsCalDb::getStrDataBaseName() const
{
    return strDataBaseName;
}

void clsCalDb::setStrDataBaseName(const QString &value)
{
    strDataBaseName = value;
}

bool clsCalDb::openDataBase()
{
    if(isDataBaseIsOpen)
        return true;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->strDataBaseName);

    if(!db.open())
    {
        //        qDebug()<< db.lastError();
        //        qDebug()<< "Failed to connect.";
        isDataBaseIsOpen = false;
    }
    else
    {
        //        qDebug()<< "Data Base connected!";
        isDataBaseIsOpen = true;
    }
    return isDataBaseIsOpen;

}

bool clsCalDb::initTable()
{
    QSqlQuery qry;
    QString tmpSql=QString("CREATE TABLE IF NOT EXISTS %1 ("
                           "id INTEGER UNIQUE PRIMARY KEY, "
                           "channel INTGEGER, "
                           "frequency NUMERIC, "
                           "zValue NUMERIC, "
                           "aValue NUMERIC, "
                           "dataType VARCHAR(10))").arg(getTableName());

    //dataType O openData, S shortData, Lm loadData Ls standard Data;
    //qDebug()<<tmpSql;
    qry.prepare(tmpSql);
    if(!qry.exec())
    {
        qDebug() << qry.lastError();
        hasTable = false;
        return false;
    }
    else
    {
        //qDebug() << "Table created!";
        hasTable = true;
        return true;
    }
}

QList<double> clsCalDb::getCalData(
        double freq, int channal,QString type)
{
    QSqlQuery query;

    QString tmp= QString(
                "SELECT "
                "ZVALUE, AVALUE "
                "FROM"
                " %1"
                " WHERE"
                " CHANNEL=%2"
                " AND"
                " FREQUENCY=%3 AND "
                "DATATYPE ='%4'"
                )
            .arg(getTableName())
            .arg(channal)
            .arg(freq)
            .arg(type);

    query.prepare(tmp);

    if(query.exec())
    {
        if(query.next())
        {
            double z = query.value(0).toDouble();
            double a = query.value(1).toDouble();

            return QList<double>()<<z<<a;

        }
        else
        {
            return QList<double>();
        }
    }
    else
    {
        qDebug()<<"Query dataBase Error!";
        return QList<double>();
    }
}

bool clsCalDb::insertRecord(double freq, int channal,
                            double z, double a, QString type)
{
    if(getCalData(freq,channal,type).length()>0)
    {
        //update
        QString strSql = QString("UPDATE"
                                 " %1"
                                 " SET"
                                 " zValue=%2,"
                                 " aValue=%3 "
                                 " where"
                                 " channel = %4"
                                 " and "
                                 "frequency = %5"
                                 " and"
                                 " DATATYPE='%6'")
                .arg(getTableName()).arg(z).arg(a).arg(channal).arg(freq).arg(type);

        //qDebug()<< strSql;
        QSqlQuery query;
        query.prepare(strSql);

        return query.exec();
    }
    else
    {
        //insert
        QString strSql = QString("INSERT INTO"
                                 " %1"
                                 " (CHANNEL,FREQUENCY,ZVALUE,AVALUE,DATATYPE)"
                                 " VALUES(%2,%3,%4,%5,'%6')")
                .arg(getTableName()).arg(channal).arg(freq).arg(z).arg(a).arg(type);
        //qDebug()<< strSql;
        QSqlQuery query;
        query.prepare(strSql);

        return query.exec();
    }
}

QString clsCalDb::getTableName()
{
    return QString("MTC_%1").arg(clsRS::getInst().meterSeries);
}

bool clsCalDb::deleteRecord(double freq, int channel, QString type)
{
    QString strSql =
            QString("DELETE FROM"
                    " %1"
                    " WHERE"
                    " CHANNEL=%2"
                    " AND"
                    " FREQUENCY=%3"
                    " AND"
                    " DATATYPE='%4'")
                    .arg(getTableName())
                    .arg(channel)
                    .arg(freq)
                    .arg(type);
    QSqlQuery query;
    query.prepare(strSql);
    return query.exec();
}


