#include "tabledirector.h"
#include "basic/basic.h"
#include <QDebug>
#include "data/data_sample_types.h"

TableDirector::TableDirector(QString fileName)
//    :mFileName(QString::fromLocal8Bit(fileName.c_str()))
{
    mFileName = fileName;


    mFile.setFileName(mFileName);
    if (mFile.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        throwRuntimeError(("Can't open file " + mFileName).toLocal8Bit().constData(), NULL);
    }
    mInStream.setDevice(&mFile);

}

TableDirector::~TableDirector()
{
    mFile.close();
}

void TableDirector::process()
{
    processMagic();
    processDataDefinitions();
    processDataSamples();
}

void TableDirector::processMagic()
{
    ;//pass
}

void TableDirector::processDataDefinitions()
{
    QString header = mInStream.readLine().trimmed();
    QStringList paramList = header.split(QRegExp("\\s+"));
    if (paramList.size() == 0) {
        throwRuntimeError(("Invalid header in " + mFileName).toLocal8Bit().constData(), NULL);
    }

    int i=0;
    for (const auto& parameterName : paramList) {
        std::shared_ptr<DataAttribute> da = std::make_shared<DataAttribute>();
        da->setId(i++);
        da->setName(parameterName.toStdString());
        da->setSize(8);
        da->setDataType(DataAttribute::DataType::DOUBLE);
        da->setShowType(DataAttribute::ShowType::DECIMAL);
        da->setMultiplier(1.);
        da->setDimension({1});
        da->setDataSource(mFileName.toStdString());
        mDataDefinitions.push_back(da);
    }
    for (auto definition : mDataDefinitions) {
        builder->addDataDefinition(definition);
        mIsHex.push_back(false);
    }

}

void TableDirector::processDataSamples()
{

    QString currentString;
    int lineNumber = 0;

    if (!mInStream.atEnd()) {
        try {
            currentString = mInStream.readLine().trimmed();
            lineNumber++;
            auto list = currentString.split(QRegExp("\\s+"));
            if ((list[0][0]) == QChar('%')) {
                if (list.size() > static_cast<int>(mDataDefinitions.size())) {
                    throwRuntimeError("Invalid format string\n", NULL);
                }
                for (int i = 0; i < list.size(); ++i) {
                    if ((list[i].toLower() == "%f")
                            || (list[i].toLower() == "%e")
                            || (list[i].toLower() == "%g")) {
                        mIsHex[i] = false;
                    } else if (list[i].toLower() == "%x") {
                        mIsHex[i] = true;
                    } else {
                        throwRuntimeError("Invalid format string\n", NULL);
                    }
                }
            } else {
                treatCurrentDataString(currentString, lineNumber);
            }
        } catch (std::exception &error) {
            if (mIsDataSourceCheckingStrict) {
               throw ;
            } else {
                setCorrect(false);
                QString errorString = QString::fromStdString(error.what()) + ". <b>Error at line: "
                        + QString::number(lineNumber) + ":</b><i> " + currentString + "</i>";
                addErrorToErrorsList(errorString);
            }
        }
    }



    while (!mInStream.atEnd()) {
        try {
            currentString = mInStream.readLine().trimmed();
            lineNumber++;
            treatCurrentDataString(currentString, lineNumber);
        } catch (std::exception &error) {
            if (mIsDataSourceCheckingStrict) {
               throw ;
            } else {
                setCorrect(false);
                QString errorString = QString::fromStdString(error.what()) + ". <b>Error at line: "
                        + QString::number(lineNumber) + ":</b><i> " + currentString + "</i>";
                addErrorToErrorsList(errorString);
            }
        }
    }


}

void TableDirector::treatCurrentDataString(QString &currentString, int lineNumber)
{
    if (currentString.size() != 0) {
        QStringList paramList = currentString.split(QRegExp("\\s+"));
        if (paramList.size() != 0) {
            bool ok = true;
            double time = paramList[0].toDouble(&ok);
            if (ok == false)
                throw std::runtime_error("Invalid time");
            for (int i = 0; i < paramList.size(); ++i) {
                if (i < static_cast<int>(mDataDefinitions.size())) {
                    bool ok = true;
                    double value = 0;
                    if (mIsHex[i]) {
                        value = static_cast<double>(paramList[i].toULong(&ok, 16));
                    } else {
                        value = paramList[i].toDouble(&ok);
                    }
                    if (ok == false)
                        throw std::runtime_error("Can't convert value to double ");
//                    std::shared_ptr<DataSample> sample = std::make_shared<DoubleDataSample> (
//                                            mDataDefinitions[i], time, std::initializer_list<double>{value}
//                                            );

                    std::unique_ptr<DataSample> sample (new DoubleDataSample (
                                            mDataDefinitions[i], time, std::initializer_list<double>{value}
                                            ));
                    builder->addDataSample(std::move(sample));
                } else {
                    if (mIsDataSourceCheckingStrict) {
                       throwRuntimeError(("Too many data in string: " + QString::number(lineNumber).toStdString() + "\n").c_str(), NULL);
                    } else {
                        setCorrect(false);
                        QString errorString = "Too many data in string: " + QString::number(lineNumber) + "\n";
                        addErrorToErrorsList(errorString);
                    }
                }
            }
        }
    }
}
