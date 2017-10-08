#ifndef CSV_DIRECTOR_H
#define CSV_DIRECTOR_H

#include "director.h"
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "basic/minicsv.h"
#include "basic/basic.h"


/**
 * @brief CSVDirector - Director for *.csv input file treatment
 *
 */
class CSVDirector:public Director {
public:
    /**
     * @brief
     *
     * @param fName
     */
    CSVDirector(std::string fName, char delim = ',');
    /**
     * @brief
     *
     */
    ~CSVDirector() {
        fIn.close();
    }
    /**
     * @brief
     *
     */
    void setParameterNameLineNumber(int nameLineNumber) { mParameterNameLineNumber = nameLineNumber; }
    void setParameterUnitsLineNumber(int unitsLineNumber) { mParameterUnitsLineNumber = unitsLineNumber; }
    void setParameterDescriptionLineNumber(int descriptionLineNumber) { mParameterDescriptionLineNumber = descriptionLineNumber; }
    void setArgumentColumn(int argumentColumn) { mArgumentColumn = argumentColumn; }

    void process();

    int progress() const;
private:
    std::vector<std::shared_ptr<DataAttribute>> dataDefinitions; /**< TODO */
    std::string fileName; /**< TODO */
    csv::ifstream fIn; /**< TODO */
    char delimeter; /**< delimeter of fields in *.csv file */
    int64_t mFileSize;
    int64_t mCurrentFilePosition;
    int mParameterNameLineNumber;
    int mParameterUnitsLineNumber;
    int mParameterDescriptionLineNumber;
    int mArgumentColumn;

    std::vector<std::vector<std::string>> mPrereadCash;
    /**
     * @brief
     *
     */
    void processMagic();
    /**
     * @brief
     *
     */
    void processDataDefinitions();
    /**
     * @brief
     *
     */
    void processDataSamples();


};

#endif //CSV_DIRECTOR_H

