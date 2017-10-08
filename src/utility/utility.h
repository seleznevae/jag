#ifndef UTILITY_H
#define UTILITY_H

#include <QFlags>
#include <QSettings>
class QSettings;

namespace utility {

enum Position {posLeft   = 0x0001,
               posTop    = 0x0002,
               posRight  = 0x0004,
               posBottom = 0x0008,
               posNone   = 0x0010
};
Q_DECLARE_FLAGS(Positions, Position)
Q_DECLARE_OPERATORS_FOR_FLAGS(Positions)

}

inline void saveInSettings(QSettings& settingsObj, const QString &name, const utility::Positions &positions)
{
    settingsObj.setValue(name, static_cast<int>(positions));
}

inline bool loadFromSettings(QSettings& settingsObj, const QString &name, utility::Positions& positions)
{
    QVariant varSetting;
    if ((varSetting = settingsObj.value(name)).isValid()) {
        positions = static_cast<utility::Position>(varSetting.toInt());
        return true;
    } else {
        return false;
    }
}


#endif // UTILITY_H
