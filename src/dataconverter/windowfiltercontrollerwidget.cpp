#include "windowfiltercontrollerwidget.h"
#include "ui_windowfiltercontrollerwidget.h"
#include <QDoubleValidator>

WindowFilterControllerWidget::WindowFilterControllerWidget(QWidget *parent) :
    FilterControllerWidget(parent),
    ui(new Ui::WindowFilterControllerWidget)
{
    ui->setupUi(this);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);

    connect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging(int)));

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->beginTimeLineEdit->setValidator(doubleValidator);
    ui->endTimeLineEdit->setValidator(doubleValidator);
}

WindowFilterControllerWidget::~WindowFilterControllerWidget()
{
    delete ui;
}

void WindowFilterControllerWidget::setFilter(std::shared_ptr<DataConverter> filter)
{
    if ((mFilter = std::dynamic_pointer_cast<WindowFilter>(filter)) == nullptr) {
        qWarning() << "Failed to initialize MarginFilterControllerWidget";
        return;
    }
    disconnect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    disconnect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
    disconnect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging(int)));
    initializeState();
    connect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging(int)));
    connect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    connect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
}


void WindowFilterControllerWidget::treatBeginTimeChanging()
{
    if (mFilter) {
        mFilter->setBeginTime(ui->beginTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->beginTimeLineEdit->clearFocus();
}

void WindowFilterControllerWidget::treatEndTimeChanging()
{
    if (mFilter) {
        mFilter->setEndTime(ui->endTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->endTimeLineEdit->clearFocus();
}

void WindowFilterControllerWidget::treatWindowWidthChanging(int width)
{
    if (mFilter) {
        mFilter->setWindowWidth(width);
        mModel->updateOutputData();
    }
}

void WindowFilterControllerWidget::initializeState()
{
    if (mFilter) {
        ui->beginTimeLineEdit->setText(QString::number(mFilter->beginTime()));
        ui->endTimeLineEdit->setText(QString::number(mFilter->endTime()));
        ui->windowWidthSpinBox->setValue(mFilter->windowWidth());
    }
}
