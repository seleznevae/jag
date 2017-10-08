#ifndef EVENTSOURCEADAPTER_H
#define EVENTSOURCEADAPTER_H

#include "datasource/abstractdatasourceadapter.h"
#include "eventsource.h"
#include "core/graphcontainer.h"

/*!
 \brief EventSourceAdapter adapts interface of EventSource  to the abstract interface of AbstractDataSourceAdapter
 so that it can be used in DataChoiseWidget.

*/
class EventSourceAdapter : public AbstractDataSourceAdapter
{
public:
    EventSourceAdapter();
    virtual ~EventSourceAdapter();

    virtual QVector<std::tuple<QString, int>> getDataList() const;  //return tuple<name, dimension>
    virtual QString getDataSourceName() const;
    virtual bool isDataReady() const;

    virtual QString getDataSourceAlias() const;

    virtual void addDataToPlotEdit(GraphContainer*, int primaryIndex, int secondaryIndex);
    virtual void addCurveDataToPlotEdit(GraphContainer*, int xPrimaryIndex, int xSecondaryIndex,
                                        int yPrimaryIndex, int ySecondaryIndex);

    void setEventSource(const EventSource* commandSource);
    virtual QString getDescription(int primaryIndex);

    virtual int getMaxNameLength() const;
    virtual int progress() const;

    virtual bool isDataSourceValid() const;

    virtual bool isDataCorrect() const;
    virtual void setCorrectness(bool isCorrect);
    virtual QStringList errorsList() const;
private:
    const EventSource* mEventSource;
    mutable QVector<std::tuple<QString, int>> mDataList;
};

#endif // EVENTSOURCEADAPTER_H
