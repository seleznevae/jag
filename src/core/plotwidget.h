#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "qcustomplot.h"
#include "core/graphmodel.h"
#include "core/plotlayoutmodel.h"
#include <QVector>
#include <QRubberBand>
//#include "basic/selectionframe.h"
#include "fullScreen/QFullScreenAdapter.h"
#include <QPoint>



class PlotWidgetState;
class PlotWidgetArrowDrawingState;
class PlotWidgetFastSecondaryProcessingState;
class PlotWidgetLevelDrawingState;
class PlotWidgetMeasuringState;
class PlotWidgetMotionState;
class PlotWidgetNormalState;
class PlotWidgetXYZoomState;
class PlotWidgetXZoomState;
class PlotWidgetYZoomState;


/*!
 \brief PlotWidget is  a core drawing widget.

PlotWidget is a subclass of QCustomPlot. The main purpose of this class is to request all axis and graph properties that PlotLayoutModel
contains, to draw them properly
and update its contents in case of some changes when PlotLayoutModel notifies about them.

*/
class PlotWidget : public QCustomPlot, public QFullScreenAdapter
{
    Q_OBJECT
public:
    enum class PlotWidgetMode {
                               ArrowDrawing,
                               Common,
                               FastSecondaryProcessing,
                               LevelDrawing,
                               Measuring,
                               Motion,
                               ZoomX,
                               ZoomY,
                               ZoomXY,
                              };

    enum class ZoomState  {Off,
                            On
                           };
    enum class State {Off,
                      On
                     };
    enum class ZoomType {LeftButtonZoom,
                         RightButtonZoom
                         };
    explicit PlotWidget(QWidget *parent = 0);
    void setLayoutModel(const PlotLayoutModel *layoutModel);

//    void addModel(const GraphModel *newModel);
//    void removeModel(const GraphModel *newModel);

    ~PlotWidget();

    PlotWidgetMode plotWidgetState() const;
    const PlotLayoutModel* plotLayoutModel() const;
    QVector<QCPItemTracer*>  tracerVector() const;

    QPixmap toPixmap(int width=0, int height=0, double scale=1.0);
    bool savePng(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
    bool saveJpg(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
    bool saveBmp(const QString &fileName, int width=0, int height=0, double scale=1.0);
    void toPainter(QCPPainter *painter, int width = 0, int height = 0);


    //friends
    friend class PlotWidgetArrowDrawingState;
    friend class PlotWidgetFastSecondaryProcessingState;
    friend class PlotWidgetLevelDrawingState;
    friend class PlotWidgetMeasuringState;
    friend class PlotWidgetState;
    friend class PlotWidgetMotionState;
    friend class PlotWidgetNormalState;
    friend class PlotWidgetXYZoomState;
    friend class PlotWidgetXZoomState;
    friend class PlotWidgetYZoomState;




protected:
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );

    virtual void paintEvent ( QPaintEvent * event ) ;

    virtual void resizeEvent(QResizeEvent * event);
    virtual void leaveEvent(QEvent * event );

    virtual void dropEvent(QDropEvent * event );
    virtual void dragEnterEvent(QDragEnterEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent * event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);

signals:
    void xZoomRequest(double, double);
    void yZoomRequest(double, double, int);
    void xyZoomRequest(double, double, double, double, int);
    void currentCoordinates(double, double);
    void plotWidgetMovingIsEnabled(bool); //to desable plot frames moving in case user captured graph for moving
    void externalGraphMovingToExistingAxisRequest(GraphModel*, AxisModel*);
    void externalGraphMovingToNewAxisRequest(GraphModel*, int);
    void fastSecondaryProcessingRequest(double start, double finish, int indexOfVisibleAxis);

public slots:

    void setPlotWidgetState(PlotWidgetMode);

    void updateEverything();
    void updateAppearance();

    void setXRangeForAllAxis(QCPRange);

private slots:
    void updateTracers();
    void updateAnchors();
//    void emitPlotWidgetMovingIsEnabled(bool);

private:
    const PlotLayoutModel * mLayoutModel;

    QVector<QCPGraph*> mFunctionVector;
    QVector<QCPCurve*> mCurveVector;
    QVector<QCPAbstractPlottable*> mGraphVector;

    QVector<QCPGraph*> mEventsVector;
    QVector<QCPGraph*> mLevelsVector;
    QVector<QCPItemText*> mTextLabelsVector;
    QVector<QCPItemLine*> mArrowsVector;
//    QVector<QCPAxisRect*> mAxisRectVector;


//    QVector<const GraphModel*> modelVector;
//    QVector<QCPGraph*> graphVector;

    void timerEvent(QTimerEvent * event);
    int mTimerId;
    int mUpdateCounter;
    int mUpdateEverythingCounter;
    int mUpdateAppearanceCounter;

    int numberOfPreviousPlots;  //for dirty tricking
    QCPMarginGroup *mAxisLeftRightMarginGroup;


    PlotWidgetMode mMode;

    QVector<QCPItemTracer*>  mTracerVector;
    QVector<QCPItemTracer*>  mAnchorVector;

    QCPLayoutGrid *mBottomRightLegendLayout;

    PlotWidgetState *mState;
    PlotWidgetState *mWidgetEmptyState;
    PlotWidgetArrowDrawingState *mWidgetArrowDrawingState;
    PlotWidgetFastSecondaryProcessingState *mWidgetFastSecondaryProcessingState;
    PlotWidgetLevelDrawingState *mWidgetLevelDrawingState;
    PlotWidgetMeasuringState *mWidgetMeasuringState;
    PlotWidgetMotionState *mWidgetMotionState;
    PlotWidgetNormalState *mWidgetNormalState;
    PlotWidgetXYZoomState *mWidgetXYZoomState;
    PlotWidgetXZoomState *mWidgetXZoomState;
    PlotWidgetYZoomState *mWidgetYZoomState;

    PlotStyle mPlotStyle;

    void treatUpdateRequest();
    void createGraphics();
    void createAppearance();
    int getIndexOfAxisRectByPoint(const QPoint& point);

    void increaseWidthOfObjects(double widDev, double lineWidthDev);

    void recountLastAxisStretch();

    void addAxisRectToLayout(int axisNumber, QCPLayoutElement *element);
    void addLegendRectToLayout(int legendNumber, QCPLayoutElement *element);
    void addLegendToLayout();
    void addTitleToLayout();
};

#endif // PLOTWIDGET_H
