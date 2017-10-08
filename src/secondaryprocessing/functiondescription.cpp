#include "functiondescription.h"

FunctionDescription::FunctionDescription()
{
}

QString FunctionDescription::getHtmlDescription() const
{
    return mHtmlDescription;
}

void FunctionDescription::addName(const QString &str)
{
    mHtmlDescription += "<h3>" + str + "</h3>";
}

void FunctionDescription::addDecription(const QString &str)
{
    mHtmlDescription += str + "<br>";
}

void FunctionDescription::addPicture(const QString &source, const QString &alt)
{
    mHtmlDescription += "<img src=\"" + source + "\" alt=\"" + alt + "\">";
}

void FunctionDescription::addParameters(const QString &str)
{
    mHtmlDescription += "<h3>Parameters</h3>";
    mHtmlDescription += str;
}

void FunctionDescription::addReturnValue(const QString &str)
{
    mHtmlDescription += "<h3>Return value</h3>";
    mHtmlDescription += str;
}

void FunctionDescription::addErrorHandling(const QString &str)
{
    mHtmlDescription += "<h3>Error handling</h3>";
    mHtmlDescription += str;
}

void FunctionDescription::addNotes(const QString &str)
{
    mHtmlDescription += "<h3>Notes</h3>";
    mHtmlDescription += str;
}


