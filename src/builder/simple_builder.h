#ifndef SIMPLE_BUILDER_H
#define SIMPLE_BUILDER_H

#include "builder.h"
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::ofstream;

/**
 * @brief Old class not used any more. This class should be deleted soon
 *
 */
class SimpleBuilder: public Builder {
public:
    /**
     * @brief
     *
     * @param file
     */
    SimpleBuilder(string file);
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
    virtual void addDataSample(shared_ptr<DataSample>);
    /**
     * @brief
     *
     */
    virtual ~SimpleBuilder();

private:
    string fileName; /**< TODO */
    ofstream fout; /**< TODO */
};

#endif //SIMPLE_BUILDER_H
