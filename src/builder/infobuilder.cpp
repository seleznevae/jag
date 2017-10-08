#include "infobuilder.h"
#include <stdexcept>
#include <iostream>
#include "basic/basic.h"
#include "data/data_sample_types.h"
#include <algorithm>

InfoBuilder::InfoBuilder(std::string fileName_)
    :fileName(fileName_)
{
    fOut.open(fileName, std::fstream::out | std::fstream::binary);
    if (!fOut) {
        std::cerr << "Can't open file " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

}

void InfoBuilder::addDataDefinition(shared_ptr<DataAttribute> da)
{
    dataAttributeVector.push_back(da);
}

void InfoBuilder::addDataSample(std::unique_ptr<DataSample> dataSample)
{
   if (idToDataNumberMap.find(dataSample->getDataAttribute()->getId()) == idToDataNumberMap.end())
       idToDataNumberMap[dataSample->getDataAttribute()->getId()] = 1;
   else
       idToDataNumberMap[dataSample->getDataAttribute()->getId()]++;

}

InfoBuilder::~InfoBuilder()
{
    std::string separator = "  ";
    auto dataAttributeVectorCopy = dataAttributeVector;
    std::sort(dataAttributeVectorCopy.begin(), dataAttributeVectorCopy.end(),
              [](const shared_ptr<DataAttribute> & arg1, const shared_ptr<DataAttribute> & arg2)
                {
                    return arg1->getName().size() < arg2->getName().size() ? true : false;

                }

              );
    auto nameLength = std::max(int(dataAttributeVectorCopy.back()->getName().size()), 15);

    std::sort(dataAttributeVectorCopy.begin(), dataAttributeVectorCopy.end(),
              [](const shared_ptr<DataAttribute> & arg1, const shared_ptr<DataAttribute> & arg2)
                {
                    return arg1->getDataSource().size() < arg2->getDataSource().size() ? true : false;

                }

              );
    auto dataSourceLength = std::max(int(dataAttributeVectorCopy.back()->getDataSource().size()), 15);
    auto idLength = 8;
    auto dataNumberLength = 12;
    auto sizeLength = 5;
    auto multiplierLength = 15;
    auto showTypeLength = 15;
    auto dataTypeLength = 15;

    fOut.width(nameLength);
    fOut << "PARAMETER" << separator;
    fOut.width(dataSourceLength);
    fOut << "DATA SOURCE"  << separator;
    fOut.width(idLength);
    fOut << "ID" << separator;
    fOut.width(dataNumberLength);
    fOut << "DATA NUMBER" << separator;
    fOut.width(sizeLength);
    fOut << "SIZE" << separator;
    fOut.width(multiplierLength);
    fOut << "MULTIPLIER" << separator;
    fOut.width(showTypeLength);
    fOut << "SHOW TYPE" << separator;
    fOut.width(dataTypeLength);
    fOut << "DATA TYPE" << separator;

    fOut << separator << separator;
    fOut << "DIMENSIONS" << separator;
    fOut << std::endl;

    for (auto dataAttribute : dataAttributeVector) {
        fOut.width(nameLength);
        fOut << dataAttribute->getName() << separator;
        fOut.width(dataSourceLength);
        fOut << dataAttribute->getDataSource() << separator;
        fOut.width(idLength);
        fOut << dataAttribute->getId() << separator;
        fOut.width(dataNumberLength);
        fOut << idToDataNumberMap[dataAttribute->getId()] << separator;
        fOut.width(sizeLength);
        fOut << dataAttribute->getSize() << separator;
        fOut.width(multiplierLength);
        fOut << dataAttribute->getMultiplier() << separator;

        fOut.width(showTypeLength);
        switch (dataAttribute->getShowType()) {
            case DataAttribute::ShowType::BINARY:
                fOut << "BINARY" << separator;
                break;
            case DataAttribute::ShowType::OCTAL:
                fOut << "OCTAL" << separator;
                break;
            case DataAttribute::ShowType::DECIMAL:
                fOut << "DECIMAL" << separator;
                break;
            case DataAttribute::ShowType::HEXADECIMAL:
                fOut << "HEXADECIMAL" << separator;
                break;
            case DataAttribute::ShowType::IGNORE:
                fOut << "IGNORE" << separator;
                break;
        }

        fOut.width(dataTypeLength);
        switch (dataAttribute->getDataType()) {
            case DataAttribute::DataType::DOUBLE:
                fOut << "DOUBLE" << separator;
                break;
            case DataAttribute::DataType::FLOAT:
                fOut << "FLOAT" << separator;
                break;
            case DataAttribute::DataType::INT8:
                fOut << "INT8" << separator;
                break;
            case DataAttribute::DataType::UINT8:
                fOut << "UINT8" << separator;
                break;
            case DataAttribute::DataType::INT16:
                fOut << "INT16" << separator;
                break;
            case DataAttribute::DataType::UINT16:
                fOut << "UINT16" << separator;
                break;
            case DataAttribute::DataType::INT32:
                fOut << "INT32" << separator;
                break;
            case DataAttribute::DataType::UINT32:
                fOut << "UINT32" << separator;
                break;
            case DataAttribute::DataType::INT64:
                fOut << "INT64" << separator;
                break;
            case DataAttribute::DataType::UINT64:
                fOut << "UINT64" << separator;
                break;
            default:
                break;
        }

        fOut << separator << separator;
        for (auto i: dataAttribute->getDimension())
            fOut << i << separator;
        fOut << std::endl;
    }
    fOut.close();

}

