#include "utility/settings.h"
#include <QSettings>
#include <QString>
#include <QVariant>
#include "global/global_definitions.h"
#include <QMargins>

void saveInSettings(QSettings& settingsObj, const QString &name, const QVector<global::FastSecondaryProcessingRequest>& requestVector)
{
    QList<QVariant> listToSave;
    for (const auto & request : requestVector){
        listToSave << request.mFunctionString;
        listToSave << static_cast<int>(request.mNewGraphLocation);
    }
    settingsObj.setValue(name, listToSave);
}


void saveInSettings(QSettings& settingsObj, const QString &name, const QColor    &value) { settingsObj.setValue(name, value); }
void saveInSettings(QSettings& settingsObj, const QString &name, const QFont     &value) { settingsObj.setValue(name, value); }
void saveInSettings(QSettings& settingsObj, const QString &name, const QPen      &value) { settingsObj.setValue(name, value); }
void saveInSettings(QSettings &settingsObj, const QString &name, const QBrush    &value) { settingsObj.setValue(name, value); }

void saveInSettings(QSettings& settingsObj, const QString &name, const QMargins  &value)
{
    settingsObj.setValue(name + "/left", value.left());
    settingsObj.setValue(name + "/top", value.top());
    settingsObj.setValue(name + "/right", value.right());
    settingsObj.setValue(name + "/bottom", value.bottom());
}




////////////////////////////////////////////////////////////////////////////////////

bool loadFromSettings(QSettings& settingsObj, const QString &name, QVector<global::FastSecondaryProcessingRequest>& requestVector)
{
    QVariant varSetting;
    if (!(varSetting = settingsObj.value(name)).isValid())
        return false;
    requestVector.clear();
    QList<QVariant> savedList = varSetting.toList();
    if (savedList.size() % 2 == 1)
        return false;
    for (int i = 0; i < savedList.size(); i += 2) {
        requestVector << global::FastSecondaryProcessingRequest {savedList[i].toString(),
                         static_cast<global::FastSecondaryProcessingRequest::NewGraphLocation>(savedList[i + 1].toInt())
                                                                };
    }
    return true;
}

template <typename T>
static bool basic1SettingsLoadingTemplate(QSettings &settingsObj, const QString &name, T &value, T(QVariant::*getValueFunc)()const)
{
    QVariant varSetting;
    if ((varSetting = settingsObj.value(name)).isValid()) {
        value = (varSetting.*getValueFunc)();
        return true;
    } else {
        return false;
    }
}

template <typename T>
static bool basic2SettingsLoadingTemplate(QSettings &settingsObj, const QString &name, T &value, T(QVariant::*getValueFunc)(bool*)const)
{
    QVariant varSetting;
    if ((varSetting = settingsObj.value(name)).isValid()) {
        bool ok;
        value = (varSetting.*getValueFunc)(&ok);
        return ok;
    } else {
        return false;
    }
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, bool &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::toBool);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, int &value)
{
    return basic2SettingsLoadingTemplate(settingsObj, name, value, &QVariant::toInt);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, double &value)
{
    return basic2SettingsLoadingTemplate(settingsObj, name, value, &QVariant::toDouble);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, QString &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::toString);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, QColor &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::value<QColor>);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, QFont &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::value<QFont>);
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, QPen &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::value<QPen>);
}

bool loadFromSettings(QSettings& settingsObj, const QString &name, QMargins &margins)
{
    int value;
    if (loadFromSettings(settingsObj, name + "/left", value) == false)
        return false;
    margins.setLeft(value);
    if (loadFromSettings(settingsObj, name + "/top", value) == false)
        return false;
    margins.setTop(value);
    if (loadFromSettings(settingsObj, name + "/right", value) == false)
        return false;
    margins.setRight(value);
    if (loadFromSettings(settingsObj, name + "/bottom", value) == false)
        return false;
    margins.setBottom(value);

    return true;
}

bool loadFromSettings(QSettings &settingsObj, const QString &name, QBrush &value)
{
    return basic1SettingsLoadingTemplate(settingsObj, name, value, &QVariant::value<QBrush>);
}


