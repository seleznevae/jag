#include "defaultdimensionwidget.h"
#include "ui_defaultdimensionwidget.h"
#include "global/global_definitions.h"

DefaultDimensionWidget::DefaultDimensionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefaultDimensionWidget)
{
    ui->setupUi(this);

    mParameterListModel = new QStringListModel(global::gParametersForDimensionList);
    mUnitsListModel = new QStringListModel(global::gUnitsForUnitList);
    ui->parameterListView->setModel(mParameterListModel);
    ui->unitsListView->setModel(mUnitsListModel);

    connect(ui->addPushButton, SIGNAL(clicked()),
            this, SLOT(addPair()));
    connect(ui->removePushButton, SIGNAL(clicked()),
            this, SLOT(deleteCurrentRow()));

    connect(ui->parameterListView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(activateUnitByParameterIndex(const QModelIndex &)));
    connect(ui->unitsListView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(activateParameterByUnitsIndex(const QModelIndex &)));
}

DefaultDimensionWidget::~DefaultDimensionWidget()
{
    delete mParameterListModel;
    delete mUnitsListModel;
    delete ui;
}

void DefaultDimensionWidget::applyChangesToGlobalLists()
{
    global::gParametersForDimensionList = mParameterListModel->stringList();
    global::gUnitsForUnitList = mUnitsListModel->stringList();
}

void DefaultDimensionWidget::activateParameterByUnitsIndex(const QModelIndex & index)
{
    int row = index.row();
    ui->parameterListView->setCurrentIndex(mParameterListModel->index(row));
}

void DefaultDimensionWidget::activateUnitByParameterIndex(const QModelIndex &index)
{
    int row = index.row();
    ui->unitsListView->setCurrentIndex(mUnitsListModel->index(row));
}

void DefaultDimensionWidget::addPair()
{
    QStringList parameterList = mParameterListModel->stringList();
    QStringList dimensionList = mUnitsListModel->stringList();

    parameterList << ui->newNameLineEdit->text();
    dimensionList << ui->newunitLineEdit->text();

    mParameterListModel->setStringList(parameterList);
    mUnitsListModel->setStringList(dimensionList);
}

void DefaultDimensionWidget::deleteCurrentRow()
{
    int row = ui->parameterListView->currentIndex().row();
    QStringList parameterList = mParameterListModel->stringList();
    QStringList dimensionList = mUnitsListModel->stringList();
    if (row >= 0 && row < parameterList.size()) {
        parameterList.removeAt(row);
        dimensionList.removeAt(row);
    }
    mParameterListModel->setStringList(parameterList);
    mUnitsListModel->setStringList(dimensionList);
}
