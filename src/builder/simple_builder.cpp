#include "simple_builder.h"



SimpleBuilder::SimpleBuilder(string file)
    :fileName(file)
{
    fout.open(fileName, std::fstream::out);    
}


SimpleBuilder::~SimpleBuilder()
{
    fout.close();
}


void SimpleBuilder::addDataDefinition(shared_ptr<DataAttribute> da)
{
    fout << da->getName() << " id is " << da->getId() << std::endl;

}


void SimpleBuilder::addDataSample(shared_ptr<DataSample> ds)
{
    fout << "id is " << ds->getDataAttribute()->getId() << "; time is " << ds->getTime() << "; value is " << ds->getDoubleValue()[0] << std::endl;
}
