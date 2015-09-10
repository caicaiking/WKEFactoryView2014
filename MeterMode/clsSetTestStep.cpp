#include "clsSetTestStep.h"
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
#include <QLayout>
#include <QBoxLayout>
#include <QDebug>

clsSetTestStep::clsSetTestStep(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    sngTestDisplay->setMessage(tr("没有数据可以显示"),0);
    meter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(meter);

    connect(meter,SIGNAL(biasStatus(bool)),this,SLOT(biasSlot(bool)));
    meterWidget->setLayout(layout);

    connect(meter,SIGNAL(signalTestResult(QString)),this,SLOT(showRes(QString)));

    initSheetTitle();
}

void clsSetTestStep:: initSheetTitle()
{

    tbTaskList->verticalHeader()->setVisible(false);
    tbTaskList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbTaskList->setSelectionMode(QAbstractItemView::SingleSelection);
    tbTaskList->setSelectionBehavior(QTableView::SelectRows);
    this->tbTaskList->setColumnCount(2);
    this->tbTaskList->setColumnWidth(0,50);
    this->tbTaskList->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    this->tbTaskList->setHorizontalHeaderItem(0,getTableItem(tr("序号"),true));
    this->tbTaskList->setHorizontalHeaderItem(1,getTableItem(tr("简述"),true));

}

/*!
 * \brief clsSetTestStep::getTableItem 获取单元格
 * \param content 要显示的内容
 * \param isTitle 是否是标题，是标题有加粗效果
 * \return 返回一个item 指针
 */
QTableWidgetItem* clsSetTestStep::getTableItem(const QString &content,bool isTitle)
{
    QTableWidgetItem * item= new QTableWidgetItem();

    item->setText(content);
    if(isTitle)
        item->setFont(QFont("楷体", 12, QFont::Bold));
    else
        item->setFont(QFont("楷体", 11));
    item->setBackgroundColor(QColor(Qt::gray));
    //item->setTextAlignment(Qt::AlignHCenter/*|Qt::AlignVCenter*/);
    return item;
}



void clsSetTestStep::showRes(QString value)
{

    sngTestDisplay->setMessage(value,meter->getCountTestItems());
}

void clsSetTestStep::biasSlot(bool value)
{
    if(!value)
        btnTurnOffBias->setIcon(QIcon(":/Icons/BiasOff.png"));
    else
        btnTurnOffBias->setIcon(QIcon(":/Icons/BiasOn.png"));
}

void clsSetTestStep::setTestSteps(const QStringList &value)
{

    this->steps.clear();

    for(int i=0; i< value.length();i++)
    {
        WKEMeterMode *tmpMeter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
        tmpMeter->setCondition(value.at(i));
        steps.append(tmpMeter);
    }

    showTaskList();

    if(tbTaskList->rowCount()>=1)
    {
        tbTaskList->setCurrentCell(0,0);
        meter->setCondition(steps.at(0)->getConditon());
    }
}

const QStringList clsSetTestStep::getTestSteps()
{
    QStringList tmp;
    for(int i =0; i< steps.length(); i++)
    {
        tmp.append(steps.at(i)->getConditon());
    }
    return tmp;
}


/*!
 * \brief clsMeterMode::showTaskList
 * 显示任务
 */
void clsSetTestStep::showTaskList()
{

    tbTaskList->clear();
    this->tbTaskList->setRowCount(this->steps.length());

    initSheetTitle();
    for(int i=0; i< steps.length();i++)
    {
        tbTaskList->setItem(i,0,getTableItem(QString::number(i+1),false));
        tbTaskList->setItem(i,1,getTableItem(steps.at(i)->getBrief(),false));
    }
}



void clsSetTestStep::on_btnOk_clicked()
{
    this->accept();
}

void clsSetTestStep::on_btnCancel_clicked()
{
    this->reject();
}



void clsSetTestStep::on_btnTurnOffBias_clicked()
{
    meter->turnOffBias();
    biasSlot(false);
}

void clsSetTestStep::on_btnTest_clicked()
{
    sngTestDisplay->setMessage(tr("更新测试条件"),0);
    meter->updateGPIB();
    sngTestDisplay->setMessage(tr("正在测试"),0);
    meter->singleTrig();
}


void clsSetTestStep::on_btnNewStep_clicked()
{
    WKEMeterMode * tmpMeter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);

    tmpMeter->setCondition(meter->getConditon());
    steps.append(tmpMeter);

    showTaskList();
    tbTaskList->setCurrentCell(tbTaskList->rowCount()-1,0);

}

void clsSetTestStep::on_btnSave_clicked()
{
    if(!steps.isEmpty())
    {
        int row = tbTaskList->currentRow();

        if(row<0)
            return;

        steps[row]->setCondition(meter->getConditon());
        showTaskList();
        tbTaskList->setCurrentCell(row,0);
    }
    else
    {
        WKEMeterMode *tmpMeter = clsMeterModeFactory::getFunction(clsRS::getInst().meterSeries);
        tmpMeter->setCondition(meter->getConditon());
        steps.append(tmpMeter);
        showTaskList();
        tbTaskList->setCurrentCell(0,0);
    }

}



void clsSetTestStep::on_tbTaskList_clicked(const QModelIndex &index)
{
    int intSelectRow = index.row();

    meter->setCondition(steps[intSelectRow]->getConditon());
}

void clsSetTestStep::on_btnUp_clicked()
{
    //没有选择 返回
      if(tbTaskList->selectedItems().length()<=0)
          return;
      //判断选择行数
      int intSelectedRow = tbTaskList->selectedItems().at(0)->row();

      if(intSelectedRow<1)
          return;

      steps.swap(intSelectedRow,intSelectedRow-1);
      showTaskList();
      tbTaskList->setCurrentCell(intSelectedRow-1,0);
}

void clsSetTestStep::on_btnDown_clicked()
{
    //没有选择 返回
        if(tbTaskList->selectedItems().length()<=0)
            return;
        //判断选择行数
        int intSelectedRow = tbTaskList->selectedItems().at(0)->row();

        if((intSelectedRow+1)>=steps.length())
            return;

        steps.swap(intSelectedRow,intSelectedRow+1);
        showTaskList();
        tbTaskList->setCurrentCell(intSelectedRow+1,0);
}

void clsSetTestStep::on_btnDelete_clicked()
{

    if(tbTaskList->selectedItems().length()<=0)
        return;

    int intSelectedRow = tbTaskList->selectedItems().at(0)->row();


    steps.removeAt(intSelectedRow);

    showTaskList();

    if(intSelectedRow >0)
        tbTaskList->setCurrentCell(intSelectedRow-1,0);
}

void clsSetTestStep::on_btnClear_clicked()
{
    steps.clear();
    showTaskList();
}
