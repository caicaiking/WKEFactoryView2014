#include "frmPointEditor.h"
#include "UserfulFunctions.h"
#include "doubleType.h"
#include "NumberInput.h"
frmPointEditor::frmPointEditor(const double min, const double max,
                               const bool blLog,const SweepType t, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    this->min=min;
    this->max=max;
    this->blLog = blLog;
    this->t=t;

    points.clear();
    setTable();
}

void frmPointEditor::setPoints(QList<double> points)
{
    this->points = points;
    showTestData();
}

void frmPointEditor::setSweepType(SweepType t)
{
    this->t=t;
}

void frmPointEditor::setMaxAndMinValue(double max, double min)
{
    this->max=max;
    this->min=min;
}

QList<double> frmPointEditor::getStaticPoints(const double min, const double max,
                                              const bool blLog, /*const SweepType t, */
                                              int numPoints)
{
    QList<double> points;
    points.clear();
    if(!blLog)
    {
        double space = max-min;
        double av= space/(numPoints-1);

        for(int i=0; i< numPoints-1; i++)
        {
            points.append(min+av*i);
        }
        points.append(max);
    }
    else
    {
        double logMin=log10(min);
        double logMax=log10(max);
        double dis = (logMax-logMin)/(numPoints-1);

        for(int i=0;i<numPoints-1;i++)
        {
            points << pow(10.0,logMin+i*dis);

        }
        points << max;
    }
    if(points.at(points.length()-1)==points.at(points.length()-2))
        points.removeLast();


    points = rmvPP(points);

    return points;
}

void frmPointEditor::setTable()
{
    this->tbvDataShow->verticalHeader()->setVisible(false);
    tbvDataShow->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbvDataShow->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbvDataShow->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbvDataShow->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbvDataShow->setColumnCount(2);
    tbvDataShow->setRowCount(0);

    tbvDataShow->setHorizontalHeaderItem(0,getItem(tr("序号")));
    tbvDataShow->setHorizontalHeaderItem(1,getItem(tr("测试点")));
}

QTableWidgetItem * frmPointEditor::getItem(QString value)
{
    QTableWidgetItem * item= new QTableWidgetItem();
    item->setFont(QFont("楷体", 12, QFont::Bold));
    item->setText(value);
    item->setBackgroundColor(QColor(Qt::gray));
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    return item;
}

QTableWidgetItem * frmPointEditor::getContentItem(QString value)
{
    QTableWidgetItem * item= new QTableWidgetItem();
    item->setFont(QFont("楷体", 12));
    item->setText(value);
    item->setBackgroundColor(QColor(Qt::white));
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    return item;
}

void frmPointEditor::on_btnCancel_clicked()
{
    this->reject();
}

void frmPointEditor::on_btnOk_clicked()
{
    this->accept();
}

void frmPointEditor::on_btn10_clicked()
{
    setTestPoint(10);
}

void frmPointEditor::setTestPoint(int value)
{
    points.clear();
    if(!blLog)
    {
        double space = max-min;
        double av= space/(value-1);

        for(int i=0; i<value-1; i++)
        {
            points.append(min+av*i);
        }
        points.append(max);
    }
    else
    {
        double logMin=log10(min);
        double logMax=log10(max);
        double dis = (logMax-logMin)/(value-1);

        for(int i=0;i<value-1;i++)
        {
            points << pow(10.0,logMin+i*dis);

        }
        points << max;
    }

    if(points.at(points.length()-1)==points.at(points.length()-2))
        points.removeLast();
    showTestData();
}

//显示Points 里面的数据
void frmPointEditor::showTestData()
{
    points = rmvPP(points);

    tbvDataShow->clear();
    setTable();

    tbvDataShow->setRowCount(points.length());
    doubleType dt;
    for(int i=0; i<points.length();i++)
    {
        dt.setData(points.at(i),"");
        tbvDataShow->setItem(i,0,getContentItem(QString::number(i+1)));
        tbvDataShow->setItem(i,1,getContentItem(
                                 dt.formateToString(6)+UserfulFunctions::getSweepTypeUnit(t)));
    }
}

void frmPointEditor::on_btn20_clicked()
{
    setTestPoint(20);
}

void frmPointEditor::on_btn50_clicked()
{
    setTestPoint(50);
}

void frmPointEditor::on_btn100_clicked()
{
    setTestPoint(100);
}

void frmPointEditor::on_btn200_clicked()
{
    setTestPoint(200);
}

void frmPointEditor::on_btn400_clicked()
{
    setTestPoint(400);
}

void frmPointEditor::on_btn800_clicked()
{
    setTestPoint(800);
}

void frmPointEditor::on_btn1200_clicked()
{
    setTestPoint(1200);
}

void frmPointEditor::on_btn1600_clicked()
{
    setTestPoint(1600);
}

void frmPointEditor::on_btnCustom_clicked()
{
    NumberInput dlg;
    dlg.setWindowTitle(tr("输入自定义点数"));

    if(dlg.exec()==QDialog::Accepted)
    {
        double input = dlg.getNumber();
        int value = (int)input;
        setTestPoint(value);
    }
}

void frmPointEditor::on_btnNewPoint_clicked()
{
    NumberInput dlg;
    dlg.setWindowTitle(tr("输入自定义测试点"));

    if(dlg.exec()==QDialog::Accepted)
    {
        double input = dlg.getNumber();

        if(!points.contains(input))
        {
            points.append(input);
            showTestData();
        }
    }
}

void frmPointEditor::on_btnSort_clicked()
{
    //qDebug()<<"Points "<< points;
    for(int i=0; i< points.length(); i++)
    {
        for(int j=i+1;j<points.length();j++)
        {
            //qDebug()<<points.at(i)<< " -- " <<points.at(j);
            if(points.at(i)>points.at(j))
            {
                points.swap(i,j);
            }
        }
    }

    showTestData();
}

void frmPointEditor::on_btnDel_clicked()
{
    int selectRow= this->tbvDataShow->currentRow();

    if(points.length()<selectRow)
        return ;
    else
        points.removeAt(selectRow);

    showTestData();
}

QList<double> frmPointEditor::rmvPP(QList<double> points)
{
    QList<double> tmpPoint;

    for(int i=0;i<points.length();i++)
    {
        if(!tmpPoint.contains(points.at(i)))
            tmpPoint.append(points.at(i));
    }

    for(int i=0; i< tmpPoint.length(); i++)
    {
        for(int j=i+1;j<tmpPoint.length()-1;j++)
        {
            if(tmpPoint.at(i)>tmpPoint.at(j))
            {
                tmpPoint.swap(i,j);
            }
        }
    }

    return tmpPoint;
}
