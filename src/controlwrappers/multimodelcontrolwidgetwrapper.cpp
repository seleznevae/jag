#include "multimodelcontrolwidgetwrapper.h"
#include <assert.h>



MultiModelControlWidgetWrapper::MultiModelControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new MultiModelControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<MultiModelControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Graphs properties"));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));
}

void MultiModelControlWidgetWrapper::setModelVector(QVector<GraphModel *> newModelVector)
{
    if (newModelVector.size() == 1) {
        if (newModelVector[0]->graphType() == GraphModel::GraphType::Function)
            setHeaderText(tr("Graph properties"));
        else
            setHeaderText(tr("Curve properties"));
    } else {
        bool areAllModelsCurves = true;
        for (auto model : newModelVector) {
            if (model->graphType() == GraphModel::GraphType::Function) {
                areAllModelsCurves = false;
                break;
            }
        }
        if (areAllModelsCurves)
            setHeaderText(tr("Curves properties"));
        else
            setHeaderText(tr("Graphs properties"));
    }
    mControlWidgetCopy->setModelVector(newModelVector);
}

void MultiModelControlWidgetWrapper::setLayoutModel(PlotLayoutModel *model)
{
    mControlWidgetCopy->setLayoutModel(model);
}

void MultiModelControlWidgetWrapper::initializeStateFromModels()
{
    mControlWidgetCopy->initializeStateFromModels();
}

void MultiModelControlWidgetWrapper::treatModelDeleting()
{
    mControlWidgetCopy->treatModelDeleting();
}


