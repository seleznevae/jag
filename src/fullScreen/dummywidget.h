#ifndef DUMMYWIDGET_H
#define DUMMYWIDGET_H

#include <QWidget>

class DummyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DummyWidget(QWidget *parent = 0);

signals:

public slots:

protected:

    virtual void keyPressEvent(QKeyEvent *event);
//    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // DUMMYWIDGET_H
