#include "axisprefeditingwidget.h"
#include "ui_axisprefeditingwidget.h"

AxisPrefEditingWidget::AxisPrefEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxisPrefEditingWidget)
{
    ui->setupUi(this);

    ui->endingComboBox->addItem(QString("None"),    QCPLineEnding::esNone);
    ui->endingComboBox->addItem(QString("Arrow"),   QCPLineEnding::esSpikeArrow);

    connectAll();
}

AxisPrefEditingWidget::~AxisPrefEditingWidget()
{
    delete ui;
}

void AxisPrefEditingWidget::setAxisPref(const style::AxisPref &pref)
{
    disconnectAll();
    ui->isMovingToTheCenterEnabledCheckBox->setChecked(pref.mIsAxisDrawingAtCenterEnabled);
    ui->endingComboBox->setCurrentValue(pref.mAxisLineEnding);
    connectAll();
}

style::AxisPref AxisPrefEditingWidget::axisPref()
{
    style::AxisPref ret;
    ret.mIsAxisDrawingAtCenterEnabled = ui->isMovingToTheCenterEnabledCheckBox->isChecked();
    ret.mAxisLineEnding = ui->endingComboBox->currentValue<QCPLineEnding::EndingStyle>();
    return ret;
}

void AxisPrefEditingWidget::connectAll()
{
    connect(ui->isMovingToTheCenterEnabledCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatAxisPrefChanging()));
    connect(ui->endingComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatAxisPrefChanging()));
}

void AxisPrefEditingWidget::disconnectAll()
{
    disconnect(ui->isMovingToTheCenterEnabledCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatAxisPrefChanging()));
    disconnect(ui->endingComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatAxisPrefChanging()));
}

void AxisPrefEditingWidget::treatAxisPrefChanging()
{
    emit axisPrefChanged();
}
