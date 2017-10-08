#ifndef PLOTLAYOUTMODEL_H
#define PLOTLAYOUTMODEL_H

#include <QObject>
#include "graphmodel.h"
#include "core/axismodel.h"
#include "plotStyle/plotstyle.h"
#include "event/timeevent.h"
#include "anchor/anchor.h"
#include "core/axisstate.h"
#include "global/global_definitions.h"
#include "core/plotpreferences.h"
/*!
 \brief PlotLayoutModel is  a core class that encapsulates all major layout properties of a plot

 PlotLayoutModel contains  one or several AxisModel objects. All plot layout changes (graph deletions,
 additions, axis deletions...) are implemented through PlotLayoutModel methods. PlotLayoutModel gathers all
 signals from AxisModel and GraphModel objects, that it contains, and emits signals if necessary to PlotWidget for
 redrawing.

*/

class PlotLayoutModel : public QObject
{
    Q_OBJECT
public:
    enum class ScalingMode { Auto,
                             Manual
                           };

    enum  class LayoutState { EachSeparate,
                              AllInOne,
                              Complex
                            };
    enum class ScaleType {Linear,
                          Logarithmic
                         };


    explicit PlotLayoutModel(QObject *parent = 0);

    GraphModel* addPlot(const QVector<double> xData, const QVector<double> yData, const QString &name);
    void addCommand(const TimeEvent* newEvent);
    void addCommand(double time);
    void removeCommand(TimeEvent* newEvent);
    void addAnchor(double time);
    void removeAnchor(Anchor* anchor);
    void removeArrowedText(ArrowedText* arrowedText);
    void removeHorizontalLevel(HorizontalLevel* level);

    void removeObjects(QVector<GraphModel*> graphVector, QVector<TimeEvent*> commandVector,
                       QVector<Anchor*> anchVector, QVector<ArrowedText*> arrTextVector, QVector<HorizontalLevel*> horLevel);

    QVector<GraphModel*> getGraphModelVector() const;
    QVector<AxisModel*> getAxisModelVector() const;
    QVector<AxisModel*> getVisibleAxisModelVector() const;
    std::tuple<double, double> getXRange() const;
    std::tuple<double, double> getXMaXTotalRange() const;

    bool containsAxis(AxisModel*) const;
    bool containsGraph(GraphModel*) const;
    bool isLegendVisible() const;
    bool areEventsVisible() const;
    QVector<TimeEvent*> commandsVector() const;
    bool areUnitsVisible() const;
    QFont legendFont() const;
    QFont commandFont() const;
    QFont yLabelFont() const;
    QFont xAxisFont() const;
    QFont yAxisFont() const;
    QFont arrowedTextFont() const;
    QString xLabel() const;
    QFont xLabelFont() const;
    double xTickRotation() const;
    double commandLabelRotation() const;
    QString title() const;
//    QFont titleFont() const;
    ScaleType scaleType() const;
    QString getInfoString(double x, double y) const;
    bool isWheelZoomEnabled() const;
    QCPAxis::LabelType timeAxisType() const;
    QVector<Anchor*> anchorVector() const;
    style::LegendLocation legendLayout() const;
    bool isLegendFrameVisible() const;

    style::Grid xGrid() const { return mPlotStyle.mXGridProperties; }
    style::Grid yGrid() const { return mPlotStyle.mYGridProperties; }
    style::Grid xSubGrid() const { return mPlotStyle.mXSubGridProperties; }
    style::Grid ySubGrid() const { return mPlotStyle.mYSubGridProperties; }
    style::Label titleProperties() const { return mPlotStyle.mTitleProperties; }
    style::AxisRect axisRect() const { return mPlotStyle.mAxisRectProperties; }

    style::Tick topSubTick()    const { return mPlotStyle.mSubTickProperties; }
    style::Tick rightSubTick()  const { return mPlotStyle.mSubTickProperties; }
    style::Tick bottomSubTick() const { return mPlotStyle.mSubTickProperties; }
    style::Tick leftSubTick()   const { return mPlotStyle.mSubTickProperties; }

    style::MainTick topTick()    const { return mPlotStyle.mTickProperties; }
    style::MainTick rightTick()  const { return mPlotStyle.mTickProperties; }
    style::MainTick bottomTick() const { return mPlotStyle.mTickProperties; }
    style::MainTick leftTick()   const { return mPlotStyle.mTickProperties; }

    PlotStyle plotStyle() const { return mPlotStyle; }

    int getMaximumLegendWidth() const;
    int getMaximumDimensionWidth() const;
    int getMaximumTitleHeight() const;
    int getMaximumXLabelHeight() const;
    int getMaximumCommandWidth() const;
    int getMaximumHorizontalLevelWidth(const QFont&) const;


    PlotLayoutModel* clone() const;
    bool retrieveGraph(GraphModel*);

    void setXGrid(const style::Grid& grid);
    void setYGrid(const style::Grid& grid);
    void setXSubGrid(const style::Grid& grid);
    void setYSubGrid(const style::Grid& grid);
    void setTitleProperties(const style::Label& titleProp);
    void setAxisRect(const style::AxisRect& axisRect);
    void setTick(const style::MainTick& tick);
    void setSubTick(const style::Tick& subTick);
    void setColorPref(const style::ColorPref& colorPref);


signals:
    void layoutModelChanged();
    void layoutAppearanceChanged();
    void anchorsChanged();
    //void graphAppearanceChanged(GraphModel*);  //for future using
    //void graphDataChanged(GraphModel*);


public slots:
    void prepareDataForSecondaryProcessing();
    void somethingInternalHappened();
    void fixAppearanceChanging();
    void fixLayoutChanging();

    void composeIntellectually();
    void setLayoutState(LayoutState state);
    void moveGraphModel(GraphModel*, GraphModel::GraphMovement);
    void moveAxisModel(AxisModel*, AxisModel::AxisMovement);
    void moveGraphToAxis(GraphModel*, AxisModel*);
    void addExternalGraphToAxis(GraphModel*, AxisModel*);
    void moveGraphVectorToAxis(QVector<GraphModel*>, AxisModel*);
    void moveAxis(AxisModel* source, AxisModel* destination);
    void moveGraphToNewAxis(GraphModel *graphModel, int index);
    void moveExternalGraphToNewAxis(GraphModel *graphModel, int index);
    void moveGraphVectorToNewAxis(QVector<GraphModel*> graphModelVector, int index);
    void mergeAxis(AxisModel* source, AxisModel* destination);

    void treatChangesInAnchors();

    void setAutoDimensions();
    void convertGraphToCommand(GraphModel*);
    void addToGlobalDataSource(GraphModel*);
    void addToGlobalEventSource(TimeEvent*);

    void addBitGraph(GraphModel*, int, int);

    void removeAxisModel(AxisModel*);
    void removeAxisModel(int axisIndex);
    void removeGraphModel(GraphModel*);

    void autoscaleXRange();
    void autoscaleYRange();
    void autoscaleYRangeWithRespectToX();

    AxisModel* addNewAxis(int newAxisPosition = -1);

    void setXRange(double xRangeBegin, double xRangeEnd);
    void setYRange(double yRangeBegin, double yRangeEnd, int axisIndex);
    void setYRangeForVisibleAxis(double yRangeBegin, double yRangeEnd, int axisIndex);

    void setXRangeWrapper(double xRangeBegin, double xRangeEnd);
    void setYRangeWrapper(double yRangeBegin, double yRangeEnd, int axisIndex);
    void setYRangeForVisibleAxisWrapper(double yRangeBegin, double yRangeEnd, int axisIndex);
    void setXYRangeWrapper(double xRangeBegin, double xRangeEnd, double yRangeBegin, double yRangeEnd, int axisIndex);

    void setPlotStyle(const PlotStyle &pref);
    void setLegendFontSize(int size);
    void setYLabelFontSize(int size);
    void setXAxisFontSize(int size);
    void setYAxisFontSize(int size);
    void setXTickRotaion(double rotation);
    void setTimeEventLabelRotaion(double rotation);
    void setLegendVisibility(bool vis);
    void setDimensionVisibility(bool vis);
    void setXLabel(QString label);
    void setXLabelFontSize(int size);
    void setTitle(const QString& title);
//    void setTitleFontSize(int size);
    void setScaleType(ScaleType scale);
    void setTimeEventFontSize(int size);
    void setArrowedTextFontSize(int size);
    void setTimeAxisType(QCPAxis::LabelType);
    void setLegendLayout(style::LegendLocation);
    void setLegendFrameVisibility(bool);

    void addArrowedTextByTime(double time);

    //history
    void stepBackInHistory();
    void stepForwardInHistory();

private slots:
    void applyState(const QVector<AxisState> &state);
    void updateAxisHistory();
    void clearAxisHistory();

private:

    LayoutState mState;
//    QVector< QVector<GraphModel*> > mLayoutVector;
    QVector<AxisModel*> mAxisModelVector;
    QVector<GraphModel*> mGraphModelVector;
    ScalingMode xScalingMode;
    double mXRangeBegin;
    double mXRangeEnd;

    PlotStyle mPlotStyle;
    PlotPreferences mPreferences;

    QString mTitle;
    ScaleType mScaleType;

    QVector<TimeEvent*> mEventsVector;
    QVector<Anchor*> mAnchorVector;
    QVector<QVector<AxisState>> mAxisHistory;
    int mHistoryPosition;


};

#endif // PLOTLAYOUTMODEL_H
