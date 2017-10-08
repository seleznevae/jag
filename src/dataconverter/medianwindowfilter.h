#ifndef MEDIANWINDOWFILTER_H
#define MEDIANWINDOWFILTER_H

#include "dataconverter/dataconverter.h"

class MedianWindowFilter : public DataConverter
{
public:

    MedianWindowFilter();

    int windowWidth() const;

    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);

    void setWindowWidth(int width);

    virtual
    std::shared_ptr<DataConverter>
    clone() const {
        auto ret = std::make_shared<MedianWindowFilter>();
        ret->setWindowWidth(mWindowWidth);
        return ret;
    }

    double beginTime() const { return mBeginTime; }
    double endTime() const { return mEndTime; }

    void setBeginTime(double time);
    void setEndTime(double time);


private:
    double mBeginTime;
    double mEndTime;
    int mWindowWidth;
};

#endif // MEDIANWINDOWFILTER_H
