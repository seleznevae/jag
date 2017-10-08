#ifndef PLOTCONTROLWIDGETWRAPPER_H
#define PLOTCONTROLWIDGETWRAPPER_H


#include "controlWidgets/plotcontrolwidget.h"
#include "controlwrappers/controlwidgetwrapper.h"

class PlotControlWidgetWrapper: public ControlWidgetWrapper
{
    Q_OBJECT
public:
    PlotControlWidgetWrapper(QWidget *parent=0);
    ~PlotControlWidgetWrapper() { }

    void setLayoutModel(PlotLayoutModel*);

public slots:
    void initializeState();
    void treatXLowLimitRequest();
    void treatXHighLimitRequest();
    void treatXLabelChanging();
    void treatTitleChanging();
    void treatScaleTypeChanging();
    void treatTimeAxisTypeChanging();
    void setMinWidth(int);
    void treatXTickRotationChanging();
    void treatCommandLabelRotationChanging();

signals:
//    void settingXRangeRequest(double, double);
//    void legendChangingVisibilityRequest(bool);
//    void dimensionChangingVisibilityRequest(bool);



private:
    PlotControlWidget *mControlWidgetCopy;
};

#endif // PLOTCONTROLWIDGETWRAPPER_H



