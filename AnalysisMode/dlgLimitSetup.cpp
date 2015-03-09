#include "dlgLimitSetup.h"
#include <QDebug>
#include "UserfulFunctions.h"
#include <QColorDialog>
#include "clsMultiLimits.h"
#include "clsMultiLimitSetting.h"
dlgLimitSetup::dlgLimitSetup(WKEInstrument*value,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->meter = value;
    connect(this->btnTraceALimit,SIGNAL(Clicked()),this,SLOT(traceAClick()));
    connect(this->btnTraceBLimit,SIGNAL(Clicked()),this,SLOT(traceBClick()));
    connect(this->clrTraceAUp,SIGNAL(Clicked()),this,SLOT(setTraceAUpColor()));
    connect(this->clrTraceADown,SIGNAL(Clicked()),this,SLOT(setTraceADownColor()));
    connect(this->clrTraceBUp,SIGNAL(Clicked()),this,SLOT(setTraceBUpColor()));
    connect(this->clrTraceBDown,SIGNAL(Clicked()),this,SLOT(setTraceBDownColor()));

    grpTraceA->setTitle(tr("设定 %1 上下限").arg(meter->getItem1()));
    grpTraceB->setTitle(tr("设定 %1 上下限").arg(meter->getItem2()));
    updateTable();


}

void dlgLimitSetup::setCurveLimit(const clsCurveLimit &curveLimit)
{

    grpTraceA->setChecked(curveLimit.blTraceALimit);
    grpTraceB->setChecked(curveLimit.blTraceBLimit);
    chkPassSound->setChecked(curveLimit.blPassSound);
    chkFailSound->setChecked(curveLimit.blFailSound);

    traceAUpColor = curveLimit.traceAUp;
    traceADownColor = curveLimit.traceADown;
    traceBUpColor = curveLimit.traceBUp;
    traceBDownColor = curveLimit.traceBDown;

    traceALimit = curveLimit.cmlTraceALimit;
    traceBLimit = curveLimit.cmlTraceBLimit;

    int select = curveLimit.intSlect;
    if(select< tabWidget->count())
        tabWidget->setCurrentIndex(select);

    updateButtons();
}



void dlgLimitSetup::updateTable()
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
    multiCurveLimit.setItem(meter->getItem1(),meter->getItem2());
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

void dlgLimitSetup::updateButtons()
{
    grpTraceA->setTitle(tr("设定 %1 上下限").arg(meter->getItem1()));
    grpTraceB->setTitle(tr("设定 %1 上下限").arg(meter->getItem2()));

    btnTraceALimit->setText(traceALimit.showLimits(UserfulFunctions::getSuffix(meter->getItem1())));
    btnTraceBLimit->setText(traceBLimit.showLimits(UserfulFunctions::getSuffix(meter->getItem2())));

    clrTraceAUp->setStyleSheet(getStyleSheet(traceAUpColor));
    clrTraceADown->setStyleSheet(getStyleSheet(traceADownColor));
    clrTraceBUp->setStyleSheet(getStyleSheet(traceBUpColor));
    clrTraceBDown->setStyleSheet(getStyleSheet(traceBDownColor));

}

clsCurveLimit dlgLimitSetup::getCurveLimit()
{
    clsCurveLimit tmp;
    tmp.blTraceALimit = grpTraceA->isChecked();
    tmp.blTraceBLimit = grpTraceB->isChecked();

    tmp.cmlTraceALimit = traceALimit;
    tmp.cmlTraceBLimit = traceBLimit;

    tmp.traceAUp = traceAUpColor;
    tmp.traceADown = traceADownColor;
    tmp.traceBUp = traceBUpColor;
    tmp.traceBDown = traceBDownColor;
    tmp.blPassSound = chkPassSound->isChecked();
    tmp.blFailSound = chkFailSound->isChecked();

    tmp.intSlect = tabWidget->currentIndex();

    return tmp;
}


void dlgLimitSetup::traceAClick()
{
    frmSetLimit limitA;
    limitA.setLimits(this->traceALimit);
    limitA.setWindowTitle(tr("设定 %1 上下限").arg(meter->getItem1()));
    if(limitA.exec())
    {
        traceALimit = limitA.getMeterLimit();
        btnTraceALimit->setText(traceALimit.showLimits(UserfulFunctions::getSuffix(meter->getItem1())));
    }
}

void dlgLimitSetup::traceBClick()
{
    frmSetLimit limitB;
    limitB.setLimits(this->traceBLimit);
    limitB.setWindowTitle(tr("设定 %1 上下限").arg(meter->getItem2()));
    if(limitB.exec())
    {
        traceBLimit = limitB.getMeterLimit();
        btnTraceBLimit->setText(traceBLimit.showLimits(UserfulFunctions::getSuffix(meter->getItem2())));
    }
}

void dlgLimitSetup::on_btnCancel_clicked()
{
    this->reject();
}

void dlgLimitSetup::on_btnOk_clicked()
{
    this->accept();
}

QString dlgLimitSetup::getStyleSheet(const QColor & color)
{
    return QString("background-color: rgb(%1, %2, %3);;")
            .arg(color.toRgb().red()).arg(color.toRgb().green())
            .arg(color.toRgb().blue());
}
clsMultiCurveLimt dlgLimitSetup::getMultiCurveLimit() const
{
    return multiCurveLimit;
}

void dlgLimitSetup::setMultiCurveLimit(const clsMultiCurveLimt &value)
{
    multiCurveLimit = value;
    updateTable();
}

void dlgLimitSetup::setTraceAUpColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    clrTraceAUp->setStyleSheet(getStyleSheet(color));
    this->traceAUpColor=color;
}

void dlgLimitSetup::setTraceADownColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    clrTraceADown->setStyleSheet(getStyleSheet(color));
    this->traceADownColor=color;
}

void dlgLimitSetup::setTraceBUpColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    clrTraceBUp->setStyleSheet(getStyleSheet(color));
    this->traceBUpColor=color;
}

void dlgLimitSetup::setTraceBDownColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    clrTraceBDown->setStyleSheet(getStyleSheet(color));
    this->traceBDownColor=color;
}

void dlgLimitSetup::on_btnAddLimit_clicked()
{
    clsMultiLimitSetting dlg;
    clsMultiLimits tmpLimit;
    dlg.setConditon(meter->getItem1(),meter->getItem2(),tmpLimit);
    if(dlg.exec())
    {
        tmpLimit = dlg.getLimts();
        tmpLimit.setItems(meter->getItem1(),meter->getItem2());
        this->multiCurveLimit.limits.append(tmpLimit);
        updateTable();
    }
}

void dlgLimitSetup::on_tbMultiLimit_doubleClicked(const QModelIndex &index)
{
    int row=index.row();

    if(row>=this->multiCurveLimit.limits.length())
        return;

    clsMultiLimitSetting dlg;
    clsMultiLimits tmpLimit = multiCurveLimit.limits.at(row);
    dlg.setConditon(meter->getItem1(),meter->getItem2(),tmpLimit);
    if(dlg.exec())
    {
        tmpLimit = dlg.getLimts();
        multiCurveLimit.limits.replace(row,tmpLimit);
        updateTable();
        this->tbMultiLimit->setCurrentCell(row,0);
    }
}

void dlgLimitSetup::on_btnDeletLimit_clicked()
{
    int row = this->tbMultiLimit->currentRow();

    if(row >= multiCurveLimit.limits.length())
        return;
    multiCurveLimit.limits.removeAt(row);

    updateTable();
}

void dlgLimitSetup::on_btnClearAllLimit_clicked()
{
    multiCurveLimit.limits.clear();
    updateTable();
}
