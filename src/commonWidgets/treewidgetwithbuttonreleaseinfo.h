#ifndef TREEWIDGETWITHBUTTONRELEASEINFO_H
#define TREEWIDGETWITHBUTTONRELEASEINFO_H

#include <QTreeWidget>

class TreeWidgetWithButtonReleaseInfo : public QTreeWidget
{
public:
    TreeWidgetWithButtonReleaseInfo(QWidget * parent = 0);

    int buttonReleaseXCoord() const;

protected:
    void mouseReleaseEvent(QMouseEvent* event);

    int mReleaseXCoord;
};

#endif // TREEWIDGETWITHBUTTONRELEASEINFO_H
