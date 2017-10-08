#ifndef HORIZONTALLEVELCONTROLWIDGET_H
#define HORIZONTALLEVELCONTROLWIDGET_H

#include <QWidget>
#include "controlWidgets/controlwidget.h"
class HorizontalLevel;

namespace Ui {
class HorizontalLevelControlWidget;
}

class HorizontalLevelControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit HorizontalLevelControlWidget(QWidget *parent = 0);
    ~HorizontalLevelControlWidget();

    void setHorizontalLevel(HorizontalLevel* level);

    void setExtendedMode(bool);
public slots:
    void treatArrowedTextDeleting(QObject*);

    void treatLevelChanging();
    void treatLabelChanging();

signals:
    void widgetIsHiding();

private:
    Ui::HorizontalLevelControlWidget *ui;

    HorizontalLevel *mHorizontalLevel;

    void initializeState();
};

#endif // HORIZONTALLEVELCONTROLWIDGET_H
