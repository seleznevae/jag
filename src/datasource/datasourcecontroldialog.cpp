#include "datasourcecontroldialog.h"
#include "ui_datasourcecontroldialog.h"
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"

//bool DataSourceControlDialog::isDataSourceControlDialogActive = false;

DataSourceControlDialog::DataSourceControlDialog(QVector<DataSource*> dataSourceVector, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSourceControlDialog), mDataSourceVector(dataSourceVector)
{

    ui->setupUi(this);
//    isDataSourceControlDialogActive = true;
    for (auto dataSource : mDataSourceVector) {
        DataSourceControlWidget *dataSourceWidget = new DataSourceControlWidget(this);
        dataSourceWidget->setDataSource(dataSource);
        ui->dataSourceTabWidget->addTab(dataSourceWidget, dataSource->getDataSourceAlias());
    }

    ui->okPushButton->setFocus();

    connect(ui->okPushButton, SIGNAL(clicked()),
            this, SLOT(finishDataSourceEditing()));
    connect(ui->dataSourceTabWidget,SIGNAL(tabCloseRequested(int)),
            this, SLOT(treatDataSourceDeletingRequest(int)));


}

DataSourceControlDialog::~DataSourceControlDialog()
{
    delete ui;
}

void DataSourceControlDialog::finishDataSourceEditing()
{
    hide();
//    accept();
//    isDataSourceControlDialogActive = false;
}

void DataSourceControlDialog::addDataSource(DataSource *dataSource)
{
    DataSourceControlWidget *dataSourceWidget = new DataSourceControlWidget(this);
    dataSourceWidget->setDataSource(dataSource);
    ui->dataSourceTabWidget->addTab(dataSourceWidget, dataSource->getDataSourceAlias());
    ui->dataSourceTabWidget->setCurrentIndex(ui->dataSourceTabWidget->count() - 1);
    mDataSourceVector.push_back(dataSource);
    ui->okPushButton->setFocus();
}

void DataSourceControlDialog::deleteDataSourceTab(DataSource *dataSource)
{
    int index = mDataSourceVector.indexOf(dataSource);
    if (index != -1) {
        auto tab = ui->dataSourceTabWidget->widget(index);
        tab->setParent(NULL);
        delete tab;
        mDataSourceVector.remove(index);
    }
}

void DataSourceControlDialog::treatDataSourceDeletingRequest(int index)
{
    if (index >= 0 && index < mDataSourceVector.size()) {
        treatDataSourceDeletingRequest(mDataSourceVector[index]);
    }
}


void DataSourceControlDialog::treatDataSourceDeletingRequest(DataSource* dataSource)
{
    int index = mDataSourceVector.indexOf(dataSource);
    if (index != -1) {
        global::gJagMainWindowPointer->removeDataSource(mDataSourceVector[index]);
    }
}
