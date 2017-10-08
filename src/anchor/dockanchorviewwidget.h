#ifndef DOCKANCHORVIEWWIDGET_H
#define DOCKANCHORVIEWWIDGET_H

#include <QDockWidget>
#include "anchor/anchortablewidget.h"
#include <QToolButton>



class DockAnchorViewWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockAnchorViewWidget(QWidget *parent = 0);

    void setPlotWidget(const PlotWidget*);

signals:
    void anchorViewWidgetBecameInvisible();

public slots:


protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    AnchorTableWidget *mAnchorTableWidget;
    QToolButton *mFixComWidthButton;

};

#endif // DOCKANCHORVIEWWIDGET_H
