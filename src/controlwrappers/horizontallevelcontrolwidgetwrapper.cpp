#include "horizontallevelcontrolwidgetwrapper.h"
#include "core/horizontallevel.h"
#include "assert.h"

HorizontalLevelControlWidgetWrapper::HorizontalLevelControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new HorizontalLevelControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<HorizontalLevelControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Horiz. level properties"));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));
}


void HorizontalLevelControlWidgetWrapper::setHorizontalLevel(HorizontalLevel *horizontalLevel)
{
    mControlWidgetCopy->setHorizontalLevel(horizontalLevel);
}
