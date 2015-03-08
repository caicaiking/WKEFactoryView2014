#include "clsMultiLimitStatusShow.h"

clsMultiLimitStatusShow::clsMultiLimitStatusShow(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags() &~Qt::WindowCloseButtonHint &~Qt::WindowContextHelpButtonHint);
}


void clsMultiLimitStatusShow::on_lblClose_clicked()
{
    this->hide();
}
WKEInstrument *clsMultiLimitStatusShow::getMeter() const
{
    return meter;
}

void clsMultiLimitStatusShow::setMeter(WKEInstrument *value)
{
    meter = value;
}



void clsMultiLimitStatusShow::updateTable()
{

    tbMultiLimit->clear();
    tbMultiLimit->verticalHeader()->setVisible(false);
    tbMultiLimit->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbMultiLimit->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbMultiLimit->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QStringList header;
    header<<tr("频率范围")<<tr(" %1 上下限").arg(meter->getItem1())<<tr(" %1 上下限").arg(meter->getItem2());

    for(int x=0;x<tbMultiLimit->columnCount(); x++)
    {
        QTableWidgetItem * item= new QTableWidgetItem();

        item->setText(header[x]);
        item->setFont(QFont("楷体", 12/*, QFont::Bold*/));
        item->setBackgroundColor(QColor(Qt::gray));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tbMultiLimit->setHorizontalHeaderItem(x,item);
    }

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
    }

}

void clsMultiLimitStatusShow::setCurveLimit(const clsCurveLimit &value)
{
    this->curveLimit=value;
    updateTable();
}

void clsMultiLimitStatusShow::setMultiCurveLimit(const clsMultiCurveLimt &value)
{
    this->multiCurveLimit = value;
    updateTable();
}
