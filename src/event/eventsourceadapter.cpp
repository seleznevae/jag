#include "event/eventsourceadapter.h"

EventSourceAdapter::EventSourceAdapter()
    :mEventSource(NULL)
{
}

EventSourceAdapter::~EventSourceAdapter()
{

}

QVector<std::tuple<QString, int> > EventSourceAdapter::getDataList() const
{
    if (mEventSource) {
        if (mDataList.isEmpty()) {
            QVector<QString> eventList = mEventSource->getEventList();
            for (const auto &eventName : eventList) {
                mDataList.push_back(std::tuple<QString, int>(eventName, 1));
            }
        }
        return mDataList;
    }
    return QVector<std::tuple<QString, int> >();
}

QString EventSourceAdapter::getDataSourceName() const
{
    if (mEventSource) {
        return mEventSource->name();
    }
    return QString();
}

bool EventSourceAdapter::isDataReady() const
{
    if (mEventSource) {
        return mEventSource->isDataReady();
    }
    return false;
}

QString EventSourceAdapter::getDataSourceAlias() const
{
    if (mEventSource) {
        return mEventSource->alias();
    }
    return QString();
}

void EventSourceAdapter::addDataToPlotEdit(GraphContainer *showEditWidget, int primaryIndex, int /*secondaryIndex*/)
{

    TimeEvent event = mEventSource->getEventByIndex(primaryIndex);
    showEditWidget->addNewEvent(&event);

}

void EventSourceAdapter::addCurveDataToPlotEdit(GraphContainer *showEditWidget, int xPrimaryIndex, int xSecondaryIndex, int yPrimaryIndex, int ySecondaryIndex)
{
    Q_UNUSED(xPrimaryIndex);
    Q_UNUSED(xSecondaryIndex);
    Q_UNUSED(ySecondaryIndex);
    int primaryIndex = yPrimaryIndex;

    TimeEvent event = mEventSource->getEventByIndex(primaryIndex);
    showEditWidget->addNewEvent(&event);
}

void EventSourceAdapter::setEventSource(const EventSource *commandSource)
{
    mEventSource = commandSource;
}

QString EventSourceAdapter::getDescription(int primaryIndex)
{
    if (mEventSource) {
        return mEventSource->getDescriptionByIndex(primaryIndex);
    }
    return QString();
}

int EventSourceAdapter::getMaxNameLength() const
{
    if (mEventSource) {
        return mEventSource->getMaxNameLength();
    }
    return 20;
}

int EventSourceAdapter::progress() const
{
    if (mEventSource) {
        return mEventSource->progress();
    }
    return 75;
}

bool EventSourceAdapter::isDataSourceValid() const
{
    return true;
}

bool EventSourceAdapter::isDataCorrect() const
{
    return true;
}

void EventSourceAdapter::setCorrectness(bool /*isCorrect*/)
{
    ; //nothing
}

QStringList EventSourceAdapter::errorsList() const
{
    return QStringList();
}
