#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "cls6440MeterMode.h"
#include "clsUserFunctionMeterMode.h"
#include <QLayout>
clsMeterMode::clsMeterMode(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    lblConnectionType->setText(clsRS::getInst().getConnectionType() + QObject::tr("连接"));
    lblTime->showTime();
    lblStatus->setStatus(IDEL);
    skWidget->setCurrentIndex(0);

    //sngTestDisplay->setMessage(tr("没有测试数据可以显示！"),0);
    sngTestDisplay->setMessage("L,512.03nH,PASS,C,-2.003nF,FAIL,R,1.2354mΩ,PASS,C,-2.003nF,FAIL",4);
    //this->showMaximized();

    //设置显示表的表头
    setTableTitle();

    meter=0;

}

void clsMeterMode::on_skWidget_currentChanged(int arg1)
{
    if(arg1==0)
    {
        grpSettings->setTitle(QObject::tr("测试步骤设定"));
    }
    else
    {
        grpSettings->setTitle(QObject::tr("测试结果显示"));
    }
}

void clsMeterMode::on_btnRep_clicked()
{
    skWidget->setCurrentIndex(1);

    btnRep->setIcon(QIcon(":/Icons/stop.png"));
    qApp->processEvents();
    while(btnRep->isChecked())
    {
        qDebug()<< "rep test";
        UserfulFunctions::sleepMs(150);
        qApp->processEvents();
    }
    btnRep->setIcon(QIcon(":/Icons/repeat.png"));
}

void clsMeterMode::on_btnSetup_clicked()
{
    if(!btnSetup->isChecked())
    {
        grpSetup->setVisible(true);
        btnTrig->setVisible(true);
        btnNewTask->setVisible(true);
        btnSaveTask->setVisible(true);
    }
    else
    {
        grpSetup->setVisible(false);
        btnTrig->setVisible(false);
        btnNewTask->setVisible(false);
        btnSaveTask->setVisible(false);
    }
}

void clsMeterMode::closeEvent(QCloseEvent *event)
{
    btnSetup->setChecked(false);
    qApp->processEvents();
    event->accept();
}

void clsMeterMode::on_btnAdd_clicked()
{
    if(!meter)
    {
        lblInfo->showMessage(tr("<font color=red> 请选择测试步骤类型，然后在点击添加！</font>"),5);
        return;
    }
    if(meter->getType()==MeterFunction)
    {
        cls6440MeterMode *tmpMeter = new cls6440MeterMode();
        tmpMeter->setCondition(meter->getTestCondition());
        stpInfo.meterFunction = meter->getTestCondition();
        list.append(tmpMeter);
    }
    else if(meter->getType()==UF)
    {
        clsUserFunctionMeterMode *tmpUF = new clsUserFunctionMeterMode();
        tmpUF->setCondition(meter->getTestCondition());
        stpInfo.userFunction= meter->getTestCondition();
        list.append(tmpUF);
    }
    else
    {
        //在这里什么都不做。
    }


    showTaskList();
}

void clsMeterMode::on_btnMeter_clicked()
{
    skWidget->setCurrentIndex(0);

    if(meter)
    {
        delete meter;
    }
    QLayout *layout = wdgStep->layout();
    if(layout)
        delete layout;

    meter = new cls6440MeterMode();
    meter->setCondition(stpInfo.meterFunction);
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(meter);

    wdgStep->setLayout(hboxLayout);

    meter->setVisible(true);
}

void clsMeterMode::on_btnAddtionFunction_clicked()
{
    skWidget->setCurrentIndex(0);
    if(meter)
    {
        delete meter;
    }
    QLayout *layout = wdgStep->layout();
    if(layout)
        delete layout;

    meter = new clsUserFunctionMeterMode();
    meter->setCondition(stpInfo.userFunction);
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(meter);
    wdgStep->setLayout(hboxLayout);
    meter->setVisible(true);
}

void clsMeterMode::on_btnTrig_clicked()
{

    if(meter)
    {
        connect(meter,SIGNAL(testResult(QString,int)),sngTestDisplay,SLOT(setMessage(QString,int)));
        lblStatus->setStatus(BUSY);
        meter->trig();
        UserfulFunctions::sleepMs(2000);
        lblStatus->setStatus(IDEL);
    }
}


void clsMeterMode::showTaskList()
{

    tbTaskList->clear();
    this->tbTaskList->setRowCount(this->list.length());

    setTableTitle();
    for(int i=0; i< list.length();i++)
    {
        tbTaskList->setItem(i,0,getTableItem(QString::number(i+1)));
        tbTaskList->setItem(i,1,getTableItem(list.at(i)->getBrief()));
    }
}

QTableWidgetItem* clsMeterMode::getTableItem(const QString &content,bool isTitle)
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

void clsMeterMode::setTableTitle()
{
    tbTaskList->verticalHeader()->setVisible(false);
    tbTaskList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbTaskList->setSelectionMode(QAbstractItemView::SingleSelection);
    tbTaskList->setSelectionBehavior(QTableView::SelectRows);
    this->tbTaskList->setColumnCount(2);
    this->tbTaskList->setColumnWidth(0,50);
    this->tbTaskList->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    this->tbTaskList->setHorizontalHeaderItem(0,getTableItem(tr("序号"),true));
    this->tbTaskList->setHorizontalHeaderItem(1,getTableItem(tr("内容"),true));
}



void clsMeterMode::on_btnDelete_clicked()
{
    if(tbTaskList->selectedItems().length()<=0)
        return;

    int intSelectedRow = tbTaskList->selectedItems().at(0)->row();
    list.removeAt(intSelectedRow);

    showTaskList();
}

void clsMeterMode::on_btnUp_clicked()
{
    //没有选择 返回
    if(tbTaskList->selectedItems().length()<=0)
        return;
    //判断选择行数
    int intSelectedRow = tbTaskList->selectedItems().at(0)->row();

    if(intSelectedRow<1)
        return;

    list.swap(intSelectedRow,intSelectedRow-1);
    showTaskList();
    tbTaskList->setCurrentCell(intSelectedRow-1,0);
}

void clsMeterMode::on_btnDown_clicked()
{
    //没有选择 返回
    if(tbTaskList->selectedItems().length()<=0)
        return;
    //判断选择行数
    int intSelectedRow = tbTaskList->selectedItems().at(0)->row();

    if((intSelectedRow+1)>=list.length())
        return;

    list.swap(intSelectedRow,intSelectedRow+1);
    showTaskList();
    tbTaskList->setCurrentCell(intSelectedRow+1,0);

}
