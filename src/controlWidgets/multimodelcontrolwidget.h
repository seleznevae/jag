#ifndef MULTIMODELCONTROLWIDGET_H
#define MULTIMODELCONTROLWIDGET_H

#include <QWidget>


#include "core/graphmodel.h"
#include <QVector>
#include "core/plotlayoutmodel.h"
#include "controlWidgets/controlwidget.h"

class FastenToolButton;
class QWidget;
namespace Ui {
class MultiModelControlWidget;
}

/*!
 \brief MultiModelControlWidget is  a widget to show and change properties of several graphs simultaneously.

*/
class MultiModelControlWidget : public ControlWidget
{
    Q_OBJECT

public:
    explicit MultiModelControlWidget(QWidget *parent = 0);
    ~MultiModelControlWidget();

    void setModelVector(QVector<GraphModel*> newModelVector);
    void setLayoutModel(PlotLayoutModel *model);

    void setExtendedMode(bool);
signals:
    void widgetIsHiding();

public slots:
    void initializeStateFromModels();

private slots:
    void treatVisibilityChanging();
    void treatLegendabilityChanging();
    void treatNameChanging();
    void treatDimensionChanging();
    void treatLineWidthChanging();
    void treatLineColorChanging();
//    void showLineColorDialog();
    void treatLineStyleChanging();
    void treatInterpolationChanging();

    void showMarkerIconChoiceDialog();
    void showBrushTextureChoiceDialog();

    void treatScatterShapeChanging();
    void treatScatterSizeChanging();
    void treatScatterDecimationChanging();
    void treatScatterBrushColorChanging();
    void treatScatterPenColorChanging();
    void treatScatterPenWidthChanging();
    void treatBrushStyleChanging();
    void treatBrushBasisChanging();
    void treatBrushColorChanging();
    void treatStringRepresentationChanging();
    void treatDataLabelsVisibilityChanging();
    void treatDataLabelsColorChanging();
//    void showBrushColorDialog();


    void treatX0Changing();
    void treatKXChanging();
    void treatDXChanging();
    void treatY0Changing();
    void treatKYChanging();
    void treatDYChanging();

//    void treatXA0Changing();
//    void treatXA1Changing();
//    void treatYB0Changing();
//    void treatYB1Changing();

    void copyGraphProperties();
    void pasteGraphProperties();

private slots:
    void treatFastenToolButtonPressed();

public slots:
    void treatModelDeleting();

private:
    Ui::MultiModelControlWidget *ui;

    QVector<GraphModel*> mGraphVector;
    PlotLayoutModel *mLayoutModel;

    QVector<FastenToolButton*> mFastenToolButtonVector;
    QVector<QWidget*> mTabsVector;

    void disconnectFromModels();
    void connectToModels();
public:
    static QVector<bool> sTabVisibilityVector;
    static QVector<QString> sTabNamesVector;
};

#endif // MULTIMODELCONTROLWIDGET_H
