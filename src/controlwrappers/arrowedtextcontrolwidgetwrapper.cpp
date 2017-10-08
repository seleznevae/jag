#include "arrowedtextcontrolwidgetwrapper.h"
#include "assert.h"


ArrowedTextControlWidgetWrapper::ArrowedTextControlWidgetWrapper(QWidget *parent)
    :ControlWidgetWrapper(new ArrowedTextControlWidget(), parent), mControlWidgetCopy(NULL)
{
    mControlWidgetCopy = dynamic_cast<ArrowedTextControlWidget*>(mControlWidget);
    assert(mControlWidgetCopy != NULL);

    setHeaderText(tr("Arrowed text properties"));

    connect(mControlWidgetCopy, SIGNAL(widgetIsHiding()),
            this, SLOT(treatHidingOfInternalWidget()));
}

void ArrowedTextControlWidgetWrapper::setArrowedText(ArrowedText *arrowedText)
{
    mControlWidgetCopy->setArrowedText(arrowedText);
}


