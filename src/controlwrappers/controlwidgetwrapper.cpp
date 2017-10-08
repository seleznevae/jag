#include "controlwidgetwrapper.h"
#include "global/global_definitions.h"

//using namespace QSint;

ControlWidgetWrapper::ControlWidgetWrapper(QWidget* controlWidget, QWidget *parent)
    :WidgetWrapperWithHeader(parent) /*ActionGroup(parent)*/, mControlWidget(controlWidget)
{
    QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Preferred);
    pol.setHorizontalStretch(50);
    if (mControlWidget) {
        mControlWidget->setSizePolicy(pol);
        addWidget(mControlWidget);
    }

    setHeader(true);
    setExpandable(true);




//    switch (global::gControlWidgetsStyle) {
//        case global::ControlWidgetsStyle::AndroidPanel:
//            setScheme(AndroidPanelScheme::defaultScheme());
//            break;
//        case global::ControlWidgetsStyle::MacPanel:
//            setScheme(MacPanelScheme::defaultScheme());
//            break;
//        case global::ControlWidgetsStyle::WinWistaPanel:
//            setScheme(WinVistaPanelScheme::defaultScheme());
//            break;
//        case global::ControlWidgetsStyle::WinXPPanel:
//            setScheme(WinXPPanelScheme::defaultScheme());
//            break;
//        case global::ControlWidgetsStyle::WinXPPanel2:
//            setScheme(WinXPPanelScheme2::defaultScheme());
//            break;
//    }

}


void ControlWidgetWrapper::show()
{
//    ActionGroup::show();
    WidgetWrapperWithHeader::show();
    if (mControlWidget)
        mControlWidget->show();
}

void ControlWidgetWrapper::treatHidingOfInternalWidget()
{
    hide();
}



void ControlWidgetWrapper::resizeEvent(QResizeEvent *event)
{
//    ActionGroup::resizeEvent(event);
    WidgetWrapperWithHeader::resizeEvent(event);
    //    controlWidget->setMinimumWidth(width() - 10);
}


