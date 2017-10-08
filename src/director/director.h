#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <memory>
#include "../builder/builder.h"
#include "../filter/filter.h"
#include "../builder/nullbuilder.h"
#include "../filter/nullfilter.h"
#include "../data/data_sample.h"
#include <QStringList>


/**
 * @brief reads data files, filters them
 *
 * Director - a base class for all other directors. Director object reads particular data files, converts
 * data definitions and data samples to a specific format?
 *  filters data and passes
 * that data to a builder class for writing output files.
 */
class Director {
public:
    Director()
        : builder(std::make_shared<NullBuilder>()), filter(std::make_shared<NullFilter>()),
          mIsCorrect(true), mIsDataSourceCheckingStrict(true)
    {
    }
    /**
     * @brief  Set builder, which Director passes data to.
     *
     * @param b - shared pointer to a Builder object
     */
    void setBuilder(std::shared_ptr<Builder> b) { builder = b; }
    /**
     * @brief  Set filter, that validates read data for following treatment and passing to a builder
     *
     * @param f - shared pointer to a Filter object
     */
    void setFilter(std::shared_ptr<Filter> f) { filter = f; }


    /**
     * @brief starts processing.
     *
     *  Starts reading file, creating and sending DataAttribute and DataSample packages.
     */
    virtual void process() = 0;

    virtual ~Director() { }

    /**
     * @brief starts reading and checking magic number of the input file
     *
     */
    virtual void processMagic() = 0;

    /**
     * @brief reads data definitions, creates DataAttribute objects with data definitions and send them to builder
     *
     */
    virtual void processDataDefinitions() = 0;

    /**
     * @brief reads data samples, creates DataSample objects and, if Filter determins them as valid, send them to builder
     *
     */
    virtual void processDataSamples() = 0;

    bool isCorrect() const { return mIsCorrect; }
    void setCorrect(bool isCorrect) { mIsCorrect = isCorrect; }
    void setStrictChecking(bool isCheckingStrict) { mIsDataSourceCheckingStrict =  isCheckingStrict;}
    QStringList errorsList() const { return mErrorsList; }
protected:
    std::shared_ptr<Builder> builder; /**< TODO */
    std::shared_ptr<Filter> filter; /**< TODO */

    bool mIsCorrect;
    bool mIsDataSourceCheckingStrict;
protected:
    QStringList mErrorsList;
    void addErrorToErrorsList(const QString &error);
};

#endif //DIRECTOR_H
