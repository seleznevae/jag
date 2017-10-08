#ifndef PLOTHIERARCHYWIDGET_H
#define PLOTHIERARCHYWIDGET_H

#include <QTreeWidget>
#include "core/plotlayoutmodel.h"
#include <QVector>
#include <QPushButton>
#include "event/timeevent.h"
#include <QMenu>
#include "commonWidgets/ipushbutton.h"
#include <QSignalMapper>

class IPushButton;
class HorizontalLevel;
class PlotShowEditWidget;
/*!
 \brief PlotHierarchyWidget shows current heirarchy of the plot with axis, graphs, anchors, commands and arrowed texts.

By clicking on a certain item in PlotHierarchyWidget user can activate corresponding ControlWidget. Also PlotHierarchyWidget
 contains kill buttons to emit requests for objects (graphs, anchors, commands) deleting.

*/
class PlotHierarchyWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PlotHierarchyWidget(QWidget *parent = 0);

    void setPlotLayoutModel(const PlotLayoutModel* model);
    void setPlotShowEditWidget(PlotShowEditWidget*);

    GraphModel* currentGraph();
    AxisModel* currentAxis();

    QVector<GraphModel*> selectedGraphs() const;
signals:
    void axesVectorActivated(QVector<AxisModel*> );
    void graphActivated(GraphModel*);
    void graphVectorActivated(QVector<GraphModel*>);
    void timeEventActivated(TimeEvent*);
    void anchorActivated(Anchor*);
    void arrowedTextActivated(ArrowedText*);
    void horizontalLevelActivated(HorizontalLevel*);
    void minimumWidth(int);

public slots:
    void update();
    void activateNextGraph();
    void activateGraph(int index);
    void activateAxis(int index);
    void treatDeletingRequest();
    void treatEventDeletingRequest();
    void treatAnchorDeletingRequest();
    void treatGraphToCommandConversionRequest();
    void treatGraphAddingToDataSourceRequest();
    void treatRetrievingNthBitRequest(int nBit);
    void treatRetrievingBitsRequest();
    void treatCommandAddingToEventSourceRequest();


    void enablePlotDeleting(bool);
    void saveActivatedItems();

    void treatPossibleVisibilityChanges(QTreeWidgetItem* item, int column);


private slots:
    void treatItemActivation( QTreeWidgetItem * item);

protected:
    void dropEvent(QDropEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);
    void mousePressEvent(QMouseEvent * event);

private:
    bool mIsPlotDeletingEnabled;
    PlotShowEditWidget *mPlotShowEditWidget;
    const PlotLayoutModel* mLayoutModel;
    QVector<AxisModel*> mAxisModelVector;
    QVector<GraphModel*> mGraphModelVector;
    QVector<TimeEvent*> mEventVector;
    QVector<Anchor*> mAnchorVector;
    QVector<ArrowedText*> mArrowedTextVector;
    QVector<HorizontalLevel*> mHorizontalLevelVector;

    //QVector<AxisModel*> mAxisModelVector;
   // QVector<QVector*>
    IPushButton* mKillPlotButton;
    QTreeWidgetItem* mPlotItem;
    QVector<QTreeWidgetItem*> mAxisItemVector;
    QVector<QTreeWidgetItem*> mGraphItemVector;
    QVector<IPushButton*> mKillButtonVector;
    QVector<QTreeWidgetItem*> mCommandItemVector;
    QVector<IPushButton*> mKillCommandButtonVector;
    QVector<QTreeWidgetItem*> mAnchorItemVector;
    QVector<IPushButton*> mKillAnchorButtonVector;
    QVector<QTreeWidgetItem*> mArrowedTextItemVector;
    QVector<IPushButton*> mKillArrowedTextButtonVector;
    QVector<QTreeWidgetItem*> mHorizontalLevelItemVector;
    QVector<IPushButton*> mKillHorizontalLevelButtonVector;

    QVector<QTreeWidgetItem*> mSavedActivatedItems;

    QMenu *mGraphContextMenu;
    QMenu *mCommandContextMenu;
    QSignalMapper *mNBitSignalMapper;
    QVector<const QObject*> previouslySelectedObjects;

    void initializeState();
    void clearInternalState();
    QVector<const QObject*> getSelectedObjects() const;
};

#endif // PLOTHIERARCHYWIDGET_H
