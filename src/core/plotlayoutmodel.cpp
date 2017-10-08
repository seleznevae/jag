#include "plotlayoutmodel.h"
#include "basic/bassic.h"
#include "basic/basic.h"
#include "global/global_definitions.h"
#include "secondaryprocessing/datacontainer.h"
#include <algorithm>
#include "event/eventsource.h"



PlotLayoutModel::PlotLayoutModel(QObject *parent) :
    QObject(parent), mState(LayoutState::EachSeparate), xScalingMode(ScalingMode::Auto),
    mXRangeBegin(std::numeric_limits<double>::lowest()), mXRangeEnd(std::numeric_limits<double>::max()),
    mPreferences(global::gDefaultPlotPreferences),
    mTitle(""), mScaleType(ScaleType::Linear), mHistoryPosition(0)
{

    connect(this, SIGNAL(layoutModelChanged()),
            this, SLOT(clearAxisHistory()));

    //DEBUG
//    QVector<double> xVector;
//    QVector<double> yVector;
//    for (int i = 0; i < 69; ++i) {
//        xVector.push_back(std::cos(i/5.)*i/7.);
//        yVector.push_back(std::sin(i/5.)*i/7.);
//    }
//    auto graph = addPlot(xVector, yVector, "new name");
//    graph->setGraphType(GraphModel::GraphType::Curve);
    //DEBUG
}

GraphModel* PlotLayoutModel::addPlot(const QVector<double> xData, const QVector<double> yData, const QString &name)
{
    GraphModel* newGraphModel = new GraphModel(this);
    newGraphModel->setName(name);
    newGraphModel->setOriginData(xData, yData);
    newGraphModel->setWidth(global::gDefaultPlotPreferences.mLineWidth);
    newGraphModel->setLineInterpolation(global::gDefaultPlotPreferences.mLineInterpolation);
    newGraphModel->setScatterSize(global::gDefaultPlotPreferences.mScatterSize);
    newGraphModel->setScatterDecimation(global::gDefaultPlotPreferences.mScatterDecimation);

//    //setting unique color
//    foreach (auto color, global::gGraphColorList) {
//        int pr = 0;
//        foreach (auto graph, mGraphModelVector) {
//            if (color == graph->getPen().color()) {
//                pr = 1;
//                break;
//            }
//        }
//        if (pr == 0) {
//            newGraphModel->setColor(color);
//            break;
//        }
//    }
    //setting unique color
    for (int i = 0; i < mPlotStyle.mColorPref.mGraphColorList.size(); ++i) {
        auto color = mPlotStyle.mColorPref.mGraphColorList[i];
        int pr = 0;
        foreach (auto graph, mGraphModelVector) {
            if (color == graph->getPen().color()) {
                pr = 1;
                break;
            }
        }
        if (pr == 0) {
            newGraphModel->setColor(color);
            newGraphModel->setBrushColor(mPlotStyle.mColorPref.mBrushColorList[i]);
            break;
        }
    }

    mGraphModelVector.push_back(newGraphModel);
    if (mState == LayoutState::Complex || mState == LayoutState::EachSeparate) {
        AxisModel *newAxisModel = new AxisModel(this);
        newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);
        newAxisModel->addGraphModel(newGraphModel);
        mAxisModelVector.push_back(newAxisModel);
//        connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
//                this, SLOT(somethingInternalHappened()));
        connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
                this, SLOT(fixLayoutChanging()));
        connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
                this, SLOT(fixAppearanceChanging()));
        connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
        connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
                this, SLOT(removeAxisModel(AxisModel*)));
        if (xScalingMode == ScalingMode::Manual) {
            newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
        }
        //TODO x,y scaling
    } else if (mState == LayoutState::AllInOne) {
        if (mAxisModelVector.size() == 0) {
             AxisModel *newAxisModel = new AxisModel(this);
                     newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

             mAxisModelVector.push_back(newAxisModel);
//             connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
//                     this, SLOT(somethingInternalHappened()));
             connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
                     this, SLOT(fixLayoutChanging()));
             connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
                     this, SLOT(fixAppearanceChanging()));
             connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                     this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
             connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
                     this, SLOT(removeAxisModel(AxisModel*)));
        }
        mAxisModelVector[0]->addGraphModel(newGraphModel);
        //TODO x,y scaling
    }
//    connect(newGraphModel, SIGNAL(appearanceChanged()),
//            this, SLOT(somethingInternalHappened()));
//    connect(newGraphModel, SIGNAL(dataChanged()),
//            this, SLOT(somethingInternalHappened()));
    connect(newGraphModel, SIGNAL(appearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    connect(newGraphModel, SIGNAL(dataChanged()),
            this, SLOT(fixLayoutChanging()));
    connect(newGraphModel, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
            this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
    connect(newGraphModel, SIGNAL(deletingRequest(GraphModel*)),
            this , SLOT(removeGraphModel(GraphModel*)));
    if (xScalingMode == ScalingMode::Auto) {
        autoscaleXRange();
    }

    emit layoutModelChanged();
    return newGraphModel;
}

void PlotLayoutModel::addCommand(const TimeEvent *newEvent)
{
    TimeEvent *event = new TimeEvent(this);
    event->setName(newEvent->name());
    event->setEventTime(newEvent->eventTime());
    event->setDescription(newEvent->description());
    event->setPen(mPlotStyle.mEventProperties.mPen);
    mEventsVector.push_back(event);
    emit layoutModelChanged();

    connect(event, SIGNAL(timeEventChanged()),
            this, SLOT(fixAppearanceChanging()));
}

void PlotLayoutModel::addCommand(double time)
{
    TimeEvent *newEvent = new TimeEvent(this);
    newEvent->setEventTime(time);
    //searching for the unique name
    for (int i = 0; true; ++i) {
        QString newName = "C" + QString::number(i);
        if (std::find_if(mEventsVector.begin(), mEventsVector.end(), [newName](TimeEvent * com) {return newName == com->name();})
                == mEventsVector.end()) {
                    newEvent->setName(newName);
                    break;
        }
    }
    newEvent->setPen(mPlotStyle.mEventProperties.mPen);
    mEventsVector.push_back(newEvent);
    emit layoutModelChanged();
    connect(newEvent, SIGNAL(timeEventChanged()),
            this, SLOT(fixAppearanceChanging()));
}

void PlotLayoutModel::removeCommand(TimeEvent *newEvent)
{
    int index = mEventsVector.indexOf(newEvent);
    if (index != -1) {
        disconnect(newEvent, SIGNAL(timeEventChanged()),
                this, SLOT(fixAppearanceChanging()));
        delete newEvent;
        mEventsVector.remove(index);
        emit layoutModelChanged();
    }
}

void PlotLayoutModel::addAnchor(double time)
{
    Anchor *anchor = new Anchor(time);
    mAnchorVector.push_back(anchor);
    connect(anchor, SIGNAL(anchorChanged()),
            this, SLOT(treatChangesInAnchors()));
    treatChangesInAnchors();
}

void PlotLayoutModel::removeAnchor(Anchor *anchor)
{
    int index = mAnchorVector.indexOf(anchor);
    if (index != -1) {
        mAnchorVector.remove(index);
        disconnect(anchor, SIGNAL(anchorChanged()),
                this, SLOT(treatChangesInAnchors()));
        anchor->setParent(NULL);
        anchor->deleteLater();
        emit layoutAppearanceChanged();
        emit anchorsChanged();
    }
}

void PlotLayoutModel::removeArrowedText(ArrowedText *arrowedText)
{
    for (auto axisModel : mAxisModelVector) {
        int index = axisModel->arrowedTextVector().indexOf(arrowedText);
        if (index != -1) {
            axisModel->removeArrowedText(arrowedText);
            emit layoutAppearanceChanged();
            return;
        }
    }

}

void PlotLayoutModel::removeHorizontalLevel(HorizontalLevel *level)
{
    for (auto axisModel : mAxisModelVector) {
        int index = axisModel->horizontalLevelVector().indexOf(level);
        if (index != -1) {
            axisModel->removeHorizontalLevel(level);
            emit layoutAppearanceChanged();
            return;
        }
    }
}

void PlotLayoutModel::removeObjects(QVector<GraphModel *> graphVector, QVector<TimeEvent *> commandVector,
                                    QVector<Anchor *> anchVector, QVector<ArrowedText*> arrTextVector, QVector<HorizontalLevel*> horLevelVector)
{
    //maybe not very good (too many signals will be emitted
    for (auto graph : graphVector)
        removeGraphModel(graph);
    for (auto command : commandVector)
        removeCommand(command);
    for (auto anchor : anchVector)
        removeAnchor(anchor);
    for (auto arrText : arrTextVector)
        removeArrowedText(arrText);
    for (auto horLevel : horLevelVector)
        removeHorizontalLevel(horLevel);
}


QVector<GraphModel *> PlotLayoutModel::getGraphModelVector() const
{
    return mGraphModelVector;
}

QVector<AxisModel *> PlotLayoutModel::getAxisModelVector() const
{
    return mAxisModelVector;
}

QVector<AxisModel *> PlotLayoutModel::getVisibleAxisModelVector() const
{
    QVector<AxisModel *> ret;
    for (const auto & axisModel : mAxisModelVector) {
        if (axisModel->isVisible()) {
            ret.push_back(axisModel);
        }
    }
    return ret;
}

std::tuple<double, double> PlotLayoutModel::getXRange() const
{
    return std::tuple<double, double>(mXRangeBegin, mXRangeEnd);
}

std::tuple<double, double> PlotLayoutModel::getXMaXTotalRange() const
{
    double begin =  DBL_MAX;
    double end   =  -DBL_MAX;

    for (auto graphModel : mGraphModelVector) {
        auto range = graphModel->getXRange();
        if (begin > std::get<0>(range))
            begin = std::get<0>(range);
        if (end < std::get<1>(range))
            end = std::get<1>(range);
    }
    return std::tuple<double, double>(begin, end);

}

bool PlotLayoutModel::isLegendVisible() const
{
    return mPlotStyle.mLegendProperties.mIsLegendVisible;
}

bool PlotLayoutModel::areEventsVisible() const
{
    if (mEventsVector.size() == 0)
        return false;

    for (const auto & event : mEventsVector) {
        if (event->eventTime() > mXRangeEnd || event->eventTime() < mXRangeBegin)
            return false;
    }
    return true;
}

QVector<TimeEvent*> PlotLayoutModel::commandsVector() const
{
    return mEventsVector;
}

bool PlotLayoutModel::areUnitsVisible() const
{
    return mPreferences.mUnitsVisibility;
}

QFont PlotLayoutModel::legendFont() const
{
    return mPlotStyle.mLegendProperties.mLegendFont;
}

QFont PlotLayoutModel::commandFont() const
{
    return mPlotStyle.mEventProperties.mLabel.mFont;
}

QFont PlotLayoutModel::yLabelFont() const
{
    return mPlotStyle.mYLabelProperties.mFont;
}

QFont PlotLayoutModel::xAxisFont() const
{
    return mPlotStyle.mXTickLabelProperties.mFont;
}



QFont PlotLayoutModel::yAxisFont() const
{
    return mPlotStyle.mYTickLabelProperties.mFont;
}

QFont PlotLayoutModel::arrowedTextFont() const
{
    return mPlotStyle.mArrowedTextProperties.mLabelPrefs.mFont;
}

QString PlotLayoutModel::xLabel() const
{
    return mPreferences.mXLabelString;
}

QFont PlotLayoutModel::xLabelFont() const
{
    return mPlotStyle.mXLabelProperties.mFont;
}

double PlotLayoutModel::xTickRotation() const
{
    return mPlotStyle.mXTickRotation;
}

double PlotLayoutModel::commandLabelRotation() const
{
    return mPlotStyle.mEventLabelRotation;
}

QString PlotLayoutModel::title() const
{
    return mTitle;
}

//QFont PlotLayoutModel::titleFont() const
//{
//    return mPreferences.mTitleFont;
//}

PlotLayoutModel::ScaleType PlotLayoutModel::scaleType() const
{
    return mScaleType;
}

QString PlotLayoutModel::getInfoString(double x, double y) const
{
//    QString infoString = "x:" + QString::number(x) + "    y:" + QString::number(y) + "    ";
//    gDoubleToStringConversionPrecision

//    QString infoString = "<h3>x:" + QString::number(x, 'g', gDoubleToStringConversionPrecision)
//            + "    y:" + QString::number(y, 'g', gDoubleToStringConversionPrecision) + "    ";

    QString infoString = "<h3>x:" + valueToString(x, GraphModel::StringRepresentation::Decimal)
            + "    y:" + valueToString(y, GraphModel::StringRepresentation::Decimal) + "    ";

    infoString += "&nbsp;&nbsp;&nbsp;&nbsp;";
    foreach (auto axis, mAxisModelVector) {
        if (axis->isVisible()) {
            auto graphVector = axis->getFunctionModelVector();
            foreach (auto graph, graphVector) {
                QString colorName = graph->getPen().color().name();
    //            infoString += graph->getName() +":" + QString::number(graph->getValue(x)) + "    ";

//                infoString += "&nbsp;<font color=" +colorName+ ">" + graph->getName() +": "
//                            + QString::number(graph->getValue(x), 'g', gDoubleToStringConversionPrecision) + "    </font>";

                infoString += "&nbsp;<font color=" +colorName+ ">" + graph->getName() +": "
                            + valueToString(graph->getValue(x), graph->stringRepresentation()) + "    </font>";
            }
            infoString += "&nbsp;&nbsp;&nbsp;&nbsp;";
        }
    }

    infoString += "</h3>";
    return infoString;
}

bool PlotLayoutModel::isWheelZoomEnabled() const
{
    return global::gWheelZoomEnabled;
}

QCPAxis::LabelType PlotLayoutModel::timeAxisType() const
{
    return mPreferences.mTimeAxisType;
}

QVector<Anchor *> PlotLayoutModel::anchorVector() const
{
    return mAnchorVector;
}

style::LegendLocation PlotLayoutModel::legendLayout() const
{
    return mPlotStyle.mLegendProperties.mLocation;
}

bool PlotLayoutModel::isLegendFrameVisible() const
{
    return mPlotStyle.mLegendProperties.mIsLegendFrameVisible;;
}

int PlotLayoutModel::getMaximumLegendWidth() const
{
    if (!isLegendVisible())
        return 0;
    if (legendLayout() != style::LegendLocation::Left
            && legendLayout() != style::LegendLocation::Right)
        return 0;
    int maxLen = 0;
    for (GraphModel* graph : mGraphModelVector) {
        if (graph->getName().size() > maxLen) {
            maxLen = graph->getName().size();
        }
    }
    QFontMetrics fontMetrics(mPlotStyle.mXTickLabelProperties.mFont);
//    return fontMetrics.width("O", maxLen);
    return fontMetrics.width(QString(maxLen, 'O'));
}

int PlotLayoutModel::getMaximumDimensionWidth() const
{
    if (!areUnitsVisible())
        return 0;
    int maxLen = 0;
    for (AxisModel* axisModel : mAxisModelVector) {
        if (axisModel->getUnitsString().size() > maxLen) {
            maxLen = axisModel->getUnitsString().size();
        }
    }

    QFontMetrics fontMetrics(mPlotStyle.mXTickLabelProperties.mFont);
//    return fontMetrics.width("O", maxLen);
    return fontMetrics.width(QString(maxLen, 'O'));
}

int PlotLayoutModel::getMaximumTitleHeight() const
{
    if (title().size() == 0) {
        return 0;
    }
    QFontMetrics fontMetrics(mPlotStyle.mTitleProperties.mFont);
    return fontMetrics.height();
}

int PlotLayoutModel::getMaximumXLabelHeight() const
{
    if (xLabel().size() == 0) {
        return 0;
    }
    QFontMetrics fontMetrics(mPlotStyle.mXLabelProperties.mFont);
    return fontMetrics.height();
}

int PlotLayoutModel::getMaximumCommandWidth() const
{
    int maxLetters = 0;
    for (const auto & com : mEventsVector) {
        if (com->name().size() >maxLetters) {
            maxLetters = com->name().size();
        }
    }
    QFontMetrics fontMetrics(mPlotStyle.mEventProperties.mLabel.mFont);
    return fontMetrics.width(QString(maxLetters, 'O'));
}

int PlotLayoutModel::getMaximumHorizontalLevelWidth(const QFont& font) const
{
    int maxWidth = 0;
    foreach (auto axisModel, mAxisModelVector) {
        maxWidth = std::max(maxWidth, axisModel->getMaximumHorizontalLevelWidth(font));
    }
    return maxWidth;
}

PlotLayoutModel *PlotLayoutModel::clone() const
{
    PlotLayoutModel *newLayoutModel = new PlotLayoutModel;
    for (auto axis : mAxisModelVector) {
        auto clonedAxis = axis->clone();
        newLayoutModel->mAxisModelVector.push_back(clonedAxis);
        connect(clonedAxis, SIGNAL(axisModelChanged(AxisModel *)),
                newLayoutModel, SLOT(fixLayoutChanging()));
        connect(clonedAxis, SIGNAL(axisAppearanceChanged()),
                newLayoutModel, SLOT(fixAppearanceChanging()));
        connect(clonedAxis, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                newLayoutModel, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
        connect(clonedAxis, SIGNAL(deletingRequest(AxisModel*)),
                newLayoutModel, SLOT(removeAxisModel(AxisModel*)));
        for (auto graph : clonedAxis->getGraphModelVector()) {
            newLayoutModel->mGraphModelVector.push_back(const_cast<GraphModel*>(graph));
            connect(graph, SIGNAL(appearanceChanged()),
               newLayoutModel, SLOT(fixAppearanceChanging()));
            connect(graph, SIGNAL(dataChanged()),
               newLayoutModel, SLOT(fixLayoutChanging()));
            connect(graph, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
               newLayoutModel, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
            connect(graph, SIGNAL(deletingRequest(GraphModel*)),
               newLayoutModel , SLOT(removeGraphModel(GraphModel*)));
        }
    }

    //TODO: maybe clone Anchors, TimeEvents too.

    newLayoutModel->mState = mState;
    newLayoutModel->xScalingMode = xScalingMode;
    newLayoutModel->mXRangeBegin = mXRangeBegin;
    newLayoutModel->mXRangeEnd = mXRangeEnd;

    newLayoutModel->mPlotStyle = mPlotStyle;
    newLayoutModel->mTitle = mTitle;
    newLayoutModel->mScaleType = mScaleType;

    //NOTE: history is not cloned
    return newLayoutModel;
}

bool PlotLayoutModel::retrieveGraph(GraphModel *graphModel)
{
    int graphIndex = mGraphModelVector.indexOf(graphModel);
    if (graphIndex != -1) {
        AxisModel* axisModel = NULL;
        foreach (auto axis, mAxisModelVector) {
            if (axis->contains(graphModel)) {
                axisModel = axis;
                break;
            }
        }
        if (axisModel == NULL)
            return false;

        axisModel->retrieveGraphModel(graphModel);
        if (axisModel->getGraphModelsNumber() == 0) {
            removeAxisModel(axisModel);
        }

        disconnect(graphModel, SIGNAL(appearanceChanged()),
                this, SLOT(fixAppearanceChanging()));
        disconnect(graphModel, SIGNAL(dataChanged()),
                this, SLOT(fixLayoutChanging()));
        disconnect(graphModel, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
                this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
        disconnect(graphModel, SIGNAL(deletingRequest(GraphModel*)),
                this , SLOT(removeGraphModel(GraphModel*)));
        mGraphModelVector.remove(graphIndex);
        graphModel->setParent(NULL);
        fixLayoutChanging();
        return true;
    }
    return false;

}

void PlotLayoutModel::setXGrid(const style::Grid &grid)
{
    if (mPlotStyle.mXGridProperties != grid) {
        mPlotStyle.mXGridProperties = grid;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setYGrid(const style::Grid &grid)
{
    if (mPlotStyle.mYGridProperties != grid) {
        mPlotStyle.mYGridProperties = grid;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setXSubGrid(const style::Grid &grid)
{
    if (mPlotStyle.mXSubGridProperties != grid) {
        mPlotStyle.mXSubGridProperties = grid;
        emit layoutAppearanceChanged();
    }
}


void PlotLayoutModel::setYSubGrid(const style::Grid &grid)
{
    if (mPlotStyle.mYSubGridProperties != grid) {
        mPlotStyle.mYSubGridProperties = grid;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTitleProperties(const style::Label &titleProp)
{
    if (mPlotStyle.mTitleProperties != titleProp) {
        mPlotStyle.mTitleProperties = titleProp;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setAxisRect(const style::AxisRect &axisRect)
{
    if (mPlotStyle.mAxisRectProperties != axisRect) {
        mPlotStyle.mAxisRectProperties = axisRect;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTick(const style::MainTick &tick)
{
    if (mPlotStyle.mTickProperties != tick) {
        mPlotStyle.mTickProperties = tick;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setSubTick(const style::Tick &subTick)
{
    if (mPlotStyle.mSubTickProperties != subTick) {
        mPlotStyle.mSubTickProperties = subTick;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setColorPref(const style::ColorPref &colorPref)
{
    if (mPlotStyle.mColorPref != colorPref) {
        mPlotStyle.mColorPref = colorPref;
        emit layoutAppearanceChanged();
    }
}


void PlotLayoutModel::prepareDataForSecondaryProcessing()
{
    global::gCurrGraphVector.clear();
    foreach (auto axisModel, mAxisModelVector) {
        foreach (auto graphModel, axisModel->getFunctionModelVector()) {
            global::gCurrGraphVector.push_back(graphModel);
        }
    }
}

bool PlotLayoutModel::containsAxis(AxisModel *model) const
{
    return mAxisModelVector.indexOf(model) == -1 ? false : true;
}

bool PlotLayoutModel::containsGraph(GraphModel *model) const
{
    return mGraphModelVector.indexOf(model) == -1 ? false : true;
}

void PlotLayoutModel::somethingInternalHappened()
{
    emit layoutModelChanged();
}

void PlotLayoutModel::fixAppearanceChanging()
{
    emit layoutAppearanceChanged();
}

void PlotLayoutModel::fixLayoutChanging()
{
    if (xScalingMode == ScalingMode::Auto) {
        autoscaleXRange();
    }
    autoscaleYRangeWithRespectToX();
    emit layoutModelChanged();
}

void PlotLayoutModel::composeIntellectually()
{
    if (mGraphModelVector.size() < 2) {
        return;
    }

    for (int axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
        for (auto graph : mGraphModelVector) {
            mAxisModelVector[axisIndex]->retrieveGraphModel(graph);
        }

        disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
                this, SLOT(fixLayoutChanging()));
        disconnect(mAxisModelVector[axisIndex], SIGNAL(axisAppearanceChanged()),
                this, SLOT(fixAppearanceChanging()));
        disconnect(mAxisModelVector[axisIndex], SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
        disconnect(mAxisModelVector[axisIndex], SIGNAL(deletingRequest(AxisModel*)),
                this, SLOT(removeAxisModel(AxisModel*)));
        mAxisModelVector[axisIndex]->setParent(NULL);
        mAxisModelVector[axisIndex]->deleteLater();
    }
    mAxisModelVector.clear();

    for (int graphIndex = 0; graphIndex < mGraphModelVector.size(); ++graphIndex) {
        int axisIndex = 0;
        bool relatedGraphFound = false;
        for (axisIndex = 0; axisIndex < mAxisModelVector.size(); ++ axisIndex) {
            QVector<const GraphModel*> graphVector = mAxisModelVector[axisIndex]->getFunctionModelVector();
            foreach (auto graph, graphVector) {
                if (global::areNamesRelated(graph->getName(), mGraphModelVector[graphIndex]->getName())) {
                    relatedGraphFound = true;
                    break;
                }
            }
            if (relatedGraphFound)
                break;
        }
        if (relatedGraphFound) {
            mAxisModelVector[axisIndex]->addGraphModel(mGraphModelVector[graphIndex]);
        } else {
            AxisModel *newAxisModel = new AxisModel(this);
            newAxisModel->addGraphModel(mGraphModelVector[graphIndex]);
            mAxisModelVector.push_back(newAxisModel);
            connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
                    this, SLOT(fixLayoutChanging()));
            connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
                    this, SLOT(fixAppearanceChanging()));
            connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                    this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
            connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
                    this, SLOT(removeAxisModel(AxisModel*)));
            newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
        }
    }

    mState = LayoutState::Complex;
//    if (xScalingMode == ScalingMode::Auto) {
//        autoscaleXRange();
//    }
    emit layoutModelChanged();
}

void PlotLayoutModel::setLayoutState(PlotLayoutModel::LayoutState newState)
{
//    if (mState != newState) {
//        if (newState == LayoutState::Complex) {
//            sendLogMessage("Trying explixitly change layout state to Complex\n");
//            return;
//        } else if (newState == LayoutState::AllInOne) {
//            decltype(mLayoutVector) newLayoutVector;
//            newLayoutVector.push_back(QVector<GraphModel*>());
//            for (auto it1 = mLayoutVector.begin(); it1 != mLayoutVector.end(); ++it1) {
//                for (auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
//                    newLayoutVector[0].push_back(*it2);
//                }
//            }
//            //TODO all axis configs should be copied too
//            mLayoutVector = newLayoutVector;
//        } else if (newState == LayoutState::EachSeparate) {
//            decltype(mLayoutVector) newLayoutVector;
//            for (auto it1 = mLayoutVector.begin(); it1 != mLayoutVector.end(); ++it1) {
//                for (auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
//                    QVector<GraphModel*> vec;
//                    vec.push_back(*it2);
//                    newLayoutVector.push_back(vec);
//                }
//            }
//            //TODO all axis configs should be copied too
//            mLayoutVector = newLayoutVector;
//        } else {
//            errorExit("Unexpected state was set in %s", __FUNCTION__);
//        }

//        laoyutChangesCounter++; //dirty trick;
//        emit layoutModelChanged();
//    }




    if (mGraphModelVector.size() == 0)
        return;

    switch (newState) {

        case LayoutState::Complex:
            errorExit("Unexpected state was set in %s", __FUNCTION__);
            break;
        case LayoutState::AllInOne:
            if (mAxisModelVector.size() > 1) {
//                foreach (auto graph, mGraphModelVector) {
//                    if (!mAxisModelVector[0]->contains(graph))
//                        mAxisModelVector[0]->addGraphModel(graph);
//                }
                foreach (auto graph, mGraphModelVector) {
                    if (!mAxisModelVector[0]->contains(graph)) {
                        for (int axisIndex = 1; axisIndex < mAxisModelVector.size(); ++ axisIndex) {
                            if (mAxisModelVector[axisIndex]->contains(graph)) {
                                mAxisModelVector[axisIndex]->retrieveGraphModel(graph);
                                mAxisModelVector[0]->addGraphModel(graph);
                                break;
                            }
                        }
                    }
                }

                for (int axisIndex = 1; axisIndex < mAxisModelVector.size(); ++ axisIndex) {
//                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
//                            this, SLOT(somethingInternalHappened()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
                            this, SLOT(fixLayoutChanging()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisAppearanceChanged()),
                            this, SLOT(fixAppearanceChanging()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(deletingRequest(AxisModel*)),
                            this, SLOT(removeAxisModel(AxisModel*)));
                    mAxisModelVector[axisIndex]->setParent(NULL);
                    delete mAxisModelVector[axisIndex];
                }
                mAxisModelVector.remove(1, mAxisModelVector.size() - 1);
                emit layoutModelChanged();
            }
            break;
        case LayoutState::EachSeparate:
            if (mGraphModelVector.size() >= 1) {
                for (GraphModel *graph : mGraphModelVector) {
                    for (AxisModel *axis : mAxisModelVector) {
                        axis->retrieveGraphModel(graph);
                    }
                }
                QVector<AxisModel*> newAxisVector;
                foreach (GraphModel* graphModel, mGraphModelVector) {
                    AxisModel *axisModel = new AxisModel(this);
                    axisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);
                    axisModel->addGraphModel(graphModel);
                    axisModel->setXRange(mXRangeBegin, mXRangeEnd);
//                    connect(axisModel, SIGNAL(axisModelChanged(AxisModel *)),
//                            this, SLOT(somethingInternalHappened()));
                    connect(axisModel, SIGNAL(axisModelChanged(AxisModel *)),
                            this, SLOT(fixLayoutChanging()));
                    connect(axisModel, SIGNAL(axisAppearanceChanged()),
                            this, SLOT(fixAppearanceChanging()));
                    connect(axisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
                    connect(axisModel, SIGNAL(deletingRequest(AxisModel*)),
                            this, SLOT(removeAxisModel(AxisModel*)));
                    newAxisVector.push_back(axisModel);
                }
                for (int axisIndex = 0; axisIndex < mAxisModelVector.size(); ++ axisIndex) {
//                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
//                            this, SLOT(somethingInternalHappened()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
                            this, SLOT(fixLayoutChanging()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisAppearanceChanged()),
                            this, SLOT(fixAppearanceChanging()));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
                    disconnect(mAxisModelVector[axisIndex], SIGNAL(deletingRequest(AxisModel*)),
                            this, SLOT(removeAxisModel(AxisModel*)));
                    mAxisModelVector[axisIndex]->setParent(NULL);
                    delete mAxisModelVector[axisIndex];
                }
                mAxisModelVector.clear();
                mAxisModelVector = newAxisVector;
                emit layoutModelChanged();
            }
            break;
        default:
            errorExit("Unexpected state was set in %s", __FUNCTION__);
            break;
    }
    mState = newState;
}

void PlotLayoutModel::moveGraphModel(GraphModel *graphModel, GraphModel::GraphMovement movement)
{
    int axisIndex = 0;
    for (axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
        if (mAxisModelVector[axisIndex]->contains(graphModel)) {
            break;
        }
    }
    if (axisIndex >= 0 && axisIndex < mAxisModelVector.size()) {
        switch (movement) {
            case GraphModel::GraphMovement::ShortUp:
                if (mAxisModelVector[axisIndex]->getGraphModelsNumber() != 1) {
                    mAxisModelVector[axisIndex]->retrieveGraphModel(graphModel);
                    AxisModel *newAxisModel = new AxisModel();
                            newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

                    newAxisModel->addGraphModel(graphModel);
                    mAxisModelVector.insert(axisIndex, newAxisModel);
                    mState = LayoutState::Complex;
                    emit layoutModelChanged();
                }
                break;
            case GraphModel::GraphMovement::LongUp:
                if (axisIndex != 0) {
                    mAxisModelVector[axisIndex]->retrieveGraphModel(graphModel);
                    mAxisModelVector[axisIndex-1]->addGraphModel(graphModel);
                    mState = LayoutState::Complex;
                    if (mAxisModelVector[axisIndex]->getGraphModelsNumber() == 0) {
                        mAxisModelVector[axisIndex]->setParent(NULL);
                        mAxisModelVector.remove(axisIndex);
                    }
                    emit layoutModelChanged();
                }
                break;
            case GraphModel::GraphMovement::LongDown:
                if (axisIndex != (mAxisModelVector.size()-1)) {
                    mAxisModelVector[axisIndex]->retrieveGraphModel(graphModel);
                    mAxisModelVector[axisIndex+1]->addGraphModel(graphModel);
                    mState = LayoutState::Complex;
                    if (mAxisModelVector[axisIndex]->getGraphModelsNumber() == 0) {
                        mAxisModelVector[axisIndex]->setParent(NULL);
                        mAxisModelVector.remove(axisIndex);
                    }
                    emit layoutModelChanged();
                }
                break;
            case GraphModel::GraphMovement::ShortDown:
                if (mAxisModelVector[axisIndex]->getGraphModelsNumber() != 1) {
                    mAxisModelVector[axisIndex]->retrieveGraphModel(graphModel);
                    AxisModel *newAxisModel = new AxisModel();
                            newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

                    newAxisModel->addGraphModel(graphModel);
                    mAxisModelVector.insert(axisIndex+1, newAxisModel);
                    mState = LayoutState::Complex;
                    emit layoutModelChanged();
                }
                break;
            default:
                doErrorExit("Unexpected movement in %s\n", __FUNCTION__);
                break;
        }
    }
}

void PlotLayoutModel::moveAxisModel(AxisModel *axisModel, AxisModel::AxisMovement motion)
{
    int axisIndex = mAxisModelVector.indexOf(axisModel);
    if (axisIndex != -1) {
        switch (motion) {
            case AxisModel::AxisMovement::Down:
                if (axisIndex != mAxisModelVector.size() - 1) {
                    auto buffer = mAxisModelVector[axisIndex];
                    mAxisModelVector.remove(axisIndex);
                    mAxisModelVector.insert(axisIndex + 1, buffer);
                    emit layoutModelChanged();
                }
                break;
            case AxisModel::AxisMovement::Up:
                if (axisIndex != 0) {
                    auto buffer = mAxisModelVector[axisIndex - 1];
                    mAxisModelVector.remove(axisIndex - 1);
                    mAxisModelVector.insert(axisIndex, buffer);
                    emit layoutModelChanged();
                }
                break;
            default:
                doErrorExit("Unexpected situation in %s\n", __FUNCTION__);
                break;
        }
    }
}

void PlotLayoutModel::moveGraphToAxis(GraphModel *graph, AxisModel *axis)
{
    int graphIndex = -1;
    int axisIndex = -1;
    if ((graphIndex = mGraphModelVector.indexOf(graph)) == -1) {
        qDebug() << "trying to move unexisting graph";
        return;
    }
    if ((axisIndex = mAxisModelVector.indexOf(axis)) == -1) {
        qDebug() << "trying to move to unexisting axis";
        return;
    }
    //source axis serching
    int axisSourceIndex = -1;
    for (axisSourceIndex = 0; axisSourceIndex < mAxisModelVector.size(); ++axisSourceIndex) {
        if (mAxisModelVector[axisSourceIndex]->getGraphModelVector().indexOf(graph) != -1)
            break;
    }
    if (axisSourceIndex == mAxisModelVector.size()) {
        errorExit("Unexpected situation", __FUNCTION__);
        return;
    }
    if (axisSourceIndex == axisIndex) {
        qDebug() << "trying to move graph to current axis";
        return;
    }

    mAxisModelVector[axisSourceIndex]->retrieveGraphModel(graph);
    mAxisModelVector[axisIndex]->addGraphModel(graph);
    mState = LayoutState::Complex;
    if (mAxisModelVector[axisSourceIndex]->getGraphModelsNumber() == 0) {
        mAxisModelVector[axisSourceIndex]->setParent(NULL);
        mAxisModelVector.remove(axisSourceIndex);
    }

    emit layoutModelChanged();



//        LayoutState mState;
//    //    QVector< QVector<GraphModel*> > mLayoutVector;
//        QVector<AxisModel*> mAxisModelVector;
//        QVector<GraphModel*> mGraphModelVector;
//        ScalingMode xScalingMode;
//        double mXRangeBegin;
//        double mXRangeEnd;
//        QVector<QColor> mColorVector;

//        bool mIsLegendVisible;
//        PlotStyle mPreferences;

//        QString mTitle;
    //        ScaleType mScaleType;
}

void PlotLayoutModel::addExternalGraphToAxis(GraphModel *graph, AxisModel *axis)
{
    qDebug() << "<<<<<<addExternalGraphToAxis";
    if (graph == nullptr || axis == nullptr)
        return;
    int axisIndex = -1;
    if ((axisIndex = mAxisModelVector.indexOf(axis)) == -1) {
        qDebug() << "trying to move to unexisting axis";
        return;
    }
    qDebug() << "<<<<<<addExternalGraphToAxis______";
    mAxisModelVector[axisIndex]->addGraphModel(graph);
    mState = LayoutState::Complex;
    mGraphModelVector.push_back(const_cast<GraphModel*>(graph));
    connect(graph, SIGNAL(appearanceChanged()),
       this, SLOT(fixAppearanceChanging()));
    connect(graph, SIGNAL(dataChanged()),
       this, SLOT(fixLayoutChanging()));
    connect(graph, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
       this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
    connect(graph, SIGNAL(deletingRequest(GraphModel*)),
       this , SLOT(removeGraphModel(GraphModel*)));

    emit layoutModelChanged();
}

void PlotLayoutModel::moveGraphVectorToAxis(QVector<GraphModel *> graphVector, AxisModel *axis)
{
    for (const auto & graph : graphVector) {
        moveGraphToAxis(graph, axis);  //may be not too good (each function will emit signal layoutModelChanged!)
    }
}

void PlotLayoutModel::moveAxis(AxisModel *source, AxisModel *destination)
{
    int axisSourceIndex = -1;
    int axisDestinationIndex = -1;
    if ((axisSourceIndex = mAxisModelVector.indexOf(source)) == -1) {
        qDebug() << "trying to move unexisting axis";
        return;
    }
    if ((axisDestinationIndex = mAxisModelVector.indexOf(destination)) == -1) {
        qDebug() << "trying to move to unexisting axis";
        return;
    }

    if (axisSourceIndex == axisDestinationIndex) {
        qDebug() << "nothing to move";
        return;
    } else if (axisSourceIndex > axisDestinationIndex) {
        auto buffer = mAxisModelVector[axisSourceIndex];
        mAxisModelVector.remove(axisSourceIndex);
        mAxisModelVector.insert(axisDestinationIndex, buffer);
        emit layoutModelChanged();
        return;
    } else if (axisSourceIndex < axisDestinationIndex) {
        auto buffer = mAxisModelVector[axisSourceIndex];
        mAxisModelVector.remove(axisSourceIndex);
        mAxisModelVector.insert(axisDestinationIndex, buffer);
        emit layoutModelChanged();
        return;
    }


//    if (axisIndex != mAxisModelVector.size() - 1) {
//        auto buffer = mAxisModelVector[axisIndex];
//        mAxisModelVector.remove(axisIndex);
//        mAxisModelVector.insert(axisIndex + 1, buffer);
//        emit layoutModelChanged();
    //    }
}



void PlotLayoutModel::moveGraphToNewAxis(GraphModel *graphModel, int index)
{
    int graphIndex = -1;
    int axisIndex = -1;
    if ((graphIndex = mGraphModelVector.indexOf(graphModel)) == -1) {
        qDebug() << "trying to move unexisting graph";
        return;
    }
    for (axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
        if (mAxisModelVector[axisIndex]->getGraphModelVector().indexOf(graphModel) != -1) {
            break;
        }
    }
    if (axisIndex == mAxisModelVector.size()) {
        qDebug() << "can't find existing axis";
        return;
    }


    if (mAxisModelVector[axisIndex]->getGraphModelsNumber() == 1) {
        auto axis = mAxisModelVector[axisIndex];
        mAxisModelVector.remove(axisIndex);
        index == 0 ?
            mAxisModelVector.push_front(axis) : mAxisModelVector.push_back(axis);
        mState = LayoutState::Complex;
        emit layoutModelChanged();
    } else {
        mAxisModelVector[axisIndex]->retrieveGraphModel(graphModel);
        AxisModel *newAxisModel = new AxisModel();
                newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

        newAxisModel->addGraphModel(graphModel);
        newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
        index == 0 ?
            mAxisModelVector.push_front(newAxisModel) : mAxisModelVector.push_back(newAxisModel);
        mState = LayoutState::Complex;

        connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
                this, SLOT(fixLayoutChanging()));
        connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
                this, SLOT(fixAppearanceChanging()));
        connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
                this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
        connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
                this, SLOT(removeAxisModel(AxisModel*)));

        emit layoutModelChanged();
    }




}

void PlotLayoutModel::moveExternalGraphToNewAxis(GraphModel *graphModel, int index)
{
    if (graphModel == nullptr)
        return;

    mGraphModelVector.push_back(const_cast<GraphModel*>(graphModel));
    connect(graphModel, SIGNAL(appearanceChanged()),
       this, SLOT(fixAppearanceChanging()));
    connect(graphModel, SIGNAL(dataChanged()),
       this, SLOT(fixLayoutChanging()));
    connect(graphModel, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
       this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
    connect(graphModel, SIGNAL(deletingRequest(GraphModel*)),
       this , SLOT(removeGraphModel(GraphModel*)));

    AxisModel *newAxisModel = new AxisModel();
            newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

    newAxisModel->addGraphModel(graphModel);
    newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
    index == 0 ?
        mAxisModelVector.push_front(newAxisModel) : mAxisModelVector.push_back(newAxisModel);
    mState = LayoutState::Complex;

    connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
            this, SLOT(fixLayoutChanging()));
    connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
    connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
            this, SLOT(removeAxisModel(AxisModel*)));

    emit layoutModelChanged();
}

void PlotLayoutModel::moveGraphVectorToNewAxis(QVector<GraphModel *> graphModelVector, int index)
{
//    int graphIndex = -1;
//    int axisIndex = -1;
//    if ((graphIndex = mGraphModelVector.indexOf(graphModel)) == -1) {
//        qDebug() << "trying to move unexisting graph";
//        return;
//    }
//    for (axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
//        if (mAxisModelVector[axisIndex]->getGraphModelVector().indexOf(graphModel) != -1) {
//            break;
//        }
//    }
//    if (axisIndex == mAxisModelVector.size()) {
//        qDebug() << "can't find existing axis";
//        return;
//    }


//    if (mAxisModelVector[axisIndex]->getGraphModelsNumber() == 1) {
//        auto axis = mAxisModelVector[axisIndex];
//        mAxisModelVector.remove(axisIndex);
//        index == 0 ?
//            mAxisModelVector.push_front(axis) : mAxisModelVector.push_back(axis);
//        mState = LayoutState::Complex;
//        emit layoutModelChanged();
//    } else {
//        mAxisModelVector[axisIndex]->removeGraphModel(graphModel);
//        AxisModel *newAxisModel = new AxisModel();
//        newAxisModel->addGraphModel(graphModel);
//        newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
//        index == 0 ?
//            mAxisModelVector.push_front(newAxisModel) : mAxisModelVector.push_back(newAxisModel);
//        mState = LayoutState::Complex;
//        emit layoutModelChanged();
//    }



    AxisModel *newAxisModel = new AxisModel();
            newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

    newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);
    index == 0 ?
        mAxisModelVector.push_front(newAxisModel) : mAxisModelVector.push_back(newAxisModel);
    mState = LayoutState::Complex;
    moveGraphVectorToAxis(graphModelVector, newAxisModel);
    newAxisModel->setXRange(mXRangeBegin, mXRangeEnd);

    connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
            this, SLOT(fixLayoutChanging()));
    connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
    connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
            this, SLOT(removeAxisModel(AxisModel*)));

    emit layoutModelChanged();

}

void PlotLayoutModel::mergeAxis(AxisModel *source, AxisModel *destination)
{

    if (source && destination && (source != destination)) {
        int sourceAxisIndex = -1;
        int targetAxisIndex = -1;
        if ((sourceAxisIndex = mAxisModelVector.indexOf(source)) == -1) {
            qDebug() << "trying to move an unexisting axis";
            return;
        }
        if ((targetAxisIndex = mAxisModelVector.indexOf(destination)) == -1) {
            qDebug() << "trying to move to an unexisting axis";
            return;
        }
        if (sourceAxisIndex == targetAxisIndex) {
            qDebug() << "merging axes must be different";
            return;
        }

        foreach (auto graph, mAxisModelVector[sourceAxisIndex]->getGraphModelVector()) {
            mAxisModelVector[sourceAxisIndex]->retrieveGraphModel(const_cast<GraphModel*>(graph));
            mAxisModelVector[targetAxisIndex]->addGraphModel(const_cast<GraphModel*>(graph));
        }
        mState = LayoutState::Complex;
        if (mAxisModelVector[sourceAxisIndex]->getGraphModelsNumber() == 0) {
            mAxisModelVector[sourceAxisIndex]->setParent(NULL);
            delete mAxisModelVector[sourceAxisIndex];
            mAxisModelVector.remove(sourceAxisIndex);
        } else {
            errorExit("Unexpectes situation in %s", __FUNCTION__);
        }
        emit layoutModelChanged();

//        mAxisModelVector[axisSourceIndex]->removeGraphModel(graph);
//        mAxisModelVector[axisIndex]->addGraphModel(graph);
//        mState = LayoutState::Complex;
//        if (mAxisModelVector[axisSourceIndex]->getGraphModelsNumber() == 0) {
//            mAxisModelVector[axisSourceIndex]->setParent(NULL);
//            mAxisModelVector.remove(axisSourceIndex);
//        }
//        emit layoutModelChanged();

    }
}

void PlotLayoutModel::treatChangesInAnchors()
{
    std::sort(mAnchorVector.begin(), mAnchorVector.end(),
              [](Anchor *arg1, Anchor *arg2) {return arg1->time() < arg2->time();});
    emit layoutAppearanceChanged();
    emit anchorsChanged();
}

void PlotLayoutModel::setAutoDimensions()
{
    using namespace global;
    for (GraphModel *graph: mGraphModelVector) {
        if (gAutoDimFillOnlyForEmpty) {
            if (graph->unit().trimmed() != "")
                continue;
        }
        bool match = false;
        for (int index = 0; index < gParametersForDimensionList.size(); ++index) {
            QRegExp regExp(gParametersForDimensionList[index]);
//            regExp.setCaseSensitivity(Qt::CaseInsensitive);
            if (regExp.exactMatch(getPrameterNameFromComplexName(graph->getName()))) {
                graph->setUnit(gUnitsForUnitList[index]);
                match = true;
                break;
            }
        }
        if (match == false) {
            graph->setUnit(gDimensionlessParameterUnitLabel);
        }
    }

}

void PlotLayoutModel::convertGraphToCommand(GraphModel *graphModel)
{
    if (mGraphModelVector.indexOf(graphModel) != -1) {
        DataContainer dataContainer(graphModel->getXData(), graphModel->getYData());
        dataContainer = DataContainerNmspc::deriv(dataContainer);
        auto iter = std::max_element(dataContainer.mYData.begin(), dataContainer.mYData.end());
        if (iter != dataContainer.mYData.end()) {
            int index = dataContainer.mYData.indexOf(*iter);
            if (index != -1) {
                double commandTime = dataContainer.mXData[index];
                addCommand(commandTime);
            }
        }
    }
}

void PlotLayoutModel::addToGlobalDataSource(GraphModel *graphModel)
{
    if (mGraphModelVector.indexOf(graphModel) != -1) {
        global::gGlobalDataSource->addData(graphModel->getXData(), graphModel->getYData(), graphModel->getName());
    }
}

void PlotLayoutModel::addToGlobalEventSource(TimeEvent *event)
{
    if (mEventsVector.indexOf(event) != -1) {
        global::gGlobalEventSource->addEvent(event);
    }
}

void PlotLayoutModel::addBitGraph(GraphModel *graphModel, int bitPosition, int bitNumber)
{
    if (mGraphModelVector.indexOf(graphModel) != -1) {
        DataContainer dataContainer(graphModel->getXData(), graphModel->getYData());
        if (bitNumber == 1) {
            dataContainer = DataContainerNmspc::getBit(dataContainer, bitPosition);
            addPlot(dataContainer.mXData, dataContainer.mYData,
                    graphModel->getName() + "/" + QString::number(bitPosition));
        } else {
            dataContainer = DataContainerNmspc::getNBits(dataContainer, bitPosition, bitNumber);
            addPlot(dataContainer.mXData, dataContainer.mYData,
                    graphModel->getName() + "/" + QString::number(bitPosition) + "-" + QString::number(bitPosition + bitNumber -1));
        }

    }
}


void PlotLayoutModel::removeAxisModel(AxisModel *axisModel)
{
    int axisIndex = mAxisModelVector.indexOf(axisModel);
    if (axisIndex != -1) {
        removeAxisModel(axisIndex);
    }
}

void PlotLayoutModel::removeAxisModel(int axisIndex)
{
    if (axisIndex < 0 || axisIndex >= mAxisModelVector.size()) {
        qDebug() << "not valid axis index in " << __FUNCTION__;
        return;
    }
    foreach (auto graphModel, mGraphModelVector) {
        if (mAxisModelVector[axisIndex]->contains(graphModel)) {
//            disconnect(graphModel, SIGNAL(appearanceChanged()),
//                    this, SLOT(somethingInternalHappened()));
//            disconnect(graphModel, SIGNAL(dataChanged()),
//                    this, SLOT(somethingInternalHappened()));
            disconnect(graphModel, SIGNAL(appearanceChanged()),
                    this, SLOT(fixAppearanceChanging()));
            disconnect(graphModel, SIGNAL(dataChanged()),
                    this, SLOT(fixLayoutChanging()));
            disconnect(graphModel, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
                    this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
            disconnect(graphModel, SIGNAL(deletingRequest(GraphModel*)),
                    this , SLOT(removeGraphModel(GraphModel*)));
            mGraphModelVector.remove(mGraphModelVector.indexOf(graphModel));
//            graphModel->setParent(NULL);
//            delete graphModel;
        }
    }
//    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
//            this, SLOT(somethingInternalHappened()));
    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisModelChanged(AxisModel *)),
            this, SLOT(fixLayoutChanging()));
    disconnect(mAxisModelVector[axisIndex], SIGNAL(axisAppearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    disconnect(mAxisModelVector[axisIndex], SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
    disconnect(mAxisModelVector[axisIndex], SIGNAL(deletingRequest(AxisModel*)),
            this, SLOT(removeAxisModel(AxisModel*)));

    mAxisModelVector[axisIndex]->setParent(NULL);
    delete mAxisModelVector[axisIndex];

    mAxisModelVector.remove(axisIndex);
    emit layoutModelChanged();
}

void PlotLayoutModel::removeGraphModel(GraphModel *graphModel)
{
    int graphIndex = mGraphModelVector.indexOf(graphModel);
    if (graphIndex == -1) {
        qDebug() << "not valid graph index in " << __FUNCTION__;
        return;
    }
    AxisModel* axisModel = NULL;
    foreach (auto axis, mAxisModelVector) {
        if (axis->contains(graphModel)) {
            axisModel = axis;
            break;
        }
    }
    if (axisModel == NULL)
        errorExit("unexpected situation in %s\n", __FUNCTION__);
    if (axisModel->getGraphModelsNumber() == 1) {
        removeAxisModel(axisModel);
        return;
    }
    axisModel->retrieveGraphModel(graphModel);
//    disconnect(graphModel, SIGNAL(appearanceChanged()),
//            this, SLOT(somethingInternalHappened()));
//    disconnect(graphModel, SIGNAL(dataChanged()),
//            this, SLOT(somethingInternalHappened()));
    disconnect(graphModel, SIGNAL(appearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    disconnect(graphModel, SIGNAL(dataChanged()),
            this, SLOT(fixLayoutChanging()));
    disconnect(graphModel, SIGNAL(movementRequest(GraphModel*, GraphModel::GraphMovement)),
            this, SLOT(moveGraphModel(GraphModel*, GraphModel::GraphMovement)));
    disconnect(graphModel, SIGNAL(deletingRequest(GraphModel*)),
            this , SLOT(removeGraphModel(GraphModel*)));
    mGraphModelVector.remove(graphIndex);
    graphModel->setParent(NULL);
    delete graphModel;
    emit layoutModelChanged();
}

void PlotLayoutModel::autoscaleXRange()
{
    xScalingMode = ScalingMode::Auto;
    QVector<AxisModel*> axisModelVector = getVisibleAxisModelVector();
    if (axisModelVector.size() != 0) {

        QVector<double> lowMarginVector;
        QVector<double> highMarginVector;
//        QVector<AxisModel*> axisModelVector = getVisibleAxisModelVector();
        foreach (auto axis, axisModelVector) {
            axis->autoscaleXRange();
            std::tuple<double, double> xRange = axis->getXRange();
            lowMarginVector.push_back(std::get<0>(xRange));
            highMarginVector.push_back(std::get<1>(xRange));
        }

        mXRangeBegin = findMinInQVector(lowMarginVector);
        mXRangeEnd = findMaxInQVector(highMarginVector);

        //maybe I should set range for all axes(not only for visible) ?
        foreach (auto axis, axisModelVector) {
            axis->setXRange(mXRangeBegin, mXRangeEnd);
        }
        emit layoutAppearanceChanged();

    }
}

void PlotLayoutModel::autoscaleYRange()
{
    if (mAxisModelVector.size() != 0) {
//        QVector<double> lowMarginVector;
//        QVector<double> highMarginVector;
        foreach (auto axis, mAxisModelVector) {
            axis->autoscaleYRange();
        }

//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::autoscaleYRangeWithRespectToX()
{
    if (mAxisModelVector.size() != 0) {
//        QVector<double> lowMarginVector;
//        QVector<double> highMarginVector;
        foreach (auto axis, mAxisModelVector) {
            axis->autoscaleYRangeWithRespectToXRange();
        }

//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

AxisModel* PlotLayoutModel::addNewAxis(int newAxisPosition)
{
    if (newAxisPosition < -1 || newAxisPosition > mAxisModelVector.size())
        return nullptr;
    AxisModel *newAxisModel = new AxisModel(this);
            newAxisModel->setBackgroundBrush(mPlotStyle.mColorPref.mAxesBackgroundBrush);

    if (newAxisPosition == -1)
        newAxisPosition = mAxisModelVector.size();
    mAxisModelVector.insert(newAxisPosition, newAxisModel);
//        connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
//                this, SLOT(somethingInternalHappened()));
    connect(newAxisModel, SIGNAL(axisModelChanged(AxisModel *)),
            this, SLOT(fixLayoutChanging()));
    connect(newAxisModel, SIGNAL(axisAppearanceChanged()),
            this, SLOT(fixAppearanceChanging()));
    connect(newAxisModel, SIGNAL(movementRequest(AxisModel*, AxisModel::AxisMovement)),
            this, SLOT(moveAxisModel(AxisModel *, AxisModel::AxisMovement)));
    connect(newAxisModel, SIGNAL(deletingRequest(AxisModel*)),
            this, SLOT(removeAxisModel(AxisModel*)));
    return newAxisModel;
}

void PlotLayoutModel::setXRange(double xRangeBegin, double xRangeEnd)
{
    if (xRangeBegin == xRangeEnd)
        return;
    if (xRangeBegin > xRangeEnd)
        std::swap(xRangeBegin, xRangeEnd);

    xScalingMode = ScalingMode::Manual;
    mXRangeBegin = xRangeBegin;
    mXRangeEnd = xRangeEnd;
    foreach (auto axis, mAxisModelVector) {
        axis->setXRange(mXRangeBegin, mXRangeEnd);
    }
//    emit layoutModelChanged();
    emit layoutAppearanceChanged();

}

void PlotLayoutModel::setYRange(double yRangeBegin, double yRangeEnd, int axisIndex)
{
    if (axisIndex >= 0 && axisIndex < mAxisModelVector.size()) {
        if (yRangeBegin == yRangeEnd)
            return;
        if (yRangeBegin > yRangeEnd)
            std::swap(yRangeBegin, yRangeEnd);
        mAxisModelVector[axisIndex]->setYRange(yRangeBegin, yRangeEnd);
    }
}

void PlotLayoutModel::setYRangeForVisibleAxis(double yRangeBegin, double yRangeEnd, int axisIndex)
{
    auto visibleAxisVector = getVisibleAxisModelVector();
    if (axisIndex >= 0 && axisIndex < visibleAxisVector.size()) {
        if (yRangeBegin == yRangeEnd)
            return;
        if (yRangeBegin > yRangeEnd)
            std::swap(yRangeBegin, yRangeEnd);
        visibleAxisVector[axisIndex]->setYRange(yRangeBegin, yRangeEnd);
    }

}

void PlotLayoutModel::setXRangeWrapper(double xRangeBegin, double xRangeEnd)
{
    updateAxisHistory();
    setXRange(xRangeBegin, xRangeEnd);
}

void PlotLayoutModel::setYRangeWrapper(double yRangeBegin, double yRangeEnd, int axisIndex)
{
    updateAxisHistory();
    setYRange(yRangeBegin, yRangeEnd, axisIndex);
}

void PlotLayoutModel::setYRangeForVisibleAxisWrapper(double yRangeBegin, double yRangeEnd, int axisIndex)
{
    updateAxisHistory();
    setYRangeForVisibleAxis(yRangeBegin, yRangeEnd, axisIndex);
}

void PlotLayoutModel::setXYRangeWrapper(double xRangeBegin, double xRangeEnd, double yRangeBegin, double yRangeEnd, int axisIndex)
{
    updateAxisHistory();
    setXRange(xRangeBegin, xRangeEnd);
    setYRangeForVisibleAxis(yRangeBegin, yRangeEnd, axisIndex);
//    something
}

void PlotLayoutModel::setPlotStyle(const PlotStyle &pref)
{
    if (mPlotStyle != pref) {
        mPlotStyle = pref;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setLegendFontSize(int size)
{
    if (mPlotStyle.mLegendProperties.mLegendFont.pointSize() != size) {
        mPlotStyle.mLegendProperties.mLegendFont.setPointSize(size);
        emit layoutModelChanged();
//        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setYLabelFontSize(int size)
{
    if (mPlotStyle.mYLabelProperties.mFont.pointSize() != size) {
        mPlotStyle.mYLabelProperties.mFont.setPointSize(size);
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setXAxisFontSize(int size)
{
    if (mPlotStyle.mXTickLabelProperties.mFont.pointSize() != size) {
        mPlotStyle.mXTickLabelProperties.mFont.setPointSize(size);
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setYAxisFontSize(int size)
{
    if (mPlotStyle.mYTickLabelProperties.mFont.pointSize() != size) {
        mPlotStyle.mYTickLabelProperties.mFont.setPointSize(size);
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setXTickRotaion(double rotation)
{
    if (mPlotStyle.mXTickRotation != rotation) {
        mPlotStyle.mXTickRotation = rotation;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTimeEventLabelRotaion(double rotation)
{
    if (mPlotStyle.mEventLabelRotation != rotation) {
        mPlotStyle.mEventLabelRotation = rotation;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setLegendVisibility(bool vis)
{
    if (mPlotStyle.mLegendProperties.mIsLegendVisible != vis) {
        mPlotStyle.mLegendProperties.mIsLegendVisible = vis;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setDimensionVisibility(bool vis)
{
    if (mPreferences.mUnitsVisibility != vis) {
        mPreferences.mUnitsVisibility = vis;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setXLabel(QString label)
{
    if (label != mPreferences.mXLabelString) {
        mPreferences.mXLabelString = label;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setXLabelFontSize(int size)
{
    if (mPlotStyle.mXLabelProperties.mFont.pointSize() != size) {
        mPlotStyle.mXLabelProperties.mFont.setPointSize(size);
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTitle(const QString &title)
{
    if (mTitle != title) {
        mTitle = title;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

//void PlotLayoutModel::setTitleFontSize(int size)
//{
//    if (mPreferences.mTitleFont.pointSize() != size) {
//        mPreferences.mTitleFont.setPointSize(size);
////        emit layoutModelChanged();
//        emit layoutAppearanceChanged();
//    }
//}

void PlotLayoutModel::setScaleType(PlotLayoutModel::ScaleType scale)
{
    if (mScaleType != scale) {
        mScaleType = scale;
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTimeEventFontSize(int size)
{
    if (mPlotStyle.mEventProperties.mLabel.mFont.pointSize() != size) {
        mPlotStyle.mEventProperties.mLabel.mFont.setPointSize(size);
//        emit layoutModelChanged();
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setArrowedTextFontSize(int size)
{
    if (mPlotStyle.mArrowedTextProperties.mLabelPrefs.mFont.pointSize() != size) {
        mPlotStyle.mArrowedTextProperties.mLabelPrefs.mFont.setPointSize(size);
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setTimeAxisType(QCPAxis::LabelType labelType)
{
    if (mPreferences.mTimeAxisType != labelType) {
        mPreferences.mTimeAxisType = labelType;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::setLegendLayout(style::LegendLocation newLegendLayout)
{
    if (mPlotStyle.mLegendProperties.mLocation != newLegendLayout) {
        mPlotStyle.mLegendProperties.mLocation = newLegendLayout;
        emit layoutAppearanceChanged();
    }

}

void PlotLayoutModel::setLegendFrameVisibility(bool visibility)
{
    if (mPlotStyle.mLegendProperties.mIsLegendFrameVisible != visibility) {
        mPlotStyle.mLegendProperties.mIsLegendFrameVisible = visibility;
        emit layoutAppearanceChanged();
    }
}

void PlotLayoutModel::addArrowedTextByTime(double time)
{

//    for (auto axis : mAxisModelVector) {
//        axis->addArrowedTextByTime(time);
//    }
    std::for_each(mAxisModelVector.begin(), mAxisModelVector.end(),
                  [time] (AxisModel* axis) { axis->addArrowedTextByTime(time);});
    emit layoutAppearanceChanged();

}

void PlotLayoutModel::stepBackInHistory()
{

    if (mHistoryPosition > 0) {
       //send current state to history if needed
        if (mHistoryPosition == mAxisHistory.size()) {
            QVector<AxisState>  currentAxisState;
            for (const auto& axis : mAxisModelVector) {
                currentAxisState.push_back(AxisState(axis));
            }
            if (mAxisHistory.isEmpty() || currentAxisState != mAxisHistory.back()) {
                mAxisHistory.push_back(currentAxisState);
            }
        }


        --mHistoryPosition;
        applyState(mAxisHistory[mHistoryPosition]);
        emit layoutAppearanceChanged();

        std::cout << "Current pos = " << mHistoryPosition << std::endl;
        int i = 0;
        for (const auto &vec : mAxisHistory) {
            using std::cout;
            using std::end;
            std::cout << "Hist " << i++ << "\n";
            for (const auto &state : vec) {
                std::cout << "state: " << state << std::endl;
            }
        }

    }
}

void PlotLayoutModel::stepForwardInHistory()
{
    if ((mHistoryPosition + 1) < mAxisHistory.size()) {
        mHistoryPosition++;
        applyState(mAxisHistory[mHistoryPosition]);
        emit layoutAppearanceChanged();

        std::cout << "Current pos = " << mHistoryPosition << std::endl;
        int i = 0;
        for (const auto &vec : mAxisHistory) {
            using std::cout;
            using std::end;
            std::cout << "Hist " << i++ << "\n";
            for (const auto &state : vec) {
                std::cout << "state: " << state << std::endl;
            }
        }
    }
}

void PlotLayoutModel::applyState(const QVector<AxisState> &stateVector)
{
    if (!stateVector.isEmpty()) {

        for (int i = 0; i < stateVector.size(); ++i) {
            auto &state = stateVector[i];
            auto &axis = mAxisModelVector[i];

            axis->setXRange(state.beginXRange(), state.endXRange());
            axis->setYRange(state.beginYRange(), state.endYRange());
            axis->setScalingModes(state.xScalingMode(), state.yScalingMode());

        }

        mXRangeBegin = stateVector[0].beginXRange();
        mXRangeEnd   = stateVector[0].endXRange();
    }


}

void PlotLayoutModel::updateAxisHistory()
{
    if (mHistoryPosition < mAxisHistory.size()) {
        mAxisHistory.remove(mHistoryPosition, mAxisHistory.size() - mHistoryPosition);
    }

    QVector<AxisState>  currentAxisState;
    for (const auto& axis : mAxisModelVector) {
        currentAxisState.push_back(AxisState(axis));
    }
    if (mAxisHistory.isEmpty() || currentAxisState != mAxisHistory.back()) {
        mAxisHistory.push_back(currentAxisState);
        mHistoryPosition++;
    }


    std::cout << "Current pos = " << mHistoryPosition << std::endl;
    int i = 0;
    for (const auto &vec : mAxisHistory) {
        using std::cout;
        using std::end;
        std::cout << "Hist " << i++ << "\n";
        for (const auto &state : vec) {
            std::cout << "state: " << state << std::endl;
        }
    }
}

void PlotLayoutModel::clearAxisHistory()
{
    for (auto &vec : mAxisHistory) {
        vec.clear();
    }
    mAxisHistory.clear();
    mHistoryPosition = 0;
}
