#ifndef GRAPHMODELFILTERCONTAINER_H
#define GRAPHMODELFILTERCONTAINER_H

#include "dataconverter/abstractfiltercontainer.h"
#include "core/graphmodel.h"

class GraphModelFilterContainer: public AbstractFilterContainer
{
public:
    GraphModelFilterContainer();
    virtual ~GraphModelFilterContainer();


    virtual void addFilter(std::shared_ptr<DataConverter> newConverter);
    virtual QVector<std::shared_ptr<DataConverter>> getFilterVector() const;
    virtual void removeFilter(int index);
    void setGraphModel(GraphModel* model);
    virtual void updateOutputData();


private:
    GraphModel *mGraphModel;

};

#endif // GRAPHMODELFILTERCONTAINER_H
