#include "widgettilemanager.h"
#include <QLayout>
#include <QSplitter>
#include <QDebug>
#include "mousepressnotifier.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>



template <typename T>
static void removeItemFromQVector(QVector<T> &vector, const T& elem) {
    int index = vector.indexOf(elem);
    if (index >= 0) {
        vector.remove(index);
    }
}

WidgetTileManager::WidgetTileManager(QWidget *parent) : QWidget(parent),
    mCurrentWidget(nullptr), mMousePressNotifier(nullptr),
    mIsCurrentWidgetMarked(false), mIsLeftButtonPressed(false),
    mMovingWidget(nullptr), mState(State::Normal), mIsMovingEnabled(false), mIsEqualizationEnabled(true),
    mMarkedWidget(nullptr), mNewLocationShowingWidget(nullptr)
{
    setLayout(new QHBoxLayout);
    mMousePressNotifier = new MousePressNotifier(this);
    connect(mMousePressNotifier, SIGNAL(clickOnWidget(QWidget*)),
            this, SLOT(setCurrentWidget(QWidget*)));

    mMarkedWidget = new MarkedWidget(this);
    mMarkedWidget->setMarkingColor(QColor(6, 150, 224));
    mMarkedWidget->setMarkingType(MarkedWidget::MarkingType::Corner);
    mMarkedWidget->hide();
    mNewLocationShowingWidget = new MarkedWidget(this);
    mNewLocationShowingWidget->setMarkingColor(Qt::green);
    mNewLocationShowingWidget->setMarkingType(MarkedWidget::MarkingType::Frame);
    mNewLocationShowingWidget->hide();
}

void WidgetTileManager::addWidget(QWidget *newWidget, Qt::Orientation orientation)
{
    if (mWidgetVector.size() == 0) {
        //delete redundant stuff
        if (layout()->count() != 0) {
            while (layout()->count()) {
                if (QWidget *widget = layout()->takeAt(0)->widget()) {
                    widget->setParent(nullptr);
                    delete widget;
                } else {
                    break; // it is supposed that control flow should not come hear
                }
            }
        }
        QSplitter *newSplitter = new QSplitter;
        newSplitter->setOrientation(orientation);
        newSplitter->addWidget(newWidget);
        layout()->addWidget(newSplitter);
        registerNewWidget(newWidget);
        return;
    }

    if (mCurrentWidget && mIsEqualizationEnabled) {
        newWidget->setGeometry(mCurrentWidget->geometry());
    }
    addWidgetToReferenceWidget(newWidget, mCurrentWidget, orientation);

}

void WidgetTileManager::removeWidget(QWidget *widgetToDelete)
{
    if (popWidget(widgetToDelete) == true) {
        widgetToDelete->deleteLater();

        if (mWidgetVector.indexOf(mCurrentWidget) == -1) {
            if (mWidgetVector.size() == 0) {
                mCurrentWidget = nullptr;
            } else {
                mCurrentWidget = mWidgetVector[0];
            }
        }
        emit currentWidgetChanged(mCurrentWidget);
    }
}

void WidgetTileManager::enableMoving(bool enabled)
{
    if (mIsMovingEnabled != enabled) {
        mIsMovingEnabled = enabled;
        if (mIsMovingEnabled == false) {
            mState = State::Normal;
            mMovingWidget = nullptr;
        }
    }
}

void WidgetTileManager::enableEqualization(bool enabled)
{
    mIsEqualizationEnabled = enabled;
}

void WidgetTileManager::markCurrentWidget(bool isMarked)
{
    mIsCurrentWidgetMarked = isMarked;
}

void WidgetTileManager::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (mCurrentWidget && mIsCurrentWidgetMarked && mWidgetVector.size() > 1) {
        mMarkedWidget->show();
        mMarkedWidget->setGeometry(QRect(mCurrentWidget->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0)),
                                         QSize(mCurrentWidget->width(), mCurrentWidget->height())));
        mMarkedWidget->raise();
    } else {
        mMarkedWidget->hide();
    }

    mNewLocationShowingWidget->hide();
    if (mState == State::Moving && mMovingWidget != widgetUnderMouse()) {

        QPainter painter(this);
        QPoint pos = mapFromGlobal(QCursor::pos());
        QPen pen(Qt::green);
        pen.setWidth(4);
        painter.setPen(pen);
        if (pos.x() < sBorderWidth) {
            mNewLocationShowingWidget->setGeometry(QRect(0, 0, sBorderWidth, height()));
            mMarkedWidget->raise();
            mNewLocationShowingWidget->show();
            return;
        } else if (pos.x() > width() - sBorderWidth ) {
            mNewLocationShowingWidget->setGeometry(QRect(width() - sBorderWidth, 0, width(), height()));
            mMarkedWidget->raise();
            mNewLocationShowingWidget->show();
            return;
        } else if (pos.y() < sBorderWidth) {
            mNewLocationShowingWidget->setGeometry(QRect(0, 0, width(), sBorderWidth));
            mMarkedWidget->raise();
            mNewLocationShowingWidget->show();
            return;
        } else if (pos.y() > height() - sBorderWidth) {
            mNewLocationShowingWidget->setGeometry(QRect(0, height() - sBorderWidth, width(), height()));
            mMarkedWidget->raise();
            mNewLocationShowingWidget->show();
            return;
        }

        QWidget *curWid = widgetUnderMouse();
        if (mWidgetVector.indexOf(curWid) != -1) {
            QPoint curWidgetCenter = curWid->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0)) + QPoint(curWid->width(), curWid->height())/2;
            double angle = 57.3 * std::atan2(-(pos.y() - curWidgetCenter.y()), pos.x() - curWidgetCenter.x());
            QPoint curWidgetOrigin     = curWid->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0));

            if (angle < 45. && angle > -45.) {
                mNewLocationShowingWidget->setGeometry(QRect(curWidgetOrigin.x() + curWid->width()/2, curWidgetOrigin.y(), curWid->width()/2, curWid->height()));
            } else if (angle < 135. && angle > 45.) {
                mNewLocationShowingWidget->setGeometry(QRect(curWidgetOrigin.x(), curWidgetOrigin.y(), curWid->width(), curWid->height()/2));
            } else if (angle < -45. && angle > -135.) {
                mNewLocationShowingWidget->setGeometry(QRect(curWidgetOrigin.x(), curWidgetOrigin.y() + curWid->height()/2, curWid->width(), curWid->height()/2));
            } else {
                mNewLocationShowingWidget->setGeometry(QRect(curWidgetOrigin.x(), curWidgetOrigin.y(), curWid->width()/2, curWid->height()));
            }
            mMarkedWidget->raise();
            mNewLocationShowingWidget->show();

//            collideRedundantSplitters();
            return;
        }


    }
}

void WidgetTileManager::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        mIsLeftButtonPressed = true;
        mButtonPressPoint = event->pos();
        mMovingWidget = widgetUnderMouse();
        mState = State::Normal;
    }
}

void WidgetTileManager::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if (mState == State::Normal && (event->pos()-mButtonPressPoint).manhattanLength() > 10) {
        mState = State::Moving;
    }
    if (mState == State::Moving && mMovingWidget != widgetUnderMouse()) {
        update();
    }

}

void WidgetTileManager::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (mState == State::Moving) {
        //drop to border
        mState = State::Normal;

        if (mMovingWidget == widgetUnderMouse())
            return;

        int xDrop = event->x();
        int yDrop = event->y();
        if (xDrop < sBorderWidth) {
            popWidget(mMovingWidget);
            addWidgetToReferenceWidget(mMovingWidget, headSplitter(), Qt::Horizontal, Order::Before);
            collideRedundantSplitters();
            return;
        } else if (xDrop > width() - sBorderWidth) {
            popWidget(mMovingWidget);
            addWidgetToReferenceWidget(mMovingWidget, headSplitter(), Qt::Horizontal, Order::After);
            collideRedundantSplitters();
            return;
        } else if (yDrop < sBorderWidth) {
            popWidget(mMovingWidget);
            addWidgetToReferenceWidget(mMovingWidget, headSplitter(), Qt::Vertical, Order::Before);
            collideRedundantSplitters();
            return;
        } else if (yDrop > height() - sBorderWidth) {
            popWidget(mMovingWidget);
            addWidgetToReferenceWidget(mMovingWidget, headSplitter(), Qt::Vertical, Order::After);
            collideRedundantSplitters();
            return;
        }

        QWidget *curWid = widgetUnderMouse();
        if (mWidgetVector.indexOf(curWid) != -1) {
            QPoint curWidgetCenter = curWid->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0)) + QPoint(curWid->width(), curWid->height())/2;
            double angle = 57.3 * std::atan2(-(event->y() - curWidgetCenter.y()), event->x() - curWidgetCenter.x());

            popWidget(mMovingWidget);
            if (angle < 45. && angle > -45.) {
                addWidgetToReferenceWidget(mMovingWidget, curWid, Qt::Horizontal, Order::After);
            } else if (angle < 135. && angle > 45.) {
                addWidgetToReferenceWidget(mMovingWidget, curWid, Qt::Vertical, Order::Before);
            } else if (angle < -45. && angle > -135.) {
                addWidgetToReferenceWidget(mMovingWidget, curWid, Qt::Vertical, Order::After);
            } else {
                addWidgetToReferenceWidget(mMovingWidget, curWid, Qt::Horizontal, Order::Before);
            }
//            collideRedundantSplitters();
            return;
        }
    }
}



void WidgetTileManager::setCurrentWidget(QWidget *widget)
{
    if (mWidgetVector.indexOf(widget) != -1) {
        if (mCurrentWidget != widget) {
            mCurrentWidget = widget;
            emit currentWidgetChanged(mCurrentWidget);
            //redraw to show new current widget
            if (mIsCurrentWidgetMarked) {
                update();
            }
        }
    }
}


void WidgetTileManager::addWidgetToReferenceWidget(QWidget *newWidget, QWidget *referenceWidget, Qt::Orientation orientation, Order order)
{
    if (referenceWidget == nullptr || newWidget == nullptr || newWidget == referenceWidget )
        return;

    //if referenceWidget is a splitter
    if (QSplitter *referenceSplitter = qobject_cast<QSplitter*>(referenceWidget)) {
        if (referenceSplitter == headSplitter()) {
            auto headSpl = headSplitter();
            layout()->removeWidget(headSpl);
            QSplitter *newSplitter = new QSplitter;
            layout()->addWidget(newSplitter);
            newSplitter->setOrientation(orientation);
            if (order == Order::Before) {
                newSplitter->addWidget(newWidget);
                newSplitter->addWidget(headSpl);
            } else {
                newSplitter->addWidget(headSpl);
                newSplitter->addWidget(newWidget);
            }
            registerNewWidget(newWidget);
        } else {
            if (QSplitter *parentSplitter = qobject_cast<QSplitter*>(referenceSplitter->parent())) {
                int index = parentSplitter->indexOf(referenceSplitter);
                if (index != -1) {
                    referenceSplitter->setParent(nullptr);
                    QSplitter *newSplitter = new QSplitter;
                    parentSplitter->insertWidget(index, newSplitter);
                    newSplitter->setOrientation(orientation);
                    if (order == Order::Before) {
                        newSplitter->addWidget(newWidget);
                        newSplitter->addWidget(referenceSplitter);
                    } else {
                        newSplitter->addWidget(referenceSplitter);
                        newSplitter->addWidget(newWidget);
                    }
                    registerNewWidget(newWidget);
                }
            }
        }
        return;
    }

    QSplitter *parentSplitter = qobject_cast<QSplitter*>(referenceWidget->parent());
    if (parentSplitter) {
        if (parentSplitter->count() == 1) {
            parentSplitter->setOrientation(orientation);
            parentSplitter->insertWidget(order == Order::After ? 1: 0, newWidget);
//                parentSplitter->addWidget(newWidget);
            parentSplitter->setSizes(QList<int>({1,1}));
            registerNewWidget(newWidget);
        } else {
            int currentIndex = parentSplitter->indexOf(referenceWidget);
            if (currentIndex != -1) {
//                referenceWidget->setParent(NULL);
//                QSplitter *newSplitter = new QSplitter;
//                newSplitter->setOrientation(orientation);
//                if (order == Order::After) {
//                    newSplitter->addWidget(referenceWidget);
//                    newSplitter->addWidget(newWidget);
//                } else {
//                    newSplitter->addWidget(newWidget);
//                    newSplitter->addWidget(referenceWidget);
//                }
//                parentSplitter->insertWidget(currentIndex, newSplitter);
//                registerNewWidget(newWidget);

                if (parentSplitter->orientation() != orientation) {
                    auto str = parentSplitter->sizes();
                    referenceWidget->setParent(NULL);
                    QSplitter *newSplitter = new QSplitter;
                    newSplitter->setOrientation(orientation);
                    if (order == Order::After) {
                        newSplitter->addWidget(referenceWidget);
                        newSplitter->addWidget(newWidget);
                    } else {
                        newSplitter->addWidget(newWidget);
                        newSplitter->addWidget(referenceWidget);
                    }
                    parentSplitter->insertWidget(currentIndex, newSplitter);
                    parentSplitter->setSizes(str);
                    registerNewWidget(newWidget);
                } else {
                    auto str = parentSplitter->sizes();
                    if (order == Order::After) {
                        parentSplitter->insertWidget(currentIndex+1, newWidget);
                        str.insert(currentIndex+1, str[currentIndex]);
                        parentSplitter->setSizes(str);
                    } else {
                        parentSplitter->insertWidget(currentIndex+0, newWidget);
                        str.insert(currentIndex, str[currentIndex]);
                        parentSplitter->setSizes(str);
                    }
                    registerNewWidget(newWidget);
                }
            }
        }
    }
}

void WidgetTileManager::registerNewWidget(QWidget *newWidget)
{
    newWidget->installEventFilter(mMousePressNotifier);
    if (mCurrentWidget != newWidget) {
        mCurrentWidget = newWidget;
        emit currentWidgetChanged(mCurrentWidget);
    }
    if (mWidgetVector.indexOf(newWidget) == -1) {
        mWidgetVector << newWidget;
    }
}


bool WidgetTileManager::popWidget(QWidget *widgetToPop)
{
    if (mWidgetVector.indexOf(widgetToPop) == -1) {
        return false;
    }
    QSplitter *headSpl = headSplitter();
    if (headSpl == nullptr)
        return false;

    bool ret = popWidgetFromSplitter(widgetToPop, headSpl);
    if (ret) {
        removeItemFromQVector(mWidgetVector, widgetToPop);
        if (mWidgetVector.size() > 0) {
            mCurrentWidget = mWidgetVector[0];
        }
        collideRedundantSplitters();
        update();
    }
    return ret;

}


bool WidgetTileManager::popWidgetFromSplitter(QWidget *widgetToPop, QSplitter *splitter)
{
    if (widgetToPop == nullptr || splitter == nullptr)
        return false;

    int index = splitter->indexOf(widgetToPop);
    if (index >= 0) {
        widgetToPop->setParent(nullptr);
        return true;
    } else {
        for (int i = 0; i < splitter->count(); ++i) {
            QSplitter* subSplitter = qobject_cast<QSplitter*>(splitter->widget(i));
            if (subSplitter) {
                bool result = popWidgetFromSplitter(widgetToPop, subSplitter);
                if (result == true)
                    return true;
            }
        }
    }
    return false;
}

QSplitter *WidgetTileManager::headSplitter()
{
    return qobject_cast<QSplitter*>(layout()->itemAt(0)->widget());
}

void WidgetTileManager::collideRedundantSplitters()
{
    if (QSplitter *headSpl = headSplitter()) {
        collideRedundantSubSplitters(headSpl);
        update();
    }
}

void WidgetTileManager::collideRedundantSubSplitters(QSplitter *splitter)
{
    if (splitter != nullptr) {
        QSet<Qt::Orientation> orientationsSet;
        orientationsSet << splitter->orientation();
        bool subSplittersWereFound = false;
        for (int i = 0; i < splitter->count(); ++i) {
            QSplitter* subSplitter = qobject_cast<QSplitter*>(splitter->widget(i));
            if (subSplitter) {
                if (subSplitter->count() == 0) {
                    subSplitter->setParent(nullptr);
                    delete subSplitter;
                    collideRedundantSubSplitters(splitter);
                    return;
                }
                subSplittersWereFound = true;
                orientationsSet << subSplitter->orientation();
            }
        }
        if (subSplittersWereFound == false) {
            return;
        }
        //case where colliding is needed
        if (orientationsSet.size() == 1) {
            for (int i = 0; i < splitter->count(); ++i) {
                QSplitter* subSplitter = qobject_cast<QSplitter*>(splitter->widget(i));
                if (subSplitter) {
                    if (subSplitter->count() == 0) {
                        subSplitter->setParent(nullptr);
                        delete subSplitter;
                    } else {
                        auto widg = subSplitter->widget(0);
                        widg->setParent(nullptr);
                        splitter->insertWidget(i, widg);
                    }
                    collideRedundantSubSplitters(splitter);
                    return;
                }
            }
        } else {
            for (int i = 0; i < splitter->count(); ++i) {
                QSplitter* subSplitter = qobject_cast<QSplitter*>(splitter->widget(i));
                if (subSplitter)
                    collideRedundantSubSplitters(subSplitter);
            }
        }
    }

}

QWidget *WidgetTileManager::widgetUnderMouse() const
{
    QWidget *wid = nullptr;
    if (mNewLocationShowingWidget->isVisible()) {
        auto v2 = mNewLocationShowingWidget->isVisible();
        mNewLocationShowingWidget->hide();
        wid = qApp->widgetAt(QCursor::pos());
        mNewLocationShowingWidget->raise();
        mNewLocationShowingWidget->setVisible(v2);
    } else {
        wid = qApp->widgetAt(QCursor::pos());
    }




    if (wid && mWidgetVector.indexOf(wid) != -1) {
        return wid;
    } else if (wid && wid == mMarkedWidget) {
        return mCurrentWidget;
    }
    return nullptr;

}





