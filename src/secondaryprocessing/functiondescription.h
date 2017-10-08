#ifndef FUNCTIONDESCRIPTION_H
#define FUNCTIONDESCRIPTION_H

#include <QString>

/*!
 \brief Contains description of special functions in secondary processing.
*/
class FunctionDescription
{
public:
    FunctionDescription();

    QString getHtmlDescription() const;

    void addName(const QString &str);
    void addDecription(const QString &str);
    void addPicture(const QString &source, const QString &alt);
    void addParameters(const QString &str);
    void addReturnValue(const QString &str);
    void addErrorHandling(const QString &str);
    void addNotes(const QString &str);
private:
    QString mHtmlDescription;
};

#endif // FUNCTIONDESCRIPTION_H
