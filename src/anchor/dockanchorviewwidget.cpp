#include "dockanchorviewwidget.h"
#include "anchor/anchortablewidget.h"
#include "global/global_definitions.h"
#include "global/global_definitions.h"
#include "QToolButton"
//#include "QHBoxLayout"

DockAnchorViewWidget::DockAnchorViewWidget(QWidget *parent) :
    QDockWidget(parent), mAnchorTableWidget(NULL)
{

    bool columnsWidthFixed = true;

    mFixComWidthButton = new QToolButton(this);
    mFixComWidthButton->setCheckable(true);
    mFixComWidthButton->setChecked(columnsWidthFixed);
    mFixComWidthButton->setIcon(global::getStyledIcon("zoom_horizontal"));
    mFixComWidthButton->setGeometry(8,8,20,15);
    mFixComWidthButton->setToolTip("Fix column width");

    mAnchorTableWidget = new AnchorTableWidget(this);
    setWidget(mAnchorTableWidget);
    mAnchorTableWidget->show();
    mAnchorTableWidget->setColumnWidthFixed(columnsWidthFixed);
    show();

    connect(mFixComWidthButton, SIGNAL(toggled(bool)),
            mAnchorTableWidget, SLOT(setColumnWidthFixed(bool)));
}

void DockAnchorViewWidget::setPlotWidget(const PlotWidget *plotWidget)
{
    if (mAnchorTableWidget) {
        mAnchorTableWidget->setPlotWidget(plotWidget);
    }
}

void DockAnchorViewWidget::closeEvent(QCloseEvent *event)
{
    hide();
    emit anchorViewWidgetBecameInvisible();
    event->ignore();
}
