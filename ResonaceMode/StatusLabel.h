#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include <QLabel>
#include "Globle.h"
class StatusLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StatusLabel(QWidget *parent = 0);

    void setStatus(Status value);
    void setStatus(bool value);
signals:

public slots:

};

#endif // STATUSLABEL_H
