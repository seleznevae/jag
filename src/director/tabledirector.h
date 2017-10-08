#ifndef TABLEDIRECTOR_H
#define TABLEDIRECTOR_H

#include "director/director.h"
#include <QFile>
#include <QTextStream>
#include <QVector>

/*!
 \brief Director for reading files with data in table format.

 First line of input data must contain variable names, e.g."time x1 x3 y45 z99" (first parameter is always considered as time).
 Second line (optional) - format line , e.g. "%f %f %x %f %f" (possible specificators - %f for floating point data, %x - for unsigned
 data in hex format.
 Other lines  - data, e.g. "1.  12.   xF1   100. -1.e5"

*/
class TableDirector : public Director
{
public:
    explicit TableDirector(QString fileName);
    ~TableDirector();

    void process();

private:
    QString mFileName;
    QFile mFile;
    QTextStream mInStream;

    QVector<bool> mIsHex;
    std::vector<std::shared_ptr<DataAttribute>> mDataDefinitions;


    void processMagic();
    void processDataDefinitions();
    void processDataSamples();

    void treatCurrentDataString(QString &string, int lineNumber);
};

#endif // TABLEDIRECTOR_H
