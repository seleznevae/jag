#ifndef MOUSEPRESSNOTIFIER_H
#define MOUSEPRESSNOTIFIER_H

#include <QObject>

class QWidget;

class MousePressNotifier : public QObject
{
    Q_OBJECT
public:
    explicit MousePressNotifier(QObject *parent = 0);


    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void clickOnWidget(QWidget*);


public slots:
};

#endif // MOUSEPRESSNOTIFIER_H
