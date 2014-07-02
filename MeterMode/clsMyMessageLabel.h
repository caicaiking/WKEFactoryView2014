#ifndef CLSMYMESSAGELABEL_H
#define CLSMYMESSAGELABEL_H

#include <QLabel>
#include <QTimer>
#include <QWidget>

class clsMyMessageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit clsMyMessageLabel(QWidget *parent = 0);

    void showMessage(QString value, int time=0);
    void showTime();
signals:

public slots:

private slots:
    void clearMessage();
    void showTimeLabel();
private:
    QTimer * timer;

};

#endif // CLSMYMESSAGELABEL_H
