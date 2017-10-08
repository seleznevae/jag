#ifndef ABSTRACTDATASOURCEADAPTER_H
#define ABSTRACTDATASOURCEADAPTER_H

#include <QVector>
#include <tuple>
#include <QString>
#include "commonWidgets/plotshoweditwidget.h"
#include "core/graphcontainer.h"

/*!
 \brief AbstractDataSourceAdapter - interface that DataChoiseWidget uses to retrieve data. It was introduced to JaG so
 that user would be able to use unified interface of DataChoiseWidgets to get data from both DataSources and EventSources.

*/
class AbstractDataSourceAdapter
{
public:
    AbstractDataSourceAdapter();
    virtual ~AbstractDataSourceAdapter();


    virtual QVector<std::tuple<QString, int>> getDataList() const = 0;  //return tuple<name, dimension>
    virtual QString getDataSourceName() const = 0;
    virtual QString getDataSourceAlias() const = 0;
    virtual bool isDataReady() const = 0;

    virtual bool isDataSourceValid() const = 0;
    virtual bool isDataCorrect() const = 0;
    virtual void setCorrectness(bool isCorrect) = 0;
    virtual QStringList errorsList() const = 0;

    virtual int getMaxNameLength() const = 0;
    virtual int progress() const = 0;

    virtual void addDataToPlotEdit(GraphContainer*, int primaryIndex, int secondaryIndex) = 0;
    virtual void addCurveDataToPlotEdit(GraphContainer*, int xPrimaryIndex, int xSecondaryIndex,
                                        int yPrimaryIndex, int ySecondaryIndex) = 0;

    virtual QString getDescription(int primaryIndex) = 0;

private:
};

#endif // ABSTRACTDATASOURCEADAPTER_H
