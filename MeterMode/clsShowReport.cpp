#include "clsShowReport.h"
#include <QDebug>
#include <QFileDialog>
#include "clsSettings.h"
#include <QMessageBox>
#include "doubleType.h"
#include "clsRuningSettings.h"
#include <QSqlDatabase>
#include <QSqlQuery>

clsShowReport::clsShowReport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    readSettings();
}

void clsShowReport::setData(clsDataStore *value)
{
    this->result= value;

    if(this->result->getStepCount() == 0)
    {
        QMessageBox::warning(this,tr("导出报表"),tr("没有可用的数据，用于导出报表！"));
        this->close();
    }
    else
    {
        this->updateButtons();
        this->exec();
    }

}

void clsShowReport::updateButtons()
{
    txtNumber->setText(QString::number(result->getTestCount()));
    txtIntrument->setText(QString("WK")+clsRS::getInst().instrumentModel);

    int stepCount = result->getStepCount();
    if(stepCount==0)
        return;
    QString conditon = result->getConditon(0);

    result->getMeter()->setCondition(conditon);
    txtTestFreq->setText(result->getMeter()->getFreq());

}

void clsShowReport::on_btnClose_clicked()
{
    this->reject();
}

void clsShowReport::on_btnExport_clicked()
{
    btnExport->setEnabled(false);
    if(QFile::exists("./Sample.xls"))
        QFile::remove("./Sample.xls");

    QFile::copy("./Sample/Sample.xls","./Sample.xls");

    // 注意啦 在这儿是创建的表头的数据表 ·················
    QString sheetName = "TitleSheet";

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");
    if( !db.isValid())
        return;   //! type error

    QString dsn = "DRIVER={Microsoft Excel Driver (*.xls)};"
                  "DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"Sample.xls\";DBQ=Sample.xls";
    db.setDatabaseName( dsn);

    // open connection
    if( !db.open())
        return;  //! db error
    // open connection
    if( !db.open())
        return;  //! db error

    QSqlQuery query(db);
    QString sSql;
    bool state;

    //创建第一个表格
    // drop the table if it's already exists
    sSql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec( sSql);

    //create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    sSql += "[TotleSpec] char(80),[Spec] char(80),[TestLotNo] char(80), [DetailSpec] char(80), [LotNo] char(80),[TestNumber] char(80),[TestFreq] char(80),[Instrument] char(80)";
    sSql += ")";
    state = query.prepare( sSql);
    if( !query.exec()) {
        printError( query.lastError());
        goto CLOSE; //! create failed
    }

    //insert a record
    sSql = QString("INSERT INTO [%1] ").arg( sheetName);
    sSql += "(TotleSpec, Spec, TestLotNo, DetailSpec, LotNo, TestNumber, TestFreq, Instrument) VALUES(:data1, :data2, :data3, :data4, :data5, :data6, :data7, :data8)";
    state = query.prepare( sSql);

    //circle
    query.bindValue(":data1",txtTotleSpec->text());
    query.bindValue(":data2",txtSpec->text());
    query.bindValue(":data3",txtTestLotNo->text());
    query.bindValue(":data4",txtDetailSpec->text());
    query.bindValue(":data5",txtLotNo->text());
    query.bindValue(":data6",txtNumber->text());
    query.bindValue(":data7",txtTestFreq->text());
    query.bindValue(":data8",txtIntrument->text());

    if( !query.exec())
    {
        printError( query.lastError());
        goto CLOSE; //! insert failed
    }

    //创建测试项目表格···························································



    //创建测试数据表格···························································



CLOSE:

    db.close();

    QString  strFilePath = QFileDialog::
            getSaveFileName(this,tr("保存Excel文件"), tmpDir,
                            tr("Excel Files (*.xls)"),0);

    if(!strFilePath.isEmpty())
    {
        if(QFile::exists(strFilePath))
            QFile::remove(strFilePath);
        QFileInfo f(strFilePath);
        tmpDir= f.absoluteDir().path();
        saveSettings();
        QFile::copy("./Sample.xls", strFilePath);
        QString path = QString("file:///") + strFilePath;
        QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));

    }
    btnExport->setEnabled(true);
}

void clsShowReport::readSettings()
{
    clsSettings settings;
    QString strNode ="Report/";

    settings.readSetting(strNode +"TmpDir",this->tmpDir);

    QString tmp;

    settings.readSetting(strNode +"TotleSpec",tmp);
    txtTotleSpec->setText(tmp);
    settings.readSetting(strNode+"TestSpec",tmp);
    txtSpec->setText(tmp);
    settings.readSetting(strNode+"DetailSpec",tmp);
    txtDetailSpec->setText(tmp);
    settings.readSetting(strNode+"LotNo",tmp);
    txtLotNo->setText(tmp);
    settings.readSetting(strNode+"TestLotNo",tmp);
    txtTestLotNo->setText(tmp);

}

void clsShowReport::saveSettings()
{
    clsSettings settings;
    QString strNode ="Report/";

    settings.writeSetting(strNode +"TmpDir",this->tmpDir);
    settings.writeSetting(strNode +"TotleSpec",this->txtTotleSpec->text());
    settings.writeSetting(strNode+"TestSpec",this->txtSpec->text());
    settings.writeSetting(strNode+"DetailSpec",this->txtDetailSpec->text());
    settings.writeSetting(strNode+"LotNo",this->txtLotNo->text());
    settings.writeSetting(strNode+"TestLotNo",this->txtTestLotNo->text());
}

void clsShowReport::printError( QSqlError error)
{
    QString sqlerr = error.text();
    qDebug( qPrintable(sqlerr));
}

