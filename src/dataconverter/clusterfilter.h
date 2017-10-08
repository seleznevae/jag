#ifndef CLUSTERFILTER_H
#define CLUSTERFILTER_H

#include "dataconverter/dataconverter.h"

class ClusterFilter : public DataConverter
{
public:
    enum class Policy {CutOff,
                       Replace};

    ClusterFilter();

    virtual
    ~ClusterFilter();

    virtual
    std::shared_ptr<DataConverter>
    clone() const;


    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);

    double relativeThreshold() const { return mRelativeThreshold; }
    int windowWidth() const {return mWindowWidth; }
    double beginTime() const { return mBeginTime; }
    double endTime() const { return mEndTime; }
    int numberOfConsecErrors() const { return mNumberOfConsecErrors; }
    Policy policy() const { return mPolicy; }


    void setPolicy(Policy policy);
    void setNumberOfConsecErrors(int number);
    void setRelativeThreshold(double threshold);
    void setWindowWidth(int width);
    void setBeginTime(double time);
    void setEndTime(double time);

protected:
    double mBeginTime;
    double mEndTime;
    int mWindowWidth;
    int mNumberOfConsecErrors;
    double mRelativeThreshold;
    Policy mPolicy;

};

#endif // CLUSTERFILTER_H
