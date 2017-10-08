#ifndef BORDERFILTER_H
#define BORDERFILTER_H

#include <dataconverter/dataconverter.h>
#include <cfloat>

class BorderFilter : public DataConverter
{
public:
    enum class FilteringType {BandPass,
                              BandStop
                              };

    enum class EliminatingLogic { Cutting,
                       ToNaN
                     };
    BorderFilter(double xLow = -DBL_MAX, double xHigh = DBL_MAX, double yLow = -DBL_MAX, double yHigh = DBL_MAX);

    void setXFilteringType(FilteringType fType);
    void setYFilteringType(FilteringType fType);

    virtual
    std::shared_ptr<DataConverter>
    clone() const {
        auto ret = std::make_shared<BorderFilter>(mXLowBorder, mXHighBorder, mYLowBorder, mYHighBorder);
        ret->setEliminatingLogic(mEliminatingLogic);
        ret->setXFilteringType(mXFilteringType);
        ret->setYFilteringType(mYFilteringType);
        return ret;
    }

    FilteringType xFilteringType() const;
    FilteringType yFilteringType() const;
    EliminatingLogic eliminatingLogic() const;

    double xLowBorder() const;
    double xHighBorder() const;
    double yLowBorder() const;
    double yHighBorder() const;

    void setXLowBorder(double arg);
    void setXHighBorder(double arg);
    void setYLowBorder(double arg);
    void setYHighBorder(double arg);
    void setEliminatingLogic(EliminatingLogic logic);

    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);
private:
    double mXLowBorder;
    double mXHighBorder;
    double mYLowBorder;
    double mYHighBorder;
    FilteringType mXFilteringType;
    FilteringType mYFilteringType;
    EliminatingLogic mEliminatingLogic;

    bool isXValueValid(double x) const;
    bool isYValueValid(double y) const;
};

#endif // BORDERFILTER_H
