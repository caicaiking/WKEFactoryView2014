#include "clsStatistics.h"
#include <QFileDialog>
#include <QDebug>
#include "UserfulFunctions.h"
#include <QMessageBox>
#include "PlotDataAnlysis.h"
#include "doubleType.h"
clsStatistics::clsStatistics(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~ Qt::WindowContextHelpButtonHint);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

}

void clsStatistics::setData(clsDataStore *value)
{
    this->result= value;

    if(this->result->getStepCount() == 0)
    {
        QMessageBox::warning(this,tr("数据统计"),tr("没有可用的数据，用于数据统计"));
        this->close();
    }
    else
    {
        this->updateButtons();
        this->exec();
    }
}

void clsStatistics::updateButtons()
{
    this->txtStep->setMaximum(result->getStepCount());
    this->txtItem->setMaximum(result->getItemCount(0));
    lblItem->setText(result->getItem(txtStep->value()-1,txtItem->value()-1));
}

void clsStatistics::on_txtStep_valueChanged(int arg1)
{
    txtItem->setMaximum(result->getItemCount(arg1-1));
    lblItem->setText(result->getItem(arg1-1,txtItem->value()-1));
}

void clsStatistics::on_txtItem_valueChanged(int arg1)
{
    lblItem->setText(result->getItem(txtStep->value()-1,arg1-1));
}


void clsStatistics::on_btnDisplay_clicked()
{
    int step = txtStep->value()-1;
    int item = txtItem->value()-1;

    QList<double> testData= result->getData(step,item);

    if(testData.length()<10)
    {
        QMessageBox::warning(this,tr("数据统计"),tr("测试数据少于10个，请多测试一些产品再分析。"));
        return;
    }

    barPlot->setTitle(QString("%1(%2)").arg(lblItem->text()).arg(UserfulFunctions::getSuffix(lblItem->text())),tr("频数"),tr(""));

    PlotDataAnlysis an;
    an.setTestData(testData.toVector());
    barPlot->setPlotDataAnalysis(an);

    doubleType dt;
    dt.setData(barPlot->getMeanValue());

    lblAverage->setText(dt.formateToString()+ UserfulFunctions::getSuffix(lblItem->text()));

    dt.setData(barPlot->getStdValue());
    lblStd->setText(dt.formateToString());

    dt.setData(barPlot->getPos3Std());
    lblPos3Sigma->setText(dt.formateToString());

    dt.setData(barPlot->getNeg3Std());
    lblNeg3Sigma->setText(dt.formateToString());
}

void clsStatistics::on_btnExport_clicked()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
    {

        QPixmap  originalPixmap = screen->grabWindow(this->winId());

        QString format = "png";
        QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

        QString fileName = QFileDialog::getSaveFileName(this, tr("保存测试图像"),
                                                        initialPath,
                                                        tr("%1 Files (*.%2);;All Files (*)")
                                                        .arg(format.toUpper())
                                                        .arg(format));
        if (!fileName.isEmpty())
            originalPixmap.save(fileName, "png");
        else
            return;
    }

}


