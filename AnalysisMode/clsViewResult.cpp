#include "clsViewResult.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include <Qwt/qwt_plot_curve.h>
#include <QFileDialog>
#include <qexcel.h>
#include <QFile>
#include <QDir>
#include <QFileInfo>

clsViewResult::clsViewResult(const QList<PlotCurves> value,
                             const QStringList Headers, QWidget *parent):
    QDialog(parent)
{

    setupUi(this);

    this->curves=value;

    txtGroup->setMaximum(curves.length());
    tvResult->setColumnCount(3);


    tvResult->verticalHeader()->setVisible(false);

    tvResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tvResult->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvResult->verticalHeader()->setResizeMode(QHeaderView::Stretch);



    for(int x=0;x<tvResult->columnCount(); x++)
    {
        QTableWidgetItem * item= new QTableWidgetItem();

        item->setText(Headers[x]);
        item->setFont(QFont("楷体", 12, QFont::Bold));
        item->setBackgroundColor(QColor(Qt::gray));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tvResult->setHorizontalHeaderItem(x,item);
    }

//    tvResult->horizontalHeader()->setStyleSheet(
//                "QHeaderView::section{background-color: rgb(217,217,217); font:  '楷体'};");

    showCurve(txtGroup->value());

    connect(txtGroup,SIGNAL(valueChanged(int)),this,SLOT(showCurve(int)));
}


void clsViewResult::setCurves(const QList<PlotCurves> &value)
{
    this->curves=value;
}

void clsViewResult::setLabels(QString sweepItems, QString strTraceA, QString strTraceB)
{
    this->strSweepItems=sweepItems;
    this->strTraceA=strTraceA;
    this->strTraceB= strTraceB;
}

void clsViewResult::on_btnCancel_clicked()
{
    this->reject();
}

void clsViewResult::on_btnSave_clicked()
{
    QString fileName=txtTitle->text();
    fileName = QFileDialog::getSaveFileName(
                this, "Export Test Result File Name", fileName,
                "Excel 97-2003 Work Book(*.xls);;CSV files (*.csv)", NULL/*, QFileDialog::DontConfirmOverwrite*/);


    if(fileName.isNull())
        return;

    if(QFile::exists(fileName))
    {
        QDir fileDir=QFileInfo(fileName).dir();
        if(!fileDir.remove(fileName))
            return;
    }

    QFileInfo temDir(fileName);

    QString fileSuffix = temDir.suffix();
    if(fileSuffix.toLower()=="xls")
        saveExcel(fileName);
    else
        saveCsv(fileName);
    qDebug()<<"Save Result End";
    qApp->processEvents();

    UserfulFunctions::sleepMs(500);
    this->accept();
}
void clsViewResult::saveExcel(QString fileName)
{
    QExcel j(fileName);


    //取得工作表数量
    // qDebug()<<"SheetCount"<<j.getSheetsCount();
    //激活一张工作表
    j.selectSheet(1);
    //设置列宽
    //j.selectSheet(1);
    j.setColumnWidth(1, 20);
    j.setColumnWidth(2, 20);
    j.setColumnWidth(3, 20);
    //j.save();
    //设置粗体
    // j.selectSheet(1);
    j.setCellFontBold(1, 1, true);
    j.setCellFontBold(1, 2, true);
    j.setCellFontBold(1, 3, true);
    //j.save();
    //设置文字大小
    // j.selectSheet(1);
    j.setCellFontSize(1,1, 10);
    j.setCellFontSize(1,2, 10);
    j.setCellFontSize(1,3, 10);
    //j.save();


    //设置单元格文字居中
    // j.selectSheet(1);
    j.setCellTextCenter(1, 1);
    j.setCellTextCenter(1, 2);
    j.setCellTextCenter(1, 3);
    //j.save();



    //插入表头
    //j.selectSheet(1);
    j.setCellString(1,1,tvResult->horizontalHeaderItem(0)->text());
    j.setCellString(1,2,tvResult->horizontalHeaderItem(1)->text());
    j.setCellString(1,3,tvResult->horizontalHeaderItem(2)->text());
    //j.save();

    //插入测试内容
    //j.selectSheet(1);

    int i=2;  //1 是表头
    QMapIterator <double ,QPointF> it(data);
    while(it.hasNext())
    {
        it.next();
        double sweepKey=it.key();
        QPointF testData= it.value();
        j.setCellTextCenter(i, 1);
        j.setCellTextCenter(i, 2);
        j.setCellTextCenter(i, 3);
        j.setCellString(i,1, QString("%1").arg(sweepKey));
        j.setCellString(i,2,QString("%1").arg(testData.x()));
        j.setCellString(i,3,QString("%1").arg(testData.y()));
        i++;
        qApp->processEvents();

    }

    j.save();
    j.close();
}

void clsViewResult::showCurve(int intSelect)
{
    intSelect-=1;
    if(intSelect>=curves.length())
        return ;

    // qDebug()<< curves.at(intSelect).cur1->dataSize();
    data.clear();

   // tvResult->setRowCount(curves.at(intSelect).cur1->data()->size());
    txtTitle->setText(curves.at(intSelect).cur1->title().text());
    for(int i=0; i< curves.at(intSelect).cur1->data()->size();i++)
    {
        QPointF ptData;
        QPointF ptX=  curves.at(intSelect).cur1->data()->sample(i);
        ptData.setX(ptX.y());
        QPointF ptY=  curves.at(intSelect).cur2->data()->sample(i);
        ptData.setY(ptY.y());
        data.insert(ptX.x(),ptData);
    }

    formateDataShow();
}

void clsViewResult::formateDataShow()
{
    int i=0;
    QMapIterator <double ,QPointF> it(data);
    while(it.hasNext())
    {
        it.next();
        tvResult->setRowCount(i+1);
        double sweepKey=it.key();
        QPointF testData= it.value();

        doubleType dt(sweepKey,"");

        QString str1 = dt.formateToString(7);

        dt.setData(testData.x(),"");
        QString str2 = dt.formateToString(7);

        dt.setData(testData.y(),"");
        QString str3= dt.formateToString(7);

        QTableWidgetItem *newItem1 = new QTableWidgetItem(str1);
        newItem1->setFont(QFont("Times", 12));
        newItem1->setTextAlignment(Qt::AlignHCenter);
        tvResult->setItem(i, 0, newItem1);

        QTableWidgetItem *newItem2 = new QTableWidgetItem(str2);
        newItem2->setFont(QFont("Times", 12));
        newItem2->setTextAlignment(Qt::AlignHCenter);
        tvResult->setItem(i, 1, newItem2);


        QTableWidgetItem *newItem3 = new QTableWidgetItem(str3);
        newItem3->setFont(QFont("Times", 12));
        newItem3->setTextAlignment(Qt::AlignHCenter);
        tvResult->setItem(i, 2, newItem3);

        i++;
    }
}



void clsViewResult::on_tvResult_doubleClicked(const QModelIndex &index)
{
    int row=index.row();
    QList<double>keys= data.keys();
    emit setMark((double)keys.at(row),1);
}


void clsViewResult::saveCsv(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream t(&file);
    t.setCodec("GBK");

    t<<tvResult->horizontalHeaderItem(0)->text()<<","
    <<tvResult->horizontalHeaderItem(1)->text()<<","
    <<tvResult->horizontalHeaderItem(2)->text()<<"\n";

    QMapIterator <double ,QPointF> it(data);
    while(it.hasNext())
    {
        it.next();
        double sweepKey=it.key();
        QPointF testData= it.value();

        t<< QString("%1").arg(sweepKey)<<","
         <<QString("%1").arg(testData.x())<<","
        <<QString("%1").arg(testData.y())<<"\n";

        qApp->processEvents();

    }

    t.flush();
    file.close();
}
