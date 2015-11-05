#ifndef CLSAPPMODE_H
#define CLSAPPMODE_H

#include "ui_clsAppMode.h"

class clsAppMode : public QMainWindow, private Ui::clsAppMode
{
    Q_OBJECT

public:
    explicit clsAppMode(QWidget *parent = 0);
};

#endif // CLSAPPMODE_H
