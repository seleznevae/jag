#include "axismodel.h"
#include <vector>
#include "basic/bassic.h"
#include <algorithm>
#include "core/horizontallevel.h"

AxisModel::AxisModel(QObject *parent) :
    QObject(parent), mXScalingMode(ScalingMode::Auto),  mYScalingMode(ScalingMode::Auto),
    mStretchFactor(1), mScaleType(ScaleType::Linear), mBackgroundBrush(Qt::NoBrush)
{
    mXRangeBegin = 0.;
    mXRangeEnd = 0.;
    mYRangeBegin = -1.e45;
    mYRangeEnd = 1.e45;

}

std::tuple<double, double> AxisModel::getXRange() const
{
    return std::tuple<double, double>(mXRangeBegin, mXRangeEnd);
}

std::tuple<double, double> AxisModel::getYRange() const
{
    return std::tuple<double, double>(mYRangeBegin, mYRangeEnd);
}

int AxisModel::getGraphModelsNumber() const
{
    return mGraphModelVector.size();
}

bool AxisModel::contains(const GraphModel *graphModel) const
{
    return mGraphModelVector.contains(const_cast<GraphModel*>(graphModel));
}


void AxisModel::addGraphModel(GraphModel *newModel)
{
    if (newModel == nullptr)
        return;

    newModel->setParent(this);

    mGraphModelVector.push_back(newModel);
    if (mXScalingMode == ScalingMode::Auto) {
        autoscaleXRange();
    } else if (mXScalingMode == ScalingMode::Manual) {
        //nothing
    }

    if (mYScalingMode == ScalingMode::Auto) {
//        autoscaleYRange();
        autoscaleYRangeWithRespectToXRange();
    } else if (mYScalingMode == ScalingMode::Manual) {
        //nothing
    }
    emit axisModelChanged(this); //maybe to many signal emitting
}

void AxisModel::addArrowedText(ArrowedText *arrowedText)
{
    arrowedText->setParent(this);
    arrowedText->setArrowedTextPrefs(PlotStyle::defaultPlotStyle().mArrowedTextProperties);
    mArrowedTextVector.push_back(arrowedText);
    connect(arrowedText,SIGNAL(arrowedTextChanged()),
            this, SIGNAL(axisAppearanceChanged()));
    emit axisAppearanceChanged();
}

void AxisModel::addHorizontalLevel(HorizontalLevel *level)
{
    level->setParent(this);
    mHorizontalLevelVector.push_back(level);
    connect(level,SIGNAL(horizontalLevelChanged()),
            this, SIGNAL(axisAppearanceChanged()));
    emit axisAppearanceChanged();
}

void AxisModel::removeGraphModel(GraphModel *newModel)
{
    auto index = mGraphModelVector.indexOf(newModel);
    if (index != -1) {
        mGraphModelVector[index]->setParent(NULL);
        mGraphModelVector[index]->deleteLater();
        mGraphModelVector.remove(index);
        if (mXScalingMode == ScalingMode::Auto) {
            autoscaleXRange();
        }
        if (mYScalingMode == ScalingMode::Auto) {
            autoscaleYRange();
        }
        emit axisModelChanged(this); //maybe to many signal emitting
    }
}

GraphModel *AxisModel::retrieveGraphModel(GraphModel *newModel)
{
    auto index = mGraphModelVector.indexOf(newModel);
    if (index != -1) {
        mGraphModelVector[index]->setParent(NULL);
        GraphModel * ret = mGraphModelVector[index];
        mGraphModelVector.remove(index);
        if (mXScalingMode == ScalingMode::Auto) {
            autoscaleXRange();
        }
        if (mYScalingMode == ScalingMode::Auto) {
            autoscaleYRange();
        }
        emit axisModelChanged(this); //maybe to many signal emitting
        return ret;
    } else {
        return nullptr;
    }
}

void AxisModel::removeArrowedText(ArrowedText *arrowedText)
{
    int index = mArrowedTextVector.indexOf(arrowedText);
    if (index == -1) {
        qDebug() << "Trying to delete unexisting arrowed text";
        return;
    }
    mArrowedTextVector[index]->setParent(NULL);
    delete mArrowedTextVector[index];
    mArrowedTextVector.remove(index);
    emit axisAppearanceChanged();
}

void AxisModel::removeHorizontalLevel(HorizontalLevel *level)
{
    int index = mHorizontalLevelVector.indexOf(level);
    if (index == -1) {
        qDebug() << "Trying to delete unexisting arrowed text";
        return;
    }
    mHorizontalLevelVector[index]->setParent(NULL);
    delete mHorizontalLevelVector[index];
    mHorizontalLevelVector.remove(index);
    emit axisAppearanceChanged();
}

void AxisModel::autoscaleXRange()
{
    QVector<double> xBeginVector;
    QVector<double> xEndVector;
    for (auto it = mGraphModelVector.begin(); it != mGraphModelVector.end(); ++it) {
        auto range = (*it)->getXRange();
        xBeginVector.push_back(std::get<0>(range));
        xEndVector.push_back(std::get<1>(range));
    }
    if (xBeginVector.size() != 0) {
        mXScalingMode = ScalingMode::Auto;
        auto xRangeBegin = findMinInQVector(xBeginVector);
        auto xRangeEnd = findMaxInQVector(xEndVector);
        if (xRangeBegin != mXRangeBegin || xRangeEnd != mXRangeEnd) {
            mXRangeBegin = xRangeBegin;
            mXRangeEnd = xRangeEnd;
//            emit axisModelChanged(this);
            emit axisAppearanceChanged();
        }
    }
}

void AxisModel::autoscaleYRange()
{
    QVector<double> yBeginVector;
    QVector<double> yEndVector;
    for (auto it = mGraphModelVector.begin(); it != mGraphModelVector.end(); ++it) {
//        auto range = (*it)->getYRange();
//        yBeginVector.push_back(std::get<0>(range));
//        yEndVector.push_back(std::get<1>(range));

        //for visibility support
        if ((*it)->isVisible()) {
            auto range = (*it)->getYRange();
            yBeginVector.push_back(std::get<0>(range));
            yEndVector.push_back(std::get<1>(range));
        }
    }
    if (yBeginVector.size() != 0) {
        mYScalingMode = ScalingMode::Auto;
        auto yRangeBegin = findMinInQVector(yBeginVector);
        auto yRangeEnd = findMaxInQVector(yEndVector);
        if (yRangeBegin != mYRangeBegin || yRangeEnd != mYRangeEnd) {
            if (yRangeBegin != yRangeEnd) {
                mYRangeBegin = yRangeBegin;
                mYRangeEnd = yRangeEnd;
                //tricky part to show line on the axis box
//                mYRangeEnd = yRangeEnd + (yRangeEnd - yRangeBegin)*4.e-3;
//                mYRangeBegin = yRangeBegin - (yRangeEnd - yRangeBegin)*4.e-3;
                //

            } else {
                mYRangeBegin = (yRangeBegin == 0) ? -1. : std::min(0., yRangeBegin*2.);
                mYRangeEnd = (yRangeBegin == 0) ? 1. : std::max(0., yRangeBegin*2.);

            }
//            emit axisModelChanged(this);
            emit axisAppearanceChanged();
        }
    }
}

void AxisModel::autoscaleYRangeWithRespectToXRange()
{
    QVector<double> yBeginVector;
    QVector<double> yEndVector;
    for (auto it = mGraphModelVector.begin(); it != mGraphModelVector.end(); ++it) {
//        auto range = (*it)->getYRange(mXRangeBegin, mXRangeEnd);
//        yBeginVector.push_back(std::get<0>(range));
//        yEndVector.push_back(std::get<1>(range));

        //for visibility support
        if ((*it)->isVisible()) {
            auto range = (*it)->getYRange(mXRangeBegin, mXRangeEnd);
            yBeginVector.push_back(std::get<0>(range));
            yEndVector.push_back(std::get<1>(range));
        }
    }
    if (yBeginVector.size() != 0) {
        auto yRangeBegin = findMinInQVector(yBeginVector);
        auto yRangeEnd = findMaxInQVector(yEndVector);
        if (yRangeBegin != mYRangeBegin || yRangeEnd != mYRangeEnd) {
            if (yRangeBegin != yRangeEnd) {
                mYRangeBegin = yRangeBegin;
                mYRangeEnd = yRangeEnd;
                //tricky part to show line on the axis box
//                mYRangeEnd = yRangeEnd + (yRangeEnd - yRangeBegin)*4.e-3;
//                mYRangeBegin = yRangeBegin - (yRangeEnd - yRangeBegin)*4.e-3;
                //

            } else {
                mYRangeBegin = (yRangeBegin == 0) ? -1. : 0;
                mYRangeEnd = (yRangeBegin == 0) ? 1. : yRangeBegin*2.;
            }
//            emit axisModelChanged(this);
            emit axisAppearanceChanged();
        }
    }
}

void AxisModel::setXRange(double newXRangeBegin, double newXRangeEnd)
{
    if (newXRangeBegin != mXRangeBegin || newXRangeEnd != mXRangeEnd) {
        mXScalingMode = ScalingMode::Manual;
        mXRangeBegin = newXRangeBegin;
        mXRangeEnd = newXRangeEnd;
        if (mYScalingMode == ScalingMode::Auto) {
            autoscaleYRangeWithRespectToXRange();
        }

//        emit axisModelChanged(this);
        emit axisAppearanceChanged();
    }
}

void AxisModel::setYRange(double newYRangeBegin, double newYRangeEnd)
{
    if (newYRangeBegin != mYRangeBegin || newYRangeEnd != mYRangeEnd) {
        mYScalingMode = ScalingMode::Manual;
        mYRangeBegin = newYRangeBegin;
        mYRangeEnd = newYRangeEnd;

        qDebug() << "AxisModel::setYRange" << newYRangeBegin << newYRangeEnd;
//        emit axisModelChanged(this);
        emit axisAppearanceChanged();
    }
}

void AxisModel::treatDeletingRequest()
{
    emit deletingRequest(this);
}

void AxisModel::setStretchFactor(double stretch)
{
    if (mStretchFactor != stretch && stretch > 0) {
        mStretchFactor = stretch;
//        emit axisModelChanged(this);
        emit axisAppearanceChanged();
    }
}

void AxisModel::setScaleType(AxisModel::ScaleType scaleType)
{
    if (mScaleType != scaleType) {
        mScaleType = scaleType;
//        emit axisModelChanged(this);
        emit axisAppearanceChanged();
    }
}

void AxisModel::treatMovementRequest(AxisModel *model, AxisModel::AxisMovement motion)
{
    emit movementRequest(model, motion);
}

void AxisModel::addArrowedTextByTime(double time)
{
    for (auto graph : mGraphModelVector) {
        double value = graph->getValue(time);
        ArrowedText *arrowedText = new ArrowedText(this);
        arrowedText->setArrowFinish(QPointF(time, value));
        //todo: change this stuff
        arrowedText->setText("y = " + QString::number(value));

        double deltaX = (mXRangeEnd - mXRangeBegin) / 10.;
        double deltaY = (mYRangeEnd - mYRangeBegin) / 5.;
        if (time > (mXRangeEnd + mXRangeBegin) * 0.5) {
            deltaX = -deltaX;
        }
        if (value > (mYRangeEnd + mYRangeBegin) * 0.5) {
            deltaY = -deltaY;
        }
        arrowedText->setTextCenter(QPointF(time + deltaX, value + deltaY));
        addArrowedText(arrowedText);
    }
}

void AxisModel::addHorizontalLevelByLevel(double level)
{
    HorizontalLevel* newLevel = new HorizontalLevel(this);
    newLevel->setLevel(level);
    newLevel->setLabel(QString::number(level));
    addHorizontalLevel(newLevel);
}

void AxisModel::setYAxisLabel(const QString &label)
{
    if (mYAxisLabel != label) {
        mYAxisLabel = label;
        emit axisAppearanceChanged();
    }
}

void AxisModel::setBackgroundBrush(const QBrush &brush)
{
    if (mBackgroundBrush != brush) {
        mBackgroundBrush = brush;
        emit axisAppearanceChanged();
    }
}

void AxisModel::setBackgroundColor(const QColor &color)
{
    if (mBackgroundBrush.color() != color) {
        mBackgroundBrush.setColor(color);
        emit axisAppearanceChanged();
    }
}

void AxisModel::setBackgroundStyle(Qt::BrushStyle style)
{
    if (mBackgroundBrush.style() != style) {
        mBackgroundBrush.setStyle(style);
        emit axisAppearanceChanged();
    }

}

void AxisModel::setBackgroundTexture(const QPixmap &texture)
{
    mBackgroundBrush.setStyle(Qt::TexturePattern);
    mBackgroundBrush.setTexture(texture);
    emit axisAppearanceChanged();
}

QVector<const GraphModel *> AxisModel::getFunctionModelVector() const
{
    QVector<const GraphModel *> ret;
    for (auto graph : mGraphModelVector) {
        if (graph->graphType() == GraphModel::GraphType::Function)
            ret.push_back(graph);
    }
    return ret;
}

QVector<const GraphModel *> AxisModel::getCurveModelVector() const
{
    QVector<const GraphModel *> ret;
    for (auto graph : mGraphModelVector) {
        if (graph->graphType() == GraphModel::GraphType::Curve)
            ret.push_back(graph);
    }
    return ret;
}

QVector<const GraphModel *> AxisModel::getGraphModelVector() const
{
    QVector<const GraphModel *> ret;
    for (auto graph : mGraphModelVector)
        ret.push_back(graph);
    return ret;
}

double AxisModel::stretchFactor() const
{
    return mStretchFactor;
}

AxisModel::ScaleType AxisModel::scaleType() const
{
    return mScaleType;
}

QBrush AxisModel::backgroundBrush() const
{
    return mBackgroundBrush;
}

QString AxisModel::yAxisLabel() const
{
    return mYAxisLabel;
}

QString AxisModel::legendTitle() const
{
    return mLegendTitle;
}

QString AxisModel::getUnitsString() const
{
    QStringList unitsList;
    for (const GraphModel* graph : mGraphModelVector) {
        QString unit = graph->unit();
        if (unitsList.indexOf(unit) == -1)
            unitsList.push_back(unit);
    }
    return "[" + unitsList.join(", ") + "]";
}

QVector<ArrowedText *> AxisModel::arrowedTextVector() const
{
    return mArrowedTextVector;
}

QVector<HorizontalLevel *> AxisModel::horizontalLevelVector() const
{
    return mHorizontalLevelVector;
}

QVector<HorizontalLevel *> AxisModel::visibleHorizontalLevelVector() const
{
    QVector<HorizontalLevel *> ret;
    for (auto level : mHorizontalLevelVector) {
        if (level->level() <= mYRangeEnd && level->level() >= mYRangeBegin) {
            ret.push_back(level);
        }
    }
    return ret;
}

bool AxisModel::isVisible() const
{
    bool ret = false;
    for (const auto & graph : mGraphModelVector) {
        if (graph->isVisible()) {
            ret = true;
            break;
        }
    }
    return ret;
}

int AxisModel::getMaximumHorizontalLevelWidth(const QFont& font) const
{
    int maxWidth = 0;
    QFontMetrics metrics(font);
    for (auto level : visibleHorizontalLevelVector()) {
        maxWidth = std::max(maxWidth, metrics.width(level->label()));
    }
    return maxWidth;
}

bool AxisModel::areHorizontalLevelsVisible() const
{
    return visibleHorizontalLevelVector().size() != 0;
}

void AxisModel::setScalingModes(AxisModel::ScalingMode xMode, AxisModel::ScalingMode yMode)
{
    //don't emit signals(this function will be called by layutModel), may be should
    mXScalingMode = xMode;
    mYScalingMode = yMode;
}

std::tuple<AxisModel::ScalingMode, AxisModel::ScalingMode> AxisModel::scalingModes() const
{
    return std::tuple<AxisModel::ScalingMode, AxisModel::ScalingMode>(mXScalingMode, mYScalingMode);
}

AxisModel::~AxisModel()
{
    emit deletingStateIsOccuring();
}

AxisModel *AxisModel::clone() const
{
    AxisModel *newAxis = new AxisModel;
    for (const auto graph : mGraphModelVector)
        newAxis->addGraphModel(graph->clone());

    //TODO: maybe clone hor. levels and arrowed texts too

    newAxis->mXRangeBegin = mXRangeBegin;
    newAxis->mXRangeEnd = mXRangeEnd;
    newAxis->mYRangeBegin = mYRangeBegin;
    newAxis->mYRangeEnd = mYRangeEnd;

    newAxis->mXScalingMode = mXScalingMode;
    newAxis->mYScalingMode = mYScalingMode;
    newAxis->mStretchFactor = mStretchFactor;
    newAxis->mScaleType = mScaleType;

    newAxis->mBackgroundBrush = mBackgroundBrush;
    newAxis->mYAxisLabel = mYAxisLabel;
    newAxis->mLegendTitle = mLegendTitle;


    ///////////


    return newAxis;
}

void AxisModel::setLegendTitle(const QString &title)
{
    if (mLegendTitle != title) {
        mLegendTitle = title;
        emit axisAppearanceChanged();
    }
}
