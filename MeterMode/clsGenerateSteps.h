#ifndef CLSGENERATESTEPS_H
#define CLSGENERATESTEPS_H

#include "ui_clsGenerateSteps.h"

class clsGenerateSteps : public QDialog, private Ui::clsGenerateSteps
{
    Q_OBJECT

public:
    explicit clsGenerateSteps(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // CLSGENERATESTEPS_H
