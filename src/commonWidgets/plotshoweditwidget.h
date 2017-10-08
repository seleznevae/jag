#ifndef PLOTSHOWEDITWIDGET_H
#define PLOTSHOWEDITWIDGET_H

#include <QWidget>
#include "core/plotlayoutmodel.h"
#include "controlWidgets/plotcontrolwidget.h"
#include "core/plotwidget.h"
#include "plotStyle/plotstyle.h"
#include <QPixmap>
#include "event/timeevent.h"
#include "core/graphcontainer.h"
#include "commonWidgets/plotwidgetwithtoolbar.h"

namespace Ui {
class PlotShowEditWidget;
}



/*!
 \brief PlotShowEditWidget contains PlotWidget, PlotHierarchyWidget and a set of different widgets to change properties of graphs,
 axis, anchors...

*/
class PlotShowEditWidget : public QWidget, public GraphContainer
{
    Q_OBJECT

public:
    explicit PlotShowEditWidget(QWidget *parent = 0);
    ~PlotShowEditWidget();

    QPixmap toPixmap();
    QPixmap toPixmap(int width, int height);
    void toPainter(QCPPainter *painter,  int width = 0, int height = 0);

    PlotWidget::PlotWidgetMode plotWidgetState() const;

    const PlotWidget* plotWidget() const;
    QVector<QString> getGraphNames() const;

    QVector<std::pair<QString, QString>> currentPlotGraphNamesAndArgs() const;

//public slots:
    virtual GraphModel* addNewGraph(const QVector<double> &x, const QVector<double> &y, const QString &name);
    virtual void addNewEvent(const TimeEvent *event);
    virtual void addNewPlotFrame(Qt::Orientation orient = Qt::Horizontal);
    void cloneCurrentPlotFrame(Qt::Orientation orient = Qt::Horizontal);

    void removeLayoutModel(PlotLayoutModel* layoutModel);

public slots:

    void treatGraphEditRequest(GraphModel*);
    void treatGraphVectorEditRequest(QVector<GraphModel*>);
    void treatAxisEditRequest(QVector<AxisModel*> );
    void treatCommandEditRequest(TimeEvent*);
    void treatAnchorEditRequest(Anchor*);
    void treatArrowedTextEditRequest(ArrowedText*);
    void treatHorizontalLevelEditRequest(HorizontalLevel*);

    void treatFastSecondaryProcessingRequest(double start, double finish, int indexOfVisibleAxis);


    void treatCurrentPlotFrameChanging(QWidget*);

    void autoscale();
    void setPlotWidgetState(PlotWidget::PlotWidgetMode);
    void combineGraphs();
    void separateGraphs();
    void composeIntellectually();
    void moveCurrentObject(AxisModel::AxisMovement);
    void stretchCurrentAxis(double factor);
    void increaseDecimationOfCurrentGraph(double factor);
    void deleteCurrentObject();
    void increaseWidthOfCurrentGraph(double);
    void switchLineScatterOfCurrentGraph(int);
    void switchLineColorOfCurrentGraph(int);
    void switchLineInterpolationOfCurrentGraph(int);
    bool toogleFullScreen();


    void setPreferences(const PlotStyle &pref);
    void emitInfoString(double, double);

    void savePng(QString fileName);
    void saveBmp(QString fileName);
    void saveJpg(QString fileName);

    void activateGraph(int index);
    void activateAxis(int index);

    void secondaryProcessing();
    void setAutoUnits();

    void stepBackInHistory();
    void stepForwardInHistory();

    void updateAppearance();
    void installToolbarActions();

    void treatExternalGraphMovingToExistingAxisRequest(GraphModel*, AxisModel*);
    void treatExternalGraphMovingToNewAxisRequest(GraphModel*, int);

signals:
    void infoStringEmitted(QString);
    void currentPlotFrameChanged();

protected:
//    void keyPressEvent(QKeyEvent *event);

private:
    Ui::PlotShowEditWidget *ui;
//    PlotLayoutModel mLayoutModel;
    QVector<PlotLayoutModel*> mLayoutModelVector;
    QVector<PlotWidget*> mPlotWidgetVector;
    PlotLayoutModel* mCurrentLayoutModel;
    PlotWidget* mCurrentPlot;

    bool isControlsVisible;
    PlotStyle mPreferences;
    
    void setControlWidgetsVisible(bool);

    void makeCurrentPair(PlotLayoutModel*, PlotWidget*);

    void connectLayout(PlotLayoutModel*);
    void disconnectLayout(PlotLayoutModel*);
    void connectPlot(PlotWidget*);
    void disconnectPlot(PlotWidget*);

    bool saveRastered(const QString &fileName, const char *format, int quality = -1);

    void addNewLayout(PlotLayoutModel*, Qt::Orientation orient);

private slots:
    void treatPlotWidgetMovingEnabledChanging(bool enabled);
};

#endif // PLOTSHOWEDITWIDGET_H
