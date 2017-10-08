#include "core/plotpreferences.h"
#include "utility/settings.h"


void PlotPreferences::saveInSettings(QSettings &settingsObj, const QString &name, const PlotPreferences &pref)
{
    ::saveInSettings(settingsObj, name + "/mLineWidth"         , pref.mLineWidth);
    ::saveInSettings(settingsObj, name + "/mLineInterpolation" , pref.mLineInterpolation);
    ::saveInSettings(settingsObj, name + "/mTimeAxisType"      , pref.mTimeAxisType);
    ::saveInSettings(settingsObj, name + "/mScatterSize"       , pref.mScatterSize);
    ::saveInSettings(settingsObj, name + "/mScatterDecimation" , pref.mScatterDecimation);
    ::saveInSettings(settingsObj, name + "/mXLabelString"      , pref.mXLabelString);
    ::saveInSettings(settingsObj, name + "/mUnitsVisibility"   , pref.mUnitsVisibility);

}

bool PlotPreferences::loadFromSettings(QSettings &settingsObj, const QString &name, PlotPreferences &pref)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mLineWidth"        , pref.mLineWidth)
              && ::loadFromSettings(settingsObj, name + "/mLineInterpolation", pref.mLineInterpolation)
              && ::loadFromSettings(settingsObj, name + "/mTimeAxisType"     , pref.mTimeAxisType)
              && ::loadFromSettings(settingsObj, name + "/mScatterSize"      , pref.mScatterSize)
              && ::loadFromSettings(settingsObj, name + "/mScatterDecimation", pref.mScatterDecimation)
              && ::loadFromSettings(settingsObj, name + "/mXLabelString"     , pref.mXLabelString)
              && ::loadFromSettings(settingsObj, name + "/mUnitsVisibility"  , pref.mUnitsVisibility);
    return ret;
}
