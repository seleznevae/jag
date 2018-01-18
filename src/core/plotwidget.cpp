#include <global/global_definitions.h>
#include "plotwidget.h"
#include "basic/bassic.h"
#include <QDebug>
#include <QApplication>
#include <cmath>
#include <algorithm>
#include <cfloat>


#include <core/horizontallevel.h>

#include <core/colormap/dummycolormapevaluator.h>

#include "core/plotwidgetstate/plotwidgetstate.h"
#include "core/plotwidgetstate/plotwidgetarrowdrawingstate.h"
#include "core/plotwidgetstate/plotwidgetleveldrawingstate.h"
#include "core/plotwidgetstate/plotwidgetmeasuringstate.h"
#include "core/plotwidgetstate/plotwidgetmotionstate.h"
#include "core/plotwidgetstate/plotwidgetnormalstate.h"
#include "core/plotwidgetstate/plotwidgetxyzoomstate.h"
#include "core/plotwidgetstate/plotwidgetxzoomstate.h"
#include "core/plotwidgetstate/plotwidgetyzoomstate.h"
#include "core/plotwidgetstate/plotwidgetfastsecondaryprocessingstate.h"

PlotWidget::PlotWidget(QWidget *parent) :
    QCustomPlot(parent), QFullScreenAdapter(this), mLayoutModel(NULL), mTimerId(0), mUpdateCounter(0), mUpdateEverythingCounter(0),
    mUpdateAppearanceCounter(0), numberOfPreviousPlots(0), mAxisLeftRightMarginGroup(new QCPMarginGroup(this)),
    mMode(PlotWidgetMode::Common),
    mBottomRightLegendLayout(NULL), mState(NULL), mWidgetEmptyState(NULL), mWidgetArrowDrawingState(NULL),
    mWidgetFastSecondaryProcessingState(NULL), mWidgetLevelDrawingState(NULL), mWidgetMeasuringState(NULL),
    mWidgetMotionState(NULL), mWidgetNormalState(NULL), mWidgetXYZoomState(NULL), mWidgetXZoomState(NULL), mWidgetYZoomState(NULL)
{

    mWidgetEmptyState = new PlotWidgetState(this);
    mWidgetArrowDrawingState = new PlotWidgetArrowDrawingState(this);
    mWidgetFastSecondaryProcessingState = new PlotWidgetFastSecondaryProcessingState(this);
    mWidgetLevelDrawingState = new PlotWidgetLevelDrawingState(this);
    mWidgetMeasuringState = new PlotWidgetMeasuringState(this);
    mWidgetMotionState = new PlotWidgetMotionState(this);
    mWidgetXYZoomState = new PlotWidgetXYZoomState(this);
    mWidgetXZoomState = new PlotWidgetXZoomState(this);
    mWidgetYZoomState = new PlotWidgetYZoomState(this);
    mWidgetNormalState = new PlotWidgetNormalState(this);
    mState = mWidgetNormalState;

    addLayer("graph", layer("axes"));
    addLayer("tracer", layer("graph"));
    addLayer("arrowed_text", layer("tracer"));

    setAcceptDrops(true);
}

void PlotWidget::setLayoutModel(const PlotLayoutModel *layoutModel)
{

    if (layoutModel != NULL) {
        disconnect(layoutModel, SIGNAL(layoutModelChanged()),
                   this, SLOT(updateEverything()));
    }
    mLayoutModel = layoutModel;
    connect(mLayoutModel, SIGNAL(layoutModelChanged()),
            this, SLOT(updateEverything()));
    connect(mLayoutModel, SIGNAL(layoutAppearanceChanged()),
            this, SLOT(updateAppearance()));
    connect(this, SIGNAL(xZoomRequest(double,double)),
            mLayoutModel, SLOT(setXRangeWrapper(double,double)));
    connect(this, SIGNAL(yZoomRequest(double,double,int)),
            mLayoutModel, SLOT(setYRangeForVisibleAxisWrapper(double,double,int)));
    connect(this, SIGNAL(xyZoomRequest(double,double,double,double,int)),
            mLayoutModel, SLOT(setXYRangeWrapper(double,double,double,double,int)));

}

PlotWidget::~PlotWidget()
{
    delete mAxisLeftRightMarginGroup;
}

PlotWidget::PlotWidgetMode PlotWidget::plotWidgetState() const
{
    return mMode;
}

const PlotLayoutModel *PlotWidget::plotLayoutModel() const
{
    return mLayoutModel;
}

QVector<QCPItemTracer *> PlotWidget::tracerVector() const
{
    return mTracerVector;
}

QPixmap PlotWidget::toPixmap(int width, int height, double scale)
{
    increaseWidthOfObjects(global::gWidthDeviationForExternalPlotting, global::gLineWidthDeviationForExternalPlotting);
    auto pixMap = QCustomPlot::toPixmap(width, height, scale);
    increaseWidthOfObjects(-global::gWidthDeviationForExternalPlotting, -global::gLineWidthDeviationForExternalPlotting);
    return pixMap;
}

bool PlotWidget::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
    increaseWidthOfObjects(global::gWidthDeviationForExternalPlotting, global::gLineWidthDeviationForExternalPlotting);
    auto ret = QCustomPlot::savePng(fileName, width, height, scale, quality);
    increaseWidthOfObjects(-global::gWidthDeviationForExternalPlotting, -global::gLineWidthDeviationForExternalPlotting);
    return ret;
}

bool PlotWidget::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
    increaseWidthOfObjects(global::gWidthDeviationForExternalPlotting, global::gLineWidthDeviationForExternalPlotting);
    auto ret = QCustomPlot::saveJpg(fileName, width, height, scale, quality);
    increaseWidthOfObjects(-global::gWidthDeviationForExternalPlotting, -global::gLineWidthDeviationForExternalPlotting);
    return ret;
}

bool PlotWidget::saveBmp(const QString &fileName, int width, int height, double scale)
{
    increaseWidthOfObjects(global::gWidthDeviationForExternalPlotting, global::gLineWidthDeviationForExternalPlotting);
    auto ret = QCustomPlot::saveBmp(fileName, width, height, scale);
    increaseWidthOfObjects(-global::gWidthDeviationForExternalPlotting, -global::gLineWidthDeviationForExternalPlotting);
    return ret;
}

void PlotWidget::toPainter(QCPPainter *painter, int width, int height)
{
    increaseWidthOfObjects(global::gWidthDeviationForExternalPlotting, global::gLineWidthDeviationForExternalPlotting);
    QCustomPlot::toPainter(painter, width, height);
    increaseWidthOfObjects(-global::gWidthDeviationForExternalPlotting, -global::gLineWidthDeviationForExternalPlotting);
}



void PlotWidget::mousePressEvent(QMouseEvent *event)
{

    if (axisRectCount() == 0) {
        QCustomPlot::mousePressEvent(event);
        return;
    }

    mState->treatMousePressEvent(event);

    QCustomPlot::mousePressEvent(event);
}

void PlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent";
    if (axisRectCount() == 0) {
        QCustomPlot::mouseReleaseEvent(event);
        return;
    }
    mState->treatMouseReleaseEvent(event);
    QCustomPlot::mouseReleaseEvent(event);
}

void PlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (axisRectCount() == 0) {
        QCustomPlot::mouseMoveEvent(event);
        return;
    }

    mState->treatMouseMoveEvent(event);

    QCustomPlot::mouseMoveEvent(event);
}

void PlotWidget::wheelEvent(QWheelEvent *event)
{
    mState->treatWheelEvent(event);

    QCustomPlot::wheelEvent(event);
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    QCustomPlot::paintEvent(event);
    mState->treatPaintEvent(event);
}

void PlotWidget::resizeEvent(QResizeEvent *event)
{
    mState->treatResizeEvent(event);
    recountLastAxisStretch();
    QCustomPlot::resizeEvent(event);
    //TODO: do it better than simple updateAppearance in the future
    updateAppearance();
}

void PlotWidget::leaveEvent(QEvent *event)
{
    mState->treatLeaveEvent(event);
    QCustomPlot::leaveEvent(event);
}

void PlotWidget::dropEvent(QDropEvent *event)
{
    mState->treatDropEvent(event);
    QCustomPlot::dropEvent(event);
//    event->acceptProposedAction();
}

void PlotWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (mState == mWidgetNormalState && qobject_cast<const GraphMimeData*>(event->mimeData())) {
        event->acceptProposedAction();
    }
    mState->treatDragEnterEvent(event);
    QCustomPlot::dragEnterEvent(event);
}

void PlotWidget::dragMoveEvent(QDragMoveEvent *event)
{
    mState->treatDragMoveEvent(event);
    QCustomPlot::dragMoveEvent(event);
}

void PlotWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    mState->treatDragLeaveEvent(event);
    QCustomPlot::dragLeaveEvent(event);
}





//void PlotWidget::addModel(const GraphModel *newModel)
//{
//    modelVector.push_back(newModel);
//    updateGraphics();
//}

//void PlotWidget::removeModel(const GraphModel *newModel)
//{
//    int index;
//    if ((index = modelVector.indexOf(newModel)) != -1) {
//        disconnect(newModel, SIGNAL(appearanceChanged()),
//                this, SLOT(updateGraphics()));
//        disconnect(newModel, SIGNAL(dataChanged()),
//                this, SLOT(updateGraphics()));
//        modelVector.remove(index);
//        updateGraphics();
//    } else {
//        sendLogMessage("Trying to delete a plot that doesn't exist");
//    }
//}


void PlotWidget::setPlotWidgetState(PlotWidget::PlotWidgetMode plotWidgetMode)
{
//    setCursor(Qt::ArrowCursor);

    mMode = plotWidgetMode;

    mState = mWidgetEmptyState;
    switch (plotWidgetMode) {
        case PlotWidgetMode::Common:
            mState = mWidgetNormalState;
            break;
        case PlotWidgetMode::ZoomX:
            mState = mWidgetXZoomState;
            break;
        case PlotWidgetMode::ZoomY:
            mState = mWidgetYZoomState;
            break;
        case PlotWidgetMode::ZoomXY:
            mState = mWidgetXYZoomState;
            break;
        case PlotWidgetMode::Measuring:
            mState = mWidgetMeasuringState;
            break;
        case PlotWidgetMode::Motion:
            mState = mWidgetMotionState;
            break;
        case PlotWidgetMode::ArrowDrawing:
            mState = mWidgetArrowDrawingState;
            break;
        case PlotWidgetMode::FastSecondaryProcessing:
            mState = mWidgetFastSecondaryProcessingState;
            break;
        case PlotWidgetMode::LevelDrawing:
            mState = mWidgetLevelDrawingState;
//        default:
//            break;
    }

    mState->initializeState();

//    if (mMotionState == State::On) {
//        setInteractions(QCP::iRangeDrag);
//    } else {
//        setInteractions(0);
//    }

//    if (mMeasuringState == State::On) {
//        updateTracers();
//    } else {
//        for (int i = 0; i < mTracerVector.size(); ++i) {
//            removeItem(mTracerVector[i]);
//            //removeItem will call delete and clear QCustomPlot internal containers, so the stuff below is supposed to be excessive
////            mTracerVector[i]->setParent(NULL);
////            delete mTracerVector[i];
//        }
//        mTracerVector.clear();
//    }

    update();
    replot();
}

void PlotWidget::updateEverything()
{
    if (mLayoutModel != NULL) {
       mUpdateEverythingCounter++;
       mTimerId = startTimer(100);
    }
}

void PlotWidget::updateAppearance()
{
    if (mLayoutModel != NULL) {
       mUpdateAppearanceCounter++;
       mTimerId = startTimer(100);
    }
}

void PlotWidget::setXRangeForAllAxis(QCPRange xRange)
{
    if (mMode == PlotWidgetMode::Motion) {
        for (int i = 0; i < axisRectCount(); ++i) {
            axisRect(i)->axis(QCPAxis::atBottom)->setRange(xRange);
        }
    }
}

void PlotWidget::updateTracers()
{
    if (mMode == PlotWidgetMode::Measuring) {
        for (auto tracer : mTracerVector) {
            removeItem(tracer);
            //removeItem will call delete and clear QCustomPlot internal containers, so the stuff below is supposed to be excessive
//            tracer->setParent(NULL);
//            delete tracer;
        }
        mTracerVector.clear();


//        for (auto graph : mGraphVector) {
//            QCPItemTracer *graphTracer = new QCPItemTracer(this);
//            addItem(graphTracer);
//            graphTracer->setGraph(graph);
//            graphTracer->setInterpolating(true);
//            graphTracer->setStyle(QCPItemTracer::tsCircle);
//            graphTracer->setPen(QPen(Qt::red));
//            graphTracer->setBrush(Qt::white);
//            graphTracer->setSize(gTracerSize);
//            graphTracer->setIterpolationType(graph->lineStyle());
//            for (int axisRectIndex = 0; axisRectIndex < axisRectCount(); ++axisRectIndex) {
//                if (axisRect(axisRectIndex)->graphs().indexOf(graph) != -1) {
//                    graphTracer->setClipAxisRect (axisRect(axisRectIndex));
//                    break;
//                }
//            }
//            mTracerVector.push_back(graphTracer);
//        }
    }
}

void PlotWidget::updateAnchors()
{

        for (auto tracer : mAnchorVector) {
            removeItem(tracer);
            //removeItem will call delete and clear QCustomPlot internal containers, so the stuff below is supposed to be excessive
//            tracer->setParent(NULL);
//            delete tracer;
        }
        mAnchorVector.clear();
        QVector<Anchor*> anchorVector = mLayoutModel->anchorVector();
        for (Anchor *anchor : anchorVector) {
            for (auto graph : mFunctionVector) {
                QCPItemTracer *graphTracer = new QCPItemTracer(this);
                addItem(graphTracer);
                graphTracer->setGraph(graph);
                graphTracer->setInterpolating(true);
                graphTracer->setStyle(anchor->style());
                graphTracer->setPen(anchor->borderPen());
                graphTracer->setBrush(anchor->brushColor());
                graphTracer->setSize(anchor->size());
                graphTracer->setIterpolationType(graph->lineStyle());
                for (int axisRectIndex = 0; axisRectIndex < axisRectCount(); ++axisRectIndex) {
                    if (axisRect(axisRectIndex)->graphs().indexOf(graph) != -1) {
                        graphTracer->setClipAxisRect (axisRect(axisRectIndex));
                        break;
                    }
                }
                graphTracer->setGraphKey(anchor->time());
                mAnchorVector.push_back(graphTracer);

                graphTracer->setLayer("tracer");
            }
        }

}

//void PlotWidget::emitPlotWidgetMovingIsEnabled(bool arg)
//{
//    emit plotWidgetMovingIsEnabled(arg);
//}


void  PlotWidget::timerEvent(QTimerEvent *) {
    if (mTimerId > 0) {
        killTimer(mTimerId);
        mTimerId = -1;
    }
    if (mUpdateEverythingCounter != 0 || mUpdateAppearanceCounter != 0) { //VERY DIRTY TRICK TO AVOID MULTIPLE REDRAWING
          //VERY DIRTY TRICK TO AVOID MULTIPLE REDRAWING
         treatUpdateRequest();
    }
}

void PlotWidget::treatUpdateRequest()
{
    if (mUpdateEverythingCounter != 0) {
        mUpdateAppearanceCounter = 0;
        mUpdateEverythingCounter = 0;
        createGraphics();
        createAppearance();
    } else if (mUpdateAppearanceCounter != 0){
        mUpdateAppearanceCounter = 0;
        mUpdateEverythingCounter = 0;
        createAppearance();
    }
}

void PlotWidget::createGraphics()
{
    //NOTE: Very dirty trick (plotLayout()->clear() deletes items from a layout, but plots without axes
    //remains somewhere in the memory and QCustomPlot tries to draw them).
    for (auto graph : mGraphVector) {
        removePlottable(graph);
    }
    numberOfPreviousPlots = 0; // not used anymore
    mGraphVector.clear();
    mFunctionVector.clear();
    mCurveVector.clear();

    QCPAxisRect *motherRect = new QCPAxisRect(this);

    auto axisModelVector = mLayoutModel->getVisibleAxisModelVector();
    for (int axisIndex = 0; axisIndex < axisModelVector.size(); ++axisIndex) {
        auto graphModelVector = axisModelVector[axisIndex]->getGraphModelVector();

        for (int modelIndex = 0; modelIndex < graphModelVector.size(); ++modelIndex) {
            auto graphModel = graphModelVector[modelIndex];
            if (graphModel->graphType() == GraphModel::GraphType::Function) {
                QCPGraph *newGraph = new QCPGraph(motherRect->axis(QCPAxis::atBottom), motherRect->axis(QCPAxis::atLeft));
                newGraph->setData(graphModelVector[modelIndex]->getXData(), graphModelVector[modelIndex]->getYData());
                addPlottable(newGraph);
                mGraphVector.push_back(newGraph);
                mFunctionVector.push_back(newGraph);
                numberOfPreviousPlots++;
                newGraph->setLayer("graph");
            } else {
                QCPCurve *newGraph = new QCPCurve(motherRect->axis(QCPAxis::atBottom), motherRect->axis(QCPAxis::atLeft));
                newGraph->setData(graphModelVector[modelIndex]->getXData(), graphModelVector[modelIndex]->getYData());
                addPlottable(newGraph);
                mGraphVector.push_back(newGraph);
                mCurveVector.push_back(newGraph);
                numberOfPreviousPlots++;
                newGraph->setLayer("graph");
            }

        }
    }
}

void addLineAndScatterStylesFromModelToGraph(const GraphModel &model, QCPGraph* graph)
{
    if (graph == nullptr)
        return;
    QCPScatterStyle scatterStyle = model.getScatterStyle();
    QCPGraph::LineStyle lineInterpolation = QCPGraph::lsNone;
    switch (model.getLineInterpolation()) {
        case GraphModel::LineStyle::lsNone: lineInterpolation = QCPGraph::lsNone;
                                            break;
        case GraphModel::LineStyle::lsLine: lineInterpolation = QCPGraph::lsLine;
                                            break;
        case GraphModel::LineStyle::lsStepLeft: lineInterpolation = QCPGraph::lsStepLeft;
                                            break;
        case GraphModel::LineStyle::lsStepRight: lineInterpolation = QCPGraph::lsStepRight;
                                        break;
        case GraphModel::LineStyle::lsStepCenter: lineInterpolation = QCPGraph::lsStepCenter;
                                            break;
        case GraphModel::LineStyle::lsImpulse: lineInterpolation = QCPGraph::lsImpulse;
                                            break;
        case GraphModel::LineStyle::lsSmooth: lineInterpolation = QCPGraph::lsLine;
           scatterStyle.setDecimation(scatterStyle.decimation()* GraphModel::SMOOTHING_POINTS_NUMBER);
                                            break;
    }
    graph->setScatterStyle(scatterStyle);
    graph->setLineStyle(lineInterpolation);
}

void addLineAndScatterStylesFromModelToGraph(const GraphModel &model, QCPCurve* graph)
{
    if (graph == nullptr)
        return;
    graph->setScatterStyle(model.getScatterStyle());
    if (model.getLineInterpolation() == GraphModel::LineStyle::lsNone)
        graph->setLineStyle(QCPCurve::lsNone);
    else
        graph->setLineStyle(QCPCurve::lsLine);
}

template <typename GraphType>
void addPropertiesFromModelToGraph(const GraphModel &model, GraphType* graph)
{
    if (graph == nullptr)
        return;

    graph->setPen(model.getPen());
    graph->setVisible(model.isVisible());

    addLineAndScatterStylesFromModelToGraph(model, graph);

//    QCPScatterStyle scatterStyle = model.getScatterStyle();
//    GraphModel::LineStyle lineInterpolation = GraphModel::LineStyle::lsNone;

//        switch (model.getLineInterpolation()) {
//            case GraphModel::LineStyle::lsNone: lineInterpolation = QCPCurve::lsNone;
//                                                break;
//            case GraphModel::LineStyle::lsLine: lineInterpolation = QCPCurve::lsLine;
//                                                break;
//            case GraphModel::LineStyle::lsStepLeft: lineInterpolation = QCPCurve::lsStepLeft;
//                                                break;
//            case GraphModel::LineStyle::lsStepCenter: lineInterpolation = QCPCurve::lsStepCenter;
//                                                break;
//        }

//        if (lineInterpolation == GraphModel::LineStyle::lsSmooth) {
//            lineInterpolation = QCPCurve::lsLine;
//            scatterStyle.setDecimation(scatterStyle.decimation() * GraphModel::SMOOTHING_POINTS_NUMBER);
//        }
//    }
//    graph->setScatterStyle(scatterStyle);
//    graph->setLineStyle(lineInterpolation);

    graph->setName(model.getName());
    graph->setBrush(model.brush());
    graph->setFillBasis(model.basisFillType());
}



void PlotWidget::createAppearance()
{
//    setBackground(QBrush(global::gDefaultPlotBackgroundColor));


    mState->setInteractions();

    mBottomRightLegendLayout = NULL;
    for (auto eventGraph : mEventsVector) {
        removePlottable(eventGraph);
    }
    mEventsVector.clear();

    for (auto textLabel : mTextLabelsVector) {
        removeItem(textLabel);
    }
    mTextLabelsVector.clear();

    for (auto arrow : mArrowsVector) {
        removeItem(arrow);
    }
    mArrowsVector.clear();

    for (auto levelGraph : mLevelsVector) {
        removePlottable(levelGraph);
    }
    mLevelsVector.clear();

    int currentGraphIndex = 0;

    plotLayout()->clear();

    if (mGraphVector.size() == 0) {
        replot();
        return;
    }


    //setting negative row spacing to make AxisRects closer
    auto legendLayout = mLayoutModel->legendLayout();
    if (legendLayout == style::LegendLocation::Right || legendLayout == style::LegendLocation::Left
            || style::isLegendFloating(legendLayout) /*== PlotLayoutModel::LegendLocation::Floating*/
            || legendLayout == style::LegendLocation::TopCombined
            || legendLayout == style::LegendLocation::BottomCombined
            || legendLayout == style::LegendLocation::TopCombinedInRow
            || legendLayout == style::LegendLocation::BottomCombinedInRow ) {
        plotLayout()->setRowSpacing(-10);
    } else {
        plotLayout()->setRowSpacing(0);
    }

    //for future
//    mGraphVector.clear();
//    mAxisRectVector.clear();
    //for future

    //to make legend closer
    plotLayout()->setColumnSpacing(-10);

    if (!mAxisLeftRightMarginGroup->isEmpty()) {
        mAxisLeftRightMarginGroup->clear();
    }

    //very dirty trick (plotLayout()->clear() deletes items from a layout, but plots without axes
    //        remains somewhere in the memory and QCustomPlot tries to draw them(WTF?)

//    for (int i = 0; i < numberOfPreviousPlots; ++i) {
//        removePlottable(0);
//    }
//    numberOfPreviousPlots = 0;


    addTitleToLayout();


    mPlotStyle = mLayoutModel->plotStyle();
    setBackground(mPlotStyle.mColorPref.mBackgroundColor);
    plotLayout()->setMargins(mPlotStyle.mMargins);
    auto axisModelVector = mLayoutModel->getVisibleAxisModelVector();
    for (int axisIndex = 0; axisIndex < axisModelVector.size(); ++axisIndex) {
//        QCPAxisRect *axisRect = mAxisRectVector[axisIndex];
        QCPAxisRect *axisRect = new QCPAxisRect(this);

        auto background = axisModelVector[axisIndex]->backgroundBrush();
        if (background.style() != Qt::NoBrush) {
            axisRect->setBackground(background);
        }

        addAxisRectToLayout(axisIndex, axisRect);

        auto graphModelVector = axisModelVector[axisIndex]->getGraphModelVector();
        for (int modelIndex = 0; modelIndex < graphModelVector.size(); ++modelIndex) {
            const auto &model = graphModelVector[modelIndex];
            if (model->graphType() == GraphModel::GraphType::Function) {
                QCPGraph *newGraph = qobject_cast<QCPGraph*>(mGraphVector[currentGraphIndex++]);
                if (!newGraph)
                    continue;
                newGraph->setKeyAxis(axisRect->axis(QCPAxis::atBottom));
                newGraph->setValueAxis(axisRect->axis(QCPAxis::atLeft));
//                newGraph->setPen(model->getPen());
//                newGraph->setVisible(model->isVisible());
//                newGraph->setScatterStyle(model->getScatterStyle());

//                newGraph->setLineStyle(model->getLineInterpolation());
//                newGraph->setName(model->getName());
//                newGraph->setBrush(model->brush());
//                newGraph->setFillBasis(model->basisFillType());

                addPropertiesFromModelToGraph(*model, newGraph);

                //DEBUG
//                {
//                static int i = 0;
//                i++;
//                if (i % 2 == 0) {
//                    shared_ptr<DummyColorMapEvaluator> ev (new DummyColorMapEvaluator(this));
//                    newGraph->enableCustomColormap(true);
//                    newGraph->setColorMapEvaluator(ev);
//                }
//                }

                if (auto colorMapEvaluator = model->colorMapEvaluator()) {
                    newGraph->enableCustomColormap(true);
                    newGraph->setColorMapEvaluator(colorMapEvaluator);
                }
                //DEBUG

            } else {
                QCPCurve *newGraph = qobject_cast<QCPCurve *>(mGraphVector[currentGraphIndex++]);
                if (!newGraph)
                    continue;
                newGraph->setKeyAxis(axisRect->axis(QCPAxis::atBottom));
                newGraph->setValueAxis(axisRect->axis(QCPAxis::atLeft));
//                newGraph->setPen(model->getPen());
//                newGraph->setVisible(model->isVisible());
//                newGraph->setScatterStyle(model->getScatterStyle());
//                if (model->getLineInterpolation() == QCPGraph::lsNone)
//                    newGraph->setLineStyle(QCPCurve::lsNone);
//                else
//                    newGraph->setLineStyle(QCPCurve::lsLine);
//                newGraph->setName(model->getName());
//                newGraph->setBrush(model->brush());
//                newGraph->setFillBasis(model->basisFillType());

                addPropertiesFromModelToGraph(*model, newGraph);
            }
        }


        auto xRange = axisModelVector[axisIndex]->getXRange();
        auto yRange = axisModelVector[axisIndex]->getYRange();
        axisRect->axis(QCPAxis::atBottom)->setRange(std::get<0>(xRange), std::get<1>(xRange));
        axisRect->axis(QCPAxis::atTop   )->setRange(std::get<0>(xRange), std::get<1>(xRange));
        axisRect->axis(QCPAxis::atLeft  )->setRange(std::get<0>(yRange), std::get<1>(yRange));
        axisRect->axis(QCPAxis::atRight )->setRange(std::get<0>(yRange), std::get<1>(yRange));

        axisRect->axis(QCPAxis::atRight)->setVisible(true);
        axisRect->axis(QCPAxis::atTop)->setVisible(true);
        axisRect->axis(QCPAxis::atLeft)->setVisible(true);
        axisRect->axis(QCPAxis::atBottom)->setVisible(true);

        axisRect->setMarginGroup(QCP::msLeft|QCP::msRight, mAxisLeftRightMarginGroup);

        //setting fonts for axis
        axisRect->axis(QCPAxis::atLeft)->setTickLabelFont(mPlotStyle.mYTickLabelProperties.mFont);
        axisRect->axis(QCPAxis::atBottom)->setTickLabelFont(mPlotStyle.mXTickLabelProperties.mFont);

        //setting tickEvaluationPolicy
        axisRect->axis(QCPAxis::atLeft  )->setTickEvaluationPolicy(global::gYTickEvaluationPolicy);
        axisRect->axis(QCPAxis::atRight )->setTickEvaluationPolicy(global::gYTickEvaluationPolicy);
        axisRect->axis(QCPAxis::atBottom)->setTickEvaluationPolicy(global::gXTickEvaluationPolicy);
        axisRect->axis(QCPAxis::atTop   )->setTickEvaluationPolicy(global::gXTickEvaluationPolicy);

//        axisRect->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(true);
//        axisRect->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(true);
//        QPen subGridPen =axisRect->axis(QCPAxis::atLeft)->grid()->subGridPen ();
//        subGridPen.setWidthF(global::gSubGridPenWidth);
//        subGridPen.setColor(global::gDefaultSubGridColor);
//        subGridPen.setStyle(Qt::DashDotLine);
//        axisRect->axis(QCPAxis::atLeft)->grid()->setSubGridPen(subGridPen);
//        axisRect->axis(QCPAxis::atBottom)->grid()->setSubGridPen(subGridPen);


        //to enable subGrid with non integer width
        axisRect->axis(QCPAxis::atLeft  )->grid()->setAntialiasedSubGrid(true);
        axisRect->axis(QCPAxis::atBottom)->grid()->setAntialiasedSubGrid(true);
        axisRect->axis(QCPAxis::atLeft  )->grid()->setSubGridPen(mLayoutModel->ySubGrid().mPen);
        axisRect->axis(QCPAxis::atBottom)->grid()->setSubGridPen(mLayoutModel->xSubGrid().mPen);
        axisRect->axis(QCPAxis::atLeft  )->grid()->setSubGridVisible(mLayoutModel->ySubGrid().mIsVisible);
        axisRect->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(mLayoutModel->xSubGrid().mIsVisible);

//        qDebug() << "mLayoutModel->xSubGrid().mPen" << mLayoutModel->xSubGrid().mPen.widthF();

//        QPen gridPen =axisRect->axis(QCPAxis::atLeft)->grid()->pen ();
//        gridPen.setWidthF(global::gGridPenWidth);
//        gridPen.setColor(global::gDefaultGridColor);
//        gridPen.setStyle(Qt::SolidLine);
//        axisRect->axis(QCPAxis::atLeft)->grid()->setPen(gridPen);
//        axisRect->axis(QCPAxis::atBottom)->grid()->setPen(gridPen);
//        axisRect->axis(QCPAxis::atBottom)->grid()->setPen(gridPen);
//        axisRect->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(gridPen);
//        axisRect->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(gridPen);

        QPen gridPen = mLayoutModel->yGrid().mPen; //temporary
        axisRect->axis(QCPAxis::atLeft  )->grid()->setAntialiased(true);
        axisRect->axis(QCPAxis::atBottom)->grid()->setAntialiased(true);
        axisRect->axis(QCPAxis::atLeft  )->grid()->setPen(mLayoutModel->yGrid().mPen);
        axisRect->axis(QCPAxis::atBottom)->grid()->setPen(mLayoutModel->xGrid().mPen);
        axisRect->axis(QCPAxis::atLeft  )->grid()->setVisible(mLayoutModel->yGrid().mIsVisible);
        axisRect->axis(QCPAxis::atBottom)->grid()->setVisible(mLayoutModel->xGrid().mIsVisible);
        axisRect->axis(QCPAxis::atLeft  )->grid()->setZeroLinePen(mLayoutModel->yGrid().mPen);
        axisRect->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(mLayoutModel->xGrid().mPen);




        //tick counting
        QFontMetrics xLabelFontMetrics(mLayoutModel->xAxisFont());
        int maxTickLabelWidth = xLabelFontMetrics.width("0") * (global::gDoubleToStringConversionPrecision+3);
        int tickXCount = (width() - mLayoutModel->getMaximumDimensionWidth() - mLayoutModel->getMaximumLegendWidth())
                / (maxTickLabelWidth*1.3*global::gXTickSpacing);
        axisRect->axis(QCPAxis::atBottom)->setAutoTickCount(tickXCount);
        axisRect->axis(QCPAxis::atTop)->setAutoTickCount(tickXCount);

        //not optimal way to count it in each iteration, but it is easier to trace
        double sumStretchFactor = 0;
        foreach (auto axisModel, axisModelVector) {
            sumStretchFactor += axisModel->stretchFactor();
        }
        QFontMetrics yLabelFontMetrics(mLayoutModel->yAxisFont());
        int maxTickLabelHeight = yLabelFontMetrics.height();
        int tickYCount = (height() - mLayoutModel->getMaximumTitleHeight() - mLayoutModel->getMaximumXLabelHeight())
                * (axisModelVector[axisIndex]->stretchFactor())/sumStretchFactor
                / (maxTickLabelHeight*2.5*global::gYTickSpacing);
        axisRect->axis(QCPAxis::atLeft)->setAutoTickCount(std::max(tickYCount, 2));
        axisRect->axis(QCPAxis::atRight)->setAutoTickCount(std::max(tickYCount, 2));
//        axisRect->axis(QCPAxis::atLeft)->setAutoTickCount(
//                            std::max(int(height()*(axisModelVector[axisIndex]->stretchFactor())/sumStretchFactor/50), 2)
//                                                          );

        axisRect->axis(QCPAxis::atBottom)->setAutoSubTicks(false);
        axisRect->axis(QCPAxis::atBottom)->setSubTickCount(1);
        axisRect->axis(QCPAxis::atTop)->setAutoSubTicks(false);
        axisRect->axis(QCPAxis::atTop)->setSubTickCount(1);

        axisRect->axis(QCPAxis::atLeft)->setAutoSubTicks(false);
        axisRect->axis(QCPAxis::atLeft)->setSubTickCount(1);
        axisRect->axis(QCPAxis::atRight)->setAutoSubTicks(false);
        axisRect->axis(QCPAxis::atRight)->setSubTickCount(1);

        if (axisIndex != axisModelVector.size() - 1
                && mPlotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled == false) {
            axisRect->axis(QCPAxis::atBottom)->setTickLabels(false);
            //axisRect->axis(QCPAxis::atBottom)->setTickLabelType(mLayoutModel->timeAxisType());
        }

        axisRect->axis(QCPAxis::atBottom)->setTickLabelType(mLayoutModel->timeAxisType());

        //ticks

//        auto axisPen = axisRect->axis(QCPAxis::atLeft)->basePen();
//        axisPen.setWidthF(global::gAxisPenWidth);
//        axisPen.setColor(mPreferences.mAxisRectProperties.mPen);

//        axisRect->axis(QCPAxis::atLeft)->setBasePen(axisPen);
//        axisRect->axis(QCPAxis::atRight)->setBasePen(gridPen);
//        axisRect->axis(QCPAxis::atBottom)->setBasePen(gridPen);
//        axisRect->axis(QCPAxis::atTop)->setBasePen(gridPen);
//        if (axisIndex == axisModelVector.size() - 1) {
//            axisRect->axis(QCPAxis::atBottom)->setBasePen(axisPen);
//        }

        style::AxisRect axisRectPref = mLayoutModel->axisRect();
        axisRect->axis(QCPAxis::atLeft  )->setBasePen(axisRectPref.mPositions.testFlag(utility::posLeft  )? axisRectPref.mPen : Qt::NoPen);
        axisRect->axis(QCPAxis::atRight )->setBasePen(axisRectPref.mPositions.testFlag(utility::posRight )? axisRectPref.mPen : Qt::NoPen);
        axisRect->axis(QCPAxis::atBottom)->setBasePen(axisRectPref.mPositions.testFlag(utility::posBottom)? axisRectPref.mPen : Qt::NoPen);
        axisRect->axis(QCPAxis::atTop   )->setBasePen(axisRectPref.mPositions.testFlag(utility::posTop   )? axisRectPref.mPen : Qt::NoPen);


        axisRect->enableXAxisDrawingAtCenter(mPlotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled);
        QCPLineEnding xAxisEnding(mPlotStyle.mXAxisPref.mAxisLineEnding);
        xAxisEnding.setWidth(8*mPlotStyle.mAxisRectProperties.mPen.widthF());
        xAxisEnding.setLength(10*mPlotStyle.mAxisRectProperties.mPen.widthF());
        axisRect->axis(QCPAxis::atBottom)->setUpperEnding(xAxisEnding);

        axisRect->enableYAxisDrawingAtCenter(mPlotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled);
        QCPLineEnding yAxisEnding(mPlotStyle.mYAxisPref.mAxisLineEnding);
        yAxisEnding.setWidth(8*mPlotStyle.mAxisRectProperties.mPen.widthF());
        yAxisEnding.setLength(10*mPlotStyle.mAxisRectProperties.mPen.widthF());
        axisRect->axis(QCPAxis::atLeft)->setUpperEnding(yAxisEnding);










//        axisRect->axis(QCPAxis::atLeft)->setTickPen(axisPen);
//        axisRect->axis(QCPAxis::atRight)->setTickPen(axisPen);
//        axisRect->axis(QCPAxis::atBottom)->setTickPen(gridPen);
//        axisRect->axis(QCPAxis::atTop)->setTickPen(axisPen);
//        axisRect->axis(QCPAxis::atLeft)->setSubTickPen(axisPen);
//        axisRect->axis(QCPAxis::atRight)->setSubTickPen(axisPen);
//        axisRect->axis(QCPAxis::atBottom)->setSubTickPen(gridPen);
//        axisRect->axis(QCPAxis::atTop)->setSubTickPen(axisPen);
//        if (axisIndex == axisModelVector.size() - 1) {

//            axisRect->axis(QCPAxis::atBottom)->setTickPen(axisPen);
//            axisRect->axis(QCPAxis::atBottom)->setSubTickPen(axisPen);
//            axisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(mLayoutModel->xTickRotation());
//        }





        axisRect->axis(QCPAxis::atRight)->setTickLabels(false);
        axisRect->axis(QCPAxis::atTop)->setTickLabels(false);


        axisRect->axis(QCPAxis::atTop)->setTicks(true);
        axisRect->axis(QCPAxis::atRight)->setTicks(true);
        axisRect->axis(QCPAxis::atBottom)->setTicks(true);
        axisRect->axis(QCPAxis::atLeft)->setTicks(true);

        //tick
//        auto setTick = [](QCPAxis* axis, const style::MainTick &tickPref) {
//            axis->setTickPen      (tickPref.mIsVisible ? tickPref.mPen : Qt::NoPen);
//            axis->setTickLengthIn (tickPref.mLengthIn);
//            axis->setTickLengthOut(tickPref.mLengthOut);
//            axis->setSubTickCount (tickPref.mSubTickCount);
//        };
//        setTick(axisRect->axis(QCPAxis::atLeft  ), mLayoutModel->leftTick());
//        setTick(axisRect->axis(QCPAxis::atTop   ), mLayoutModel->topTick());
//        setTick(axisRect->axis(QCPAxis::atRight ), mLayoutModel->rightTick());
//        setTick(axisRect->axis(QCPAxis::atBottom), mLayoutModel->bottomTick());

//        //subtick
//        auto setSubTick = [](QCPAxis* axis, const style::Tick &tickPref) {
//            axis->setSubTickPen      (tickPref.mIsVisible ? tickPref.mPen : Qt::NoPen);
//            axis->setSubTickLengthIn (tickPref.mLengthIn);
//            axis->setSubTickLengthOut(tickPref.mLengthOut);
//        };
//        setSubTick(axisRect->axis(QCPAxis::atLeft  ), mLayoutModel->leftSubTick());
//        setSubTick(axisRect->axis(QCPAxis::atTop   ), mLayoutModel->topSubTick());
//        setSubTick(axisRect->axis(QCPAxis::atRight ), mLayoutModel->rightSubTick());
//        setSubTick(axisRect->axis(QCPAxis::atBottom), mLayoutModel->bottomSubTick());


        //tick
        int axisNumber = axisModelVector.size();
        auto setTick = [axisIndex, axisNumber](QCPAxis* axis, const style::MainTick &tickPref, utility::Position pos) {
            bool drawTick = tickPref.mPositions.testFlag(pos);
            if (!tickPref.mTicksOnInternalAxisRectEnabled) {
                switch (pos) {
                    case utility::posTop:
                        if (axisIndex != 0)
                            drawTick = false;
                        break;
                    case utility::posBottom:
                        if (axisIndex != axisNumber - 1)
                            drawTick = false;
                        break;
                    default:
                        break;
                }
            }
            axis->setTickPen      (drawTick ? tickPref.mPen : Qt::NoPen);
            axis->setTickLengthIn (tickPref.mLengthIn);
            axis->setTickLengthOut(tickPref.mLengthOut);
            axis->setSubTickCount (tickPref.mSubTickCount);
            axis->enableEdgeTicksModification(  (axis->axisType() == QCPAxis::atTop || axis->axisType() == QCPAxis::atBottom)
                                                ? global::gIsEdgeXTicksModificationEnabled
                                                : global::gIsEdgeYTicksModificationEnabled);
        };
        setTick(axisRect->axis(QCPAxis::atLeft  ), mLayoutModel->leftTick()  , utility::posLeft);
        setTick(axisRect->axis(QCPAxis::atTop   ), mLayoutModel->topTick()   , utility::posTop);
        setTick(axisRect->axis(QCPAxis::atRight ), mLayoutModel->rightTick() , utility::posRight);
        setTick(axisRect->axis(QCPAxis::atBottom), mLayoutModel->bottomTick(), utility::posBottom);

        //subtick
        auto setSubTick = [axisIndex, axisNumber](QCPAxis* axis, const style::Tick &tickPref, utility::Position pos) {
            bool drawTick = tickPref.mPositions.testFlag(pos);
            if (!tickPref.mTicksOnInternalAxisRectEnabled) {
                switch (pos) {
                    case utility::posTop:
                        if (axisIndex != 0)
                            drawTick = false;
                        break;
                    case utility::posBottom:
                        if (axisIndex != axisNumber - 1)
                            drawTick = false;
                        break;
                    default:
                        break;
                }
            }
            axis->setSubTickPen      (drawTick ? tickPref.mPen : Qt::NoPen);
            axis->setSubTickLengthIn (tickPref.mLengthIn);
            axis->setSubTickLengthOut(tickPref.mLengthOut);
        };
        setSubTick(axisRect->axis(QCPAxis::atLeft  ), mLayoutModel->leftSubTick()  , utility::posLeft);
        setSubTick(axisRect->axis(QCPAxis::atTop   ), mLayoutModel->topSubTick()   , utility::posTop);
        setSubTick(axisRect->axis(QCPAxis::atRight ), mLayoutModel->rightSubTick() , utility::posRight);
        setSubTick(axisRect->axis(QCPAxis::atBottom), mLayoutModel->bottomSubTick(), utility::posBottom);

        //double to string conversions precision setting for tick labels
        axisRect->axis(QCPAxis::atLeft)->setNumberPrecision(global::gDoubleToStringConversionPrecision);
        axisRect->axis(QCPAxis::atBottom)->setNumberPrecision(global::gDoubleToStringConversionPrecision);

        if (axisModelVector[axisIndex]->scaleType() == AxisModel::ScaleType::Logarithmic) {
            axisRect->axis(QCPAxis::atLeft )->setScaleType(QCPAxis::stLogarithmic);
            axisRect->axis(QCPAxis::atRight)->setScaleType(QCPAxis::stLogarithmic);
        } else {
            axisRect->axis(QCPAxis::atLeft )->setScaleType(QCPAxis::stLinear);
            axisRect->axis(QCPAxis::atRight)->setScaleType(QCPAxis::stLinear);
        }
        if (mLayoutModel->scaleType() == PlotLayoutModel::ScaleType::Logarithmic) {
            axisRect->axis(QCPAxis::atBottom)->setScaleType(QCPAxis::stLogarithmic);
            axisRect->axis(QCPAxis::atTop   )->setScaleType(QCPAxis::stLogarithmic);
        } else {
            axisRect->axis(QCPAxis::atBottom)->setScaleType(QCPAxis::stLinear);
            axisRect->axis(QCPAxis::atTop   )->setScaleType(QCPAxis::stLinear);
        }

        // add extra space at right so that the most right tick label will be visible
        if (legendLayout != style::LegendLocation::Right || !mLayoutModel->isLegendVisible()) {
            auto xMax = std::get<1>(xRange);
            QFontMetrics fontMetrics(mLayoutModel->xAxisFont());
            int padding = fontMetrics.width(QString::number(xMax, 'g', global::gDoubleToStringConversionPrecision)) / 2 + 5;
            padding -= mLayoutModel->getMaximumHorizontalLevelWidth(mLayoutModel->commandFont());
            if (padding > 0)
                axisRect->axis(QCPAxis::atRight)->setPadding(padding);
        }

        //a piece of magic
        if (legendLayout == style::LegendLocation::Right || legendLayout== style::LegendLocation::Left
                || style::isLegendFloating(legendLayout)/* == PlotLayoutModel::LegendLocation::Floating*/
                || legendLayout == style::LegendLocation::TopCombined
                || legendLayout == style::LegendLocation::BottomCombined
                || legendLayout == style::LegendLocation::TopCombinedInRow
                || legendLayout == style::LegendLocation::BottomCombinedInRow) {
            axisRect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
        } else {
            axisRect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom|QCP::msTop);
        }

        double axisRectGap = mPlotStyle.mAxisRectProperties.mGap;
        if (axisIndex == 0) {
//            double gapForTopYLabel = mLayoutModel->yAxisFont().pointSize()/2;
            double gapForTopYLabel = QFontMetrics(mLayoutModel->yAxisFont()).ascent()/2;
            if (mLayoutModel->areEventsVisible()) {
//                axisRect->setMargins(QMargins(0, 10 + mLayoutModel->commandFont().pointSize(), 0, 0));
                double verticalGap = 0;
                if (mLayoutModel->commandLabelRotation() != 0.) {
                    verticalGap +=  mLayoutModel->getMaximumCommandWidth()*abs(sin(mLayoutModel->commandLabelRotation()/57.3));
                }
                if (mLayoutModel->title() == "") {
                    axisRect->setMargins(QMargins(0, 5 + std::max(5 + mLayoutModel->commandFont().pointSize() + verticalGap, gapForTopYLabel) , 0, 0));
                } else {
                    axisRect->setMargins(QMargins(0, 5 + verticalGap , 0, 0));
                }
            } else {
//                axisRect->setMargins(QMargins(0, 5, 0, 0));
                axisRect->setMargins(QMargins(0, 5 + gapForTopYLabel , 0, 0)); //to ensure top Y axis label visibility
            }
        } else {
            axisRect->setMargins(QMargins(0, axisRectGap, 0, 0));
        }

        //setting devaitions for y edge labels
        if (global::gIsEdgeYTicksModificationEnabled) {
            if (axisModelVector.size() == 1) {
                axisRect->axis(QCPAxis::atLeft)->setLabelDeviationForLeftAxis(QCPAxis::dontMove);
            } else if (axisIndex == 0) {
                axisRect->axis(QCPAxis::atLeft)->setLabelDeviationForLeftAxis(QCPAxis::moveOnlyBottom);
            } else if (axisIndex == axisModelVector.size() - 1) {
                axisRect->axis(QCPAxis::atLeft)->setLabelDeviationForLeftAxis(QCPAxis::moveOnlyTop);
            } else {
                axisRect->axis(QCPAxis::atLeft)->setLabelDeviationForLeftAxis(QCPAxis::moveTopAndBotton);
            }
        } else {
            //don't move labels if we can't guarantee edge ticks
            axisRect->axis(QCPAxis::atLeft)->setLabelDeviationForLeftAxis(QCPAxis::dontMove);
        }

        //xLabel
        if (axisIndex == axisModelVector.size() - 1 && mLayoutModel->xLabel() != "") {
            axisRect->axis(QCPAxis::atBottom)->setLabel(mLayoutModel->xLabel());
            axisRect->axis(QCPAxis::atBottom)->setLabelFont(mPlotStyle.mXLabelProperties.mFont);
            axisRect->axis(QCPAxis::atBottom)->setLabelColor(mPlotStyle.mXLabelProperties.mColor);
        }



        //yLabel + units
        QString yLabel = axisModelVector[axisIndex]->yAxisLabel();
        if (mLayoutModel->areUnitsVisible()) {
            yLabel += axisModelVector[axisIndex]->getUnitsString();
        }
        if (yLabel.size() != 0) {
            axisRect->axis(QCPAxis::atLeft)->setLabel(yLabel);
            axisRect->axis(QCPAxis::atLeft)->setLabelFont(mPlotStyle.mYLabelProperties.mFont);
            axisRect->axis(QCPAxis::atLeft)->setLabelColor(mPlotStyle.mYLabelProperties.mColor);
         }

        //setting invisible ticks for commands
        if (mLayoutModel->areEventsVisible()) {
            if (axisIndex == 0) {
                QPen invPen(QColor(250,250,250,0));
                axisRect->axis(QCPAxis::atTop)->setTickPen(invPen);
            }
        }


        //arrowedText
        auto arrowedTextVector = axisModelVector[axisIndex]->arrowedTextVector();
        for (auto arrowedText : arrowedTextVector) {
            QCPItemText *textLabel = new QCPItemText(this);
            addItem(textLabel);
            textLabel->setClipToAxisRect (true);
            textLabel->setClipAxisRect (axisRect);
            textLabel->position->setAxes(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
            textLabel->setPositionAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

            textLabel->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel->position->setCoords(arrowedText->textCenter());
            textLabel->setText(arrowedText->text());
            textLabel->setFont(mLayoutModel->arrowedTextFont());
            textLabel->setColor(mPlotStyle.mColorPref.mFontColor);

//            QPen arrowPen(global::gDefaultPlotGraphicalPrimitivesColor);
//            arrowPen.setWidthF(arrowedText->lineWidthF());
//            textLabel->setPen(arrowPen);

            textLabel->setPen(arrowedText->pen());
            textLabel->setColor(mPlotStyle.mArrowedTextProperties.mLabelPrefs.mColor);

            textLabel->setSelectable(false);
            QMargins margins(arrowedText->pen().widthF() + 1, arrowedText->pen().widthF() + 1,
                             arrowedText->pen().widthF() + 1, arrowedText->pen().widthF() + 1);
            textLabel->setPadding(margins);
            textLabel->setBrush(arrowedText->backgroundColor());
            textLabel->setLayer("arrowed_text");

            QCPItemLine *arrow = new QCPItemLine(this);
            addItem(arrow);
            arrow->setClipToAxisRect (true);
            arrow->setClipAxisRect (axisRect);
            arrow->start->setAxes(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
            arrow->end->setAxes(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
            arrow->start->setType(QCPItemPosition::ptPlotCoords);
            arrow->end->setType(QCPItemPosition::ptPlotCoords);
            arrow->end->setCoords(arrowedText->arrowFinish());
            arrow->setHead(arrowedText->arrowStyle());
            arrow->setPen(arrowedText->pen());
            arrow->setLayer("arrowed_text");

            switch (arrowedText->arrowBasisInRanges(axisRect->axis(QCPAxis::atBottom)->range().size(), axisRect->axis(QCPAxis::atLeft)->range().size())) {
                case ArrowedText::ArrowBasis::Right:
                    arrow->start->setParentAnchor(textLabel->right);
                    break;
                case ArrowedText::ArrowBasis::Left:
                    arrow->start->setParentAnchor(textLabel->left);
                    break;
                case ArrowedText::ArrowBasis::Top:
                    arrow->start->setParentAnchor(textLabel->top);
                    break;
                case ArrowedText::ArrowBasis::Bottom:
                    arrow->start->setParentAnchor(textLabel->bottom);
                    break;
                default:
                    arrow->start->setParentAnchor(textLabel->bottom);
                    break;
            };
            arrow->setSelectable(false);

            mTextLabelsVector.push_back(textLabel);
            mArrowsVector.push_back(arrow);
        }


        //events
        if (mLayoutModel->areEventsVisible()) {
            if (axisIndex == 0) {
                 axisRect->axis(QCPAxis::atTop)->setTickLabels(true);
                 axisRect->axis(QCPAxis::atTop)->setTicks(true);
                 axisRect->axis(QCPAxis::atTop)->setAutoSubTicks(false);
                 axisRect->axis(QCPAxis::atTop)->setSubTickCount(0);
//                 axisRect->axis(QCPAxis::atTop)->setTickLabelPadding(-3);
            }/* else {
                axisRect->axis(QCPAxis::atTop)->setTicks(false);
            }*/

            QCPRange xRange = axisRect->axis(QCPAxis::atBottom)->range();
            QCPRange yRange = axisRect->axis(QCPAxis::atLeft)->range();
            double xRangeMin = xRange.lower;
            double xRangeMax = xRange.upper;
            double yCommandMin = yRange.lower;
            double yCommandMax = yRange.upper;

            double dxData = xRangeMax - xRangeMin;
            double commandDx = dxData / 10000;

            QVector<TimeEvent*> comVector = mLayoutModel->commandsVector();
            QVector<double> commandTickVector;
            QVector<QString> commandTickLabelsVector;
            for (TimeEvent* command : comVector) {
                QCPGraph *newEventGraph = new QCPGraph(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
                newEventGraph->setKeyAxis(axisRect->axis(QCPAxis::atBottom));
                newEventGraph->setValueAxis(axisRect->axis(QCPAxis::atLeft));

//                newEventGraph->setData(QVector<double>({std::min(command->commandTime() - dxData, xRangeMin), command->commandTime(), command->commandTime() + commandDx, std::max(command->commandTime() + dxData, xRangeMax)}),
//                                         QVector<double>({yCommandMin - 3*(yCommandMax - yCommandMin), yCommandMin, yCommandMax, yCommandMax + 3*(yCommandMax - yCommandMin)}));

                newEventGraph->setData(QVector<double>({std::max(std::min(command->eventTime() - dxData, xRangeMin), command->eventTime() - command->brushDuration()),
                                                          command->eventTime(),
                                                          command->eventTime() + commandDx,
                                                          std::min(std::max(command->eventTime() + dxData, xRangeMax), command->eventTime() + command->brushDuration())}),
                                         QVector<double>({yCommandMin - 3*(yCommandMax - yCommandMin), yCommandMin - 3*(yCommandMax - yCommandMin), yCommandMax + 3*(yCommandMax - yCommandMin), yCommandMax + 3*(yCommandMax - yCommandMin)}));

                newEventGraph->setPen(command->pen());

                newEventGraph->setBrush(command->brush());
                if (command->brushBasis() == TimeEvent::Left) {
                    newEventGraph->setFillBasis(QCPGraph::HighBasis);
                } else {
                    newEventGraph->setFillBasis(QCPGraph::LowBasis);
                }

                addPlottable(newEventGraph);
                mEventsVector.push_back(newEventGraph);

                double commandLabelDeviation = command->labelDeviation()*(xRangeMax-xRangeMin) /
                        (width() - mLayoutModel->getMaximumLegendWidth() - mLayoutModel->getMaximumDimensionWidth());
                commandTickVector << (command->eventTime() + commandLabelDeviation);
                commandTickLabelsVector << command->name();
            }
            if (axisIndex == 0) {
//                axisRect->axis(QCPAxis::atTop)->setRange(std::get<0>(xRange), std::get<1>(xRange));
                axisRect->axis(QCPAxis::atTop)->setRange(xRangeMin, xRangeMax);
                axisRect->axis(QCPAxis::atTop)->setAutoTicks(false);
                axisRect->axis(QCPAxis::atTop)->setAutoTickLabels(false);
                axisRect->axis(QCPAxis::atTop)->setTickVector(commandTickVector);
                axisRect->axis(QCPAxis::atTop)->setTickVectorLabels(commandTickLabelsVector);
                axisRect->axis(QCPAxis::atTop)->setTickLabelFont(mLayoutModel->commandFont());
                axisRect->axis(QCPAxis::atTop)->setTickLabelRotation(mLayoutModel->commandLabelRotation());

            }
        }

        //levels
        if (axisModelVector[axisIndex]->areHorizontalLevelsVisible()) {
            axisRect->axis(QCPAxis::atRight)->setTickLabels(true);
            axisRect->axis(QCPAxis::atRight)->setTicks(true);
            axisRect->axis(QCPAxis::atRight)->setAutoSubTicks(false);
            axisRect->axis(QCPAxis::atRight)->setSubTickCount(0);

            QVector<double> levelTickVector;
            QVector<QString> levelTickLabelsVector;

            QCPRange xRange = axisRect->axis(QCPAxis::atBottom)->range();
            double dxData = xRange.upper - xRange.lower;

            QVector<HorizontalLevel*> levelVector = axisModelVector[axisIndex]->visibleHorizontalLevelVector();
            for (HorizontalLevel* level : levelVector) {

                QCPGraph *newLevelGraph = new QCPGraph(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));
                newLevelGraph->setKeyAxis(axisRect->axis(QCPAxis::atBottom));
                newLevelGraph->setValueAxis(axisRect->axis(QCPAxis::atLeft));

                newLevelGraph->setData(QVector<double>({xRange.lower - 2.*dxData, xRange.upper + 2.*dxData}),
                                         QVector<double>({level->level(), level->level()}));

//                QPen pen = mPreferences.mYGridProperties.mPen;
//                pen.setWidthF(global::gEventLineWidth);
//                pen.setStyle(Qt::DashDotLine);
                newLevelGraph->setPen(mPlotStyle.mHorLevelProperties.mPen);

                addPlottable(newLevelGraph);
                mLevelsVector.push_back(newLevelGraph);

                levelTickVector << level->level();
                levelTickLabelsVector << level->label();
            }

            axisRect->axis(QCPAxis::atRight)->setRange(axisRect->axis(QCPAxis::atLeft)->range());
            axisRect->axis(QCPAxis::atRight)->setAutoTicks(false);
            axisRect->axis(QCPAxis::atRight)->setAutoTickLabels(false);
            axisRect->axis(QCPAxis::atRight)->setTickVector(levelTickVector);
            axisRect->axis(QCPAxis::atRight)->setTickVectorLabels(levelTickLabelsVector);
            axisRect->axis(QCPAxis::atRight)->setTickLabelFont(mPlotStyle.mHorLevelProperties.mLabel.mFont);
            axisRect->axis(QCPAxis::atRight)->setTickLabelColor(mPlotStyle.mHorLevelProperties.mLabel.mColor);
            //axisRect->axis(QCPAxis::atRight)->setTickLabelRotation(mLayoutModel->commandLabelRotation()); //TODO: add normal treatment
        }



    }

    for (int i = 0; i < axisRectCount(); ++i) {
//        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(global::gDefaultPlotFontColor);
//        axisRect(i)->axis(QCPAxis::atLeft)->setTickLabelColor(global::gDefaultPlotFontColor);
//        axisRect(i)->axis(QCPAxis::atTop)->setTickLabelColor(global::gDefaultPlotFontColor);
//        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(global::gDefaultPlotFontColor);

//        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(mPlotStyle.mColorPref.mFontColor);
//        axisRect(i)->axis(QCPAxis::atLeft  )->setTickLabelColor(mPlotStyle.mColorPref.mFontColor);
//        axisRect(i)->axis(QCPAxis::atTop   )->setTickLabelColor(mPlotStyle.mColorPref.mFontColor);
//        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(mPlotStyle.mColorPref.mFontColor);

        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(mPlotStyle.mXTickLabelProperties.mColor);
        axisRect(i)->axis(QCPAxis::atLeft  )->setTickLabelColor(mPlotStyle.mYTickLabelProperties.mColor);
        axisRect(i)->axis(QCPAxis::atTop   )->setTickLabelColor(mPlotStyle.mXTickLabelProperties.mColor);
        axisRect(i)->axis(QCPAxis::atBottom)->setTickLabelColor(mPlotStyle.mYTickLabelProperties.mColor);
    }


    for (int i = 0; i < axisRectCount(); ++i) {
        connect(axisRect(i)->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)),
                this, SLOT(setXRangeForAllAxis(QCPRange)));
    }


    //NOTE: should be called after graph properties(brush, pen..) are set
    addLegendToLayout();

    recountLastAxisStretch();

    updateTracers();
    updateAnchors();

    replot();



}

int PlotWidget::getIndexOfAxisRectByPoint(const QPoint &point)
{
    int x_coord = point.x();
    int y_coord = point.y();
    for (int axisRectIndex = 0; axisRectIndex < axisRectCount(); ++axisRectIndex) {
        QRect axisRec = axisRect(axisRectIndex)->rect();
        if (((x_coord >= axisRec.x()) && (x_coord <= axisRec.x()+ axisRec.width()))
            && ((y_coord >= axisRec.y()) && (y_coord <= axisRec.y()+ axisRec.height()))) {
                return axisRectIndex;
        }
    }
    return -1;
}

void PlotWidget::increaseWidthOfObjects(double widDev, double lineWidthDev)
{
    for (int axisIndex = 0; axisIndex < axisRectCount(); ++axisIndex) {
        QPen subGridPen =axisRect(axisIndex)->axis(QCPAxis::atLeft)->grid()->subGridPen ();
        subGridPen.setWidthF(subGridPen.widthF() + widDev);
        axisRect(axisIndex)->axis(QCPAxis::atLeft)->grid()->setSubGridPen(subGridPen);
        axisRect(axisIndex)->axis(QCPAxis::atBottom)->grid()->setSubGridPen(subGridPen);

        QPen gridPen =axisRect(axisIndex)->axis(QCPAxis::atLeft)->grid()->pen ();
        gridPen.setWidthF(gridPen.widthF() + widDev);
        axisRect(axisIndex)->axis(QCPAxis::atLeft)->grid()->setPen(gridPen);
        axisRect(axisIndex)->axis(QCPAxis::atBottom)->grid()->setPen(gridPen);

        auto axisPen = axisRect(axisIndex)->axis(QCPAxis::atLeft)->basePen();
        axisPen.setWidthF(axisPen.widthF() + widDev);
        axisRect(axisIndex)->axis(QCPAxis::atLeft)->setBasePen(axisPen);
        axisRect(axisIndex)->axis(QCPAxis::atRight)->setBasePen(gridPen);
        axisRect(axisIndex)->axis(QCPAxis::atBottom)->setBasePen(gridPen);
        axisRect(axisIndex)->axis(QCPAxis::atTop)->setBasePen(gridPen);
        if (axisIndex == axisRectCount() - 1) {
            axisRect(axisIndex)->axis(QCPAxis::atBottom)->setBasePen(axisPen);
        }
    }

    for (auto graph : mGraphVector) {
        auto graphPen = graph->pen();
        graphPen.setWidthF(graphPen.widthF() + lineWidthDev);
        graph->setPen(graphPen);
    }

    for (auto command : mEventsVector) {
        auto commandPen = command->pen();
        commandPen.setWidthF(commandPen.widthF() + lineWidthDev);
        command->setPen(commandPen);
    }


}

void PlotWidget::recountLastAxisStretch()
{
    //TODO: consider all possible cases of title and axis label position
    auto legendLayout = mLayoutModel->legendLayout();
    if (legendLayout == style::LegendLocation::Right
            || legendLayout == style::LegendLocation::Left
            || style::isLegendFloating(legendLayout) /*== PlotLayoutModel::LegendLocation::Floating*/) {

        auto axisModelVector = mLayoutModel->getVisibleAxisModelVector();
        if (axisModelVector.size() == 0)
            return;
        if (axisModelVector.size() == 1)
            return;

        QVector<double> stretchFactors;
        double sumStretch = 0;
        for (const auto & axis : axisModelVector) {
            stretchFactors.push_back(axis->stretchFactor());
            sumStretch += stretchFactors.back();
        }

        double bottomStuffSize = QFontMetrics(mLayoutModel->xAxisFont()).height();
        if (mLayoutModel->xLabel() != "") {
            bottomStuffSize += 10 + QFontMetrics(mLayoutModel->xLabelFont()).height();
        }

        double titleStuffSize = 0.;
        if (mLayoutModel->title() != "") {
            titleStuffSize += QFontMetrics(mLayoutModel->titleProperties().mFont).height();
        }

        double topStuffSize = 0.;
        if (mLayoutModel->areEventsVisible()) {
            topStuffSize += 5 + QFontMetrics(mLayoutModel->commandFont()).height();
        }

        double availableHeight = height() - titleStuffSize;

        double heightForGraphs = availableHeight - topStuffSize - bottomStuffSize;

        double desirableTopGraphHeight = heightForGraphs * stretchFactors.front() / sumStretch;
        double desirableBottomGraphHeight = heightForGraphs * stretchFactors.back() / sumStretch;

        if (stretchFactors.size() == 2) {
            plotLayout()->setRowStretchFactor(0 + (mLayoutModel->title() == "" ? 0 : 1), 1.);
            plotLayout()->setRowStretchFactor(stretchFactors.size() - 1 + (mLayoutModel->title() == "" ? 0 : 1), desirableBottomGraphHeight/desirableTopGraphHeight);
            return;
        }

        double topGraphPlusTopStuff = desirableTopGraphHeight + topStuffSize;
        double bottomGraphPlusBottomStuff = desirableBottomGraphHeight + bottomStuffSize;

        double k = bottomGraphPlusBottomStuff / topGraphPlusTopStuff;
        double ostStretch = sumStretch - stretchFactors.front() - stretchFactors.back();

        double zhelTopFactor = (topGraphPlusTopStuff)*ostStretch/(availableHeight - desirableTopGraphHeight - desirableTopGraphHeight*k);
        double zhelBottomFactor = zhelTopFactor * k;

        if (zhelTopFactor < stretchFactors.front())
            zhelTopFactor = stretchFactors.front();
        if (zhelBottomFactor < stretchFactors.back())
            zhelBottomFactor = stretchFactors.back();

    //    qDebug() << "zhelTopFactor" << zhelTopFactor;
    //    qDebug() << "zhelBottomFactor" << zhelBottomFactor;

        plotLayout()->setRowStretchFactor(0 + (mLayoutModel->title() == "" ? 0 : 1), zhelTopFactor);
        plotLayout()->setRowStretchFactor(stretchFactors.size() - 1 + (mLayoutModel->title() == "" ? 0 : 1), zhelBottomFactor);

        if (mBottomRightLegendLayout) {
            double x1 = desirableBottomGraphHeight/2.;
            double x2 = desirableBottomGraphHeight/2. + bottomStuffSize;
            mBottomRightLegendLayout->setRowStretchFactor(0, x1);
            mBottomRightLegendLayout->setRowStretchFactor(2, x2);
        }

    }



    return;
}

void PlotWidget::addAxisRectToLayout(int axisNumber, QCPLayoutElement *element)
{
    int axisRowNumber = (mLayoutModel->title() == "" ? 0 : 1);
    int plotColumnNumber = -1;
    switch (mLayoutModel->legendLayout()) {
        case style::LegendLocation::Right:
        case style::LegendLocation::FloatingLeft:
        case style::LegendLocation::FloatingRight:
        case style::LegendLocation::FloatingTop:
        case style::LegendLocation::FloatingBottom:
        case style::LegendLocation::FloatingTopLeft:
        case style::LegendLocation::FloatingTopRight:
        case style::LegendLocation::FloatingBottomLeft:
        case style::LegendLocation::FloatingBottomRight:
            axisRowNumber += axisNumber * 1;
            plotColumnNumber = 0;
            break;
        case style::LegendLocation::Left:
            axisRowNumber += axisNumber * 1;
            plotColumnNumber = 1;
            break;
        case style::LegendLocation::TopSeparate:
            axisRowNumber += 1 + axisNumber * 2;
            plotColumnNumber = 0;
            break;
        case style::LegendLocation::BottomSeparate:
            axisRowNumber += axisNumber * 2;
            plotColumnNumber = 0;
            break;
        case style::LegendLocation::TopCombined:
        case style::LegendLocation::TopCombinedInRow:
            axisRowNumber += 1 + axisNumber * 1;
            plotColumnNumber = 0;
            break;
        case style::LegendLocation::BottomCombined:
        case style::LegendLocation::BottomCombinedInRow:
            axisRowNumber += axisNumber * 1;
            plotColumnNumber = 0;
            break;
        default:
            break;
    }
    plotLayout()->addElement(axisRowNumber, plotColumnNumber, element);

    //stretch y axis
    auto axisModelVector = mLayoutModel->getVisibleAxisModelVector();
    plotLayout()->setRowStretchFactor(axisRowNumber,
                                      axisModelVector[axisNumber]->stretchFactor());
}

void PlotWidget::addLegendRectToLayout(int /*legendNumber*/, QCPLayoutElement */*element*/)
{

}

void PlotWidget::addLegendToLayout()
{
    if (mLayoutModel->isLegendVisible()) {
        int legendRowNumber = (mLayoutModel->title() == "" ? 0 : 1);
        int legendColumnNumber = -1;
        int legendRowNumberIncrement = 1;
        switch (mLayoutModel->legendLayout()) {
            case style::LegendLocation::Right:
            case style::LegendLocation::FloatingLeft:
            case style::LegendLocation::FloatingRight:
            case style::LegendLocation::FloatingTop:
            case style::LegendLocation::FloatingBottom:
            case style::LegendLocation::FloatingTopLeft:
            case style::LegendLocation::FloatingTopRight:
            case style::LegendLocation::FloatingBottomLeft:
            case style::LegendLocation::FloatingBottomRight:
                 legendColumnNumber = 1;
                 legendRowNumberIncrement = 1;
                 break;
            case style::LegendLocation::Left:
                 legendColumnNumber = 0;
                 legendRowNumberIncrement = 1;
                 break;
            case style::LegendLocation::TopSeparate:
                 legendColumnNumber = 0;
                 legendRowNumberIncrement = 2;
                 break;
            case style::LegendLocation::BottomSeparate:
                 legendRowNumber++;
                 legendColumnNumber = 0;
                 legendRowNumberIncrement = 2;
                 break;
            case style::LegendLocation::TopCombined:
            case style::LegendLocation::TopCombinedInRow:
                 legendColumnNumber = 0;
                 legendRowNumberIncrement = 0;
                 break;
            case style::LegendLocation::BottomCombined:
            case style::LegendLocation::BottomCombinedInRow:
                 legendRowNumber += mLayoutModel->getVisibleAxisModelVector().size();
                 legendColumnNumber = 0;
                 legendRowNumberIncrement = 0;
                 break;
            default:
                 break;
        }

        int currentGraphIndex = 0;
        QCPLegend *arLegend = NULL;
        auto axisModelVector = mLayoutModel->getVisibleAxisModelVector();
        PlotStyle preferences = mLayoutModel->plotStyle();
        for (int axisIndex = 0; axisIndex < axisModelVector.size(); ++axisIndex) {
            auto graphModelVector = axisModelVector[axisIndex]->getGraphModelVector();

            if (mLayoutModel->legendLayout() == style::LegendLocation::TopCombined
                    || mLayoutModel->legendLayout() == style::LegendLocation::BottomCombined
                    || mLayoutModel->legendLayout() == style::LegendLocation::TopCombinedInRow
                    || mLayoutModel->legendLayout() == style::LegendLocation::BottomCombinedInRow) {
                if (axisIndex == 0) {
                    arLegend = new QCPLegend;
                }
            } else {
                arLegend = new QCPLegend;
            }
//            arLegend->setBrush(global::gDefaultPlotBackgroundColor);
            arLegend->setBrush(preferences.mColorPref.mBackgroundColor);

            arLegend->setFont(preferences.mLegendProperties.mLegendFont);
            arLegend->setTextColor(preferences.mColorPref.mFontColor);
            arLegend->setLegendFrameCornerRadius(preferences.mLegendProperties.mLegendFrameCornerRadius);
            arLegend->enableLegendFrameShadowEnable(preferences.mLegendProperties.mIsLegendFrameShadowEnabled);

            QCPLayoutGrid *legendLayout = new QCPLayoutGrid;

            if (style::isLegendFloating(mLayoutModel->legendLayout()) /*== PlotLayoutModel::LegendLocation::Floating*/) {
                switch (mLayoutModel->legendLayout()) {
                    case style::LegendLocation::FloatingLeft:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignLeft);
                        break;
                    case style::LegendLocation::FloatingRight:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignRight);
                        break;
                    case style::LegendLocation::FloatingTop:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignTop);
                        break;
                    case style::LegendLocation::FloatingBottom:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignBottom);
                        break;
                    case style::LegendLocation::FloatingTopLeft:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignLeft|Qt::AlignTop);
                        break;
                    case style::LegendLocation::FloatingTopRight:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignRight|Qt::AlignTop);
                        break;
                    case style::LegendLocation::FloatingBottomLeft:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignLeft|Qt::AlignBottom);
                        break;
                    case style::LegendLocation::FloatingBottomRight:
                        axisRect(axisIndex)->insetLayout()->addElement(arLegend, Qt::AlignRight|Qt::AlignBottom);
                        break;
                    default:
                        break;

                }
                axisRect(axisIndex)->insetLayout()->setMargins(QMargins(12, 12, 12, 12));
            } else {
                plotLayout()->addElement(legendRowNumber, legendColumnNumber, legendLayout);
            }

            switch (mLayoutModel->legendLayout()) {
                case style::LegendLocation::Right:
                case style::LegendLocation::Left:
                    {
                    QCPLayoutElement *dummyElement = new QCPLayoutElement;
                    legendLayout->addElement(0,0,dummyElement);
                    legendLayout->addElement(1,0,arLegend);
                    legendLayout->addElement(2,0,dummyElement);
                    legendLayout->setRowStretchFactor(1, 0.01);
                    if (mLayoutModel->isLegendFrameVisible()) {
                        arLegend->setLayer(QLatin1String("legend"));
                    }
                    }
                    break;
                case style::LegendLocation::TopSeparate:
                case style::LegendLocation::BottomSeparate:
                    {
                    QCPLayoutElement *dummyElement = new QCPLayoutElement;
                    legendLayout->addElement(0,0,dummyElement);
                    legendLayout->addElement(0,1,arLegend);
                    legendLayout->addElement(0,2,dummyElement);
                    legendLayout->setColumnStretchFactor(1, 0.01);
                    plotLayout()->setRowStretchFactor(legendRowNumber /*+ (mLayoutModel->isDimensionVisible() ? 1: 0)*/, 0.01);
                    if (mLayoutModel->isLegendFrameVisible()) {
                        arLegend->setLayer(QLatin1String("legend"));
                    }
                    }
                     break;
                case style::LegendLocation::TopCombined:
                case style::LegendLocation::BottomCombined:
                case style::LegendLocation::TopCombinedInRow:
                case style::LegendLocation::BottomCombinedInRow:
                    if (axisIndex == 0){
                        QCPLayoutElement *dummyElement = new QCPLayoutElement;
                        legendLayout->addElement(0,0,dummyElement);
                        legendLayout->addElement(0,1,arLegend);
                        legendLayout->addElement(0,2,dummyElement);
                        legendLayout->setColumnStretchFactor(1, 0.01);
                        plotLayout()->setRowStretchFactor(legendRowNumber /*+ (mLayoutModel->isDimensionVisible() ? 1: 0)*/, 0.01);
                        //To make a frame outside legend uncomment text below
                        //NOTE: for TopCombined and BottomCombined this code should be called only once!
                        if (mLayoutModel->isLegendFrameVisible()) {
                            arLegend->setLayer(QLatin1String("legend"));
                        }
                    }
                    break;
                case style::LegendLocation::FloatingLeft:
                case style::LegendLocation::FloatingRight:
                case style::LegendLocation::FloatingTop:
                case style::LegendLocation::FloatingBottom:
                case style::LegendLocation::FloatingTopLeft:
                case style::LegendLocation::FloatingTopRight:
                case style::LegendLocation::FloatingBottomLeft:
                case style::LegendLocation::FloatingBottomRight:
                    if (mLayoutModel->isLegendFrameVisible()) {
                        arLegend->setLayer(QLatin1String("legend"));
                    }
                     break;
            }

            if (axisIndex == axisModelVector.size() - 1) {
                mBottomRightLegendLayout = legendLayout;
            }


            int colLegendCounterAddition = 0;
            for (int modelIndex = 0; modelIndex < graphModelVector.size(); ++modelIndex) {
    //            QCPGraph *newGraph = createGraphFromModelInAxes(graphModelVector[modelIndex], axisRect);
                QCPAbstractPlottable *newGraph = mGraphVector[currentGraphIndex++];
                const auto &model = graphModelVector[modelIndex];

                //NOTE: newGraph properties (pen, brush...) should already be defined
                    if (model->isLegendable()) {
                        QCPPlotTitle *legendTitle = nullptr;
                        if (modelIndex == 0 && axisModelVector[axisIndex]->legendTitle() != "") {
                                legendTitle = new QCPPlotTitle(this);
                                legendTitle->setFont(mLayoutModel->legendFont());
//                                legendTitle->setTextColor(global::gDefaultPlotFontColor);
                                legendTitle->setTextColor(preferences.mColorPref.mFontColor);
                                legendTitle->setText(axisModelVector[axisIndex]->legendTitle());
                                legendTitle->setLayer(QLatin1String("legend"));
                                colLegendCounterAddition = 1;
                        }

                        switch (mLayoutModel->legendLayout()) {
                            case style::LegendLocation::Right:
                            case style::LegendLocation::Left:
                            case style::LegendLocation::FloatingLeft:
                            case style::LegendLocation::FloatingRight:
                            case style::LegendLocation::FloatingTop:
                            case style::LegendLocation::FloatingBottom:
                            case style::LegendLocation::FloatingTopLeft:
                            case style::LegendLocation::FloatingTopRight:
                            case style::LegendLocation::FloatingBottomLeft:
                            case style::LegendLocation::FloatingBottomRight:
                                 if (legendTitle) {
                                     arLegend->addElement(0, 0, legendTitle);
                                 }
                                 arLegend->addItemVer(new QCPPlottableLegendItem(arLegend, newGraph));
                                 break;
                            case style::LegendLocation::TopSeparate:
                            case style::LegendLocation::BottomSeparate:
                                if (legendTitle) {
                                    arLegend->addElement(0, 0, legendTitle);
                                }
                                 arLegend->addItemHor(new QCPPlottableLegendItem(arLegend, newGraph));
                                 break;
                            case style::LegendLocation::TopCombined:
                            case style::LegendLocation::BottomCombined:
                                if (legendTitle) {
                                    arLegend->addElement(axisIndex, 0, legendTitle);
                                }
                                 arLegend->addElement(axisIndex, modelIndex + colLegendCounterAddition, new QCPPlottableLegendItem(arLegend, newGraph));
                                 break;
                            case style::LegendLocation::TopCombinedInRow:
                            case style::LegendLocation::BottomCombinedInRow:
                                //TODO: check this formulas, not sure they are correct
                                if (legendTitle) {
                                    arLegend->addElement(0, axisIndex+currentGraphIndex-1, legendTitle);
                                }
                                {
                                 arLegend->addElement(0, currentGraphIndex - 1 + colLegendCounterAddition*(axisIndex+1), new QCPPlottableLegendItem(arLegend, newGraph));
                                }
                                 break;

                        }
                    }
            }
            legendRowNumber += legendRowNumberIncrement;

        }


        //shrinking legend size
        if (axisModelVector.size() > 0
           && (mLayoutModel->legendLayout() == style::LegendLocation::Right
              || mLayoutModel->legendLayout() == style::LegendLocation::Left)) {
            plotLayout()->setColumnStretchFactor(legendColumnNumber /*+ (mLayoutModel->isDimensionVisible() ? 1: 0)*/, 0.01);
        }

    }
}

void PlotWidget::addTitleToLayout()
{
    if (mLayoutModel->title() != "") {
        QCPPlotTitle *plotTitle = new QCPPlotTitle(this, mLayoutModel->title());
//        plotTitle->setTextColor(global::gDefaultPlotFontColor);
        plotTitle->setTextColor(mLayoutModel->titleProperties().mColor);
        plotTitle->setFont(mLayoutModel->titleProperties().mFont);
        plotTitle->setMinimumMargins(QMargins(0, 0, 0 ,10));
        if (mLayoutModel->areEventsVisible()) {
//            double verticalGap = mLayoutModel->commandFont().pointSize();
//            if (mLayoutModel->commandLabelRotation() != 0.) {
//                verticalGap += mLayoutModel->getMaximumCommandWidth()*abs(sin(mLayoutModel->commandLabelRotation()/57.3));
//            }
//            qDebug() << "vertical gap--------->" << verticalGap;
//            plotTitle->setMinimumMargins(QMargins(0, 0, 0 ,15 + verticalGap));

            plotTitle->setMinimumMargins(QMargins(0, 0, 0 ,15 + mLayoutModel->commandFont().pointSize()));
        }

        switch (mLayoutModel->legendLayout()) {
            case style::LegendLocation::Left:
                plotLayout()->addElement(0, 1, plotTitle);
                break;
            default:
                plotLayout()->addElement(0, 0, plotTitle);
                break;
        }

    }
}



