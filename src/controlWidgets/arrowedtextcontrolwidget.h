#ifndef ARROWEDTEXTCONTROLWIDGET_H
#define ARROWEDTEXTCONTROLWIDGET_H

#include <QWidget>
#include "core/arrowedtext.h"
#include "controlWidgets/controlwidget.h"


namespace Ui {
class ArrowedTextControlWidget;
}

/*!
 \brief ArrowedTextControlWidget is  a widget to show and change properties of the ArrowedText.

*/
class ArrowedTextControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit ArrowedTextControlWidget(QWidget *parent = 0);

    void setArrowedText(ArrowedText* arrowedText);
    ~ArrowedTextControlWidget();

    void setExtendedMode(bool);
public slots:
    void treatNameChanging();
    void treatTextXCoordChanging();
    void treatTextYCoordChanging();
    void treatArrowXCoordChanging();
    void treatArrowYCoordChanging();
//    void treatLineWidthChanging();
    void treatPenChanging();
    void treatBackgroundColorChanging(QColor);
    void treatArrowedTextDeleting(QObject*);
    void treatArrowStyleChanging();

signals:
    void widgetIsHiding();

private:
    Ui::ArrowedTextControlWidget *ui;
    ArrowedText *mArrowedText;

    void initializeState();
};

#endif // ARROWEDTEXTCONTROLWIDGET_H
