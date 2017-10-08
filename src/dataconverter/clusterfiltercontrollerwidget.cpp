#include "clusterfiltercontrollerwidget.h"
#include "ui_clusterfiltercontrollerwidget.h"
#include <QDoubleValidator>

ClusterFilterControllerWidget::ClusterFilterControllerWidget(QWidget *parent) :
    FilterControllerWidget(parent),
    ui(new Ui::ClusterFilterControllerWidget)
{
    ui->setupUi(this);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->beginTimeLineEdit->setValidator(doubleValidator);
    ui->endTimeLineEdit->setValidator(doubleValidator);
}

ClusterFilterControllerWidget::~ClusterFilterControllerWidget()
{
    delete ui;
}


void ClusterFilterControllerWidget::setFilter(std::shared_ptr<DataConverter> filter)
{
    if ((mFilter = std::dynamic_pointer_cast<ClusterFilter>(filter)) == nullptr) {
        qDebug() << "wrong initializtion of MarginFilterControllerWidget";
        return;
    }
    disconnectAll();
    initializeState();
    connectAll();

}

void ClusterFilterControllerWidget::treatBeginTimeChanging()
{
    if (mFilter) {
        mFilter->setBeginTime(ui->beginTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->beginTimeLineEdit->clearFocus();
}

void ClusterFilterControllerWidget::treatEndTimeChanging()
{
    if (mFilter) {
        mFilter->setEndTime(ui->endTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->endTimeLineEdit->clearFocus();
}

void ClusterFilterControllerWidget::treatWindowWidthChanging()
{
    if (mFilter) {
        mFilter->setWindowWidth(ui->windowWidthSpinBox->value());
        disconnectAll();
        initializeState();
        connectAll();
        mModel->updateOutputData();
    }
    ui->windowWidthSpinBox->clearFocus();
}

void ClusterFilterControllerWidget::treatThresholdChanging()
{
    if (mFilter) {
        mFilter->setRelativeThreshold(ui->relativeThresholdDoubleSpinBox->value());
        mModel->updateOutputData();
    }
    ui->relativeThresholdDoubleSpinBox->clearFocus();
}

void ClusterFilterControllerWidget::treatNumberOfConsecErrorsChanging()
{
    if (mFilter) {
        mFilter->setNumberOfConsecErrors(ui->numberOfConsecErrorsSpinBox->value());
        disconnectAll();
        initializeState();
        connectAll();
        mModel->updateOutputData();
    }
    ui->numberOfConsecErrorsSpinBox->clearFocus();
}

void ClusterFilterControllerWidget::treatPolicyChanging()
{
    if (mFilter) {
        if (ui->cutOffRadioButton->isChecked()) {
            mFilter->setPolicy(ClusterFilter::Policy::CutOff);
        } else if (ui->replaceRadioButton->isChecked()) {
            mFilter->setPolicy(ClusterFilter::Policy::Replace);
        }
        mModel->updateOutputData();
    }
    ui->relativeThresholdDoubleSpinBox->clearFocus();
}

void ClusterFilterControllerWidget::initializeState()
{
    if (mFilter) {
        ui->beginTimeLineEdit->setText(QString::number(mFilter->beginTime()));
        ui->endTimeLineEdit->setText(QString::number(mFilter->endTime()));
        ui->windowWidthSpinBox->setValue(mFilter->windowWidth());
        ui->relativeThresholdDoubleSpinBox->setValue(mFilter->relativeThreshold());
        ui->numberOfConsecErrorsSpinBox->setValue(mFilter->numberOfConsecErrors());

        auto policy = mFilter->policy();
        if (policy == ClusterFilter::Policy::CutOff) {
            ui->cutOffRadioButton->setChecked(true);
        } else if (policy == ClusterFilter::Policy::Replace) {
            ui->replaceRadioButton->setChecked(true);
        }
    }
}

void ClusterFilterControllerWidget::connectAll()
{
    connect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    connect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
    connect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging()));
    connect(ui->relativeThresholdDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatThresholdChanging()));
    connect(ui->numberOfConsecErrorsSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatNumberOfConsecErrorsChanging()));
    connect(ui->cutOffRadioButton, SIGNAL(clicked()),
            this, SLOT(treatPolicyChanging()));
    connect(ui->replaceRadioButton, SIGNAL(clicked()),
            this, SLOT(treatPolicyChanging()));

}

void ClusterFilterControllerWidget::disconnectAll()
{
    disconnect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    disconnect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
    disconnect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging()));
    disconnect(ui->relativeThresholdDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatThresholdChanging()));
    disconnect(ui->numberOfConsecErrorsSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatNumberOfConsecErrorsChanging()));
    disconnect(ui->cutOffRadioButton, SIGNAL(clicked()),
            this, SLOT(treatPolicyChanging()));
    disconnect(ui->replaceRadioButton, SIGNAL(clicked()),
            this, SLOT(treatPolicyChanging()));
}
