#include "axiscontrolwidgetwrapper.h"
#include <assert.h>


AxisControlWidgetWrapper::AxisControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new AxisControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<AxisControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Axis properties"));

    connect(mControlWidgetCopy, SIGNAL(movementRequest(AxisModel*,AxisModel::AxisMovement)),
            this, SIGNAL(movementRequest(AxisModel*,AxisModel::AxisMovement)));
    connect(mControlWidgetCopy, SIGNAL(settingYRangeRequest(double, double)),
            this, SIGNAL(settingYRangeRequest(double, double)));
    connect(mControlWidgetCopy, SIGNAL(deletingRequest()),
            this, SIGNAL(deletingRequest()));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));

}

void AxisControlWidgetWrapper::setAxisModels(QVector<AxisModel*> models)
{
    mControlWidgetCopy->setAxisModels(models);
}

void AxisControlWidgetWrapper::treatMoveUpRequest()
{
    mControlWidgetCopy->treatMoveUpRequest();
}

void AxisControlWidgetWrapper::treatMoveDownRequest()
{
    mControlWidgetCopy->treatMoveDownRequest();
}

void AxisControlWidgetWrapper::treatSetYMarginRequest()
{
//    mControlWidgetCopy->treatSetYMarginRequest();
}

void AxisControlWidgetWrapper::treatScalingTypeChanging()
{
    mControlWidgetCopy->treatScalingTypeChanging();
}

void AxisControlWidgetWrapper::initializeState()
{
    mControlWidgetCopy->initializeState();
}

void AxisControlWidgetWrapper::treatDeletingRequest()
{
    mControlWidgetCopy->treatDeletingRequest();
}

void AxisControlWidgetWrapper::treatStretchFactorChanging()
{
    mControlWidgetCopy->treatStretchFactorChanging();
}

void AxisControlWidgetWrapper::treatAxisModelRemoval()
{
    mControlWidgetCopy->treatAxisModelRemoval();
}


