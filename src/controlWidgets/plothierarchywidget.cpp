#include "plothierarchywidget.h"
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include "graph/graphiconengine.h"
#include <algorithm>
#include "commonWidgets/bitchoisedialog.h"
#include <QScrollBar>
#include "core/horizontallevel.h"
#include "commonWidgets/plotshoweditwidget.h"


PlotHierarchyWidget::PlotHierarchyWidget(QWidget *parent) :
    QTreeWidget(parent), mIsPlotDeletingEnabled(false), mLayoutModel(NULL), mKillPlotButton(NULL), mGraphContextMenu(NULL), mCommandContextMenu(NULL)
{

    setHeaderLabel("Plot hierarchy");
    //activation by 2 clicks
//    connect (this, SIGNAL (itemActivated ( QTreeWidgetItem *, int)),
//             this, SLOT (treatItemActivation(QTreeWidgetItem *)));
    //activation by one click
    connect (this, SIGNAL (itemClicked ( QTreeWidgetItem *, int)),
             this, SLOT (treatItemActivation(QTreeWidgetItem *)));


    setAcceptDrops(true);
    setDragEnabled(true);
//    setDragDropMode(QAbstractItemView::InternalMove);


    mGraphContextMenu = new QMenu(this);
    QAction *convertToCommandAction = new QAction(QString("Convert to command"), this);
    connect(convertToCommandAction, SIGNAL(triggered()),
            this, SLOT(treatGraphToCommandConversionRequest()));
    mGraphContextMenu->addAction(convertToCommandAction);
    QAction *addToDataSourceAction = new QAction(QString("Add to data source"), this);
    connect(addToDataSourceAction, SIGNAL(triggered()),
            this, SLOT(treatGraphAddingToDataSourceRequest()));
    mGraphContextMenu->addAction(addToDataSourceAction);

    mGraphContextMenu->addSeparator();
    QAction *getBitAction = new QAction(QString("Retrieve ... bits"), this);
    mGraphContextMenu->addAction(getBitAction);
    connect(getBitAction, SIGNAL(triggered()),
            this, SLOT(treatRetrievingBitsRequest()));

    mNBitSignalMapper = new QSignalMapper(this);
    for (int i = 0; i < 32; ++i) {
        QAction *getNBitAction = new QAction(QString("Retrieve " + QString::number(i) + "th bit"), this);
        mGraphContextMenu->addAction(getNBitAction);
        connect(getNBitAction, SIGNAL(triggered()), mNBitSignalMapper, SLOT(map()));
        mNBitSignalMapper->setMapping(getNBitAction, i);
    }
    connect(mNBitSignalMapper, SIGNAL(mapped(int)),
            this, SLOT(treatRetrievingNthBitRequest(int)));


//    QAction *getNBitAction = new QAction(QString("Retrieve Nth bit"), this);
//    connect(getNBitAction, SIGNAL(triggered()),
//            this, SLOT(treatGraphAddingToDataSourceRequest()));
//    mGraphContextMenu->addAction(getNBitAction);

    mCommandContextMenu = new QMenu(this);
    QAction *addToEventSourceAction = new QAction(QString("Add to event source"), this);
    connect(addToEventSourceAction, SIGNAL(triggered()),
            this, SLOT(treatCommandAddingToEventSourceRequest()));
    mCommandContextMenu->addAction(addToEventSourceAction);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(treatPossibleVisibilityChanges(QTreeWidgetItem*,int)));
}

void PlotHierarchyWidget::setPlotLayoutModel(const PlotLayoutModel *model)
{
    if (mLayoutModel != model) {
        clearInternalState();
        mLayoutModel = model;
        initializeState();
    }

}

void PlotHierarchyWidget::setPlotShowEditWidget(PlotShowEditWidget *widget)
{
    mPlotShowEditWidget = widget;
}

GraphModel* PlotHierarchyWidget::currentGraph()
{
    return    mGraphItemVector.indexOf(currentItem()) != -1 ?
                mGraphModelVector[mGraphItemVector.indexOf(currentItem())] : NULL;
}

AxisModel* PlotHierarchyWidget::currentAxis()
{
    return    mAxisItemVector.indexOf(currentItem()) != -1 ?
                mAxisModelVector[mAxisItemVector.indexOf(currentItem())] : NULL;
}

QVector<GraphModel *> PlotHierarchyWidget::selectedGraphs() const
{
    auto selectedObjects = getSelectedObjects();
    QVector<GraphModel *> ret;
    for (auto graph : mGraphModelVector) {
        if (selectedObjects.indexOf(graph) != -1) {
            ret.push_back(graph);
        }
    }
    return ret;
}



void PlotHierarchyWidget::update()
{
    int scrollOffset = verticalScrollBar()->value();
    previouslySelectedObjects = getSelectedObjects();
    auto model = mLayoutModel;
    clearInternalState();
    mLayoutModel = model;
    initializeState();
    //Checking in case we don't need to scroll because we were at the top previously.
    if (scrollOffset != 0) {
        verticalScrollBar()->setValue(scrollOffset);
    }

}

void PlotHierarchyWidget::activateNextGraph()
{

}

void PlotHierarchyWidget::activateGraph(int index)
{

//            QVector<QTreeWidgetItem*> mGraphItemVector;
//    int graphCounter = 0;
//    int axisIndex = 0;
//    int graphIndex = 0;
//    for (axisIndex = 0; axisIndex < mGraphItemVector.size(); ++axisIndex) {
//        for (graphIndex = 0; graphIndex < mGraphItemVector[axisIndex]->size(); ++graphIndex) {
//            if (graphCounter == index)
//                break;
//            else
//                ++graphCounter;
//        }
//    }

//    if (axisIndex < mGraphItemVector.size() && graphIndex < mGraphItemVector[axisIndex]->size()) {

//    }
    if (index >=0 && index < mGraphItemVector.size()) {
        setCurrentItem(mGraphItemVector[index]);
        emit graphActivated(mGraphModelVector[index]);
    }
}

void PlotHierarchyWidget::activateAxis(int index)
{
    if (index >=0 && index < mAxisItemVector.size()) {
        setCurrentItem(mAxisItemVector[index]);
        emit axesVectorActivated(QVector<AxisModel*> {mAxisModelVector[index]});

    }
}

void PlotHierarchyWidget::treatDeletingRequest()
{
    IPushButton * senderButton = NULL;

    if ((senderButton = qobject_cast<IPushButton *>(sender()))) {

        if (mKillPlotButton && senderButton == mKillPlotButton) {
            auto layoutModel = mLayoutModel;
            setPlotLayoutModel(nullptr);
            mPlotShowEditWidget->removeLayoutModel(const_cast<PlotLayoutModel*>(layoutModel));
            return;
        }

        int index = -1;
        if ((index = mKillButtonVector.indexOf(senderButton)) != -1) {
            if (mSavedActivatedItems.indexOf(mGraphItemVector[index]) == -1) {
                mGraphModelVector[index]->treatDeletingRequest();
                update();
                return;
            }
        } else if ((index = mKillCommandButtonVector.indexOf(senderButton)) != -1) {
            if (mSavedActivatedItems.indexOf(mCommandItemVector[index]) == -1) {
                const_cast<PlotLayoutModel*>(mLayoutModel)->removeCommand(mEventVector[index]);
                update();
                return;
            }
        } else if ((index = mKillAnchorButtonVector.indexOf(senderButton)) != -1) {
            if (mSavedActivatedItems.indexOf(mAnchorItemVector[index]) == -1) {
                const_cast<PlotLayoutModel*>(mLayoutModel)->removeAnchor(mAnchorVector[index]);
                update();
                return;
            }
        } else if ((index = mKillArrowedTextButtonVector.indexOf(senderButton)) != -1) {
            if (mSavedActivatedItems.indexOf(mArrowedTextItemVector[index]) == -1) {
                const_cast<PlotLayoutModel*>(mLayoutModel)->removeArrowedText(mArrowedTextVector[index]);
                update();
                return;
            }
        } else if ((index = mKillHorizontalLevelButtonVector.indexOf(senderButton)) != -1) {
            if (mSavedActivatedItems.indexOf(mHorizontalLevelItemVector[index]) == -1) {
                const_cast<PlotLayoutModel*>(mLayoutModel)->removeHorizontalLevel(mHorizontalLevelVector[index]);
                update();
                return;
            }
        }


        //must delete all objects simultaneously, if delete them sequentially it won't work properly for some reasons
        QVector<GraphModel*>    graphsToDelete;
        QVector<TimeEvent*> commandsToDelete;
        QVector<Anchor*>        anchorToDelete;
        QVector<ArrowedText*>   arrTextToDelete;
        QVector<HorizontalLevel*>   horLevelToDelete;
        for (int i = 0; i <  mGraphItemVector.size(); ++ i) {
            if (mSavedActivatedItems.indexOf(mGraphItemVector[i]) != -1) {
                graphsToDelete.push_back(mGraphModelVector[i]);
            }
        }
        for (int i = 0; i <  mCommandItemVector.size(); ++ i) {
            if (mSavedActivatedItems.indexOf(mCommandItemVector[i]) != -1) {
                commandsToDelete.push_back(mEventVector[i]);
            }
        }
        for (int i = 0; i <  mAnchorItemVector.size(); ++ i) {
            if (mSavedActivatedItems.indexOf(mAnchorItemVector[i]) != -1) {
                anchorToDelete.push_back(mAnchorVector[i]);
            }
        }
        for (int i = 0; i <  mArrowedTextItemVector.size(); ++ i) {
            if (mSavedActivatedItems.indexOf(mArrowedTextItemVector[i]) != -1) {
                arrTextToDelete.push_back(mArrowedTextVector[i]);
            }
        }
        for (int i = 0; i <  mHorizontalLevelItemVector.size(); ++ i) {
            if (mSavedActivatedItems.indexOf(mHorizontalLevelItemVector[i]) != -1) {
                horLevelToDelete.push_back(mHorizontalLevelVector[i]);
            }
        }
        const_cast<PlotLayoutModel*>(mLayoutModel)->removeObjects(graphsToDelete, commandsToDelete, anchorToDelete, arrTextToDelete, horLevelToDelete);
        update();
    }
}

void PlotHierarchyWidget::treatEventDeletingRequest()
{
    IPushButton * senderObject = NULL;
    if ((senderObject = qobject_cast<IPushButton *>(sender()))) {
        int index = mKillCommandButtonVector.indexOf(senderObject);
        if (index != -1) {
            const_cast<PlotLayoutModel*>(mLayoutModel)->removeCommand(mEventVector[index]);
        }
    }
}

void PlotHierarchyWidget::treatAnchorDeletingRequest()
{
    IPushButton * senderObject = NULL;
    if (((senderObject = qobject_cast<IPushButton *>(sender())))) {
        int index = mKillAnchorButtonVector.indexOf(senderObject);
        if (index != -1) {
            const_cast<PlotLayoutModel*>(mLayoutModel)->removeAnchor(mAnchorVector[index]);
        }
    }
}

void PlotHierarchyWidget::treatGraphToCommandConversionRequest()
{
    QTreeWidgetItem *curItem = currentItem();
    int itemIndex = mGraphItemVector.indexOf(curItem);
    if (itemIndex != -1) {
        const_cast<PlotLayoutModel*>(mLayoutModel)->convertGraphToCommand(mGraphModelVector[itemIndex]);
    }
}

void PlotHierarchyWidget::treatGraphAddingToDataSourceRequest()
{
    QTreeWidgetItem *curItem = currentItem();
    int itemIndex = mGraphItemVector.indexOf(curItem);
    if (itemIndex != -1) {
        const_cast<PlotLayoutModel*>(mLayoutModel)->addToGlobalDataSource(mGraphModelVector[itemIndex]);
    }
}

void PlotHierarchyWidget::treatRetrievingNthBitRequest(int nBit)
{

    QTreeWidgetItem *curItem = currentItem();
    int itemIndex = mGraphItemVector.indexOf(curItem);
    if (itemIndex != -1) {
        const_cast<PlotLayoutModel*>(mLayoutModel)->addBitGraph(mGraphModelVector[itemIndex], nBit, 1);
    }
}

void PlotHierarchyWidget::treatRetrievingBitsRequest()
{
    QTreeWidgetItem *curItem = currentItem();
    int itemIndex = mGraphItemVector.indexOf(curItem);
    if (itemIndex != -1) {
        BitChoiseDialog bitChoiseDialog;
        if (bitChoiseDialog.exec() == QDialog::Accepted) {
              auto checkedBits = bitChoiseDialog.checkedBits();
              if (bitChoiseDialog.shouldCombineBits()) {
                  using std::tuple;
                  QVector<tuple<int/*position*/,int/*number of bits*/>> bitNumberVector;
                  for (const auto& x : checkedBits) {
                      if (bitNumberVector.isEmpty() ||
                          (x - std::get<0>(bitNumberVector.back()) - std::get<1>(bitNumberVector.back()) != 0)) {
                          bitNumberVector.push_back(tuple<int,int>(x, 1));
                      } else {
                          auto last = bitNumberVector.back();
                          bitNumberVector.pop_back();
                          ++std::get<1>(last);
                          bitNumberVector.push_back(last);
                      }
                  }
                  for (auto posNumberTuple : bitNumberVector) {
                      const_cast<PlotLayoutModel*>(mLayoutModel)->addBitGraph(mGraphModelVector[itemIndex],
                                                 std::get<0>(posNumberTuple), std::get<1>(posNumberTuple));
                  }

              } else {
                  for (auto nBit : checkedBits) {
                      const_cast<PlotLayoutModel*>(mLayoutModel)->addBitGraph(mGraphModelVector[itemIndex], nBit, 1);
                  }
              }
        }
    }

}

void PlotHierarchyWidget::treatCommandAddingToEventSourceRequest()
{
    QTreeWidgetItem *curItem = currentItem();
    int itemIndex = mCommandItemVector.indexOf(curItem);
    if (itemIndex != -1) {
        const_cast<PlotLayoutModel*>(mLayoutModel)->addToGlobalEventSource(mEventVector[itemIndex]);
    }
}

void PlotHierarchyWidget::enablePlotDeleting(bool enable)
{
   mIsPlotDeletingEnabled = enable;
}

void PlotHierarchyWidget::saveActivatedItems()
{
    mSavedActivatedItems = QVector<QTreeWidgetItem *>::fromList(selectedItems());
}

void PlotHierarchyWidget::treatPossibleVisibilityChanges(QTreeWidgetItem *item, int /*column*/)
{
    if (mLayoutModel != NULL) {
        if (mGraphModelVector.size() == mGraphItemVector.size()) {
            int i = mGraphItemVector.indexOf(item);
            if (i != -1) {
                if (mGraphModelVector[i]->isVisible()  != (mGraphItemVector[i]->checkState(0) == Qt::Checked)) {
                    mGraphModelVector[i]->setVisibility(mGraphItemVector[i]->checkState(0) == Qt::Checked);
                }
            }
        }

    }

}

void PlotHierarchyWidget::treatItemActivation(QTreeWidgetItem *item)
{
    QVector<QTreeWidgetItem *> activatedItems = QVector<QTreeWidgetItem *>::fromList(selectedItems());
    if (activatedItems.size() > 1) {
        QVector<GraphModel*> activatedGraphs;
        QVector<AxisModel*> activatedAxes;
        for (const auto &item : activatedItems) {
            int index = 0;
            if ((index = mGraphItemVector.indexOf(item)) != -1) {
                activatedGraphs.push_back(mGraphModelVector[index]);
            }
            if ((index = mAxisItemVector.indexOf(item)) != -1) {
                activatedAxes.push_back(mAxisModelVector[index]);
            }
        }
        if (activatedGraphs.size() > 0) {
            emit graphVectorActivated(activatedGraphs);
            return;
        }
        if (activatedAxes.size() > 0) {
            emit axesVectorActivated(activatedAxes);
            return;
        }
    }


    if (selectedItems().indexOf(item) == -1) {
        return;
    }

    for (int axisIndex = 0; axisIndex < mAxisItemVector.size(); ++axisIndex) {
        if (item == mAxisItemVector[axisIndex]) {
//            qDebug() << mAxisModelVector[axisIndex] << "was chosen";
            clearFocus();
            emit axesVectorActivated(QVector<AxisModel*> {mAxisModelVector[axisIndex]});
            return;
        }
    }
    for (int graphIndex = 0; graphIndex < mGraphItemVector.size(); ++graphIndex) {
        if (item == mGraphItemVector[graphIndex]) {
            clearFocus();
            emit graphActivated(mGraphModelVector[graphIndex]);
            return;
        }
    }
    for (int commandIndex = 0; commandIndex < mCommandItemVector.size(); ++commandIndex) {
        if (item == mCommandItemVector[commandIndex]) {
            clearFocus();
            emit timeEventActivated(mEventVector[commandIndex]);
            return;
        }
    }
    for (int anchorIndex = 0; anchorIndex < mAnchorItemVector.size(); ++anchorIndex) {
        if (item == mAnchorItemVector[anchorIndex]) {
            clearFocus();
            emit anchorActivated(mAnchorVector[anchorIndex]);
            return;
        }
    }
    for (int arrTextIndex = 0; arrTextIndex < mArrowedTextItemVector.size(); ++arrTextIndex) {
        if (item == mArrowedTextItemVector[arrTextIndex]) {
            clearFocus();
            emit arrowedTextActivated(mArrowedTextVector[arrTextIndex]);
            return;
        }
    }
    for (int horLevelIndex = 0; horLevelIndex < mHorizontalLevelItemVector.size(); ++horLevelIndex) {
        if (item == mHorizontalLevelItemVector[horLevelIndex]) {
            clearFocus();
            emit horizontalLevelActivated(mHorizontalLevelVector[horLevelIndex]);
            return;
        }
    }

}

void PlotHierarchyWidget::dropEvent(QDropEvent *event)
{
   QTreeWidgetItem *dragItem =  currentItem();
   QTreeWidgetItem *dropItem = itemAt(event->pos());

   QVector<QTreeWidgetItem *> activatedItems = QVector<QTreeWidgetItem *>::fromList(selectedItems());
   //trying to drop many items
   if (activatedItems.size() > 1) {
        QVector<QTreeWidgetItem *> activatedGraphItems;
        for (const auto& item : activatedItems) {
            if (mGraphItemVector.indexOf(item) != -1) {
                activatedGraphItems.push_back(item);
            }
        }
        if (activatedGraphItems.size() != 0) {
            QVector<GraphModel *> activatedGraphs;
            foreach (const auto & dragItem, activatedGraphItems) {
                activatedGraphs.push_back(mGraphModelVector[mGraphItemVector.indexOf(dragItem)]);
            }

            // moving graph to another existing axis
            if (mAxisItemVector.indexOf(dropItem) != -1) {
                 AxisModel* axisTarget = mAxisModelVector[mAxisItemVector.indexOf(dropItem)];
                 const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphVectorToAxis(activatedGraphs, axisTarget);
                 return;
            }

            // moving graph to another existing axis, determined by one of its graphs
            if (mGraphItemVector.indexOf(dropItem) != -1) {
                 auto parentItemForDrop = dropItem->parent();
                 if (mAxisItemVector.indexOf(parentItemForDrop) != -1) {
                     AxisModel* axisTarget = mAxisModelVector[mAxisItemVector.indexOf(parentItemForDrop)];
                     const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphVectorToAxis(activatedGraphs, axisTarget);
                     return;
                 }
            }

            // moving graph to new axis at the top
            if (dropItem == mPlotItem) {
                const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphVectorToNewAxis(activatedGraphs, 0);
                return;
            }

             // moving graph to new axis at the bottom
            if (!dropItem) {

//               GraphModel* graphSource = mGraphModelVector[mGraphItemVector.indexOf(dragItem)];
               const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphVectorToNewAxis(activatedGraphs, 1);
               return;
            }
        }
   }



   //old stuff

   if (dragItem && dropItem) {
       qDebug() << "dragItem" << dragItem->text(0);
       qDebug() << "dropItem" << dropItem->text(0);

       qDebug() << "mGraphItemVector.indexOf(dragItem)" << mGraphItemVector.indexOf(dragItem);
       qDebug() << "mAxisItemVector.indexOf(dropItem)" << mAxisItemVector.indexOf(dropItem);

       // moving graph to another existing axis
       if (mGraphItemVector.indexOf(dragItem) != -1 && mAxisItemVector.indexOf(dropItem) != -1) {
            GraphModel* graphSource = mGraphModelVector[mGraphItemVector.indexOf(dragItem)];
            AxisModel* axisTarget = mAxisModelVector[mAxisItemVector.indexOf(dropItem)];
            const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphToAxis(graphSource, axisTarget);
            return;
       }

       // moving graph to another existing axis(axis is determined by one of the graph in it)
       if (mGraphItemVector.indexOf(dragItem) != -1 && mGraphItemVector.indexOf(dropItem) != -1) {
            auto parentItemForDrop = dropItem->parent();
            if (mAxisItemVector.indexOf(parentItemForDrop) != -1) {
                GraphModel* graphSource = mGraphModelVector[mGraphItemVector.indexOf(dragItem)];
                AxisModel* axisTarget = mAxisModelVector[mAxisItemVector.indexOf(parentItemForDrop)];
                const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphToAxis(graphSource, axisTarget);
                return;
            }
       }



       // moving axis
       if (mAxisItemVector.indexOf(dragItem) != -1 && mAxisItemVector.indexOf(dropItem) != -1) {
            AxisModel* axisSource = mAxisModelVector[mAxisItemVector.indexOf(dragItem)];
            AxisModel* axisTarget = mAxisModelVector[mAxisItemVector.indexOf(dropItem)];
            const_cast<PlotLayoutModel*>(mLayoutModel)->moveAxis(axisSource, axisTarget);
            return;
       }

       // moving graph to new axis at the top
       if (mGraphItemVector.indexOf(dragItem) != -1 && dropItem == mPlotItem) {
           GraphModel* graphSource = mGraphModelVector[mGraphItemVector.indexOf(dragItem)];
           const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphToNewAxis(graphSource, 0);
           return;
       }

       // merging axes
       if (mAxisItemVector.indexOf(dragItem) != -1 && mGraphItemVector.indexOf(dropItem) != -1) {
            GraphModel* graphTarget = mGraphModelVector[mGraphItemVector.indexOf(dropItem)];
            AxisModel* axisSource = mAxisModelVector[mAxisItemVector.indexOf(dragItem)];
            AxisModel* axisTarget = NULL;
            foreach (auto axis, mAxisModelVector) {
                if (axis->contains(graphTarget)) {
                    axisTarget = axis;
                    const_cast<PlotLayoutModel*>(mLayoutModel)->mergeAxis(axisSource, axisTarget);
                    return;
                }
            }
       }

    } else if (dragItem && !dropItem) {
            // moving graph to new axis at the bottom
           if (mGraphItemVector.indexOf(dragItem) != -1) {
                GraphModel* graphSource = mGraphModelVector[mGraphItemVector.indexOf(dragItem)];
                const_cast<PlotLayoutModel*>(mLayoutModel)->moveGraphToNewAxis(graphSource, 1);
                return;
           }
    }



}

void PlotHierarchyWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem * item = itemAt(event->pos());
    if (mGraphItemVector.indexOf(item) != -1) {
        mGraphContextMenu->exec(event->globalPos());
    } else if (mCommandItemVector.indexOf(item) != -1) {
        mCommandContextMenu->exec(event->globalPos());
    }

}

void PlotHierarchyWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    clearFocus();
}

//void PlotHierarchyWidget::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "mousePressEvent------";

//    QWidget * widgetUnder = childAt (event->pos());
//    if (mKillButtonVector.indexOf(qobject_cast<IPushButton*>(widgetUnder)) != -1) {
//        qDebug() << "found!!!!!!!!";
//    }



//    QTreeWidget::mousePressEvent(event);
//}



void PlotHierarchyWidget::clearInternalState() {
    clear();
    if (mLayoutModel != NULL) {

        disconnect(mLayoutModel, SIGNAL(layoutModelChanged()),
                this, SLOT(update()));
        disconnect(mLayoutModel, SIGNAL(layoutAppearanceChanged()),
                this, SLOT(update()));
        disconnect(mLayoutModel, SIGNAL(anchorsChanged()),
                this, SLOT(update()));

        mLayoutModel = NULL;
        mPlotItem = NULL;


        mAxisModelVector.clear();
        mGraphModelVector.clear();
        mAxisItemVector.clear();
        mGraphItemVector.clear();

        mEventVector.clear();
        mCommandItemVector.clear();

        mAnchorVector.clear();
        mAnchorItemVector.clear();

        mArrowedTextVector.clear();
        mArrowedTextItemVector.clear();

        mHorizontalLevelVector.clear();
        mHorizontalLevelItemVector.clear();


        if (mKillPlotButton) {
            mKillPlotButton->setParent(NULL);
            delete mKillPlotButton;
            mKillPlotButton = nullptr;
        }

        foreach (auto btn, mKillButtonVector) {
            btn->setParent(NULL);
            delete btn;
        }
        mKillButtonVector.clear();

        foreach (auto btn, mKillCommandButtonVector) {
            btn->setParent(NULL);
            delete btn;
        }
        mKillCommandButtonVector.clear();

        foreach (auto btn, mKillAnchorButtonVector) {
            btn->setParent(NULL);
            delete btn;
        }
        mKillAnchorButtonVector.clear();

        foreach (auto btn, mKillArrowedTextButtonVector) {
            btn->setParent(NULL);
            delete btn;
        }
        mKillArrowedTextButtonVector.clear();

        foreach (auto btn, mKillHorizontalLevelButtonVector) {
            btn->setParent(NULL);
            delete btn;
        }
        mKillHorizontalLevelButtonVector.clear();
    }

}

QVector<const QObject *> PlotHierarchyWidget::getSelectedObjects() const
{
    QVector<const QObject *> selectedObjects;

    for (int i = 0; i < mGraphItemVector.size(); ++i) {
        if (mGraphItemVector[i]->isSelected()) {
            selectedObjects.push_back(mGraphModelVector[i]);
        }
    }

    for (int i = 0; i < mAxisItemVector.size(); ++i) {
        if (mAxisItemVector[i]->isSelected()) {
            selectedObjects.push_back(mAxisModelVector[i]);
        }
    }

    for (int i = 0; i < mCommandItemVector.size(); ++i) {
        if (mCommandItemVector[i]->isSelected()) {
            selectedObjects.push_back(mEventVector[i]);
        }
    }

    for (int i = 0; i < mAnchorItemVector.size(); ++i) {
        if (mAnchorItemVector[i]->isSelected()) {
            selectedObjects.push_back(mAnchorVector[i]);
        }
    }

    for (int i = 0; i < mArrowedTextItemVector.size(); ++i) {
        if (mArrowedTextItemVector[i]->isSelected()) {
            selectedObjects.push_back(mArrowedTextVector[i]);
        }
    }

    for (int i = 0; i < mHorizontalLevelItemVector.size(); ++i) {
        if (mHorizontalLevelItemVector[i]->isSelected()) {
            selectedObjects.push_back(mHorizontalLevelVector[i]);
        }
    }

    return selectedObjects;
}

void PlotHierarchyWidget::initializeState()
{

    if (mLayoutModel != NULL) {
        connect(mLayoutModel, SIGNAL(layoutModelChanged()),
                this, SLOT(update()));
        connect(mLayoutModel, SIGNAL(layoutAppearanceChanged()),
                this, SLOT(update()));
        connect(mLayoutModel, SIGNAL(anchorsChanged()),
                this, SLOT(update()));
        QStringList strL;
        strL << "Plot hierarchy" << "";
        setHeaderLabels(strL);

        mPlotItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Plot frame")));
        addTopLevelItem(mPlotItem);

        if (mIsPlotDeletingEnabled) {
            mKillPlotButton = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//            mKillPlotButton->setStyleSheet("IPushButton { border : none }");
            mKillPlotButton->setHierarchyWidget(this);
            mKillPlotButton->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
            mKillPlotButton->setFixedWidth(20);
            mKillPlotButton->setFixedHeight(20);
            setItemWidget(mPlotItem,1, mKillPlotButton);
            connect(mKillPlotButton, SIGNAL(clicked()),
                    this, SLOT(treatDeletingRequest()));
        } else {
            mKillPlotButton = nullptr;
        }


        //to evaluate width of 0 column
        QFont myFont = QApplication::font();
        QFontMetrics fm(myFont);
        int maxWidth = 10;
        //
        mAxisModelVector = mLayoutModel->getAxisModelVector();
        mGraphModelVector.clear();
        int currGraphNumber = 0;
        for (int axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
            QTreeWidgetItem* newAxisItem = new QTreeWidgetItem(mPlotItem, QStringList(QString("Axis: %1").arg(QString::number(axisIndex + 1) )));
            mAxisItemVector.push_back(newAxisItem);
            if (previouslySelectedObjects.indexOf(mAxisModelVector[axisIndex]) != -1) {
                newAxisItem->setSelected(true);
            }
            auto graphVector = mAxisModelVector[axisIndex]->getGraphModelVector();
            for (int graphIndex = 0; graphIndex < graphVector.size(); ++graphIndex) {
                QString graphPrefix = graphVector[graphIndex]->graphType() == GraphModel::GraphType::Function ? "g" : "c";
                QTreeWidgetItem* newGraphItem = new QTreeWidgetItem(newAxisItem,
                                               QStringList(QString(graphPrefix + "%1: ").arg(QString::number(++currGraphNumber)) + graphVector[graphIndex]->getName()));

                if (graphVector[graphIndex]->isVisible()) {
                    newGraphItem->setCheckState(0,Qt::Checked);
                } else {
                    newGraphItem->setCheckState(0,Qt::Unchecked);
                }

                //icon for item
                GraphIconEngine *eng = new GraphIconEngine(graphVector[graphIndex]);
                QIcon icon(eng);
                newGraphItem->setIcon (0,icon);

                if (maxWidth < fm.width(QString("g%1: ").arg(QString::number(currGraphNumber)) + graphVector[graphIndex]->getName()))
                    maxWidth = fm.width(QString("g%1: ").arg(QString::number(currGraphNumber)) + graphVector[graphIndex]->getName());

                mGraphItemVector.push_back(newGraphItem);
                mGraphModelVector.push_back(const_cast<GraphModel*>(graphVector[graphIndex]));
                if (previouslySelectedObjects.indexOf(graphVector[graphIndex]) != -1) {
                    newGraphItem->setSelected(true);
                }

                IPushButton *btn = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//                btn->setStyleSheet("IPushButton { border : none }");
                btn->setHierarchyWidget(this);
                btn->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
                btn->setFixedWidth(20);
                btn->setFixedHeight(20);
                setItemWidget(newGraphItem,1, btn);
                mKillButtonVector.push_back(btn);
//                connect(btn, SIGNAL(clicked()),
//                        graphVector[graphIndex], SLOT(treatDeletingRequest()));

//                connect(btn, SIGNAL(clicked()),
//                        this, SLOT(treatDeletingRequest()));
                connect(btn, SIGNAL(clicked()),
                        this, SLOT(treatDeletingRequest()));

            }

             auto arrowedTextVector = mAxisModelVector[axisIndex]->arrowedTextVector();
             for (auto arrowedText : arrowedTextVector) {
                 QTreeWidgetItem* newArrowedTextItem = new QTreeWidgetItem(newAxisItem,
                                                QStringList(arrowedText->text()));
                 mArrowedTextItemVector.push_back(newArrowedTextItem);
                 mArrowedTextVector.push_back(arrowedText);

                 //icon for item
                 GraphIconEngine *eng = new GraphIconEngine(arrowedText);
                 QIcon icon(eng);
                 newArrowedTextItem->setIcon (0,icon);
                 if (previouslySelectedObjects.indexOf(arrowedText) != -1) {
                     newArrowedTextItem->setSelected(true);
                 }

                 IPushButton *btn = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//                 btn->setStyleSheet("IPushButton { border : none }");
                 btn->setHierarchyWidget(this);
                 btn->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
                 btn->setFixedWidth(20);
                 btn->setFixedHeight(20);
                 setItemWidget(newArrowedTextItem,1, btn);
                 mKillArrowedTextButtonVector.push_back(btn);
                 connect(btn, SIGNAL(clicked()),
                         this, SLOT(treatDeletingRequest()));
             }

             auto horizontalLevelVector = mAxisModelVector[axisIndex]->horizontalLevelVector();
             for (auto level : horizontalLevelVector) {
                 QTreeWidgetItem* newHorizontalLevelItem = new QTreeWidgetItem(newAxisItem,
                                                QStringList(level->label()));
                 mHorizontalLevelItemVector.push_back(newHorizontalLevelItem);
                 mHorizontalLevelVector.push_back(level);

                 //icon for item
                 GraphIconEngine *eng = new GraphIconEngine(level);
                 QIcon icon(eng);
                 newHorizontalLevelItem->setIcon (0,icon);
                 if (previouslySelectedObjects.indexOf(level) != -1) {
                     newHorizontalLevelItem->setSelected(true);
                 }

                 IPushButton *btn = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//                 btn->setStyleSheet("IPushButton { border : none }");
                 btn->setHierarchyWidget(this);
                 btn->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
                 btn->setFixedWidth(20);
                 btn->setFixedHeight(20);
                 setItemWidget(newHorizontalLevelItem,1, btn);
                 mKillHorizontalLevelButtonVector.push_back(btn);
                 connect(btn, SIGNAL(clicked()),
                         this, SLOT(treatDeletingRequest()));
             }
        }

        mEventVector = mLayoutModel->commandsVector();
        if (mAxisModelVector.size() != 0) {
            for (auto command : mEventVector) {
                QTreeWidgetItem* newEventItem = new QTreeWidgetItem(mPlotItem, QStringList(command->name()));

                //icon for item
                GraphIconEngine *eng = new GraphIconEngine(command);
                QIcon icon(eng);
                newEventItem->setIcon (0,icon);

                mCommandItemVector.push_back(newEventItem);
                if (previouslySelectedObjects.indexOf(command) != -1) {
                    newEventItem->setSelected(true);
                }

                IPushButton *btn = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//                btn->setStyleSheet("IPushButton { border : none }");
                btn->setHierarchyWidget(this);
                btn->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
                btn->setFixedWidth(20);
                btn->setFixedHeight(20);
                setItemWidget(newEventItem,1, btn);
                mKillCommandButtonVector.push_back(btn);
//                connect(btn, SIGNAL(clicked()),
//                        this, SLOT(treatEventDeletingRequest()));

                connect(btn, SIGNAL(clicked()),
                        this, SLOT(treatDeletingRequest()));
            }
        }


        mAnchorVector = mLayoutModel->anchorVector();
        if (mAxisModelVector.size() != 0) {
            for (auto anchor : mAnchorVector) {
                QTreeWidgetItem* newAnchorItem = new QTreeWidgetItem(mPlotItem, QStringList(anchor->name()));

                //icon for item
                GraphIconEngine *eng = new GraphIconEngine(anchor);
                QIcon icon(eng);
                newAnchorItem->setIcon (0,icon);

                mAnchorItemVector.push_back(newAnchorItem);
                if (previouslySelectedObjects.indexOf(anchor) != -1) {
                    newAnchorItem->setSelected(true);
                }

                IPushButton *btn = new IPushButton(/*global::getStyledIcon("close"),"", */this);
//                btn->setStyleSheet("IPushButton { border : none }");
                btn->setHierarchyWidget(this);
                btn->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum);
                btn->setFixedWidth(20);
                btn->setFixedHeight(20);
                setItemWidget(newAnchorItem,1, btn);
                mKillAnchorButtonVector.push_back(btn);
//                connect(btn, SIGNAL(clicked()),
//                        this, SLOT(treatAnchorDeletingRequest()));

                connect(btn, SIGNAL(clicked()),
                        this, SLOT(treatDeletingRequest()));
            }
        }



//        setColumnWidth(0, width() - 30);
//        setColumnWidth(0, 400);

        setColumnWidth(0, maxWidth+115);
        setColumnWidth(1, 15);
//        setMinimumWidth(maxWidth+115);


        //26/04/16
//        auto newGeometry = geometry();
//        newGeometry.setWidth(maxWidth+140);
//        setGeometry(newGeometry);
        setMinimumWidth(maxWidth+135);







        emit minimumWidth(maxWidth+140);
//        qDebug() << "______maxWidth" << maxWidth;

//        QWidget *dualPushButtons = new QWidget();
//        QHBoxLayout *hLayout = new QHBoxLayout();
//        hLayout->addWidget(new QPushButton("Button1"));
//        hLayout->addWidget(new QPushButton("Button2"));
//        dualPushButtons->setLayout(hLayout);
//        ui->treeWidget->setItemWidget(items.value(1),0,dualPushButtons);

//        if (mGraphItemVector.size() != 0) {
////            QLabel *label = new QLabel;
////            label->setText("<font color=red>XXXXXXX</font>");
////            label->setAutoFillBackground (true);
////            setItemWidget (mGraphItemVector[0], 0, label);

//            GraphRepresentationWidget *widget = new GraphRepresentationWidget;
//            setItemWidget (mGraphItemVector[0], 1, widget);
//        }

//        /*setHorizontalScrollBarPolicy*/(Qt::ScrollBarAlwaysOff)	;



        expandAll();

    }
}
