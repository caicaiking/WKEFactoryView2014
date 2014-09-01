#ifndef PASSFAILLABEL_H
#define PASSFAILLABEL_H

#include <QLabel>

class PassFailLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PassFailLabel(QWidget *parent = 0);

    void showText(const QString &value);
signals:

public slots:


};

#endif // PASSFAILLABEL_H
