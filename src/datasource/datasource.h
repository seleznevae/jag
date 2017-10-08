#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>
#include <QVector>
#include <tuple>
#include <QThread>
#include <QMutex>
#include "datasource/graphstyle.h"
#include <memory>
#include <dataconverter/dataconverter.h>


/*!
 \brief DataSource - is a base abstract class for all data sources.

DataSource is a sublcass of QThread. So you can implement data source loading in a separate thread,
so that user won't have to wait while the hole file with data will  be loaded.

*/

class DataSource: public QThread
{
    Q_OBJECT
public:
    DataSource();
    QString getDataSourceName() const;
    QString getDataSourceAlias() const;
    virtual QVector<std::tuple<QString, int>> getDataList() const = 0;  //return tuple<name, dimension>


    virtual QString getDescriptionByIndex(int index) const = 0;
    virtual QString getDimensionByIndex(int index) const;
    virtual std::tuple<QVector<double>, QVector<double>> getDataByIndex(int primaryIndex, int secondaryIndex) const;
    virtual std::tuple<QVector<double>, QVector<double>> getRawDataByIndex(int primaryIndex, int secondaryIndex) const = 0;
    void setDataSourceName(const QString& name);
    void setDataSourceAlias(const QString& alias);

    bool isValid() const;

    virtual int getMaxNameLength() const = 0;

    virtual ~DataSource();

    virtual void saveAs(const QString&);

    virtual void run() = 0;
    bool isDataReady() const;

    virtual int progress() const = 0;

    std::shared_ptr<GraphStyle> graphStyle() const;

    double timeDeviation() const;
    void setTimeDeviation(double deviation);


    void addFilter(std::shared_ptr<DataConverter> newConverter);
    QVector<std::shared_ptr<DataConverter>> getFilterVector() const;
    void removeFilter(int index);

signals:
    void errorDuringLoading(QString);
    void nonCriticalErrorsDuringLoading();

public:
    bool isDataCorrect() const { return mIsDataCorrect; }
    void setCorrectness(bool isCorrect) { mIsDataCorrect = isCorrect; }
    QStringList errorsList() const { return mErrorsList; }

private:
    QString mName;
    QString mAlias;

protected:
    bool mIsStateValid;

    static QStringList sAliasesList;
    std::shared_ptr<GraphStyle> mGraphStyle;
    double mTimeDeviation;

    QVector<std::shared_ptr<DataConverter>> mFilterVector;

protected:
    mutable QMutex mHeaderMutex;
    mutable QMutex mDataMutex;
    bool mIsDataCorrect;
    QStringList mErrorsList;

    void addErrorToErrorsList(const QString &errorString);
    void addErrorsToErrorsList(const QStringList &errorStringList);
};


#endif // DATASOURCE_H
