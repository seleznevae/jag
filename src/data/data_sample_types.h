#ifndef DATA_SAMPLE_TYPES_H
#define DATA_SAMPLE_TYPES_H

#include "data_sample.h"


/**
 * @brief
 *
 */
class FloatDataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<float> construction.
     */
    FloatDataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }


    /**
     * @brief
     *
     * @return std::vector<float>
     */
    std::vector<float> getFloatValue() const { return data; }
private:
    std::vector<float> data; /**< TODO */
};

/**
 * @brief
 *
 */
class DoubleDataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<double> construction.
     */
    DoubleDataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<double>
     */
    std::vector<double> getDoubleValue() const { return data; }
private:
    std::vector<double> data; /**< TODO */
};

/**
 * @brief
 *
 */
class Int8DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<int8_t> construction.
     */
    Int8DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<int8_t>
     */
    std::vector<int8_t> getInt8Value() const { return data; }
private:
    std::vector<int8_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class UInt8DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<uint8_t> construction.
     */
    UInt8DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<uint8_t>
     */
    std::vector<uint8_t> getUInt8Value() const { return data; }
private:
    std::vector<uint8_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class Int16DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<int16_t> construction.
     */
    Int16DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<int16_t>
     */
    std::vector<int16_t> getInt16Value() const { return data; }
private:
    std::vector<int16_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class UInt16DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<uint16_t> construction.
     */
    UInt16DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<uint16_t>
     */
    std::vector<uint16_t> getUInt16Value() const { return data; }
private:
    std::vector<uint16_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class Int32DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<int32_t> construction.
     */
    Int32DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<int32_t>
     */
    std::vector<int32_t> getInt32Value() const { return data; }
private:
    std::vector<int32_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class UInt32DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<uint32_t> construction.
     */
    UInt32DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<uint32_t>
     */
    std::vector<uint32_t> getUInt32Value() const { return data; }
private:
    std::vector<uint32_t> data; /**< TODO */
};


/**
 * @brief
 *
 */
class Int64DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<int64_t> construction.
     */
    Int64DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<int64_t>
     */
    std::vector<int64_t> getInt64Value() const { return data; }
private:
    std::vector<int64_t> data; /**< TODO */
};

/**
 * @brief
 *
 */
class UInt64DataSample:public DataSample {
public:

    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<uint64_t> construction.
     */
    UInt64DataSample(shared_ptr<DataAttribute> da, double t, Args&&... args)
        :DataSample(da, t), data(std::forward<Args>(args)...) { }
    /**
     * @brief
     *
     * @return std::vector<uint32_t>
     */
    std::vector<uint64_t> getUInt64Value() const { return data; }
private:
    std::vector<uint64_t> data; /**< TODO */
};
#endif //DATA_SAMPLE_TYPES_H
