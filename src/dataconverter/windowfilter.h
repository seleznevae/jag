#ifndef WINDOWFILTER_H
#define WINDOWFILTER_H

#include "dataconverter/dataconverter.h"

class WindowFilter : public DataConverter
{
public:
    WindowFilter();

    int windowWidth() const;

    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);

    void setWindowWidth(int width);

    virtual
    std::shared_ptr<DataConverter>
    clone() const {
        auto ret = std::make_shared<WindowFilter>();
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

#endif // WINDOWFILTER_H
