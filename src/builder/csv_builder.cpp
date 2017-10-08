#include "csv_builder.h"
#include <stdexcept>



CSVBuilder::
CSVBuilder(string file, char delimeter_)
    :fileName(file), isHeaderComplete(false), delimeter(delimeter_)
{
    fout.open(fileName, std::fstream::out);    

}


CSVBuilder::~CSVBuilder()
{
    fout.close();
}


void CSVBuilder::addDataDefinition(shared_ptr<DataAttribute> da)
{
    if (isHeaderComplete)
        throw std::logic_error("Trying to add data definition to CSV builder with"
                " completed header!\n");

    //Check for data definitions with the same id
    for (const auto &i: dataAttributeVector)
        if (i.getId() == da->getId())
            throw std::logic_error("Trying to add data definitions to CSV builder"
                " with equal ids\n");
    
    //Check for "," in name of a parameter
    if (da->getName().find(",") != string::npos)
        throw std::logic_error("Trying to add data definitions to CSV builder"
            " with comma in the name\n");

    dataAttributeVector.push_back(*da);
}


void CSVBuilder::addDataSample(std::unique_ptr <DataSample> ds)
{
    //Creating header
    if (isHeaderComplete == false) {
        fout << "time";
        for (const auto &i: dataAttributeVector) 
            fout << delimeter << " " << i.getName();
        fout << std::endl;
        isHeaderComplete = true;
    }

    auto attribute = ds->getDataAttribute();
    for (size_t i = 0; i < dataAttributeVector.size(); ++i) {
        if (dataAttributeVector[i].getId() == attribute->getId()) {
            fout << ds->getTime();
            for (size_t j = 0; j < dataAttributeVector.size(); ++j) {
                fout << delimeter << " ";
                if (j == i) {
                    //TODO - consider all possible situations, preferred type of data representation
                    // and make it good
                    if (attribute->getDataType() == DataAttribute::DataType::FLOAT)
                        fout << ds->getFloatValue()[0];
                    else if (attribute->getDataType() == DataAttribute::DataType::DOUBLE)
                        fout << ds->getDoubleValue()[0];
                    else if (attribute->getDataType() == DataAttribute::DataType::UINT64)
                        fout << std::hex << ds->getUInt64Value()[0] << std::dec;
                    else if (attribute->getDataType() == DataAttribute::DataType::UINT32)
                        fout << std::hex << ds->getUInt32Value()[0] << std::dec;
                    else if (attribute->getDataType() == DataAttribute::DataType::UINT16)
                        fout << std::hex << ds->getUInt16Value()[0] << std::dec;
                    else if (attribute->getDataType() == DataAttribute::DataType::UINT8)
                        fout << std::hex << ds->getUInt8Value()[0] << std::dec;
                    else if (attribute->getDataType() == DataAttribute::DataType::INT64)
                        fout << ds->getInt64Value()[0];
                    else if (attribute->getDataType() == DataAttribute::DataType::INT32)
                        fout << ds->getInt32Value()[0];
                    else if (attribute->getDataType() == DataAttribute::DataType::INT16)
                        fout << ds->getInt16Value()[0];
                    else if (attribute->getDataType() == DataAttribute::DataType::INT8)
                        fout << ds->getInt8Value()[0];
                    else
                        fout << "Don't know how to output this data";
                }
            }
            fout << std::endl;
            break;
        }
    }
}
