#ifndef CLSSELECTEDREFPROPERTY_H
#define CLSSELECTEDREFPROPERTY_H

#include "ui_clsSelectedRefProperty.h"
#include "clsRefTraceProperty.h"
class clsSelectedRefProperty : public QDialog, private Ui::clsSelectedRefProperty
{
    Q_OBJECT
    
public:
    explicit clsSelectedRefProperty(const curveProperty property,QWidget *parent = 0);
    void setTraceColors(QColor traceA, QColor traceB);
    curveProperty getCurveProperty();
    int getSelected(){return this->index;}
private slots:
    void on_btnOk_clicked();
    void setTraceAColor();
    void setTraceBColor();
    void on_btnStatus_clicked(bool checked);

    void on_txtTitle_lostFocus();

signals:
    void  refTraceChanged(curveProperty value,bool isSetCurrent);

private:

    QColor traceA,traceB;
    QString title;
    bool isOn;
    int index;
    curveProperty origin;
    void updateTexts();


};

#endif // CLSSELECTEDREFPROPERTY_H
