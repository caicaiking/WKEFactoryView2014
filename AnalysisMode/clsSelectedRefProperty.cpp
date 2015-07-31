#include "clsSelectedRefProperty.h"
#include <QDebug>
#include <QColorDialog>



void clsSelectedRefProperty::setTraceColors(QColor traceA, QColor traceB)
{
    clrTraceA->setStyleSheet(QString("background-color: rgb(%1, %2, %3);;")
                             .arg(traceA.toRgb().red()).arg(traceA.toRgb().green())
                             .arg(traceA.toRgb().blue()));

    clrTraceB->setStyleSheet(QString("background-color: rgb(%1, %2, %3);;")
                             .arg(traceB.toRgb().red()).arg(traceB.toRgb().green())
                             .arg(traceB.toRgb().blue()));

    this->traceA=traceA;
    this->traceB=traceB;

}

curveProperty clsSelectedRefProperty::getCurveProperty()
{
    this->title = txtTitle->text();
    curveProperty pt(this->index,this->traceA,this->traceB,this->title,this->isOn);
    return pt;
}

void clsSelectedRefProperty::on_btnOk_clicked()
{
    qApp->processEvents();

    if(getCurveProperty()!=origin || btnSaveCurrentCurve->isChecked())
    {
        emit refTraceChanged(getCurveProperty(),btnSaveCurrentCurve->isChecked());
    }
    this->accept();
}

void clsSelectedRefProperty::setTraceAColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    clrTraceA->setStyleSheet(QString("background-color: rgb(%1, %2, %3);;")
                             .arg(color.toRgb().red()).arg(color.toRgb().green())
                             .arg(color.toRgb().blue()));
    this->traceA=color;
}


void clsSelectedRefProperty::setTraceBColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    clrTraceB->setStyleSheet(QString("background-color: rgb(%1, %2, %3);;")
                             .arg(color.toRgb().red()).arg(color.toRgb().green())
                             .arg(color.toRgb().blue()));

    this->traceB=color;
}



void clsSelectedRefProperty::on_btnStatus_clicked(bool checked)
{
    this->isOn= checked;
    if(isOn)
        btnStatus->setText("ON");
    else
        btnStatus->setText("OFF");

    btnSaveCurrentCurve->setCheckable(isOn);
}

void clsSelectedRefProperty::updateTexts()
{
    this->btnIntSelected->setText(QString("%1").arg(this->index));
    if(isOn)
        btnStatus->setText("ON");
    else
        btnStatus->setText("OFF");
    btnStatus->setChecked(isOn);

    setTraceColors(this->traceA,this->traceB);
    this->txtTitle->setText(this->title);

    btnSaveCurrentCurve->setCheckable(this->isOn);
}


clsSelectedRefProperty::clsSelectedRefProperty(const curveProperty property,
                                               QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
 setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->btnStatus->setChecked(false);
    connect(clrTraceA,SIGNAL(Clicked()),this,SLOT(setTraceAColor()));
    connect(clrTraceB,SIGNAL(Clicked()),this,SLOT(setTraceBColor()));

    this->isOn=property.isOn;
    this->traceA=property.traceA;
    this->traceB=property.traceB;
    this->title=property.title;
    this->index=property.index;
    updateTexts();

    origin =property;
}



void clsSelectedRefProperty::on_txtTitle_lostFocus()
{
    title=txtTitle->text();
}
