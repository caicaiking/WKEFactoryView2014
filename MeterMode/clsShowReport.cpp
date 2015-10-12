#include "clsShowReport.h"
#include <QDebug>
#include <QFileDialog>
#include "clsSettings.h"
#include <QMessageBox>
#include "doubleType.h"
#include "clsRuningSettings.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "clsMeterLimit.h"
#include "UserfulFunctions.h"

clsShowReport::clsShowReport(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    readSettings();
    addCmbReport();
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

void clsShowReport::on_btnExport_clicked() {
    //变量声明
    QStringList testItems;
    QStringList repItem;
    QStringList suffix;
    QList<clsMeterLimit> limits;
    QStringList tmpList;
    QStringList limitShow;

    btnExport->setEnabled(false);
    if(QFile::exists("./Sample.xls"))
        QFile::remove("./Sample.xls");

    if(cmbReport->currentText().isEmpty())
    {
        QMessageBox::warning(this,tr("报表样板"),tr("没有可用的报表样板！"));
        return;
    }
    QString reportSample= QString("./Sample/%1").arg(cmbReport->currentText());
    QFile::copy(reportSample,"./Sample.xls");

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

    //最后一个参数 本来是使用char（800） 但是由于数据的字段长度超过了255 不能正确的运行， 更改成text 但是不知道会有什么问题？？
    sSql += "[TotleSpec] char(80),[Spec] char(80),[TestLotNo] char(80), [DetailSpec] char(80), "
            "[LotNo] char(80),[TestNumber] char(80),[TestFreq] char(80),[Instrument] char(80),"
            "[TestPar] text, [TestLimits] text";
    sSql += ")";
    state = query.prepare( sSql);
    if( !query.exec()) {
        printError( query.lastError());
        goto CLOSE; //! create failed
    }

    //insert a record
    sSql = QString("INSERT INTO [%1] ").arg( sheetName);
    sSql += "(TotleSpec, Spec, TestLotNo, DetailSpec, LotNo, TestNumber, TestFreq, Instrument,TestPar,TestLimits)"
            " VALUES(:data1, :data2, :data3, :data4, :data5, :data6, :data7, :data8, :data9, :data10)";
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



    for(int i=0; i< result->getStepCount();i++)
    {
        QString strCondition = result->getConditon(i);

        result->getMeter()->setCondition(strCondition);

        QString equCCT = result->getMeter()->getEqucct();

        QStringList stepItem;
        QStringList repStepItem;


        for(int j=0; j<result->getMeter()->getCountTestItems(); j++)
        {
            //qDebug()<<result->getMeter()->getItem(j);
            QString tmpItem = result->getMeter()->getItem(j);

            //在这儿进行了一个 Theta和A的统一的一个参数表示成A，防止乱码导致运算出错
            if(tmpItem == tr("θ"))
                tmpItem="A";


            stepItem.append(result->getMeter()->getItem(j));
            suffix.append(result->getMeter()->getSuffix(j));
            limits.append(result->getMeter()->getLimit(j));

            QString LCR(tr("LCR"));

            if(LCR.contains(tmpItem))
            {
                if(equCCT== tr("串联"))
                    repStepItem.append(result->getMeter()->getItem(j)+"s");
                else
                    repStepItem.append(result->getMeter()->getItem(j)+"p");
            }
            else if(tr("A")== tmpItem)
            {
                if(equCCT== tr("串联"))
                    repStepItem.append(result->getMeter()->getItem(j)+"z");
                else
                    repStepItem.append(result->getMeter()->getItem(j)+"y");
            }
            else
            {
                repStepItem.append(result->getMeter()->getItem(j));
            }
        }

        testItems.append(stepItem);
        repItem.append(repStepItem);
    }
    query.bindValue(":data9",repItem.join(","));
    //将limit格式化显示

    for(int i=0; i< testItems.length(); i++)
    {
        clsMeterLimit limit = limits.at(i);
        QString tmpSuffix = suffix.at(i);

        QString tmpStr;
        doubleType dt;
        dt.setData(limit.getAbsLimitLow());
        tmpStr+= dt.formateWithUnit(tmpSuffix,5);
        tmpStr +=" ";

        dt.setData(limit.getAbsLimitHigh());
        tmpStr+= dt.formateWithUnit(tmpSuffix,5);

        limitShow.append(tmpStr);
    }
    query.bindValue(":data10",limitShow.join(","));


    if( !query.exec())
    {
        printError( query.lastError());
        goto CLOSE; //! insert failed
    }

    //创建测试数据表格···························································
    sheetName = "TestData";
    //创建第二个表格 用于存储数据和状态
    // drop the table if it's already exists
    sSql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec( sSql);
    //create the table (sheet in Excel file)
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);

    tmpList.clear();
    for(int i=0; i< testItems.length(); i++)
    {
        tmpList.append(QString("[ItemData%1] char(20)").arg(i));
        tmpList.append(QString(" [TestStatus%1] char(20) ").arg(i));
    }

    sSql += tmpList.join(",");
    sSql += ")";
    state = query.prepare( sSql);
    if( !query.exec()) {
        printError( query.lastError());
        goto CLOSE; //! create failed
    }

    //insert a record
    sSql = QString("INSERT INTO [%1] ").arg( sheetName);
    //写入要插入的项目
    sSql+="(";

    tmpList.clear();
    for(int i=0; i< testItems.length(); i++)
    {
        tmpList.append(QString("ItemData%1").arg(i));
        tmpList.append(QString("TestStatus%1").arg(i));
    }

    sSql+= tmpList.join(",");
    //写入要绑定Value值
    sSql+=") VALUES (";
    tmpList.clear();
    for(int i=0; i< testItems.length(); i++)
    {
        tmpList.append(QString(":ItemData%1").arg(i));
        tmpList.append(QString(":TestStatus%1").arg(i));
    }
    sSql+= tmpList.join(",");
    sSql+=")";

    state = query.prepare( sSql);

    //circle

    for(int j=0; j<result->getTestCount(); j++)
    {

        TESTDATA_STRUCT singleDutRes = result->getTestData(j);

        QList<double> lSingleDutRes;

        for(int z=0; z<singleDutRes.data.length(); z++)
        {
            for(int zz=0; zz<singleDutRes.data.at(z).length();zz++)
                lSingleDutRes.append(singleDutRes.data.at(z).at(zz));
        }

        for(int i=0; i< testItems.length(); i++)
        {
            double tmpData = lSingleDutRes.at(i);
            QString tmpSuffix = suffix.at(i);
            clsMeterLimit tmpLimit = limits.at(i);

            QString tmpSt= (tmpLimit.comparaValue(tmpData)?"P":"F");
            doubleType dt;
            dt.setData(tmpData);

            query.bindValue(QString(":ItemData%1").arg(i),dt.formateWithUnit(tmpSuffix,5));
            query.bindValue(QString(":TestStatus%1").arg(i),tmpSt);
        }


        if( !query.exec())
        {
            printError( query.lastError());
            goto CLOSE; //! insert failed
        }

    }

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

void clsShowReport::addCmbReport()
{
    QDir dir("./Sample");
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files);
    QStringList filter;
    filter<<"*.xls";
    QFileInfoList list = dir.entryInfoList(filter);

    QStringList excels;
    for(int i=0; i<list.length(); i++)
    {
         excels<<list.at(i).fileName();
    }

    if(excels.isEmpty())
        return;

    cmbReport->addItems(excels);
}
