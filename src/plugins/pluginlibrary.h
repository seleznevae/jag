#ifndef PLUGINLIBRARY_H
#define PLUGINLIBRARY_H

#include <memory>
#include <QString>
#include "boost/function.hpp"



struct jag_data_source_api;

class PluginLibrary
{
public:
    PluginLibrary(const QString &path = "");

    bool isValid();
    bool match(const QString &path) const;
    struct jag_data_source_api* load(const QString &path) const;
private:
    QString mLibPath;
    bool mIsValid;

    struct PluginImpl;
    std::shared_ptr<PluginImpl> mPimpl;
};

#endif // PLUGINLIBRARY_H
