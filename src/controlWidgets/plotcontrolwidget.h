#ifndef PLOTCONTROLWIDGET_H
#define PLOTCONTROLWIDGET_H

#include <QWidget>
#include "core/plotlayoutmodel.h"
#include "controlWidgets/controlwidget.h"
namespace Ui {
class PlotControlWidget;
}
class FastenToolButton;

/*!
 \brief AxisControlWidget is  a widget to show and change properties that concern with properties of all axis (not one particular).

*/
class PlotControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit PlotControlWidget(QWidget *parent = 0);
    ~PlotControlWidget();

    void setLayoutModel(PlotLayoutModel*);
    void setExtendedMode(bool);
public slots:
    void initializeState();
    void treatXLowLimitRequest();
    void treatXHighLimitRequest();
    void treatXLabelChanging();
    void treatTitleChanging();
    void treatTitlePropertiesChanging();

    void treatScaleTypeChanging();
    void treatTimeAxisTypeChanging();
    void setMinWidth(int);
    void treatXTickRotationChanging();
    void treatCommandLabelRotationChanging();
    void treatLegendLayoutChanging();
    void treatLegendFontSizeChanging();
    void treatUnitsFontSizeChanging();
    void treatXLabelFontSizeChanging();
    void treatXAxisFontSizeChanging();
    void treatYAxisFontSizeChanging();
//    void treatTitleFontSizeChanging();
    void treatEventFontSizeChanging();
    void treatArrowedTextFontSizeChanging();

    void treatLegendFrameVisibilityChanging();
    void treatLegendVisibilityChanging();
    void treatUnitsVisibilityChanging();

    void treatXGridChanging();
    void treatYGridChanging();
    void treatXSubGridChanging();
    void treatYSubGridChanging();
    void treatAxisRectChanging();
    void treatTickChanging();
    void treatSubTickChanging();
    void treatColorPrefChanging();
    void treatXAxisPrefChanging();
    void treatYAxisPrefChanging();



    void updateRanges();

private slots:
    void treatFastenToolButtonPressed();
    void initiliazeLayoutPreferencesFromDefault();

signals:
//    void settingXRangeRequest(double, double);
//    void legendChangingVisibilityRequest(bool);
//    void dimensionChangingVisibilityRequest(bool);



private:
    Ui::PlotControlWidget *ui;
    PlotLayoutModel *mLayoutModel;

    QVector<FastenToolButton*> mFastenToolButtonVector;
    QVector<QWidget*> mTabsVector;
public:
    static QVector<bool> sTabVisibilityVector;
    static QVector<QString> sTabNamesVector;
};

#endif // PLOTCONTROLWIDGET_H





