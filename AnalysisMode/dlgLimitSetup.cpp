#include "dlgLimitSetup.h"
#include <QDebug>
#include "UserfulFunctions.h"
#include <QColorDialog>
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
}

void dlgLimitSetup::setCurveLimit(const clsCurveLimit &curveLimit)
{
    grpTraceA->setChecked(curveLimit.blTraceALimit);
    grpTraceB->setChecked(curveLimit.blTraceBLimit);

    traceAUpColor = curveLimit.traceAUp;
    traceADownColor = curveLimit.traceADown;
    traceBUpColor = curveLimit.traceBUp;
    traceBDownColor = curveLimit.traceBDown;

    traceALimit = curveLimit.cmlTraceALimit;
    traceBLimit = curveLimit.cmlTraceBLimit;

    updateButtons();
}

void dlgLimitSetup::updateButtons()
{
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
