#ifndef HORIZONTALLEVELCONTROLWIDGETWRAPPER_H
#define HORIZONTALLEVELCONTROLWIDGETWRAPPER_H

#include "controlWidgets/horizontallevelcontrolwidget.h"
#include "controlwrappers/controlwidgetwrapper.h"

class HorizontalLevelControlWidgetWrapper : public ControlWidgetWrapper
{
public:

    explicit HorizontalLevelControlWidgetWrapper(QWidget *parent = 0);


    void setHorizontalLevel(HorizontalLevel* horizontalLevel);


private:
    HorizontalLevelControlWidget *mControlWidgetCopy;
};

#endif // HORIZONTALLEVELCONTROLWIDGETWRAPPER_H



