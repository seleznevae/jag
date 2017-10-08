#include "datasourceadapter.h"
#include "basic/bassic.h"
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"
#include "secondaryprocessing/datacontainer.h"

DataSourceAdapter::DataSourceAdapter()
    :mDataSource(NULL)
{
}

DataSourceAdapter::~DataSourceAdapter()
{

}

QVector<std::tuple<QString, int> > DataSourceAdapter::getDataList() const
{
    if (mDataSource) {
        if (mDataList.isEmpty())
            mDataList = mDataSource->getDataList();
        return mDataList;
    }
    return QVector<std::tuple<QString, int> >();
}

QString DataSourceAdapter::getDataSourceName() const
{
    if (mDataSource) {
        return mDataSource->getDataSourceName();
    }
    return QString();
}

QString DataSourceAdapter::getDataSourceAlias() const
{
    if (mDataSource) {
        return mDataSource->getDataSourceAlias();
    }
    return QString();
}

bool DataSourceAdapter::isDataReady() const
{
    if (mDataSource) {
        return mDataSource->isDataReady();
    }
    return false;
}

void DataSourceAdapter::addDataToPlotEdit(GraphContainer *showEditWidget, int primaryIndex, int secondaryIndex)
{
    auto data = mDataSource->getDataByIndex(primaryIndex, secondaryIndex);
    QString prefix;
    switch (global::gParameterNamingPolicy) {
        case global::ParamNamingPolicy::AlwaysAddAlias:
            prefix = stripDirectoryFromFileName(mDataSource->getDataSourceAlias()) + "::";
            break;
        case global::ParamNamingPolicy::AddAliasForMultSources:
            if (global::gJagMainWindowPointer->numberOfDataSources() != 1) {
                prefix = stripDirectoryFromFileName(mDataSource->getDataSourceAlias()) + "::";
            }
            break;
        case global::ParamNamingPolicy::NeverAddAlias:
            break;
        default:
            break;
    }

    auto newGraph = showEditWidget->addNewGraph(std::get<0>(data), std::get<1>(data),
//                                 stripDirectoryFromFileName(mDataSource->getDataSourceAlias())
//                                 + "::"
                                   prefix +  std::get<0>(mDataList[primaryIndex])
                                + (std::get<1>(mDataList[primaryIndex]) == 1 ? "" : "[" + QString::number(secondaryIndex) + "]"));
    if (newGraph) {
        std::shared_ptr<GraphStyle> graphStyle = mDataSource->graphStyle();
        if (graphStyle->isSpecified()) {
            newGraph->setWidth(graphStyle->lineWidth());
            newGraph->setLineStyle(graphStyle->lineStyle());
            newGraph->setLineInterpolation(graphStyle->lineInterpolation());

            newGraph->setScatterShape(graphStyle->scatterShape());
            newGraph->setScatterSize(graphStyle->scatterSize());
            newGraph->setScatterDecimation(graphStyle->scatterDecimation());

            if (graphStyle->isColorFixed()) {
                newGraph->setColor(graphStyle->lineColor());
            }
        }
        if (mDataSource->getDimensionByIndex(primaryIndex) != "") {
            newGraph->setUnit(mDataSource->getDimensionByIndex(primaryIndex));
        }
    }
}

void DataSourceAdapter::addCurveDataToPlotEdit(GraphContainer *showEditWidget, int xPrimaryIndex, int xSecondaryIndex, int yPrimaryIndex, int ySecondaryIndex)
{
    int primaryIndex = yPrimaryIndex;
    int secondaryIndex = ySecondaryIndex;

    auto yData = mDataSource->getDataByIndex(yPrimaryIndex, ySecondaryIndex);
    auto xData = mDataSource->getDataByIndex(xPrimaryIndex, xSecondaryIndex);
    DataContainer xDataContainer(std::get<0>(xData), std::get<1>(xData));
    DataContainer yDataContainer(std::get<0>(yData), std::get<1>(yData));

    DataContainer dataContainer  = DataContainerNmspc::paramCurve(yDataContainer, xDataContainer);
    Q_UNUSED(dataContainer);

    QString prefix;
    switch (global::gParameterNamingPolicy) {
        case global::ParamNamingPolicy::AlwaysAddAlias:
            prefix = stripDirectoryFromFileName(mDataSource->getDataSourceAlias()) + "::";
            break;
        case global::ParamNamingPolicy::AddAliasForMultSources:
            if (global::gJagMainWindowPointer->numberOfDataSources() != 1) {
                prefix = stripDirectoryFromFileName(mDataSource->getDataSourceAlias()) + "::";
            }
            break;
        case global::ParamNamingPolicy::NeverAddAlias:
            break;
        default:
            break;
    }

    QString newGraphName =  prefix +  std::get<0>(mDataList[primaryIndex])
            + (std::get<1>(mDataList[primaryIndex]) == 1 ? "" : "[" + QString::number(secondaryIndex) + "]");
    newGraphName += " (" + std::get<0>(mDataList[xPrimaryIndex])
            + (std::get<1>(mDataList[xPrimaryIndex]) == 1 ? "" : "[" + QString::number(xSecondaryIndex) + "]") + ")";
    auto newGraph = showEditWidget->addNewGraph(dataContainer.mXData, dataContainer.mYData,
//                                 stripDirectoryFromFileName(mDataSource->getDataSourceAlias())
//                                 + "::"
                                   newGraphName);

    newGraph->setGraphType(GraphModel::GraphType::Curve);
    if (newGraph) {
        std::shared_ptr<GraphStyle> graphStyle = mDataSource->graphStyle();
        if (graphStyle->isSpecified()) {
            newGraph->setWidth(graphStyle->lineWidth());
            newGraph->setLineStyle(graphStyle->lineStyle());
            newGraph->setLineInterpolation(graphStyle->lineInterpolation());

            newGraph->setScatterShape(graphStyle->scatterShape());
            newGraph->setScatterSize(graphStyle->scatterSize());
            newGraph->setScatterDecimation(graphStyle->scatterDecimation());

            if (graphStyle->isColorFixed()) {
                newGraph->setColor(graphStyle->lineColor());
            }
        }
        if (mDataSource->getDimensionByIndex(primaryIndex) != "") {
            newGraph->setUnit(mDataSource->getDimensionByIndex(primaryIndex));
        }
    }
}

void DataSourceAdapter::setDataSource(const DataSource *dataSource)
{
    mDataSource = dataSource;
}

QString DataSourceAdapter::getDescription(int primaryIndex)
{
    if (mDataSource) {
        return mDataSource->getDescriptionByIndex(primaryIndex);
    }
    return QString();
}

int DataSourceAdapter::getMaxNameLength() const
{
    if (mDataSource) {
        return mDataSource->getMaxNameLength();
    }
    return 20;
}

int DataSourceAdapter::progress() const
{
    if (mDataSource) {
        return mDataSource->progress();
    }
    return 75;
}

bool DataSourceAdapter::isDataSourceValid() const
{
    if (mDataSource) {
        return mDataSource->isValid();
    }
    return false;
}

bool DataSourceAdapter::isDataCorrect() const
{
    if (mDataSource) {
        return mDataSource->isDataCorrect();
    }
    return false;
}

void DataSourceAdapter::setCorrectness(bool isCorrect)
{
    if (mDataSource) {
        const_cast<DataSource*>(mDataSource)->setCorrectness(isCorrect);
    }
}

QStringList DataSourceAdapter::errorsList() const
{
    if (mDataSource) {
        return mDataSource->errorsList();
    }
    return QStringList();
}
