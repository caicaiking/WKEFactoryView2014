#ifndef DLGSORT_H
#define DLGSORT_H

#include "ui_dlgSort.h"

class dlgSort : public QDialog, private Ui::dlgSort
{
    Q_OBJECT
    
public:
    explicit dlgSort(QWidget *parent = 0);
};

#endif // DLGSORT_H
