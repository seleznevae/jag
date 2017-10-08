#include "plotcontrolwidgetwrapper.h"
#include <assert.h>



PlotControlWidgetWrapper::PlotControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new PlotControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<PlotControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Plot properties"));

//    connect(mControlWidgetCopy, SIGNAL(settingXRangeRequest(double, double)),
//            this, SIGNAL(settingXRangeRequest(double, double)));
//    connect(mControlWidgetCopy, SIGNAL(legendChangingVisibilityRequest(bool)),
//            this, SIGNAL(legendChangingVisibilityRequest(bool)));
//    connect(mControlWidgetCopy, SIGNAL(dimensionChangingVisibilityRequest(bool)),
//            this, SIGNAL(dimensionChangingVisibilityRequest(bool)));

//    auto lay = groupLayout();

}

void PlotControlWidgetWrapper::setLayoutModel(PlotLayoutModel *model)
{
    mControlWidgetCopy->setLayoutModel(model);
}

void PlotControlWidgetWrapper::initializeState()
{
    mControlWidgetCopy->initializeState();
}

void PlotControlWidgetWrapper::treatXLowLimitRequest()
{
    mControlWidgetCopy->treatXLowLimitRequest();
}

void PlotControlWidgetWrapper::treatXHighLimitRequest()
{
    mControlWidgetCopy->treatXHighLimitRequest();
}

void PlotControlWidgetWrapper::treatXLabelChanging()
{
    mControlWidgetCopy->treatXLabelChanging();
}

void PlotControlWidgetWrapper::treatTitleChanging()
{
    mControlWidgetCopy->treatTitleChanging();
}

void PlotControlWidgetWrapper::treatScaleTypeChanging()
{
    mControlWidgetCopy->treatScaleTypeChanging();
}

void PlotControlWidgetWrapper::treatTimeAxisTypeChanging()
{
    mControlWidgetCopy->treatTimeAxisTypeChanging();
}

void PlotControlWidgetWrapper::setMinWidth(int width)
{
    setMinimumWidth(width);
//    controlWidget->setMinWidth(width);
}

void PlotControlWidgetWrapper::treatXTickRotationChanging()
{
    mControlWidgetCopy->treatXTickRotationChanging();
}

void PlotControlWidgetWrapper::treatCommandLabelRotationChanging()
{
    mControlWidgetCopy->treatCommandLabelRotationChanging();
}

