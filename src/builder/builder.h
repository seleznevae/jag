#ifndef BUILDER_H
#define BUILDER_H

#include "../data/data_sample.h"
#include "../data/data_attribute.h"
#include <memory>

using std::shared_ptr;



/**
 * @brief Builder - parent abastract class for all builders
 *
 * Class inherited from Builder recieve data description and data from
 * directors and build ouput files of the particular type.
 *
 */

class Builder {
public:
    /**
     * @brief
     *
     * @param shared_ptr<DataAttribute>
     */
    virtual void addDataDefinition(shared_ptr<DataAttribute>) = 0;
    /**
     * @brief
     *
     * @param shared_ptr<DataSample>
     */
    virtual void addDataSample(std::unique_ptr <DataSample>) = 0;
    /**
     * @brief
     *
     */
    virtual ~Builder() { }

};


#endif // BUILDER_H
