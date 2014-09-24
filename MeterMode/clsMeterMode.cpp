#include "clsMeterMode.h"
#include "clsRuningSettings.h"
#include "UserfulFunctions.h"
#include "cls6440MeterMode.h"
#include "clsUserFunctionMeterMode.h"
#include <QLayout>
#include "clsMeterModeFactory.h"
#include "clsRuningSettings.h"
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

    //连接tbTaskList的选择改变信号，将saveStepButton的动作开关
    connect(tbTaskList,SIGNAL(itemSelectionChanged()),this,SLOT(enableBtnSaveStep()));
    btnSaveStep->setEnabled(false);

    //设置显示表的表头
    setTableTitle();
    meter=0;
}

/*!
 * \brief clsMeterMode::on_skWidget_currentChanged 当skWidget 改变
 * \param arg1 选择的page
 */
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

/*!
 * \brief clsMeterMode::on_btnRep_clicked
 * 连续测试按钮click 事件响应
 */
void clsMeterMode::on_btnRep_clicked()
{
    skWidget->setCurrentIndex(1);

    btnRep->setIcon(QIcon(":/Icons/stop.png"));
    qApp->processEvents();
    while(btnRep->isChecked())
    {
        for(int i=0; i<list.length();i++)
        {
            list.at(i).data()->start();
           // UserfulFunctions::sleepMs(150);
             qApp->processEvents();
        }
        UserfulFunctions::sleepMs(150);
        qApp->processEvents();
    }
    btnRep->setIcon(QIcon(":/Icons/repeat.png"));
}

/*!
 * \brief clsMeterMode::on_btnSetup_clicked
 * 当隐藏设定和打开设定时候要做的事情
 */
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

/*!
 * \brief clsMeterMode::closeEvent
 * \param event
 * 关闭事件的相应处理
 */
void clsMeterMode::closeEvent(QCloseEvent *event)
{
    btnSetup->setChecked(false);
    qApp->processEvents();
    event->accept();
}

/*!
 * \brief clsMeterMode::on_btnAdd_clicked
 * 单击添加按钮，将显示的内容添加到任务
 */
void clsMeterMode::on_btnAdd_clicked()
{
    if(!meter)
    {
        lblInfo->showMessage(tr("<font color=red> 请选择测试步骤类型，然后在点击添加！</font>"),5);
        return;
    }

    QPointer<WKEMeterMode>  tmpMeter =clsMeterModeFactory::getFunction(meter->getType(),clsRS::getInst().meterSeries);
    tmpMeter.data()->setParent(this);
    if(tmpMeter)
    {
        tmpMeter->setCondition(meter->getTestCondition());
        list.append(tmpMeter);
        showTaskList();
    }

    showTaskList();
}

/*!
 * \brief clsMeterMode::on_btnMeter_clicked
 * Meter 按钮添加事件
 */
void clsMeterMode::on_btnMeter_clicked()
{
    skWidget->setCurrentIndex(0);

    if(meter)
    {
        delete meter;
        //meter->setVisible(false);
    }
    QLayout *layout = wdgStep->layout();
    if(layout)
        delete layout;

    meter = clsMeterModeFactory::getFunction(MeterFunction,clsRS::getInst().meterSeries);
    meter.data()->setParent(this);
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(meter);

    wdgStep->setLayout(hboxLayout);

    meter->setVisible(true);
}

/*!
 * \brief clsMeterMode::on_btnAddtionFunction_clicked
 * 添加有用功能按钮
 */
void clsMeterMode::on_btnAddtionFunction_clicked()
{
    skWidget->setCurrentIndex(0);
    if(meter)
    {
        delete meter;
        //meter->setVisible(false);
    }
    QLayout *layout = wdgStep->layout();
    if(layout)
        delete layout;

    meter = clsMeterModeFactory::getFunction(UF,clsRS::getInst().meterSeries);
    meter.data()->setParent(this);
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(meter);
    wdgStep->setLayout(hboxLayout);
    meter->setVisible(true);
}

/*!
 * \brief clsMeterMode::on_btnTrig_clicked
 * 触发按钮
 */
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

/*!
 * \brief clsMeterMode::showTaskList
 * 显示任务
 */
void clsMeterMode::showTaskList()
{

    tbTaskList->clear();
    this->tbTaskList->setRowCount(this->list.length());

    setTableTitle();
    for(int i=0; i< list.length();i++)
    {
        tbTaskList->setItem(i,0,getTableItem(QString::number(i+1)));
        tbTaskList->setItem(i,1,getTableItem(list.at(i).data()->getBrief()));
    }
}

/*!
 * \brief clsMeterMode::getTableItem 获取单元格
 * \param content 要显示的内容
 * \param isTitle 是否是标题，是标题有加粗效果
 * \return 返回一个item 指针
 */
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

/*!
 * \brief clsMeterMode::setTableTitle 设置表头
 */
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


/*!
 * \brief clsMeterMode::on_btnDelete_clicked
 * 删除按钮响应
 */
void clsMeterMode::on_btnDelete_clicked()
{
    if(tbTaskList->selectedItems().length()<=0)
        return;

    int intSelectedRow = tbTaskList->selectedItems().at(0)->row();

    QPointer<WKEMeterMode> tmpMeter = list.at(intSelectedRow);
    delete tmpMeter;

    list.removeAt(intSelectedRow);

    showTaskList();

    if(intSelectedRow >0)
        tbTaskList->setCurrentCell(intSelectedRow-1,0);
}

/*!
 * \brief clsMeterMode::on_btnUp_clicked
 * 向上按钮响应
 */
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

/*!
 * \brief clsMeterMode::on_btnDown_clicked
 * 向下按钮响应
 */
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


/*!
 * \brief clsMeterMode::on_tbTaskList_clicked
 * \param index
 * 单击，显示自定义步骤
 */
void clsMeterMode::on_tbTaskList_clicked(const QModelIndex &index)
{
    int intSelectRow = index.row();
    skWidget->setCurrentIndex(0);
    if(meter)
    {
        delete meter;
    }

    QString strCondition = list.at(intSelectRow)->getTestCondition();
    FunctionType t = list.at(intSelectRow)->getType();
    QLayout *layout = wdgStep->layout();
    if(layout)
        delete layout;

    meter = clsMeterModeFactory::getFunction(t,clsRS::getInst().meterSeries);
    meter->setCondition(strCondition);

    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(meter);
    wdgStep->setLayout(hboxLayout);
    meter->setVisible(true);

}

/*!
 * \brief clsMeterMode::on_btnSaveStep_clicked
 * 保存测试步骤
 */
void clsMeterMode::on_btnSaveStep_clicked()
{
    //判断选择行数
    QList<QTableWidgetItem *> itemList = tbTaskList->selectedItems();

    if(itemList.length()<=0)
    {
        lblInfo->showMessage(tr("<font color=red>请选择要保存的步骤！</font>"),5);
        return;
    }

    int intSelectedRow = itemList.at(0)->row();

    if(!meter)
    {
        lblInfo->showMessage(tr("<font color=red> 请选择测试步骤类型，然后在点击添加！</font>"),5);
        return;
    }

    QPointer<WKEMeterMode>  tmpMeter =clsMeterModeFactory::getFunction(meter->getType(),clsRS::getInst().meterSeries);

    if(tmpMeter)
    {
        tmpMeter->setCondition(meter->getTestCondition());
        list.append(tmpMeter);
        list.swap(intSelectedRow,list.length()-1);
        list.removeLast();
        showTaskList();
    }
}

/*!
 * \brief clsMeterMode::enableSaveButton
 * 有选择的时候，将此按钮变成可以使用
 */
void clsMeterMode::enableBtnSaveStep()
{
    QList<QTableWidgetItem *> itemList = tbTaskList->selectedItems();

    if(itemList.length()<=0)
        btnSaveStep->setEnabled(false);
    else
        btnSaveStep->setEnabled(true);

}
