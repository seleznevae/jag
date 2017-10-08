#include "medianfiltercontrollerwidget.h"
#include "ui_medianfiltercontrollerwidget.h"
#include <QDoubleValidator>

MedianFilterControllerWidget::MedianFilterControllerWidget(QWidget *parent) :
    FilterControllerWidget(parent),
    ui(new Ui::MedianFilterControllerWidget)
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

MedianFilterControllerWidget::~MedianFilterControllerWidget()
{
    delete ui;
}

void MedianFilterControllerWidget::setFilter(std::shared_ptr<DataConverter> filter)
{
    if ((mFilter = std::dynamic_pointer_cast<MedianWindowFilter>(filter)) == nullptr) {
        qDebug() << "wrong initializtion of MarginFilterControllerWidget";
        return;
    }
    disconnect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    disconnect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
    disconnect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging(int)));
    initializeState();
    connect(ui->beginTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBeginTimeChanging()));
    connect(ui->endTimeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatEndTimeChanging()));
    connect(ui->windowWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatWindowWidthChanging(int)));

}

void MedianFilterControllerWidget::treatWindowWidthChanging(int width)
{
    if (mFilter) {
        mFilter->setWindowWidth(width);
        mModel->updateOutputData();
    }
}

void MedianFilterControllerWidget::treatBeginTimeChanging()
{
    if (mFilter) {
        mFilter->setBeginTime(ui->beginTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->beginTimeLineEdit->clearFocus();
}

void MedianFilterControllerWidget::treatEndTimeChanging()
{
    if (mFilter) {
        mFilter->setEndTime(ui->endTimeLineEdit->text().toDouble());
        mModel->updateOutputData();
    }
    ui->endTimeLineEdit->clearFocus();
}

void MedianFilterControllerWidget::initializeState()
{
    if (mFilter) {
        ui->windowWidthSpinBox->setValue(mFilter->windowWidth());
        ui->beginTimeLineEdit->setText(QString::number(mFilter->beginTime()));
        ui->endTimeLineEdit->setText(QString::number(mFilter->endTime()));
    }
}
