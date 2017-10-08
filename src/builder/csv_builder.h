#ifndef CSV_BUILDER_H
#define CSV_BUILDER_H

#include "builder.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::ofstream;
using std::vector;

/**
 * @brief CSVBuilder - writes data to *.csv (comma separated values) file
 *
 */
class CSVBuilder: public Builder {
public:
    /**
     * @brief Contructor
     *
     * @param file - name of the file, which data will be written to
     * @param delimeter_ - field deliemeter in the output file
     */
    CSVBuilder(string file, char delimeter_ = ',');
    /**
     * @brief
     *
     * @param shared_ptr<DataAttribute>
     */
    virtual void addDataDefinition(shared_ptr<DataAttribute>);
    /**
     * @brief
     *
     * @param shared_ptr<DataSample>
     */
    virtual void addDataSample(std::unique_ptr <DataSample>);
    /**
     * @brief
     *
     */
    virtual ~CSVBuilder();

private:
    string fileName; /**< TODO */
    ofstream fout; /**< TODO */
    bool isHeaderComplete; /**< TODO */
    vector<DataAttribute> dataAttributeVector; /**< TODO */
    char delimeter;
    
};

#endif //CSV_BUILDER_H
