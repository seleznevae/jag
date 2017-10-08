#include "graphmodel.h"
#include <algorithm>
#include <vector>
#include "basic/bassic.h"
#include <QDebug>
#include <dataconverter/affineconverter.h>
#include <dataconverter/borderfilter.h>
#include <dataconverter/windowfilter.h>
#include  <algorithm>
#include "basic/basic.h"
#include <float.h>
#include "global/global_definitions.h"
#include <QIcon>
#include "core/colormap/dummycolormapevaluator.h"
#include "../dataconverter/akima.h"


//int GraphModel::numberOfCreatedGraphModels = 0;

//std::tuple<QVector<double>, QVector<double>>
//GraphModel::identityConverter(const QVector<double>& x, const QVector<double> & y)
//{
//    return std::tuple<QVector<double>, QVector<double>> (x, y) ;
//}

GraphModel::GraphModel(QObject *parent) :
    QObject(parent), pen(Qt::blue), name("Undefined"), visibility(true),
    scatterStyle(), lineInterpolation(GraphModel::LineStyle::lsLine), affineConverter(std::make_shared<AffineConverter>()),
    mUnit(""), mBasisFillType(QCPGraph::BasisFillType::ZeroBasis), mIsLegendable(true),
    mStringRepresentation(StringRepresentation::Decimal), mGraphType(GraphType::Function)
{

//    setAffineConverter(std::make_shared<AffineConverter>(0, 1, 0, 20));
//    auto windowFilter = std::make_shared<WindowFilter> ();
//    windowFilter->setWindowWidth(50);
//    addFilter(windowFilter);
//    auto border = std::make_shared<BorderFilter> ();
//        addFilter(border);

}

GraphModel::~GraphModel()
{

}

QPen GraphModel::getPen() const
{
    return pen;
}

QString GraphModel::getName() const
{
    return name;
}

QVector<double> GraphModel::getXData() const
{
    return xData;
}

QVector<double> GraphModel::getYData() const
{
    return yData;
}

QCPScatterStyle GraphModel::getScatterStyle() const
{
    return scatterStyle;
}

bool GraphModel::isVisible() const
{
    return visibility;
}

GraphModel::LineStyle GraphModel::getLineInterpolation() const
{
    return lineInterpolation;
}

std::tuple<double, double> GraphModel::getXRange() const
{


    if (xData.size() != 0) {
//        auto xMin = findMinInQVector(xData);
//        auto xMax = findMaxInQVector(xData);
//        return std::tuple<double, double>(xMin, xMax);

        auto xMin = findMinInQVector(xData);
        auto xMax = findMaxInQVector(xData);
//        if (global::gXRangeEvaluationPolicy == global::RangeEvaluationPolicy::ExactMatching) {
//            return std::tuple<double, double>(xMin, xMax);
//        } else {
//            double delta = xMax - xMin;
//            double magnitudeFactor = std::pow(10.0, std::floor(std::log10(delta))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
//            double xMagnitudeFactor = magnitudeFactor;
//            double newXMax = std::ceil(xMax/xMagnitudeFactor)*xMagnitudeFactor;
//            while (std::abs(newXMax - xMax)/delta > 0.1) {
//                xMagnitudeFactor /= 10;
//                newXMax = std::ceil(xMax/xMagnitudeFactor)*xMagnitudeFactor;
//            }
//            xMagnitudeFactor = magnitudeFactor;
//            double newXMin = std::floor(xMin/xMagnitudeFactor)*xMagnitudeFactor;
//            while (std::abs(newXMin - xMin)/delta > 0.1) {
//                xMagnitudeFactor /= 10;
//                newXMin = std::floor(xMin/xMagnitudeFactor)*xMagnitudeFactor;
//            }
//            return std::tuple<double, double>(newXMin, newXMax);
//        }
        return global::recountRangeWithRespectToPolicy(std::tuple<double, double>(xMin, xMax), global::gXRangeEvaluationPolicy);
    } else {
        return std::tuple<double, double>(-10, 10);
    }
 }

std::tuple<double, double> GraphModel::getYRange() const
{
    if (xData.size() != 0) {
//        auto yMin = findMinInQVector(yData);
//        auto yMax = findMaxInQVector(yData);
//        return std::tuple<double, double>(yMin, yMax);
//        gRangeEvaluationPolicy = RangeEvaluationPolicy::PreserveReadability;

        auto yMin = findMinInQVector(yData);
        auto yMax = findMaxInQVector(yData);
//        if (global::gYRangeEvaluationPolicy == global::RangeEvaluationPolicy::ExactMatching) {
//            return std::tuple<double, double>(yMin, yMax);
//        } else {
//            double delta = yMax - yMin;
//            double magnitudeFactor = std::pow(10.0, std::floor(std::log10(delta))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
//            double yMagnitudeFactor = magnitudeFactor;
//            double newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
//            while (std::abs(newYMax - yMax)/delta > 0.1) {
//                yMagnitudeFactor /= 10;
//                newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
//            }
//            yMagnitudeFactor = magnitudeFactor;
//            double newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
//            while (std::abs(newYMin - yMin)/delta > 0.1) {
//                yMagnitudeFactor /= 10;
//                newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
//            }
//            return std::tuple<double, double>(newYMin, newYMax);
//        }
        return global::recountRangeWithRespectToPolicy(std::tuple<double, double>(yMin, yMax), global::gYRangeEvaluationPolicy);

    } else {
        return std::tuple<double, double>(-10, 10);
    }
}

std::tuple<double, double> GraphModel::getYRange(double xRangeBegin, double xRangeEnd) const
{
    if (mGraphType == GraphType::Function) {
         if (xData.size() != 0) {
    //         auto yMin = findMinInQVectorInRange(yData, xData, xRangeBegin, xRangeEnd);
    //         auto yMax = findMaxInQVectorInRange(yData, xData, xRangeBegin, xRangeEnd);
             auto yMin = findMinInQVectorInRange(yData, xData, xRangeBegin, xRangeEnd);
             yMin = std::min({
                              !std::isnan(yMin) ? yMin : std::numeric_limits<double>::max(),
                               getValue(xRangeBegin),
                               getValue(xRangeEnd),
                              }
                              );
             auto yMax = findMaxInQVectorInRange(yData, xData, xRangeBegin, xRangeEnd);
             yMax = std::max({
                               !std::isnan(yMax) ? yMax : std::numeric_limits<double>::lowest(),
                               getValue(xRangeBegin),
                               getValue(xRangeEnd),
                              }
                              );

//             if (global::gRangeEvaluationPolicy == global::RangeEvaluationPolicy::ExactMatching) {
//                 return std::tuple<double, double>(yMin, yMax);
//             } else {
//                 double delta = yMax - yMin;
//                 double magnitudeFactor = std::pow(10.0, std::floor(std::log10(delta))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
//                 double yMagnitudeFactor = magnitudeFactor;
//                 double newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
//                 while (std::abs(newYMax - yMax)/delta > 0.1) {
//                     yMagnitudeFactor /= 10;
//                     newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
//                 }
//                 yMagnitudeFactor = magnitudeFactor;
//                 double newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
//                 while (std::abs(newYMin - yMin)/delta > 0.1) {
//                     yMagnitudeFactor /= 10;
//                     newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
//                 }
//                 return std::tuple<double, double>(newYMin, newYMax);
//             }
             return global::recountRangeWithRespectToPolicy(std::tuple<double, double>(yMin, yMax), global::gYRangeEvaluationPolicy);


         }  else {
             return std::tuple<double, double>(-10, 10);
         }
    } else {
        int sz = xData.size();
        if (sz != yData.size() || sz == 0) {
            return std::tuple<double, double>(-10, 10);
        }
        double max_el = std::numeric_limits<double>::lowest();
        double min_el = std::numeric_limits<double>::max();
        for (int i = 0; i < sz; ++i) {
            if (xData[i] >= xRangeBegin && xData[i] <= xRangeEnd) {
                max_el = std::max(max_el, yData[i]);
                min_el = std::min(min_el, yData[i]);
            }

        }
        return std::tuple<double, double>(min_el, max_el);
    }
}

QBrush GraphModel::brush() const
{
    return mBrush;
}

QCPGraph::BasisFillType GraphModel::basisFillType() const
{
    return mBasisFillType;
}

std::shared_ptr<AffineConverter> GraphModel::getAffineConverter() const
{
    return affineConverter;
}

QVector<std::shared_ptr<DataConverter> > GraphModel::getFilterVector() const
{
    return mFilterVector;
}

double GraphModel::getValue(double x) const
{
    if (x < xData[0] || x > xData[xData.size() - 1]) {
        return NAN;
    } else {
//        auto it = std::find_if(xData.begin(), xData.end(), [x](double elem) {return elem >= x;});
        //lower_bound must be faster for ordered sequence than find_if
        auto it = std::lower_bound(xData.begin(), xData.end(), x);


        int right = it - xData.begin();
        int left = right != 0 ? right - 1 : right;
        double div;
        if (left == right)
            div = 0;
        else
            div = (x - xData[left]) / (xData[right] - xData[left]);

        switch (lineInterpolation) {
            case GraphModel::LineStyle::lsImpulse:
            case GraphModel::LineStyle::lsNone:
                if (x == xData[left])
                    return yData[left];
                else if (x == xData[right])
                    return yData[right];
                else
                    return 0;
                break;
            case GraphModel::LineStyle::lsLine:
            case GraphModel::LineStyle::lsSmooth:
                return yData[right] * div + yData[left] * (1. - div);
                break;
            case GraphModel::LineStyle::lsStepLeft:
                if (x == xData[right])
                    return yData[right];
                else
                    return yData[left];
                break;
            case GraphModel::LineStyle::lsStepRight:
                if (x == xData[left])
                    return yData[left];
                else
                    return yData[right];
                break;
            case GraphModel::LineStyle::lsStepCenter:
                if (x > 0.5*(xData[left]+xData[right]))
                    return yData[right];
                else
                    return yData[left];
                break;
            default:
                errorExit("Unexpected situation in %s", __FUNCTION__);
                return NAN;
                break;
        }
    }
    return NAN;
}

QString GraphModel::unit() const
{
    return mUnit;
}

bool GraphModel::isLegendable() const
{
    return mIsLegendable;
}

GraphModel::StringRepresentation GraphModel::stringRepresentation() const
{
    return mStringRepresentation;
}

GraphModel::GraphType GraphModel::graphType() const
{
    return mGraphType;
}

std::shared_ptr<ColorMapEvaluator> GraphModel::colorMapEvaluator() const
{
    return mColorMapEvaluator;
}



void GraphModel::setOriginData(const QVector<double> &newXOriginData, const QVector<double> &newYOriginData)
{
    xDataOrigin = newXOriginData;
    yDataOrigin = newYOriginData;
    convertDataWithConverter();
    emit dataChanged();
}

void GraphModel::setData(const QVector<double> &newXData, const QVector<double> &newYData)
{
    xData = newXData;
    yData = newYData;
    emit dataChanged();
}

void GraphModel::setPen(const QPen &newPen)
{
    if (pen != newPen) {
        pen = newPen;
        emit appearanceChanged();
    }
}

void GraphModel::setWidth(double newWidth)
{
    if (pen.widthF() != newWidth) {
        pen.setWidthF(newWidth);
        emit appearanceChanged();
    }
}

void GraphModel::setColor(const QColor &newColor)
{
    if (pen.color() != newColor) {
        pen.setColor(newColor);
        emit appearanceChanged();
    }
}

void GraphModel::setPenBrush(const QBrush &newBrush)
{
    if(pen.brush() != newBrush) {
        pen.setBrush(newBrush);
        emit appearanceChanged();
    }
}

void GraphModel::setBrushColor(QColor color)
{
    if (mBrush.color() != color) {
        mBrush.setColor(color);
        emit appearanceChanged();
    }
}

void GraphModel::setBrushStyle(Qt::BrushStyle style)
{
    if (mBrush.style() != style) {
        mBrush.setStyle(style);
        emit appearanceChanged();
    }
}

void GraphModel::setBrushTexture(const QPixmap &texture)
{
    mBrush.setStyle(Qt::TexturePattern);
    mBrush.setTexture(texture);
    emit appearanceChanged();
}

void GraphModel::setStringRepresentation(GraphModel::StringRepresentation representaion)
{
    if (mStringRepresentation != representaion) {
        mStringRepresentation = representaion;
        //at the moment there is no need to emit signals, as plotWidget shouldn't use this property
    }
}

void GraphModel::removeFilter(int index)
{
    if (index >= 0 && index < mFilterVector.size()) {
        mFilterVector.remove(index);
        convertDataWithConverter();
        emit dataChanged();
    }
}

void GraphModel::updateOutputData()
{
    convertDataWithConverter();
    emit dataChanged();
}

void GraphModel::setFillBasis(QCPGraph::BasisFillType newBasisFillType)
{
    if (mBasisFillType != newBasisFillType) {
        mBasisFillType = newBasisFillType;
        emit appearanceChanged();
    }
}

void GraphModel::copyPropertiesFromGraph(const GraphModel *graph)
{
    using namespace global;
    if (graph) {
        QPen pen = graph->getPen();
        if (gIsLineWidthCopyable)
            setWidth(pen.widthF());
        if (gIsLineColorCopyable)
            setColor(pen.color());
        if (gIsLineStyleCopyable)
            setLineStyle(pen.style());
        if (gIsInterpolationCopyable)
            setLineInterpolation(graph->getLineInterpolation());

    //    if (gIsLineColorCopyable)
    //        setPenBrush(pen.brush());
        if (gIsDimensionCopyable)
            setUnit(graph->unit());


        QBrush brush = graph->brush();
        if (gIsBrushColorCopyable)
            setBrushColor(brush.color());
        if (gIsBrushStyleCopyable)
            setBrushStyle(brush.style());
        if (gIsBrushBasisCopyable)
            setFillBasis(graph->basisFillType());

        QCPScatterStyle scatterStyle = graph->getScatterStyle();
        if (gIsScatterShapeCopyable)
            setScatterShape(scatterStyle.shape());
        if (gIsScatterSizeCopyable)
            setScatterSize(scatterStyle.size());
        if (gIsScatterDecimationCopyable)
            setScatterDecimation(scatterStyle.decimation());

        if (gIsTransformationCopyable)
            setAffineConverter(graph->getAffineConverter());

        if (gIsStringRepresentationCopyable)
            setStringRepresentation(graph->stringRepresentation());

        if (gIsFiltersCopyable) {
            mFilterVector.clear();
            for(const auto& filter : graph->mFilterVector) {
                mFilterVector.push_back(filter->clone());
            }
        }
        updateOutputData();

    }


}

GraphModel *GraphModel::clone() const
{
    GraphModel *newGraphModel = new GraphModel;

    newGraphModel->xDataOrigin = xDataOrigin;
    newGraphModel->yDataOrigin = yDataOrigin;
    newGraphModel->xData = xData;
    newGraphModel->yData = yData;

    newGraphModel->pen = pen;
    newGraphModel->name = name;
    newGraphModel->visibility = visibility;
    newGraphModel->scatterStyle = scatterStyle;
    newGraphModel->lineInterpolation = lineInterpolation;

    newGraphModel->mBrush = mBrush;
    newGraphModel->mUnit = mUnit;
    newGraphModel->mBasisFillType = mBasisFillType;
    newGraphModel->mIsLegendable = mIsLegendable;
    newGraphModel->mStringRepresentation = mStringRepresentation;
    newGraphModel->mGraphType = mGraphType;

    newGraphModel->affineConverter = std::dynamic_pointer_cast<AffineConverter>(affineConverter->clone());
    for (auto& filter : mFilterVector) {
        newGraphModel->mFilterVector.push_back(filter->clone());
    }

    return newGraphModel;
}

void GraphModel::convertDataWithConverter()
{
    if (xDataOrigin.size() != 0 && yDataOrigin.size() != 0) {
        if (*affineConverter == AffineConverter()) {
            xData = xDataOrigin;
            yData = yDataOrigin;
        } else {
            auto newData = (*affineConverter)(xDataOrigin, yDataOrigin);
            xData = std::get<0>(newData);
            yData = std::get<1>(newData);
        }

        foreach (auto filter, mFilterVector) {
            if (filter == nullptr) {
                //pass
            } else {
                auto newData = (*filter)(xData, yData);
                xData = std::get<0>(newData);
                yData = std::get<1>(newData);
            }
        }
    }

    if (lineInterpolation == LineStyle::lsSmooth
            && mGraphType == GraphType::Function) {
        std::tie(xData, yData) = akima_interp(xData, yData, SMOOTHING_POINTS_NUMBER);
    }
}






void GraphModel::setName(const QString &newName)
{
    if (name != newName) {
        name = newName;
        emit appearanceChanged();
    }
}

void GraphModel::setUnit(const QString &newUnit)
{
    if (mUnit != newUnit) {
        mUnit = newUnit;
        emit appearanceChanged();
    }
}

void GraphModel::setAffineConverter(std::shared_ptr<AffineConverter> newConverter)
{
    if (*affineConverter != *newConverter) {
        affineConverter = newConverter;
        convertDataWithConverter();
        emit dataChanged();
    }
}


void GraphModel::addFilter(std::shared_ptr<DataConverter> newFilter)
{
    if (newFilter != nullptr) {
        mFilterVector.push_back(newFilter);
        convertDataWithConverter();
        emit dataChanged();
    }
}

void GraphModel::setLineStyle(Qt::PenStyle newPenStyle)
{
    if (pen.style() != newPenStyle) {
        pen.setStyle(newPenStyle);
        emit appearanceChanged();
    }
}

void GraphModel::setVisibility(bool newVisibility)
{
    if (visibility != newVisibility) {
        visibility = newVisibility;
        emit dataChanged();  // to force possible layout reconstruction
    }
}

void GraphModel::setScatterStyleBrushColor(const QColor &color)
{
    if (scatterStyle.brush().color() != color) {
        scatterStyle.setBrush(color);
        emit appearanceChanged();
    }
}

void GraphModel::setScatterStylePenColor(const QColor &color)
{
    if (scatterStyle.pen().color() != color) {
        QPen pen = scatterStyle.pen();
        pen.setColor(color);
        pen.setStyle(Qt::SolidLine);
        scatterStyle.setPen(pen);
        emit appearanceChanged();
    }
}

void GraphModel::setDataLabelsVisibility(bool status)
{
    if (scatterStyle.dataLabelsEnabled() != status) {
        scatterStyle.enableDataLabels(status);
        emit appearanceChanged();
    }
}

void GraphModel::setDataLabelsFontColor(const QColor &color)
{
    if (scatterStyle.dataLabelsFontColor() != color) {
        scatterStyle.setDataLabelsFontColor(color);
        emit appearanceChanged();
    }
}

void GraphModel::setScatterStylePenWidth(double wid)
{
    if (scatterStyle.pen().widthF() != wid) {
        QPen pen = scatterStyle.pen();
        pen.setWidthF(wid);
        pen.setStyle(Qt::SolidLine);
        scatterStyle.setPen(pen);
        emit appearanceChanged();
    }
}

void GraphModel::setScatterStyle(const QCPScatterStyle &newScatterStyle)
{
   // if (!(scatterStyle == const_cast<QCPScatterStyle &>(newScatterStyle))) {
    if (scatterStyle.pen() != newScatterStyle.pen()
            || scatterStyle.brush() != newScatterStyle.brush()
            || scatterStyle.decimation() != newScatterStyle.decimation()
            || scatterStyle.shape() != newScatterStyle.shape()
            || scatterStyle.size() != newScatterStyle.size()) {
                scatterStyle = newScatterStyle;
                emit appearanceChanged();
    }
        // TODO operator == is not defined for QCPScatterStyle  !!!
    //}
}

void GraphModel::setScatterShape(QCPScatterStyle::ScatterShape newScatterShape)
{
    if (scatterStyle.shape() != newScatterShape) {
        scatterStyle.setShape(newScatterShape);
        emit appearanceChanged();
    }
}

void GraphModel::setGraphType(GraphModel::GraphType type)
{
    if (mGraphType != type) {
        mGraphType = type;
        emit dataChanged();
    }
}

//void GraphModel::setScatterPixmap(const QPixmap &pixmap)
//{
//    //NOTE: There is no equality operator for pixmaps.
////    if (scatterStyle.pixmap() != pixmap) {
//       scatterStyle.setShape(QCPScatterStyle::ScatterShape::ssPixmap);
//       scatterStyle.setPixmap(pixmap);
//        emit appearanceChanged();
//       //    }
//}

void GraphModel::setScatterIcon(const QIcon &icon)
{
    scatterStyle.setShape(QCPScatterStyle::ScatterShape::ssPixmap);
    scatterStyle.setIcon(icon);
    emit appearanceChanged();
}



void GraphModel::setScatterSize(double newScatterSize)
{
    if (scatterStyle.size() != newScatterSize) {
        scatterStyle.setSize(newScatterSize);
//        if (scatterStyle.shape() == QCPScatterStyle::ScatterShape::ssPixmap) {
//            QIcon icon(scatterStyle.pixmap());
//            scatterStyle.setPixmap(icon.pixmap(newScatterSize, newScatterSize));
//            scatterStyle.setPixmap(scatterStyle.pixmap().scaled(newScatterSize, newScatterSize));
//        }
        emit appearanceChanged();
    }
}

void GraphModel::setScatterDecimation(int newDecimation)
{

    if (scatterStyle.decimation() != newDecimation) {
        scatterStyle.setDecimation(newDecimation);
        emit appearanceChanged();
    }
}

void GraphModel::setLineInterpolation(GraphModel::LineStyle newLineInterpolation)
{
    if (lineInterpolation != newLineInterpolation) {
        auto old_interpolation = lineInterpolation;
        lineInterpolation = newLineInterpolation;
        if (lineInterpolation == LineStyle::lsSmooth
                || old_interpolation == LineStyle::lsSmooth) {
            convertDataWithConverter();
            emit dataChanged();
        } else {
            emit appearanceChanged();
        }
    }
}

void GraphModel::setLegendability(bool legendability)
{
    if (mIsLegendable != legendability) {
        mIsLegendable = legendability;
        emit appearanceChanged();
    }
}

void GraphModel::makeMovement(GraphModel::GraphMovement movement)
{
    emit movementRequest(this, movement);
}

void GraphModel::treatDeletingRequest()
{
    emit deletingRequest(this);
}

void GraphModel::setBrush(QBrush brush)
{
    if (mBrush != brush) {
        mBrush = brush;
        emit appearanceChanged();
    }
}




