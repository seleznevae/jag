#ifndef DATASOURCEADAPTER_H
#define DATASOURCEADAPTER_H

#include <QString>
#include "datasource/abstractdatasourceadapter.h"
#include "datasource/datasource.h"
#include "core/graphcontainer.h"


/*!
 \brief EventSourceAdapter adapts interface of DataSource  to the abstract interface of AbstractDataSourceAdapter
 so that it can be used in DataChoiseWidget.

*/
class DataSourceAdapter: public AbstractDataSourceAdapter
{
public:
    DataSourceAdapter();
    virtual ~DataSourceAdapter();

    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
    virtual QString getDataSourceName() const;
    virtual QString getDataSourceAlias() const;
    virtual bool isDataReady() const;

    virtual void addDataToPlotEdit(GraphContainer*, int primaryIndex, int secondaryIndex) ;
    virtual void addCurveDataToPlotEdit(GraphContainer*, int xPrimaryIndex, int xSecondaryIndex,
                                        int yPrimaryIndex, int ySecondaryIndex);

    void setDataSource(const DataSource* dataSource);
    virtual QString getDescription(int primaryIndex);

    virtual int getMaxNameLength() const;
    virtual int progress() const;

    virtual bool isDataSourceValid() const;

    virtual bool isDataCorrect() const;
    virtual void setCorrectness(bool isCorrect);
    virtual QStringList errorsList() const;

private:
    const DataSource* mDataSource;
    mutable QVector<std::tuple<QString, int>> mDataList;
};

#endif // DATASOURCEADAPTER_H
