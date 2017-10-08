#ifndef GRAPHICONENGINE_H
#define GRAPHICONENGINE_H

#include <QIconEngine>
#include "core/graphmodel.h"
#include "event/timeevent.h"
#include "anchor/anchor.h"
#include "core/arrowedtext.h"

class HorizontalLevel;
/*!
 \brief Draws icons for graphs, commands, anchors and arrowed texts in PlotHierarchyWidget.

*/
class GraphIconEngine : public QIconEngine
{
//    Q_OBJECT
public:
    explicit GraphIconEngine(const GraphModel*model);
    explicit GraphIconEngine(const TimeEvent*model);
    explicit GraphIconEngine(const Anchor*model);
    explicit GraphIconEngine(const ArrowedText*model);
    explicit GraphIconEngine(const HorizontalLevel*model);


public:
    virtual void paint(QPainter * painter, const QRect & rect, QIcon::Mode mode, QIcon::State state);
    virtual QIconEngine *clone() const;

private:
    const GraphModel *mGraphModel;
    const TimeEvent *mEventModel;
    const Anchor *mAnchorModel;
    const ArrowedText *mArrowedTextModel;
    const HorizontalLevel *mHorizontalLevelModel;

    void drawScatterShape(QPainter *painter, double x, double y) const;

};

#endif // GRAPHICONENGINE_H
