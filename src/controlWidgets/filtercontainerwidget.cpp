#include "filtercontainerwidget.h"
#include "ui_filtercontainerwidget.h"
#include "dataconverter/borderfilter.h"
#include "dataconverter/marginfiltercontrollerwidget.h"
#include "dataconverter/windowfiltercontrollerwidget.h"
#include "dataconverter/windowfilter.h"
#include "dataconverter/medianwindowfilter.h"
#include "dataconverter/medianfiltercontrollerwidget.h"
#include <QPushButton>
#include <dataconverter/filterchoisedialog.h>
#include "dataconverter/clusterfilter.h"
#include "dataconverter/clusterfiltercontrollerwidget.h"
#include "global/global_definitions.h"

FilterContainerWidget::FilterContainerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterContainerWidget),
    mModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->addFilterPushButton, SIGNAL(clicked()),
            this, SLOT(addFilter()));

}

FilterContainerWidget::~FilterContainerWidget()
{
    delete ui;
}

void FilterContainerWidget::setModel(std::shared_ptr<AbstractFilterContainer> model)
{
    if (mModel != model) {
        mModel = model;
        clearState();
        initializeState();
    }
}

void FilterContainerWidget::clearModel()
{
    mModel = nullptr;
    clearState();
}

void FilterContainerWidget::addFilter()
{
    if (mModel) {
        //TODO
//        std::shared_ptr<BorderFilter> filter = std::make_shared<BorderFilter>();



//        std::shared_ptr<WindowFilter> filter = std::make_shared<WindowFilter>();

        std::shared_ptr<DataConverter> filter;
        FilterchoiseDialog filterChoiseDialog(&filter, this);
        filterChoiseDialog.exec();
        if (filter) {
            mModel->addFilter(filter);
            addFilterAndKillWidget(filter);
        }
    }
}

void FilterContainerWidget::treatKillRequest()
{
    QPushButton *killButton = qobject_cast<QPushButton *>(QObject::sender());
    int index = mKillButtonVector.indexOf(killButton);
    if (index != -1) {
        disconnect(killButton, SIGNAL(clicked()),
                this, SLOT(treatKillRequest()));
        mKillButtonVector[index]->setParent(NULL);
        delete  mKillButtonVector[index];
        mKillButtonVector.remove(index);
        mFilterControllerWidgetVector[index]->setParent(NULL);
        delete  mFilterControllerWidgetVector[index];
        mFilterControllerWidgetVector.remove(index);
        mModel->removeFilter(index);
     }
}

void FilterContainerWidget::initializeState()
{
    if (mModel != nullptr) {
        mFilterVector = mModel->getFilterVector();
        foreach (auto filter, mFilterVector) {
//            auto controllerWidget = createFilterControlWidget(filter);
//            controllerWidget->setModel(mModel);
//            ui->filterControllerLayout->addWidget(controllerWidget);
//            mFilterControllerWidgetVector.push_back(controllerWidget);
//            QPushButton *killButton = new QPushButton;
//            ui->filterDeleterLayout->addWidget(killButton);
//            mKillButtonVector.push_back(killButton);
            addFilterAndKillWidget(filter);
        }
    }
}

void FilterContainerWidget::clearState()
{

    mFilterVector.clear();

    foreach (auto widget, mFilterControllerWidgetVector) {
        widget->setParent(NULL);
        delete widget;
    }
    mFilterControllerWidgetVector.clear();

    foreach (auto widget, mKillButtonVector) {
        widget->setParent(NULL);
        delete widget;
    }
    mKillButtonVector.clear();
}

FilterControllerWidget* FilterContainerWidget::createFilterControlWidget(std::shared_ptr<DataConverter> filter)
{
    if (std::shared_ptr<BorderFilter> borderFilter = std::dynamic_pointer_cast<BorderFilter>(filter)) {
        MarginFilterControllerWidget *controllerWidget = new MarginFilterControllerWidget(NULL);
        controllerWidget->setFilter(borderFilter);
        return controllerWidget;
    } else if (std::shared_ptr<WindowFilter> windowFilter = std::dynamic_pointer_cast<WindowFilter>(filter)) {
        WindowFilterControllerWidget *controllerWidget = new WindowFilterControllerWidget(NULL);
        controllerWidget->setFilter(windowFilter);
        return controllerWidget;
    } else if (std::shared_ptr<MedianWindowFilter> medianFilter = std::dynamic_pointer_cast<MedianWindowFilter>(filter)) {
        MedianFilterControllerWidget *controllerWidget = new MedianFilterControllerWidget(NULL);
        controllerWidget->setFilter(medianFilter);
        return controllerWidget;
    } else if (std::shared_ptr<ClusterFilter> clusterFilter = std::dynamic_pointer_cast<ClusterFilter>(filter)) {
        ClusterFilterControllerWidget *controllerWidget = new ClusterFilterControllerWidget(NULL);
        controllerWidget->setFilter(clusterFilter);
        return controllerWidget;
    }else {
        qWarning() << "Unsupported type of filter in " << __FUNCTION__;
        return nullptr;
    }
    return nullptr;

}

void FilterContainerWidget::addFilterAndKillWidget(std::shared_ptr<DataConverter> filter)
{
//    auto controllerWidget = createFilterControlWidget(filter);
//    controllerWidget->setModel(mModel);
//    ui->filterControllerLayout->addWidget(controllerWidget);
//    mFilterControllerWidgetVector.push_back(controllerWidget);
//    QPushButton *killButton = new QPushButton(QIcon(":/close"), QString());
//    ui->filterDeleterLayout->addWidget(killButton);
//    mKillButtonVector.push_back(killButton);
//    connect(killButton, SIGNAL(clicked()),
//            this, SLOT(treatKillRequest()));

    int rows = ui->filtersLayout->rowCount();
    auto controllerWidget = createFilterControlWidget(filter);
    controllerWidget->setModel(mModel);
    ui->filtersLayout->addWidget(controllerWidget, rows, 0);
    mFilterControllerWidgetVector.push_back(controllerWidget);
    QPushButton *killButton = new QPushButton(global::getStyledIcon("close"), QString());
    ui->filtersLayout->addWidget(killButton, rows, 1);
    mKillButtonVector.push_back(killButton);
    connect(killButton, SIGNAL(clicked()),
            this, SLOT(treatKillRequest()));
}
