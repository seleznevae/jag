#include "director/director.h"
#include "global/global_definitions.h"
#include <stdexcept>

void Director::addErrorToErrorsList(const QString &errorString)
{
    if (mErrorsList.size() <= global::gErrorsListCapacity) {
        if (!mErrorsList.contains(errorString)) {
            mErrorsList << errorString;
        }
    } else {
        throw std::runtime_error("Too many errors in data source.");
    }
}
