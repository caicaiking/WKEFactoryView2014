#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QWidget>
class ClickedLabel : public QLabel
{
    Q_OBJECT
signals:

    void Clicked();
public:
    ClickedLabel(QWidget *parent=0): QLabel(parent)
    {
        setText(m_str);
    }

    ~ClickedLabel() {}
protected:
    void mouseReleaseEvent( QMouseEvent* );
private:
    QString m_str;
};


#endif // CLICKEDLABEL_H
