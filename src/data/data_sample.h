#ifndef DATA_SAMPLE_H
#define DATA_SAMPLE_H

#include "data_attribute.h"
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <vector>

using std::shared_ptr;
using std::logic_error;

//TODO rebuild documentation to include std::vector
/**
 * @brief
 *
 */
class DataSample {
public:
    /**
     * @brief
     *
     * @param da
     * @param t
     */
    DataSample(shared_ptr<DataAttribute> da, double t): dataAttribute(da), time(t) { }

    /**
     * @brief Extract  __double__  value from the data sample.
     *
     * If type of data sample doesn't match type __double__, std::logic_error is thrown.
     *
     * @return std::vector<double>
     */
    virtual std::vector<double>   getDoubleValue() const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __float__  value from the data sample.
     *
     * If type of data sample doesn't match type __float__, std::logic_error is thrown.
     *
     * @return std::vector<float>
     */
    virtual std::vector<float>    getFloatValue()  const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __int8_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __int8_t__, std::logic_error is thrown.
     *
     * @return std::vector<int8_t>
     */
    virtual std::vector<int8_t>   getInt8Value()   const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __uint8_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __uint8_t__, std::logic_error is thrown.
     *
     * @return std::vector<uint8_t>
     */
    virtual std::vector<uint8_t>  getUInt8Value()  const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __int16_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __int16_t__, std::logic_error is thrown.
     *
     * @return std::vector<int16_t>
     */
    virtual std::vector<int16_t>  getInt16Value()  const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __uint16_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __uint16_t__, std::logic_error is thrown.
     *
     * @return std::vector<uint16_t>
     */
    virtual std::vector<uint16_t> getUInt16Value() const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __int32_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __int32_t__, std::logic_error is thrown.
     *
     * @return std::vector<int32_t>
     */
    virtual std::vector<int32_t>  getInt32Value()  const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __uint32_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __uint32_t__, std::logic_error is thrown.
     *
     * @return std::vector<uint32_t>
     */
    virtual std::vector<uint32_t> getUInt32Value() const { throw logic_error("Invalid Type Conversion"); }

    /**
     * @brief Extract  __int64_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __int64_t__, std::logic_error is thrown.
     *
     * @return std::vector<int64_t>
     */
    virtual std::vector<int64_t>  getInt64Value()  const { throw logic_error("Invalid Type Conversion"); }
    /**
     * @brief Extract  __uint64_t__  value from the data sample.
     *
     * If type of data sample doesn't match type __uint64_t__, std::logic_error is thrown.
     *
     * @return std::vector<uint64_t>
     */
    virtual std::vector<uint64_t> getUInt64Value() const { throw logic_error("Invalid Type Conversion"); }

    /**
     * @brief
     *
     */
    virtual ~DataSample() {  }
    /**
     * @brief Geta data attributes of the data int the sample
     *
     * @return shared_ptr<DataAttribute>
     */
    shared_ptr<DataAttribute> getDataAttribute() const { return dataAttribute; }
    /**
     * @brief Get time of the sample
     *
     * @return double
     */
    double getTime() const {return time;}

    void setDataAttribute(shared_ptr<DataAttribute> newAttr)  { dataAttribute = newAttr; }

private:
   shared_ptr<DataAttribute> dataAttribute; /**< TODO */
   double time; /**< TODO */
   
};

#endif // DATASAMPLE_H
