#include "pluginlibrary.h"
#include "boost/dll.hpp"
#include "datasource/plugindatasource.h"
#include "boost/function.hpp"



struct PluginLibrary::PluginImpl {
public:
    PluginImpl(const QString &fileName)
        :mLib(nullptr)
    {
       mLib = std::move( std::unique_ptr<boost::dll::shared_library>{new boost::dll::shared_library(fileName.toStdWString().c_str())});
       mFuncMatch = mLib->get<int (const wchar_t *)>(JAG_MATCH_FUNCTION_NAME);
       mFuncLoad = mLib->get<struct jag_data_source_api* (const wchar_t *)>(JAG_LOAD_FUNCTION_NAME);
    }

    std::unique_ptr<boost::dll::shared_library> mLib;
    boost::function<int (const wchar_t *)> mFuncMatch;
    boost::function<struct jag_data_source_api* (const wchar_t *)> mFuncLoad;
};



PluginLibrary::PluginLibrary(const QString &path)
    :mLibPath(path), mIsValid(path == "" ? false : true), mPimpl(nullptr)
{
    if (mLibPath != "") {
        try {
            mPimpl = std::shared_ptr<PluginImpl>(new PluginImpl(mLibPath));
        } catch (...) {
            mIsValid = false;
        }
    }
}

bool PluginLibrary::isValid()
{
    return mIsValid;
}

bool PluginLibrary::match(const QString &path) const
{
    if (!mIsValid)
        return false;
    return mPimpl->mFuncMatch(path.toStdWString().c_str());
}

jag_data_source_api *PluginLibrary::load(const QString &path) const
{
    if (!mIsValid)
        return nullptr;
    return mPimpl->mFuncLoad(path.toStdWString().c_str());
}
