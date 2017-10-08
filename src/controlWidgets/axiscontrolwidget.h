#ifndef AXISCONTROLWIDGET_H
#define AXISCONTROLWIDGET_H

#include <QWidget>
#include "core/axismodel.h"
#include "controlWidgets/controlwidget.h"
namespace Ui {
class AxisControlWidget;
}



/*!
 \brief AxisControlWidget is  a widget to show and change axis properties.

*/
class AxisControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit AxisControlWidget(QWidget *parent = 0);
    ~AxisControlWidget();

    void setAxisModels(QVector<AxisModel*>);
    void setExtendedMode(bool);
signals:
    void movementRequest(AxisModel*, AxisModel::AxisMovement);
    void settingYRangeRequest(double, double);
    void deletingRequest();

    void widgetIsHiding();

public slots:
    void treatMoveUpRequest();
    void treatMoveDownRequest();
//    void treatSetYMarginRequest();
    void treatSetYHighMarginRequest();
    void treatSetYLowMarginRequest();

    void treatScalingTypeChanging();
    void initializeState();
    void treatDeletingRequest();
    void treatStretchFactorChanging();
    void treatAxisModelRemoval();
    void treatYLabelChanging();
    void treatLegendTitleChanging();

    void treatBackgroundStyleChanging();
    void treatBackgroundColorChanging();
    void showBackgroundTextureChoiceDialog();

private:
    Ui::AxisControlWidget *ui;
//    AxisModel *mAxisModelVector;
    QVector<AxisModel *> mAxisModelVector;

    void disconnectWidgets();
    void connectWidgets();
};

#endif // AXISCONTROLWIDGET_H
