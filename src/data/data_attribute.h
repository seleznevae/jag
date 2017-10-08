#ifndef DATA_ATTRIBUTE_H
#define DATA_ATTRIBUTE_H

#include <string>
#include <vector>
#include <iostream>
#include <QString>

using std::string;
using std::vector;


 
/**
 * @brief Class describing datum
 *
 * DataAttribute class describes in details all attributes of datum (type of datum,
 * show type, unique identificator, name, size, multiplier, dimensions).
 */
class DataAttribute {
public:

    using IdType = unsigned int;

    /**
     * @brief
     *
     */
    DataAttribute();

    /**
     * @brief DataType determines type of data in datum
     *
     * DataType     |  Description
     *--------------|----------------
     * DOUBLE       | 8-byte float number
     * FLOAT        | 4-byte float number
     * INT8         | signed 1 byte integer
     * INT16        | signed 2 byte integer
     * INT32        | signed 4 byte integer
     * INT64        | signed 8 byte integer
     * UINT8        | unsigned 1 byte integer
     * UINT16       | unsigned 2 byte integer
     * UINT32       | unsigned 4 byte integer
     * UINT64       | unsigned 8 byte integer
     */
    enum class DataType {DOUBLE, FLOAT, INT8, UINT8,
        INT16, UINT16, INT32, UINT32, INT64, UINT64, SPECIAL1, DESCRIPTION, DIMENSION, IGNORE};
    /**
     * @brief ShowType determines preferred means of representation of the parameter
     *
     * ShowType    |    Description
     * ------------|-----------------
     * BINARY      |  Show in binary mode
     * OCTAL       |  Show in octal mode
     * DECIMAL     |  Show in decimal mode
     * HEXADECIMAL |  Show in hexadecimal mode
     * IGNORE      |  Ignore parameter
     *
     */
    enum class ShowType {BINARY, OCTAL, DECIMAL, HEXADECIMAL, IGNORE};
   
    /**
     * @brief
     *
     * @return unsigned int
     */
    unsigned int   getId()         const;
    /**
     * @brief
     *
     * @return string
     */
    string         getName()       const;
    /**
     * @brief
     *
     * @return unsigned short
     */
    unsigned short getSize()       const;
    /**
     * @brief
     *
     * @return DataType
     */
    DataType       getDataType()   const;
    /**
     * @brief
     *
     * @return ShowType
     */
    ShowType       getShowType()   const;
    /**
     * @brief
     *
     * @return double
     */
    double         getMultiplier() const;
    /**
     * @brief
     *
     * @return vector<int>
     */
    vector<size_t>    getDimension()  const;

    /**
     * @brief getDataNumber returns total  number of data specimens
     *
     *  For example. If DataAttribute refers an m*n matrix of datas than
     * getDataNumber returns size_t value = m * n;
     *
     * @return size_t
     */
    size_t getDataNumber() const {
        size_t ret = 1;
        //DEBUG
        //std::cerr << "in getDataNumber size is = " << dimension.size() << std::endl;
                    //DEBUG
        for (auto i : dimension)
            ret *= i;
        return ret;
    }

    /**
     * @brief
     *
     * @param arg
     */
    std::string getDataSource() const;

    /**
     * @brief
     *
     * @param arg
     */
    void setId(unsigned int arg)      ;
    /**
     * @brief
     *
     * @param arg
     */
    void setName(string arg)          ;
    /**
     * @brief
     *
     * @param arg
     */
    void setSize(unsigned short arg)  ;
    /**
     * @brief
     *
     * @param arg
     */
    void setDataType(DataType arg)    ;
    /**
     * @brief
     *
     * @param arg
     */
    void setShowType(ShowType arg)    ;
    /**
     * @brief
     *
     * @param arg
     */
    void setMultiplier(double arg)    ;
    /**
     * @brief
     *
     * @param arg
     */
    void setDimension(vector<size_t> arg);
    /**
     * @brief
     *
     * @param arg
     */
    void setDataSource(std::string arg);

    void setDescription(QString desciption) { mDestiption = desciption; }
    QString description() const { return mDestiption; }

    void setDimensionString(QString dimension) { mDimensionString = dimension; }
    QString dimensionString() const { return mDimensionString; }

    bool isDataCorrect() const { return mIsDataCorrect; }
    void setCorrectness(bool isCorrect) { mIsDataCorrect = isCorrect; }
private:
    unsigned int id; /**< Unique identificator of the datum parameter*/
    string name; /**< Parameter name */
    unsigned short size; //in bytes /**< size of parameter datum sample */
    DataType dataType; /**< TODO */
    ShowType showType; /**< TODO */
    double multiplier; /**< TODO */
    vector<size_t> dimension; /**< TODO */
    string dataSource;
    QString mDestiption;
    QString mDimensionString;
    bool mIsDataCorrect;
};

 
#endif //DATA_ATTRIBUTE_H
