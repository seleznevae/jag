#ifndef EVENTSOURCE_H
#define EVENTSOURCE_H

#include "event/timeevent.h"
#include "QVector"
#include <QString>
#include <string>
#include <iostream>
#include "basic/minicsv.h"

/*!
 \brief Container for commands.

 Can be created by loading special *.jef files - essentially *.csv files
 with structure " event_name , event_time, [event_description]"
*/
class EventSource: public QObject
{
    Q_OBJECT
public:
    EventSource();
    EventSource(const QString &fileName);
    virtual ~EventSource();

    QString name() const { return mName;}
    QString alias() const { return mAlias;}

    void setName(const QString &newName);

    void setAlias(const QString& alias);

    TimeEvent getEventByIndex(int index) const;
    QString getDescriptionByIndex(int index) const;
    QVector<QString> getEventList() const;
    void addEvent(const TimeEvent* newEvent);

    bool isDataReady() const;

    virtual int getMaxNameLength() const;
    virtual int progress() const;

    virtual void saveAs(const QString&);

signals:
    void eventSourceChanged();
private:

    static QStringList sAliasesList;

    QVector<TimeEvent*> mCommandVector;
    QString mName;
    QString mAlias;
    std::string mFileName;
    csv::ifstream fIn;

};

#endif // EVENTSOURCE_H
