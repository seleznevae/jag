#ifndef COMMANDCONTROLWIDGET_H
#define COMMANDCONTROLWIDGET_H

#include <QWidget>
#include "event/timeevent.h"
#include "controlWidgets/controlwidget.h"
namespace Ui {
class EventControlWidget;
}


/*!
 \brief EventControlWidget is  a widget to show and change TimeEvent properties.

*/
class EventControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit EventControlWidget(QWidget *parent = 0);
    ~EventControlWidget();

    void setTimeEvent(TimeEvent *newEvent);

    void setExtendedMode(bool);

public slots:
    void treatNameChanging();
    void treatTimeChanging();
    void treatDescriptionChanging();
    void treatEventDeleting(QObject* event);
    void treatLabelDeviationChanging();

    void treatPenWidthChanging();
    void treatPenStyleChanging();
    void treatPenColorChanging();

//    void treatBrushColorChanging();
    void treatBrushColorChanging(QColor);
//    void showBrushColorDialog();
    void treatBrushStyleChanging();
    void treatBrushBasisChanging();
    void treatBrushDurationChanging();

    void treatArrowedTextCreationRequest();

signals:
    void widgetIsHiding();
    void arrowedTextCreationRequest(double);

private:
    Ui::EventControlWidget *ui;

    TimeEvent *mEvent;
    void initializeState();
};

#endif // COMMANDCONTROLWIDGET_H
