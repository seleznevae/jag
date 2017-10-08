#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MyTabWidget(QWidget *parent = 0);

signals:
    void tabMoved(int, int);


protected:
    virtual void paintEvent(QPaintEvent *event);


public slots:

};

#endif // MYTABWIDGET_H
