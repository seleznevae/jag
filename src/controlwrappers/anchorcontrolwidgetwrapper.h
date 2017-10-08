#ifndef ANCHORCONTROLWIDGETWRAPPER_H
#define ANCHORCONTROLWIDGETWRAPPER_H


#include <anchor/anchorcontrolwidget.h>
#include "controlwrappers/controlwidgetwrapper.h"

class AnchorControlWidgetWrapper: public ControlWidgetWrapper
{
    Q_OBJECT

public:
    AnchorControlWidgetWrapper(QWidget *parent=0);
    ~AnchorControlWidgetWrapper() { }

    void setAnchor(Anchor *anchor);

public slots:
    void treatNameChanging();
    void treatTimeChanging();
    void treatAnchorDeleting(QObject *);

signals:
    void arrowedTextCreationRequest(double);



private:
    AnchorControlWidget *mControlWidgetCopy;
};
#endif // ANCHORCONTROLWIDGETWRAPPER_H


