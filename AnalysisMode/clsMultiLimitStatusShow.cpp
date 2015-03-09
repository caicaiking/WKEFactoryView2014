#include "clsMultiLimitStatusShow.h"
#include "doubleType.h"
clsMultiLimitStatusShow::clsMultiLimitStatusShow(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint  );
    //setWindowFlags(windowFlags() &~Qt::WindowCloseButtonHint &~Qt::WindowContextHelpButtonHint);

    this->tbMultiLimit->setStyleSheet("QTableWidget::item:selected { background-color: rgb(255, 0, 0) }");
}


void clsMultiLimitStatusShow::on_lblClose_clicked()
{
    this->hide();
}

void clsMultiLimitStatusShow::setMeter(WKEInstrument *value)
{
    meter = value;
    this->curveLimit.setItem(meter->getItem1(),meter->getItem2());
    this->multiCurveLimit.setItem(meter->getItem1(),meter->getItem2());
    updateTable();
}

void clsMultiLimitStatusShow::updateStatus()
{
    this->updateTable();
}

void clsMultiLimitStatusShow::updateTable()
{

    tbMultiLimit->clear();
    tbMultiLimit->verticalHeader()->setVisible(false);
    tbMultiLimit->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbMultiLimit->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tbMultiLimit->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tbMultiLimit->setColumnCount(4);
    tbMultiLimit->setRowCount(multiCurveLimit.limits.length());

    QStringList header;
    header<<tr("频率范围")<<tr(" %1 上下限").arg(meter->getItem1())<<tr(" %1 上下限").arg(meter->getItem2())<<tr("状态");

    for(int x=0;x<tbMultiLimit->columnCount(); x++)
    {
        QTableWidgetItem * item= new QTableWidgetItem();

        item->setText(header[x]);
        item->setFont(QFont("楷体", 12/*, QFont::Bold*/));
        item->setBackgroundColor(QColor(Qt::gray));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tbMultiLimit->setHorizontalHeaderItem(x,item);
    }

    if(this->curveLimit.intSlect!=0)
    {
        tbMultiLimit->setRowCount(multiCurveLimit.limits.length());

        for(int i=0; i< multiCurveLimit.limits.length(); i++)
        {
            int maxHeight ;
            QLabel *lblFreq = new QLabel();
            clsMultiLimits tmpLimit =multiCurveLimit.limits.at(i);
            lblFreq->setText(tmpLimit.getFreqRange());
            tbMultiLimit->setCellWidget(i,0,lblFreq);
            maxHeight = lblFreq->height();

            QLabel *lblItem1 = new QLabel();
            lblItem1->setText(tmpLimit.getLimit1Show());
            tbMultiLimit->setCellWidget(i,1,lblItem1);
            maxHeight =(maxHeight< lblItem1->height()? lblItem1->height(): maxHeight);

            QLabel *lblItem2 = new QLabel();
            lblItem2->setText(tmpLimit.getLimit2Show());
            tbMultiLimit->setCellWidget(i,2,lblItem2);
            maxHeight =(maxHeight< lblItem2->height()? lblItem2->height(): maxHeight);
            tbMultiLimit->setRowHeight(i,maxHeight);

            QLabel *lblItem3 = this->getStatusLabel(this->multiCurveLimit.limits[i].getStatus());

            tbMultiLimit->setCellWidget(i,3,lblItem3);
            maxHeight =(maxHeight< lblItem2->height()? lblItem2->height(): maxHeight);
            tbMultiLimit->setRowHeight(i,maxHeight);
        }
    }
    else
    {
        tbMultiLimit->setRowCount(1);
        int maxHeight ;
        QLabel *lblFreq = new QLabel();

        lblFreq->setText(tr("扫描范围"));
        tbMultiLimit->setCellWidget(0,0,lblFreq);
        maxHeight = lblFreq->height();

        QLabel *lblItem1 = new QLabel();
        lblItem1->setText(curveLimit.getLimit1Show());
        tbMultiLimit->setCellWidget(0,1,lblItem1);
        maxHeight =(maxHeight< lblItem1->height()? lblItem1->height(): maxHeight);

        QLabel *lblItem2 = new QLabel();
        lblItem2->setText(curveLimit.getLimit2Show());
        tbMultiLimit->setCellWidget(0,2,lblItem2);
        maxHeight =(maxHeight< lblItem2->height()? lblItem2->height(): maxHeight);
        tbMultiLimit->setRowHeight(0,maxHeight);

        QLabel *lblItem3 = this->getStatusLabel(this->curveLimit.status);

        tbMultiLimit->setCellWidget(0,3,lblItem3);
        maxHeight =(maxHeight< lblItem2->height()? lblItem2->height(): maxHeight);
        tbMultiLimit->setRowHeight(0,maxHeight);
    }
}

void clsMultiLimitStatusShow::setCurveLimit( clsCurveLimit value)
{
    this->curveLimit=value;
    this->curveLimit.setItem(meter->getItem1(),meter->getItem2());
    updateTable();
}

void clsMultiLimitStatusShow::setMultiCurveLimit( clsMultiCurveLimt value)
{
    this->multiCurveLimit = value;
    this->multiCurveLimit.setItem(meter->getItem1(),meter->getItem2());
    updateTable();
}

void clsMultiLimitStatusShow::setInfo(QString value)
{
    QStringList list = value.split(",");

    if(this->curveLimit.intSlect==0)
    {
        clsCurveLimit tmpLimit = this->curveLimit;
        tmpLimit.resetStatus();

        if(list.length()<3)
            return;
        tmpLimit.compareValue(list.at(1).toDouble(),list.at(2).toDouble());
        setLabel(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble(),tmpLimit.status);

    }
    else
    {
        clsMultiCurveLimt tmpLimt = this->multiCurveLimit;
        tmpLimt.resetStatus();
        if(list.length()<3)
            return;

        int value = multiCurveLimit.inLimit(list.at(0).toDouble());

        if(value< this->tbMultiLimit->rowCount())
            tbMultiLimit->setCurrentCell(value,0);
        tmpLimt.compareValue(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble());
        setLabel(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble(),tmpLimt.getStatus());
    }
}

void clsMultiLimitStatusShow::setLabel(double freq,
                                       double item1, double item2, bool status)
{
    doubleType dt;
    QString feq;
    dt.setData(freq,"");
    feq= dt.formateToString(6)+"Hz";
    dt.setData(item1);
    QString strItem1;
    strItem1=dt.formateToString(6)+UserfulFunctions::getSuffix(meter->getItem1());
    dt.setData(item2);
    QString strItem2;
    strItem2 = dt.formateToString(6)+UserfulFunctions::getSuffix(meter->getItem2());

    QString strStatus;
    strStatus =(status?tr("<font color=\"green\">通过</font>"):tr("<font color=\"red\">失败</font>"));

    QString label = tr("频率：%1,%2:%3,%4:%5,%6:(%7)").arg(feq)
            .arg(meter->getItem1()).arg(strItem1)
            .arg(meter->getItem2()).arg(strItem2)
            .arg(tr("状态")).arg(strStatus);
    lblStatus->setText(label);

}

QLabel* clsMultiLimitStatusShow::getStatusLabel(const bool &value)
{
    QLabel *lable = new QLabel;

    if(value)
    {
        lable->setText(tr("√"));
        lable->setStyleSheet("background-color: green;");
    }
    else
    {
        lable->setText(tr("×"));
         lable->setStyleSheet("background-color: red;");
    }

    lable->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    return lable;
}

