#ifndef PLOTPREFERENCES_H
#define PLOTPREFERENCES_H

#include <QString>
#include "core/graphmodel.h"

class QSettings;
struct PlotPreferences {

    PlotPreferences()
        : mLineWidth(1),
          mLineInterpolation(GraphModel::LineStyle::lsLine), mTimeAxisType(QCPAxis::ltNumber),
          mScatterSize(8), mScatterDecimation(1), mXLabelString("time, s"), mUnitsVisibility(false)
    { }

    bool operator==(const PlotPreferences & arg) const {
        if (mLineWidth == arg.mLineWidth
                && mLineInterpolation == arg.mLineInterpolation
                && mTimeAxisType == arg.mTimeAxisType
                && mScatterSize == arg.mScatterSize
                && mScatterDecimation == arg.mScatterDecimation
                && mXLabelString == arg.mXLabelString
                && mUnitsVisibility == arg.mUnitsVisibility
                )
            return true;
        else
            return false;
    }
    bool operator!=(const PlotPreferences & arg) const {
        return !(*this==arg);
    }

    static void saveInSettings(QSettings &settingsObj, const QString &name, const PlotPreferences &pref);
    static bool loadFromSettings(QSettings &settingsObj, const QString &name, PlotPreferences &pref);

    double mLineWidth;
    GraphModel::LineStyle mLineInterpolation;
    QCPAxis::LabelType mTimeAxisType;
    double mScatterSize;
    int mScatterDecimation;
    QString mXLabelString;
    bool mUnitsVisibility;

private:
};


#endif // PLOTPREFERENCES_H
