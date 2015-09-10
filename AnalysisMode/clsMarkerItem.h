#ifndef CLSMARKERITEM_H
#define CLSMARKERITEM_H

#include "ui_clsMarkerItem.h"

class clsMarkerItem : public QWidget, private Ui::clsMarkerItem
{
    Q_OBJECT

public:
    explicit clsMarkerItem(QWidget *parent = 0);
    void setNumber(int number);
    void setText(QString text);
    void setActive(int value);
    bool getIsActive()
    {
        return this->isActive;
    }

signals:
    void clicked(int number);

private slots:
    void on_btnNumber_clicked();
private:
    int number;
    bool isActive;
};

#endif // CLSMARKERITEM_H
