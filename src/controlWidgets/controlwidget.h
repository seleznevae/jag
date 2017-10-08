#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

    virtual void setExtendedMode(bool) = 0;
signals:

public slots:
};

#endif // CONTROLWIDGET_H
