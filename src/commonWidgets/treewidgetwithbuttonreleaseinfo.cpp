#include "treewidgetwithbuttonreleaseinfo.h"
#include "QMouseEvent"

TreeWidgetWithButtonReleaseInfo::TreeWidgetWithButtonReleaseInfo(QWidget * parent)
    :QTreeWidget(parent), mReleaseXCoord(-1)
{

}

int TreeWidgetWithButtonReleaseInfo::buttonReleaseXCoord() const
{
    return mReleaseXCoord;
}

void TreeWidgetWithButtonReleaseInfo::mouseReleaseEvent(QMouseEvent *event)
{
    mReleaseXCoord = event->x();
    QTreeWidget::mouseReleaseEvent(event);
}

