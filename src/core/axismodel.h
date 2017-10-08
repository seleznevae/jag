#ifndef AXISMODEL_H
#define AXISMODEL_H

#include <QObject>
#include "core/graphmodel.h"
#include <tuple>
#include "core/arrowedtext.h"

class HorizontalLevel;

/*!
 \brief AxisModel is  a core class that encapsulates all major properties of axis.

 AxisModel contains one or several GraphModel objects.

*/
class AxisModel : public QObject
{
    Q_OBJECT
public:
    enum class ScalingMode { Auto,
                             Manual
                           };
    enum class AxisMovement { Up,
                              Down
                             };
    enum class ScaleType {Linear,
                          Logarithmic
                         };

    explicit AxisModel(QObject *parent = 0);

    std::tuple<double, double> getXRange() const;
    std::tuple<double, double> getYRange() const;
    int getGraphModelsNumber() const;
    bool contains(const GraphModel*) const;

    void addGraphModel(GraphModel* newModel);
    void addArrowedText(ArrowedText* arrowedText);
    void addHorizontalLevel(HorizontalLevel* level);
    void removeGraphModel(GraphModel* newModel);
    GraphModel* retrieveGraphModel(GraphModel* newModel);
    void removeArrowedText(ArrowedText* arrowedText);
    void removeHorizontalLevel(HorizontalLevel* level);
    void autoscaleXRange();
    void autoscaleYRange();
    void autoscaleYRangeWithRespectToXRange();
    void setXRange(double newXRangeBegin, double newXRangeEnd);
    QVector<const GraphModel*> getFunctionModelVector() const;
    QVector<const GraphModel*> getCurveModelVector() const;
    QVector<const GraphModel*> getGraphModelVector() const;
    double stretchFactor() const;
    ScaleType scaleType() const;
    QBrush backgroundBrush() const;
    QString yAxisLabel() const;
    QString legendTitle() const;

    QString getUnitsString() const;
    QVector<ArrowedText*> arrowedTextVector() const;
    QVector<HorizontalLevel *> horizontalLevelVector() const;
    QVector<HorizontalLevel *> visibleHorizontalLevelVector() const;

    bool isVisible() const;
    int getMaximumHorizontalLevelWidth(const QFont& font) const;

    bool areHorizontalLevelsVisible() const;

    void setScalingModes(ScalingMode xMode, ScalingMode yMode);
    std::tuple<ScalingMode, ScalingMode> scalingModes() const;

    ~AxisModel();

    AxisModel* clone() const;
signals:
    void axisModelChanged(AxisModel *);
    void axisAppearanceChanged();
    void movementRequest(AxisModel*, AxisModel::AxisMovement);
    void deletingRequest(AxisModel*);
    void deletingStateIsOccuring();

public slots:
    void setLegendTitle(const QString& title);
    void setYRange(double newYRangeBegin, double newYRangeEnd);
    void treatDeletingRequest();
    void setStretchFactor(double stretch);
    void setScaleType(ScaleType scaleType);
    void treatMovementRequest(AxisModel*, AxisModel::AxisMovement);
    void addArrowedTextByTime(double time);
    void addHorizontalLevelByLevel(double level);
    void setYAxisLabel(const QString &label);

    void setBackgroundBrush(const QBrush&);
    void setBackgroundColor(const QColor&);
    void setBackgroundStyle(Qt::BrushStyle);
    void setBackgroundTexture(const QPixmap &);

private:
    QVector<GraphModel*> mGraphModelVector;
    QVector<ArrowedText*> mArrowedTextVector;
    QVector<HorizontalLevel*> mHorizontalLevelVector;

    double mXRangeBegin;
    double mXRangeEnd;
    double mYRangeBegin;
    double mYRangeEnd;
    ScalingMode mXScalingMode;
    ScalingMode mYScalingMode;
    double mStretchFactor;
    ScaleType mScaleType;
    QBrush mBackgroundBrush;
    QString mYAxisLabel;
    QString mLegendTitle;
};

#endif // AXISMODEL_H
