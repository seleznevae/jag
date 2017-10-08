#include "comboboxwithtips.h"
#include <QMouseEvent>
#include <QToolTip>
#include <QDebug>


ComboBoxWithTips::ComboBoxWithTips(QWidget *Parent):
    QComboBox(Parent)
{
                setMouseTracking(true);
                connect(this, SIGNAL(highlighted(int)),
                        this, SLOT(showTipForItem(int)));
}

void ComboBoxWithTips::addItems(const QStringList &texts, QVector<QStringList> tipVectors)
{

    QComboBox::addItems(texts);
    mTipVector += tipVectors;
    //    mTipVector
}

void ComboBoxWithTips::addItems(QMap<QString, QStringList> valuesToTipsMap)
{
    foreach (QString item, valuesToTipsMap.keys()) {
       QComboBox::addItem(item);
       mTipVector << valuesToTipsMap[item];
    }
}

void ComboBoxWithTips::clear()
{
    QComboBox::clear();
    mTipVector.clear();
}

void ComboBoxWithTips::showTipForItem(int index)
{
    if (index >= 0 && index < count()) {
        QString tipString = mTipVector[index].join("\n");
        QToolTip::showText(mapToGlobal(QPoint(width(), (index)*(double)height()/*mMousePoint.y()*/))
                                     , tipString,
                           this, rect());
    }
}

