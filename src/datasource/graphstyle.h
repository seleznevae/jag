#ifndef GRAPHSTYLE_H
#define GRAPHSTYLE_H

#include "core/qcustomplot.h"
#include "core/graphmodel.h"

class GraphStyle
{
public:
    GraphStyle();

    void setPen(QPen newPen);
    void setScatterStyle(QCPScatterStyle newStyle);
    void setLineInterpolation(GraphModel::LineStyle interpolation);
    void setSpecifiedProperty(bool specified);
    void setColorFixed(bool fixed);
    void setColor(QColor);


    bool isSpecified() const { return mIsSpecified;  }
    bool isColorFixed() const { return mIsColorFixed;  }
    double lineWidth() const { return mPen.widthF(); }
    QColor lineColor() const { return mPen.color(); }
//    QColor lineColor() const { return mPen.color();  }
    Qt::PenStyle lineStyle() const { return mPen.style(); }
    QPen pen() const { return mPen; }

    QCPScatterStyle::ScatterShape scatterShape() const { return mScatterStyle.shape(); }
    double scatterSize() const { return mScatterStyle.size(); }
    int scatterDecimation() const { return mScatterStyle.decimation(); }
    QCPScatterStyle scatterStyle() const { return mScatterStyle; }

    GraphModel::LineStyle lineInterpolation() const { return mLineInterpolation; }

private:
    QPen mPen;
    QCPScatterStyle mScatterStyle;
    GraphModel::LineStyle mLineInterpolation;

    bool mIsSpecified;
    bool mIsColorFixed;
};

#endif // GRAPHSTYLE_H
