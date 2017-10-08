#ifndef IDENTITYCONVERTER_H
#define IDENTITYCONVERTER_H

#include "dataconverter/dataconverter.h"

class IdentityConverter : public DataConverter
{
public:
    IdentityConverter();

    virtual
    std::tuple<QVector<double>, QVector<double>>
    operator() (const QVector<double>&, const QVector<double> &);

    virtual
    std::shared_ptr<DataConverter>
    clone() const {  return std::make_shared<IdentityConverter>(); }

};

#endif // IDENTITYCONVERTER_H
