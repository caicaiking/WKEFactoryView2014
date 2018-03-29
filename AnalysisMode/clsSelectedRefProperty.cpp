#include "clsSelectedRefProperty.h"
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>


clsSelectedRefProperty::clsSelectedRefProperty(const curveProperty property,
                                               QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    this->setAutoFillBackground(true);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->btnStatus->setChecked(false);
    connect(btnTraceA,SIGNAL(clicked()),this,SLOT(setTraceAColor()));
    connect(btnTraceB,SIGNAL(clicked()),this,SLOT(setTraceBColor()));

    this->isOn=property.isOn;
    this->traceA=property.traceA;
    this->traceB=property.traceB;
    this->title=property.title;
    this->index=property.index;
    this->refTraceType = property.refTraceType;
    this->x= property.x1;
    this->y1= property.y1;
    this->y2=property.y2;

    updateTexts();

    origin =property;
}




void clsSelectedRefProperty::setTraceColors(QColor traceA, QColor traceB)
{
    btnTraceA->setAutoFillBackground(true);
    btnTraceA->setStyleSheet(getColorSheet(traceA));

    btnTraceB->setAutoFillBackground(true);
    btnTraceB->setStyleSheet(getColorSheet(traceB));

    this->traceA=traceA;
    this->traceB=traceB;
}

curveProperty clsSelectedRefProperty::getCurveProperty()
{
    this->title = (txtTitle->text().isEmpty()? btnIntSelected->text() : txtTitle->text());
    curveProperty pt(this->index,this->traceA,this->traceB,this->title,this->isOn,refTraceType,x,y1,y2);
    return pt;
}

void clsSelectedRefProperty::on_btnOk_clicked()
{
    qApp->processEvents();

    if(refTraceType==1)
    {
        emit refTraceChanged(getCurveProperty(),true);
    }
    else if(refTraceType==2)
    {
        emit refTraceChanged(getCurveProperty(),false);
    }
    else if(refTraceType==3)
    {
        emit refTraceChanged(getCurveProperty(),x.toVector(),y1.toVector(),y2.toVector());
    }
    this->accept();
}

QString clsSelectedRefProperty::getColorSheet(QColor color)
{
    QString styleSheet =QString("background-color: rgb(%1, %2, %3)")
            .arg(color.red()).arg(color.green())
            .arg(color.blue());

    return styleSheet;
}

void clsSelectedRefProperty::setTraceAColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid())
        return;
    QString styleSheet = getColorSheet(color);
    btnTraceA->setStyleSheet(styleSheet);
    this->traceA=color;
}


void clsSelectedRefProperty::setTraceBColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid())
        return;
    btnTraceB->setStyleSheet(getColorSheet(color));
    this->traceB=color;
}



void clsSelectedRefProperty::on_btnStatus_clicked(bool checked)
{
    this->isOn= checked;
    if(isOn)
        btnStatus->setText("ON");
    else
        btnStatus->setText("OFF");

    updateTexts();
}

void clsSelectedRefProperty::updateTexts()
{
    if(this->index != 0)
    {
        this->btnIntSelected->setText(QString("%1").arg(this->index));
    }
    else
    {

        this->btnIntSelected->setText(tr("默认"));
    }
    if(isOn)
        btnStatus->setText("ON");
    else
        btnStatus->setText("OFF");

    btnStatus->setChecked(isOn);

    setTraceColors(this->traceA,this->traceB);


    this->txtTitle->setText(this->title);

    if((refTraceType==1) && isOn)
        btnSaveCurrentCurve->setChecked(true);
    else
    {
        btnSaveCurrentCurve->setChecked(false);
        refTraceType =2;
    }

    this->update();
}

void clsSelectedRefProperty::on_txtTitle_lostFocus()
{
    title=txtTitle->text();
}

void clsSelectedRefProperty::on_btnLoadDataFile_clicked()
{
    QString strDataFile = QFileDialog::getOpenFileName(
                this, tr("打开数据文件"), "./",
                "CSV files (*.csv);;", NULL/*, QFileDialog::DontConfirmOverwrite*/);

    if(strDataFile.isEmpty())
        return;

    x.clear();
    y1.clear();
    y2.clear();


    QFile file(strDataFile);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
        return;

    while(!file.atEnd())
    {
        QString strLine = file.readLine();
        QList<double> tmp = lineProcess(strLine);

        if(tmp.length()>=3)
        {
            x.append(tmp.at(0));
            y1.append(tmp.at(1));
            y2.append(tmp.at(2));
        }
    }

    file.close();
    refTraceType=3;
}

QList<double> clsSelectedRefProperty::lineProcess(QString strRes)
{
    QString sp = (QLocale().decimalPoint()=='.' ? ",":";");

    QList<double> tmp;
    tmp.clear();

    QStringList res = strRes.split(sp);

    if(res.length()<3)
        return tmp;
    bool ok1,ok2,ok3;
    double res1= res.at(0).toDouble(&ok1) ;
    double res2 = res.at(1).toDouble(&ok2);
    double res3 = res.at(2).toDouble(&ok3);

    if(ok1 && ok2 && ok3)
    {
        tmp.append(res1);
        tmp.append(res2);
        tmp.append(res3);
    }
    else
    {
        tmp.clear();
    }
    return tmp;
}

void clsSelectedRefProperty::on_btnSaveCurrentCurve_clicked(bool checked)
{
    if(checked)
        refTraceType=1;
    else
        refTraceType=2;
}

