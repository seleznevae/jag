#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVector>
#include <QSettings>
#include <type_traits>


class QString;
class QSettings;
class QVariant;
class QMargins;
namespace global {
class FastSecondaryProcessingRequest;
}




void saveInSettings(QSettings& settingsObj, const QString &name, const QVector<global::FastSecondaryProcessingRequest>& requestVector);

template <typename T, typename Dummy = typename std::enable_if<std::is_enum<T>::value>::type>
void
saveInSettings(QSettings& settingsObj, const QString &name, const T &enumValue)
{
    settingsObj.setValue(name, static_cast<int>(enumValue));
}

inline void saveInSettings(QSettings& settingsObj, const QString &name, const bool    &value) { settingsObj.setValue(name, value); }
inline void saveInSettings(QSettings& settingsObj, const QString &name, const int     &value) { settingsObj.setValue(name, value); }
inline void saveInSettings(QSettings& settingsObj, const QString &name, const double  &value) { settingsObj.setValue(name, value); }
inline void saveInSettings(QSettings& settingsObj, const QString &name, const QString &value) { settingsObj.setValue(name, value); }
void saveInSettings(QSettings& settingsObj, const QString &name, const QColor   &value);
void saveInSettings(QSettings& settingsObj, const QString &name, const QFont    &value);
void saveInSettings(QSettings& settingsObj, const QString &name, const QPen     &value);
void saveInSettings(QSettings& settingsObj, const QString &name, const QBrush   &value);
void saveInSettings(QSettings& settingsObj, const QString &name, const QMargins &value);


template <typename cont>
struct has_value_type_typedef {
    typedef char yes[1];
    typedef char no[2];

    template <typename C>
    static yes& test(typename C::value_type*);

    template <typename>
    static no& test(...);

    static const bool value = sizeof(test<cont>(nullptr)) == sizeof(yes);

};


template <typename Cont, typename Dummy = typename std::enable_if<has_value_type_typedef<Cont>::value>::type>
void
saveInSettings(QSettings& settingsObj, const QString &name, const Cont & container, Dummy* = nullptr) {
    QList<QVariant> listToSave;
    for (const auto &item : container){
        listToSave << item;
    }
    settingsObj.setValue(name, listToSave);
}


bool loadFromSettings(QSettings& settingsObj, const QString &name, QVector<global::FastSecondaryProcessingRequest>& requestVector);
bool loadFromSettings(QSettings& settingsObj, const QString &name, bool &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, int &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, double &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QString &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QColor &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QFont &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QPen &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QBrush &value);
bool loadFromSettings(QSettings& settingsObj, const QString &name, QMargins &value);

template <typename T, typename Dummy = typename std::enable_if<std::is_enum<T>::value>::type>
bool
loadFromSettings(QSettings& settingsObj, const QString &name, T &enumValue)
{
    QVariant varSetting;
    if ((varSetting = settingsObj.value(name)).isValid()) {
        enumValue = static_cast<T>(varSetting.toInt());
        return true;
    } else {
        return false;
    }
}

template <typename Cont, typename Dummy = typename std::enable_if<has_value_type_typedef<Cont>::value>::type>
bool
loadFromSettings(QSettings& settingsObj, const QString &name,  Cont & container, int predictSize = -1) {
    using val_t = typename Cont::value_type;
    QVariant varSetting;
    if (!(varSetting = settingsObj.value(name)).isValid())
        return false;
    QList<QVariant> savedList = varSetting.toList();
    if (predictSize >= 0 && savedList.size() != predictSize)
        return false;
    Cont newContainer;
    for (const auto &var : savedList) {
        if (var.canConvert<val_t>())
            newContainer << var.value<val_t>();
        else
            return false;
    }
    container = newContainer;
    return true;
}

#endif // SETTINGS_H

