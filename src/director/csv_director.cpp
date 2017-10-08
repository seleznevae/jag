#include "csv_director.h"
#include <sstream>
#include <vector>
#include "data/data_sample_types.h"
#include <stdlib.h>
#include "basic/minicsv.h"
#include "basic/basic.h"
#include <boost/lexical_cast.hpp>
//#include <QString>
#include <boost/algorithm/string/trim.hpp>
#include <QFile>
#include <cstdlib>
#include<boost/spirit/include/qi.hpp>

using namespace std;
using boost::algorithm::trim;

CSVDirector::CSVDirector(string fName, char delim)
    : fileName(fName), fIn(fName.c_str()), delimeter(delim), mFileSize(0), mCurrentFilePosition(0),
       mParameterNameLineNumber(-1), mParameterUnitsLineNumber(-1), mParameterDescriptionLineNumber(-1),
       mArgumentColumn(0)
{

        if (!fIn.is_open()) {
    //            std::cerr << "Can't open file " << fileName << std::endl;
    //            exit(EXIT_FAILURE);
            throwRuntimeError(std::string("Can't open file " + fileName).c_str(), NULL);

        }
        fIn.set_delimiter(delimeter, "##");

        QFile qFile(QString::fromLocal8Bit(fName.c_str()));
        mFileSize = qFile.size();
}



void CSVDirector::process()
{
        processMagic();
        processDataDefinitions();
        processDataSamples();
}

int CSVDirector::progress() const
{
    if (fIn.is_open()) {
        return 100. * (double)(mCurrentFilePosition) / mFileSize;
    } else {
        return 100;
    }
}

//-------------------------------

void CSVDirector::processMagic()
{
    //nothing
}

//-------------------------------

void CSVDirector::processDataDefinitions()
{
   std::vector<int> maxLineToRead = {mParameterNameLineNumber, mParameterUnitsLineNumber, mParameterDescriptionLineNumber};
   int max_line_number = *std::max_element(maxLineToRead.begin(), maxLineToRead.end());

   for (int i = 0; i < std::max(max_line_number + 1, 1); ++i) { //always read one line (to determine number of columns)
       fIn.read_line();
       std::string element = "";
       std::vector<std::string> firstLine;
       while ((element = fIn.get_delimited_str()) != "") {
            firstLine.push_back(element);
       }
       mPrereadCash.push_back(firstLine);
   }

   //setting parameter names
   if (mParameterNameLineNumber >= 0) {
        size_t i = 0;
        if (mParameterNameLineNumber < static_cast<int>(mPrereadCash.size())) {
            for (auto item : mPrereadCash[mParameterNameLineNumber]) {
                shared_ptr<DataAttribute> da = make_shared<DataAttribute>();
                da->setId(i++);
                da->setName(item);
                dataDefinitions.push_back(da);
            }
        }
   } else {
       for (size_t i = 0; i < mPrereadCash[0].size(); ++i) {
           shared_ptr<DataAttribute> da = make_shared<DataAttribute>();
           da->setId(i);
           /*if (i == 0)
                da->setName("argument");
           else
               da->setName("Parameter " + std::to_string(i));*/
           da->setName("Parameter " + std::to_string(i));
           dataDefinitions.push_back(da);
       }
   }
   for (auto & definition : dataDefinitions) {
       definition->setSize(4);
       definition->setDataType(DataAttribute::DataType::FLOAT);
       definition->setShowType(DataAttribute::ShowType::DECIMAL);
       definition->setMultiplier(1.);
       definition->setDimension({1});
       definition->setDataSource(fileName);
   }

   //setting descriptions
   if (mParameterDescriptionLineNumber >= 0) {
        if (mParameterDescriptionLineNumber < static_cast<int>(mPrereadCash.size())) {
            for (size_t i = 0; i < std::max(dataDefinitions.size(), mPrereadCash[mParameterDescriptionLineNumber].size()); ++i) {
                dataDefinitions[i]->setDescription(QString::fromStdString(mPrereadCash[mParameterDescriptionLineNumber][i]));
            }
        }
   }
   //setting units
   if (mParameterUnitsLineNumber >= 0) {
       if (mParameterUnitsLineNumber < static_cast<int>(mPrereadCash.size())) {
            for (size_t i = 0; i < std::max(dataDefinitions.size(), mPrereadCash[mParameterUnitsLineNumber].size()); ++i) {
                dataDefinitions[i]->setDimensionString(QString::fromStdString(mPrereadCash[mParameterUnitsLineNumber][i]));
            }
       }
   }

   //sending descriptions to builder
   for (auto definition : dataDefinitions)
       builder->addDataDefinition(definition) ;


   //deleting lines with definitions from cash
   std::sort(maxLineToRead.begin(), maxLineToRead.end(), std::greater<int>());
   for (auto number : maxLineToRead) {
       if (number >= 0) {
            mPrereadCash.erase(mPrereadCash.begin() + number);
       }
   }

   //adding empty fields to cash for normal processing in future
   for (auto & line : mPrereadCash) {
        for (size_t i = mPrereadCash.size(); i < dataDefinitions.size(); ++i) {
            line.push_back("");
        }
   }

}
//
//-------------------------------


void CSVDirector::processDataSamples()
{
    double currentTime=0.0;
    int64_t lineNumber = 0;
    std::string line;

    auto old_locale = std::setlocale (LC_NUMERIC, NULL);
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");
    DoAtScopeExit(std::setlocale(LC_NUMERIC, old_locale););

    while (mPrereadCash.size() != 0 || fIn.read_line()) {
        lineNumber++;
        line = fIn.get_line();
        try {
            std::vector<std::string> dataStrings;
            for (size_t i = 0; i < dataDefinitions.size(); ++i) {
                std::string dataString = mPrereadCash.size() ? mPrereadCash[0][i] : fIn.get_delimited_str();
                //trimming spaces to provide correct treatment
                trim(dataString);
                dataStrings.push_back(dataString);
            }

            if (mArgumentColumn == -1) {
                currentTime = lineNumber;
            } else {
                if (mArgumentColumn >= (int)dataStrings.size() || dataStrings[mArgumentColumn] == "")
                    throwRuntimeError("Unexpected situation in %s in simple_director", __FUNCTION__);

                currentTime = std::stod(dataStrings[mArgumentColumn]);
//                    currentTime = boost::lexical_cast<double>( dataString );
//                    currentTime = std::stod(dataString);
                    // atof is faster than boost::lexical_cast
//                    currentTime = std::atof(dataString.c_str());
                    //boost version (seems to have the same speed as std::stod)
//                    auto itBeg = dataString.begin();
//                    auto itEnd = dataString.end();
//                    if (Q_UNLIKELY(!boost::spirit::qi::parse(itBeg, itEnd,boost::spirit::qi::double_,currentTime) || itBeg != itEnd))
//                       throw std::invalid_argument("Can't convert "+ dataString);
            }

            for (size_t i = 0; i < dataDefinitions.size(); ++i) {
                const std::string dataString = dataStrings[i];

                if (dataString != "") {
//                    double datum = boost::lexical_cast<double>( dataString );
                    double datum = std::stod(dataString);
                    // atof is faster than boost::lexical_cast
//                    double datum = std::atof(dataString.c_str());
                    //boost version (seems to have the same speed as std::stod)
//                    double datum;
//                    auto itBeg = dataString.begin();
//                    auto itEnd = dataString.end();
//                    if (Q_UNLIKELY(!boost::spirit::qi::parse(itBeg, itEnd,boost::spirit::qi::double_,datum) || itBeg != itEnd))
//                       throw std::invalid_argument("Can't convert "+ dataString);



//                    shared_ptr<DataSample> sample = make_shared<FloatDataSample> (
//                                            dataDefinitions[i], currentTime, std::initializer_list<float>{static_cast<float>(datum)}
//                                            );
                    std::unique_ptr<DataSample> sample ( new FloatDataSample (
                                            dataDefinitions[i], currentTime, std::initializer_list<float>{static_cast<float>(datum)}
                                            ));
                    builder->addDataSample(std::move(sample));
                }
            }
        } catch (std::exception &error) {
            if (mIsDataSourceCheckingStrict) {
               throw ;
            } else {
                setCorrect(false);
                QString errorString = QString::fromStdString(error.what()) + ". <b>Error at line: "
                        + QString::number(lineNumber) + ":</b><i> " + QString::fromStdString(line) + "</i>";
                addErrorToErrorsList(errorString);
            }
        }
        if (lineNumber % 100) {
            mCurrentFilePosition = (int64_t)fIn.tellg();
        }
        if (mPrereadCash.size()) {
            mPrereadCash.erase(mPrereadCash.begin());
        }
    }

}

