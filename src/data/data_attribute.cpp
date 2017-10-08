#include "data_attribute.h"


DataAttribute::DataAttribute()
    :id(0), size(0), dataType(DataType::DOUBLE), showType(ShowType::DECIMAL), multiplier(1.), mIsDataCorrect(true)
{
    dimension.push_back(1);  //default value: dimension of datum is a single value
}

unsigned int
DataAttribute::getId()         const
    { return id;}

string        
DataAttribute::getName()       const
    {return name;}

unsigned short 
DataAttribute::getSize()       const
    {return size;}

DataAttribute::DataType      
DataAttribute::getDataType()   const
    {return dataType;}

DataAttribute::ShowType      
DataAttribute::getShowType()   const
    {return showType;}

double        
DataAttribute::getMultiplier() const
    {return multiplier;}

vector<size_t>
DataAttribute::getDimension()  const
{return dimension;}

std::string DataAttribute::getDataSource() const
{
    return dataSource;
}


/////////////////////////////////////////////////////////////////////////////    
/////////////////////////////////////////////////////////////////////////////    

void DataAttribute::setId(unsigned int arg)
    { id = arg; }

void DataAttribute::setName(string arg)
    { name = arg;} 

void DataAttribute::setSize(unsigned short arg)
    {size = arg;}

void DataAttribute::setDataType(DataType arg)   
    {dataType = arg; }

void DataAttribute::setShowType(ShowType arg)   
    { showType = arg;}

void DataAttribute::setMultiplier(double arg) 
    { multiplier = arg;}

void DataAttribute::setDimension(vector<size_t> arg)
{dimension = arg;}

void DataAttribute::setDataSource(std::string arg)
    {dataSource = arg;}
