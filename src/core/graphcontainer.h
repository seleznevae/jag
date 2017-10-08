#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include "core/graphmodel.h"
#include <QVector>
#include "event/timeevent.h"

class GraphContainer
{
public:
    GraphContainer();
    virtual ~GraphContainer() { }

    virtual GraphModel* addNewGraph(const QVector<double> &x, const QVector<double> &y, const QString &name) = 0;
    virtual void addNewEvent(const TimeEvent *command) = 0;
};

#endif // GRAPHCONTAINER_H
