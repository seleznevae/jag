#include "marginfiltercontrollerwidget.h"
#include "ui_marginfiltercontrollerwidget.h"
#include "QDebug"
#include "basic/basic.h"
#include <QDoubleValidator>

MarginFilterControllerWidget::MarginFilterControllerWidget(QWidget *parent) :
    FilterControllerWidget(parent),
    ui(new Ui::MarginFilterControllerWidget),
    mFilter(nullptr)
{
    ui->setupUi(this);

    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);

    connectAll();

    mLogicButtonGroup = new QButtonGroup(this);
    mLogicButtonGroup->addButton(ui->cuttingRadioButton);
    mLogicButtonGroup->addButton(ui->toNaNRadioButton);

    mXbuttonGroup = new QButtonGroup(this);
    mXbuttonGroup->addButton(ui->xStopRadioButton);
    mXbuttonGroup->addButton(ui->xPassRadioButton);

    mYbuttonGroup = new QButtonGroup(this);
    mYbuttonGroup->addButton(ui->yStopRadioButton);
    mYbuttonGroup->addButton(ui->yPassRadioButton);

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->xLowMarginLineEdit->setValidator(doubleValidator);
    ui->xHighMarginLineEdit->setValidator(doubleValidator);
    ui->yLowMarginLineEdit->setValidator(doubleValidator);
    ui->yHighMarginLineEdit->setValidator(doubleValidator);
}

MarginFilterControllerWidget::~MarginFilterControllerWidget()
{
    delete ui;
}

void MarginFilterControllerWidget::setFilter(std::shared_ptr<DataConverter> filter)
{
//    if ((mFilter = dynamic_cast<std::shared_ptr<BorderFilter>>(filter)) == NULL) {
    if ((mFilter = std::dynamic_pointer_cast<BorderFilter>(filter)) == nullptr) {
        qDebug() << "wrong initializtion of MarginFilterControllerWidget";
        return;
    }
    initializeState();
}

void MarginFilterControllerWidget::setXLowMargin()
{
    if (mFilter) {
        mFilter->setXLowBorder(ui->xLowMarginLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->xLowMarginLineEdit->clearFocus();
}

void MarginFilterControllerWidget::setXHighMargin()
{
    if (mFilter) {
        mFilter->setXHighBorder(ui->xHighMarginLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->xHighMarginLineEdit->clearFocus();
}

void MarginFilterControllerWidget::setYLowMargin()
{
    if (mFilter) {
        mFilter->setYLowBorder(ui->yLowMarginLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->yLowMarginLineEdit->clearFocus();
}

void MarginFilterControllerWidget::setYHighMargin()
{
    if (mFilter) {
        mFilter->setYHighBorder(ui->yHighMarginLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->yHighMarginLineEdit->clearFocus();
}

void MarginFilterControllerWidget::setXFilteringType(bool arg)
{
    if (arg == true) { //annalise only "on" signal not "off"
        if (mFilter) {
            if (ui->xPassRadioButton->isChecked()) {
                mFilter->setXFilteringType(BorderFilter::FilteringType::BandPass);
            } else if (ui->xStopRadioButton->isChecked()) {
                mFilter->setXFilteringType(BorderFilter::FilteringType::BandStop);
            }
            mModel->updateOutputData();
        }
    }
    ui->xPassRadioButton->clearFocus();
}


void MarginFilterControllerWidget::setYFilteringType(bool arg)
{
    if (arg == true) { //annalise only "on" signal not "off"
        if (mFilter) {
            if (ui->yPassRadioButton->isChecked()) {
                mFilter->setYFilteringType(BorderFilter::FilteringType::BandPass);
            } else if (ui->yStopRadioButton->isChecked()) {
                mFilter->setYFilteringType(BorderFilter::FilteringType::BandStop);
            }
            mModel->updateOutputData();
        }
    }
    ui->yStopRadioButton->clearFocus();
}

void MarginFilterControllerWidget::setFilteringLogic(bool arg)
{
    if (arg == true) { //annalise only "on" signal not "off"
        if (mFilter) {
            if (ui->cuttingRadioButton->isChecked()) {
                mFilter->setEliminatingLogic(BorderFilter::EliminatingLogic::Cutting);
            } else if (ui->toNaNRadioButton->isChecked()) {
                mFilter->setEliminatingLogic(BorderFilter::EliminatingLogic::ToNaN);
            }
            mModel->updateOutputData();
        }
    }
    ui->cuttingRadioButton->clearFocus();
}

void MarginFilterControllerWidget::initializeState()
{
    disconnectAll();
    ui->xLowMarginLineEdit->setText(QString::number(mFilter->xLowBorder()));
    ui->xHighMarginLineEdit->setText(QString::number(mFilter->xHighBorder()));
    ui->yLowMarginLineEdit->setText(QString::number(mFilter->yLowBorder()));
    ui->yHighMarginLineEdit->setText(QString::number(mFilter->xHighBorder()));

    if (mFilter->xFilteringType() == BorderFilter::FilteringType::BandPass) {
        ui->xPassRadioButton->setChecked(true);
    } else if (mFilter->xFilteringType() == BorderFilter::FilteringType::BandStop) {
        ui->xStopRadioButton->setChecked(true);
    } else {
        errorExit("Unexpected situation %s\n", __FUNCTION__);
    }

    if (mFilter->yFilteringType() == BorderFilter::FilteringType::BandPass) {
        ui->yPassRadioButton->setChecked(true);
    } else if (mFilter->yFilteringType() == BorderFilter::FilteringType::BandStop) {
        ui->yStopRadioButton->setChecked(true);
    } else {
        errorExit("Unexpected situation %s\n", __FUNCTION__);
    }

    if (mFilter->eliminatingLogic() == BorderFilter::EliminatingLogic::Cutting) {
        ui->cuttingRadioButton->setChecked(true);
    } else if (mFilter->eliminatingLogic() == BorderFilter::EliminatingLogic::ToNaN) {
        ui->toNaNRadioButton->setChecked(true);
    } else {
        errorExit("Unexpected situation %s\n", __FUNCTION__);
    }

    connectAll();

}

void MarginFilterControllerWidget::connectAll()
{
    connect(ui->xLowMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setXLowMargin()));
    connect(ui->xHighMarginLineEdit, SIGNAL(editingFinished()), // using SIGNAL editingFinished cases bugs
            this, SLOT(setXHighMargin()));
    connect(ui->yLowMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setYLowMargin()));
    connect(ui->yHighMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setYHighMargin()));

    connect(ui->xPassRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setXFilteringType(bool)));
    connect(ui->xStopRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setXFilteringType(bool)));
    connect(ui->yPassRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setYFilteringType(bool)));
    connect(ui->yStopRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setYFilteringType(bool)));

    connect(ui->cuttingRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setFilteringLogic(bool)));
    connect(ui->toNaNRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setFilteringLogic(bool)));
}

void MarginFilterControllerWidget::disconnectAll()
{
    disconnect(ui->xLowMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setXLowMargin()));
    disconnect(ui->xHighMarginLineEdit, SIGNAL(editingFinished()), // using SIGNAL editingFinished cases bugs
            this, SLOT(setXHighMargin()));
    disconnect(ui->yLowMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setYLowMargin()));
    disconnect(ui->yHighMarginLineEdit, SIGNAL(editingFinished()),  // using SIGNAL editingFinished cases bugs
            this, SLOT(setYHighMargin()));

    disconnect(ui->xPassRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setXFilteringType(bool)));
    disconnect(ui->xStopRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setXFilteringType(bool)));
    disconnect(ui->yPassRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setYFilteringType(bool)));
    disconnect(ui->yStopRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setYFilteringType(bool)));

    disconnect(ui->cuttingRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setFilteringLogic(bool)));
    disconnect(ui->toNaNRadioButton, SIGNAL(clicked(bool)),
            this, SLOT(setFilteringLogic(bool)));
}
