#include "horizontallevel.h"

HorizontalLevel::HorizontalLevel(QObject *parent) : QObject(parent),
    mLevel(0.)
{

}

void HorizontalLevel::setLevel(double level)
{
    if (mLevel != level) {
        mLevel = level;
        emit horizontalLevelChanged();
    }
}

void HorizontalLevel::setLabel(const QString &label)
{
    if (mLabel != label) {
        mLabel = label;
        emit horizontalLevelChanged();
    }
}

