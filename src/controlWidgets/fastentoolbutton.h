#ifndef FASTENTOOLBUTTON_H
#define FASTENTOOLBUTTON_H

#include <QToolButton>

class FastenToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit FastenToolButton(QWidget* parent = 0);
    ~FastenToolButton();

private slots:
    void treatToggling(bool);
};

#endif // FASTENTOOLBUTTON_H
