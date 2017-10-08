#include "tableprinter.h"
#include <QFileDialog>
#include "global/global_definitions.h"
#include <QInputDialog>
#include <QString>

TablePrinter::TablePrinter()
    :mEmptyDataString("NaN")
{
}

GraphModel *TablePrinter::addNewGraph(const QVector<double> &x, const QVector<double> &y, const QString &name)
{
       mXDataVector.push_back(x);
       mYDataVector.push_back(y);
       mNameVector.push_back(name);
       return NULL;
}

void TablePrinter::addNewEvent(const TimeEvent */*command*/)
{
    ;//nothing
}

void TablePrinter::saveData()
{
    if (mXDataVector.size() == 0)
        return;

    //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//#ifdef __linux
    QFileDialog::Options option =  QFileDialog::DontUseNativeDialog;
//#else
//    QFileDialog::Option option = 0x0;
//#endif
    QString fileName =  QFileDialog::getSaveFileName(NULL, QString("Save File"),
                                                              global::gDefaultDirectory + "/untitled.txt",
                                                              QString("Text file (*.txt)"), NULL, option);
    if (fileName != "") {
        writeDataToTable(fileName);
    }
}


void TablePrinter::writeDataToTable(QString fileName)
{
    bool ok = true;
    int precision = QInputDialog::getInt(0, "Precision dialog", "Set precision", global::gDoubleToStringConversionPrecision, 4,  64, 1, &ok);
    if (!ok)
        return;
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly)) {
        return;
    }

    QTextStream outStream(&outFile);


    const QString spaceString("  ");

//        QVector<QVector<double>> mXDataVector;
    auto theLongestName = std::max_element(mNameVector.begin(), mNameVector.end(),
                               [](const QString& arg1, const QString& arg2) {return arg1.size() < arg2.size();});
    outStream.setFieldWidth(std::max(theLongestName->size(), precision + 3) + spaceString.size());
    outStream.setRealNumberNotation(QTextStream::SmartNotation);
    outStream.setFieldAlignment(QTextStream::AlignRight);
    int dataSize = mNameVector.size();
    outStream << ("time" + spaceString);
    for (int i = 0; i < dataSize; ++i) {
        outStream << (mNameVector[i] + spaceString);
    }
    outStream << "\n";


    QVector<double> mCurrentPosVector(dataSize, 0);
    while(true) {

        double time = std::numeric_limits<double>::max();
        for (int i = 0; i < dataSize; ++i) {
            if (mCurrentPosVector[i] < mXDataVector[i].size()) {
                if (mXDataVector[i][mCurrentPosVector[i]] < time) {
                    time = mXDataVector[i][mCurrentPosVector[i]];
                }
            }
        }

        if (time == std::numeric_limits<double>::max())
            break;

        outStream << (QString::number(time, 'g', precision) + spaceString);
        for (int i = 0; i < dataSize; ++i) {
            if (mCurrentPosVector[i] < mXDataVector[i].size()) {
                if (mXDataVector[i][mCurrentPosVector[i]] == time) {
                    outStream << (QString::number(mYDataVector[i][mCurrentPosVector[i]++], 'g', precision)  + spaceString);
                    continue;
                }
            }
            outStream << (mEmptyDataString + spaceString);
        }
        outStream << "\n";
    }

    outStream.flush();
    outFile.close();
}




