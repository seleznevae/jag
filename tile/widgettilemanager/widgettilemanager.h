#ifndef WIDGETTILEMANAGER_H
#define WIDGETTILEMANAGER_H

#include <QWidget>
#include "markedwidget.h"

class MousePressNotifier;
class QSplitter;


class WidgetTileManager : public QWidget
{
    Q_OBJECT
public:
    enum class State {Normal,
                      Moving,
                     };
    enum class Order {Before,
                      After,
                     };
    explicit WidgetTileManager(QWidget *parent = 0);

    QWidget* currentWidget() const { return mCurrentWidget; }
    bool isCurrentWidgetMarked() const { return mIsCurrentWidgetMarked; }
    bool isMovingEnabled() const { return mIsMovingEnabled; }
    bool isEqualiztionEnabled() const { return mIsEqualizationEnabled; }

    void addWidget(QWidget* newWidget, Qt::Orientation orientation = Qt::Horizontal);
    void removeWidget(QWidget*);
    void enableMoving(bool);
    void enableEqualization(bool);

    void markCurrentWidget(bool);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void currentWidgetChanged(QWidget*);
public slots:
    void setCurrentWidget(QWidget*);

private:
    QWidget *mCurrentWidget;
    MousePressNotifier *mMousePressNotifier;
    bool mIsCurrentWidgetMarked;
    QVector<QWidget*> mWidgetVector;
    bool mIsLeftButtonPressed;
    QPoint mButtonPressPoint;
    QWidget *mMovingWidget;
    State mState;
    bool mIsMovingEnabled;
    bool mIsEqualizationEnabled;
    MarkedWidget *mMarkedWidget;
    MarkedWidget *mNewLocationShowingWidget;

    static constexpr int sBorderWidth = 15;

    void addWidgetToReferenceWidget(QWidget *newWidget, QWidget *referenceWidget, Qt::Orientation orientation, Order order = Order::After);
    void registerNewWidget(QWidget*);

    bool popWidget(QWidget*);
    bool popWidgetFromSplitter(QWidget*, QSplitter*);
    QSplitter* headSplitter();
    void collideRedundantSplitters();
    void collideRedundantSubSplitters(QSplitter*);
    QWidget* widgetUnderMouse() const;


};

#endif // WIDGETTILEMANAGER_H
