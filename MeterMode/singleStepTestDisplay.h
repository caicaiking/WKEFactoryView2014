#ifndef SINGLESTEPTESTDISPLAY_H
#define SINGLESTEPTESTDISPLAY_H

#include "ui_singleStepTestDisplay.h"

class singleStepTestDisplay : public QWidget, private Ui::singleStepTestDisplay
{
    Q_OBJECT

public:
    explicit singleStepTestDisplay(QWidget *parent = 0);
    void setMessage(const QString &msg, int value=0);
};

#endif // SINGLESTEPTESTDISPLAY_H
