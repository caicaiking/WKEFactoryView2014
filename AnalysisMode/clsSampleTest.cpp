#include "clsSampleTest.h"
#include "doubleType.h"
clsSampleTest::clsSampleTest(WKEInstrument *value,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    meter = value;

    lblItem1->setText(meter->getItem1());
    lblItem2->setText(meter->getItem2());
    lblTest1->setText("");
    lblTest2->setText("");
}

void clsSampleTest::on_btnClose_clicked()
{
    this->reject();
    this->close();
}


void clsSampleTest::on_btnTrig_clicked()
{
    btnTrig->setEnabled(false);
    QString str = meter->trig();

    QList<double> res = UserfulFunctions::resultPro(str);

    if(res.length()>=2)
    {
        doubleType dt;
        dt.setData(res.at(0));
        lblTest1->setText(dt.formateToString()+UserfulFunctions::getSuffix(meter->getItem1()));
        dt.setData(res.at(1));
        lblTest2->setText(dt.formateToString()+UserfulFunctions::getSuffix(meter->getItem2()));
    }

    btnTrig->setEnabled(true);
    btnTrig->setFocus();

}

void clsSampleTest::on_btnOk_clicked()
{
    this->accept();
}
