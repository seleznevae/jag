#include "datachoicewidgetwrapper.h"
#include "ui_datachoicewidgetwrapper.h"
#include "global/global_definitions.h"


DataChoiceWidgetWrapper::DataChoiceWidgetWrapper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataChoiceWidgetWrapper), mTimerId(0), mDataSource(NULL)
{
    ui->setupUi(this);

    connect(ui->dataChoiceWidget, SIGNAL(checkedItemsListChanged()),
            this, SIGNAL(checkedItemsListChanged()));

}

DataChoiceWidgetWrapper::~DataChoiceWidgetWrapper()
{
    delete ui;
}

void DataChoiceWidgetWrapper::setDataSource(AbstractDataSourceAdapter *dataSource)
{
    mDataSource = dataSource;
    ui->nameLabel->setText(dataSource->getDataSourceName());
    ui->dataChoiceWidget->setDataSource(dataSource);

    if (global::gShowDataSourceLoadingProgress) {
        if (dataSource->isDataReady())  {
            ui->progressBar->hide();
        } else {
            mTimerId = startTimer(750);
        }
    } else {
        ui->progressBar->hide();
    }

}

void DataChoiceWidgetWrapper::setShowEditWidget(GraphContainer *plotShoeEditWisget)
{
    ui->dataChoiceWidget->setShowEditWidget(plotShoeEditWisget);
}

QVector<QString> DataChoiceWidgetWrapper::getCheckedItemsNames() const
{
    return ui->dataChoiceWidget->getCheckedItemsNames();
}

QVector<QString> DataChoiceWidgetWrapper::getCheckedItemsArgumentsNames() const
{
    return ui->dataChoiceWidget->getCheckedItemsArgumentsNames();
}

AbstractDataSourceAdapter *DataChoiceWidgetWrapper::dataSource() const
{
    return ui->dataChoiceWidget->dataSource();
}

int DataChoiceWidgetWrapper::currentParameterNumber() const
{
    return ui->dataChoiceWidget->currentParameterNumber();
}

void DataChoiceWidgetWrapper::updateData()
{
    ui->dataChoiceWidget->updateData();
}

void DataChoiceWidgetWrapper::addCheckedDataToShowEditWidget()
{
    ui->dataChoiceWidget->addCheckedDataToShowEditWidget();
}

void DataChoiceWidgetWrapper::activateItemByRegExString(QString regexName)
{
    ui->dataChoiceWidget->activateItemByRegExString(regexName);
}

void DataChoiceWidgetWrapper::treatItemClick(QTreeWidgetItem *item, int column)
{
    ui->dataChoiceWidget->treatItemClick(item, column);
}

void DataChoiceWidgetWrapper::setFavouriteSearchEnabled(bool arg)
{
    ui->dataChoiceWidget->setFavouriteSearchEnabled(arg);
}

void DataChoiceWidgetWrapper::setHateSearchEnabled(bool arg)
{
    ui->dataChoiceWidget->setHateSearchEnabled(arg);
}

void DataChoiceWidgetWrapper::nextSearch(QString regexName)
{
    ui->dataChoiceWidget->nextSearch(regexName);
}

void DataChoiceWidgetWrapper::prevSearch(QString regexName)
{
    ui->dataChoiceWidget->prevSearch(regexName);
}

void DataChoiceWidgetWrapper::checkCurrentItem()
{
    ui->dataChoiceWidget->checkCurrentItem();
}

void DataChoiceWidgetWrapper::checkItemsByName(QVector<QString> names, QVector<QString> args)
{
    ui->dataChoiceWidget->checkItemsByName(names, args);
}

void DataChoiceWidgetWrapper::clearAllCheckings()
{
    ui->dataChoiceWidget->clearAllCheckings();
}

void DataChoiceWidgetWrapper::expandItem(const QModelIndex &index)
{
    ui->dataChoiceWidget->expandItem(index);
}

void DataChoiceWidgetWrapper::deleteParameterFromCheckList(int index)
{
    ui->dataChoiceWidget->deleteParameterFromCheckList(index);
}

void DataChoiceWidgetWrapper::setCurrentParameterNumber(int index)
{
    ui->dataChoiceWidget->setCurrentParameterNumber(index);
}

void DataChoiceWidgetWrapper::timerEvent(QTimerEvent */*event*/)
{
    if (mDataSource) {
        if (mDataSource->isDataReady())  {
            ui->progressBar->hide();
            killTimer(mTimerId);
        } else {
            ui->progressBar->setValue(mDataSource->progress());
        }
    }
}
