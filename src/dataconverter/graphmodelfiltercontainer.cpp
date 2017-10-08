#include "graphmodelfiltercontainer.h"

GraphModelFilterContainer::GraphModelFilterContainer()
    :mGraphModel(NULL)
{
}

GraphModelFilterContainer::~GraphModelFilterContainer()
{

}

void GraphModelFilterContainer::addFilter(std::shared_ptr<DataConverter> newConverter)
{
    if (mGraphModel) {
        mGraphModel->addFilter(newConverter);
    }
}

QVector<std::shared_ptr<DataConverter> > GraphModelFilterContainer::getFilterVector() const
{
    if (mGraphModel) {
        return mGraphModel->getFilterVector();
    } else
        return QVector<std::shared_ptr<DataConverter> >();
}

void GraphModelFilterContainer::removeFilter(int index)
{
    if (mGraphModel) {
        mGraphModel->removeFilter(index);;
    }
}

void GraphModelFilterContainer::setGraphModel(GraphModel *model)
{
    if (mGraphModel != model) {
        mGraphModel = model;
    }
}

void GraphModelFilterContainer::updateOutputData()
{
    if (mGraphModel) {
        mGraphModel->updateOutputData();
    }
}
