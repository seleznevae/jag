#ifndef AXISCONTROLWIDGETWRAPPER_H
#define AXISCONTROLWIDGETWRAPPER_H

#include "controlwrappers/controlwidgetwrapper.h"
#include "controlWidgets/axiscontrolwidget.h"

class AxisControlWidgetWrapper: public ControlWidgetWrapper
{
    Q_OBJECT
public:
    explicit AxisControlWidgetWrapper(QWidget *parent=0);
    ~AxisControlWidgetWrapper() { }

    void setAxisModels(QVector<AxisModel*>);

signals:
    void movementRequest(AxisModel*, AxisModel::AxisMovement);
    void settingYRangeRequest(double, double);
    void deletingRequest();

public slots:
    void treatMoveUpRequest();
    void treatMoveDownRequest();
    void treatSetYMarginRequest();
    void treatScalingTypeChanging();
    void initializeState();
    void treatDeletingRequest();
    void treatStretchFactorChanging();
    void treatAxisModelRemoval();


private:
    AxisControlWidget *mControlWidgetCopy;
};

#endif // AXISCONTROLWIDGETWRAPPER_H




