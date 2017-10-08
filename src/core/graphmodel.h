#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QObject>
#include <QVector>
#include <QPen>
#include <QString>
#include <tuple>
#include "qcustomplot.h"
#include "dataconverter/dataconverter.h"
#include "dataconverter/affineconverter.h"
#include <memory>

class ColorMapEvaluator;

/*!
 \brief GraphModel is  a core class that encapsulates all major properties of an individual graph.

 GraphModel contains all information (data, pen properties, brush properties...) for graph drawing.

*/
class GraphModel : public QObject
{
    Q_OBJECT
public:
    enum class LineStyle { lsNone
                         ,lsLine
                         ,lsStepLeft
                         ,lsStepRight
                         ,lsStepCenter
                         ,lsImpulse
                         ,lsSmooth
                       };
    static constexpr int SMOOTHING_POINTS_NUMBER = 25;


    enum class GraphMovement {LongUp,
                              ShortUp,
                              ShortDown,
                              LongDown
                             };
    enum class StringRepresentation {Decimal,
                                     Hexadecimal,
                                     Binary,
                                     };

    enum class GraphType {Function,
                          Curve,
                          };

    GraphModel(QObject *parent = 0);

    ~GraphModel();

    QPen getPen() const;
    QString getName() const;
    QVector<double> getXData() const;
    QVector<double> getYData() const;
    QCPScatterStyle getScatterStyle() const;
    bool isVisible() const;
    GraphModel::LineStyle getLineInterpolation() const;
    std::tuple<double, double> getXRange() const;
    std::tuple<double, double> getYRange() const;
    std::tuple<double, double> getYRange(double xRangeBegin, double xRangeEnd) const;
    QBrush brush() const;
    QCPGraph::BasisFillType basisFillType() const;

    std::shared_ptr<AffineConverter> getAffineConverter() const;
    QVector<std::shared_ptr<DataConverter>> getFilterVector() const;

    double getValue(double x) const;
    QString unit() const;
    bool isLegendable() const;
    StringRepresentation stringRepresentation() const;
    GraphType graphType() const;
    std::shared_ptr<ColorMapEvaluator> colorMapEvaluator() const;
signals:
    void appearanceChanged();
    void dataChanged();
    void movementRequest(GraphModel*, GraphModel::GraphMovement);
    void deletingRequest(GraphModel*);



public slots:
    void setOriginData(const QVector<double> & newXOriginData, const QVector<double> & newYOriginData);
    void setData(const QVector<double> & newXData, const QVector<double> & newYData);
    void setPen(const QPen & newPen);
    void setWidth(double newWidth);
    void setColor(const QColor & newColor);
    void setPenBrush(const QBrush & newBrush);
    void setName(const QString & newName);
    void setUnit(const QString & newUnit);
    void setAffineConverter(std::shared_ptr<AffineConverter> newConverter);
    void addFilter(std::shared_ptr<DataConverter> newConverter);
    void setLineStyle(Qt::PenStyle);
    void setVisibility(bool);
    void setScatterStyleBrushColor(const QColor &);
    void setScatterStylePenColor(const QColor &);
    void setDataLabelsVisibility(bool status);
    void setDataLabelsFontColor(const QColor& color);
    void setScatterStylePenWidth(double);
    void setScatterStyle(const QCPScatterStyle& newScatterStyle);
    void setScatterShape(QCPScatterStyle::ScatterShape newScatterShape);
//    void setScatterPixmap(const QPixmap&);
    void setGraphType(GraphType);
    void setScatterIcon(const QIcon&);
    void setScatterSize(double newScatterSize);
    void setScatterDecimation(int newDecimation);
    void setLineInterpolation(GraphModel::LineStyle);
    void setLegendability(bool legendability);
    void makeMovement(GraphMovement);
    void treatDeletingRequest();
    void setBrush(QBrush brush);
    void setBrushColor(QColor color);
    void setBrushStyle(Qt::BrushStyle style);
    void setBrushTexture(const QPixmap &);
    void setStringRepresentation(StringRepresentation representaion);
    void removeFilter(int index);
    void updateOutputData();
    void setFillBasis(QCPGraph::BasisFillType);
    void copyPropertiesFromGraph(const GraphModel* graph);


    GraphModel *clone() const;
protected:
    QVector<double> xDataOrigin;
    QVector<double> yDataOrigin;
    QVector<double> xData;
    QVector<double> yData;

    QPen pen;
    QString name;
    bool visibility;
    QCPScatterStyle scatterStyle;
    GraphModel::LineStyle lineInterpolation;

    std::shared_ptr<ColorMapEvaluator> mColorMapEvaluator;
    std::shared_ptr<AffineConverter> affineConverter;
    QVector<std::shared_ptr<DataConverter>> mFilterVector;
    QBrush mBrush;
    QString mUnit;
    QCPGraph::BasisFillType mBasisFillType;
    bool mIsLegendable;
    StringRepresentation mStringRepresentation;
    GraphType mGraphType;

    void convertDataWithConverter();

};

#endif // GRAPHMODEL_H
