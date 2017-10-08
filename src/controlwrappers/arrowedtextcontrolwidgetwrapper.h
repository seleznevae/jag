#ifndef ARROWEDTEXTCONTROLWIDGETWRAPPER_H
#define ARROWEDTEXTCONTROLWIDGETWRAPPER_H


#include <controlWidgets/arrowedtextcontrolwidget.h>
#include "controlwrappers/controlwidgetwrapper.h"

class ArrowedTextControlWidgetWrapper : public ControlWidgetWrapper
{
    Q_OBJECT
public:
    explicit ArrowedTextControlWidgetWrapper(QWidget *parent = 0);

    void setArrowedText(ArrowedText* arrowedText);


private:
    ArrowedTextControlWidget *mControlWidgetCopy;

};

#endif // ARROWEDTEXTCONTROLWIDGETWRAPPER_H





