#ifndef NULLBUILDER_H
#define NULLBUILDER_H

#include "builder.h"

/**
 * @brief descendant of Builder class that does nothin with data that are send to him.
 *
 *  Nullbuilder object reacieves DataAttribute and DataSample packages and does nothing with them.
 */
class NullBuilder : public Builder
{
public:
    /**
     * @brief
     *
     */
    NullBuilder();

    /**
     * @brief
     *
     * @param shared_ptr<DataAttribute>
     */
    virtual void addDataDefinition(shared_ptr<DataAttribute>) { }

    /**
     * @brief
     *
     * @param shared_ptr<DataSample>
     */
    virtual void addDataSample(std::unique_ptr<DataSample>) { }
};

#endif // NULLBUILDER_H
