#include "axisrecteditingwidget.h"
#include "ui_axisrecteditingwidget.h"

AxisRectEditingWidget::AxisRectEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxisRectEditingWidget)
{
    ui->setupUi(this);
    layout()->setSpacing(0);
    layout()->setMargin(0);
    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->penChoiceWidget->setHeader("Axis rect pen");
}

AxisRectEditingWidget::~AxisRectEditingWidget()
{
    delete ui;
}

void AxisRectEditingWidget::setAxisRect(const style::AxisRect &axisRect)
{
    disconnectAll();
    ui->penChoiceWidget->setPen(axisRect.mPen);
    ui->leftCheckBox  ->setChecked(axisRect.mPositions.testFlag(utility::posLeft  ));
    ui->rightCheckBox ->setChecked(axisRect.mPositions.testFlag(utility::posRight ));
    ui->topCheckBox   ->setChecked(axisRect.mPositions.testFlag(utility::posTop   ));
    ui->bottomCheckBox->setChecked(axisRect.mPositions.testFlag(utility::posBottom));
    ui->gapSpinBox    ->setValue(axisRect.mGap);

    updateAxisRectPainter();
    connectAll();
}

style::AxisRect AxisRectEditingWidget::axisRect() const
{
    style::AxisRect ret;
    ret.mPositions = utility::Positions(0);
    if (ui->leftCheckBox->isChecked())
        ret.mPositions |= utility::posLeft;
    if (ui->topCheckBox->isChecked())
        ret.mPositions |= utility::posTop;
    if (ui->rightCheckBox->isChecked())
        ret.mPositions |= utility::posRight;
    if (ui->bottomCheckBox->isChecked())
        ret.mPositions |= utility::posBottom;

    ret.mPen = ui->penChoiceWidget->pen();
    ret.mGap = ui->gapSpinBox->value();

    return ret;
}

void AxisRectEditingWidget::connectAll()
{
    connect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SIGNAL(axisRectPropertiesChanged()));
    connect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    connect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    connect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    connect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    connect(ui->gapSpinBox, SIGNAL(valueChanged(int)),
               this, SIGNAL(axisRectPropertiesChanged()));

    connect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    connect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    connect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    connect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    connect(ui->gapSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAxisRectPainter()));

}

void AxisRectEditingWidget::disconnectAll()
{
    disconnect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SIGNAL(axisRectPropertiesChanged()));
    disconnect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    disconnect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    disconnect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(axisRectPropertiesChanged()));
    disconnect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
               this, SIGNAL(axisRectPropertiesChanged()));
    disconnect(ui->gapSpinBox, SIGNAL(valueChanged(int)),
               this, SIGNAL(axisRectPropertiesChanged()));


    disconnect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    disconnect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    disconnect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    disconnect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateAxisRectPainter()));
    disconnect(ui->gapSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateAxisRectPainter()));
}

void AxisRectEditingWidget::updateAxisRectPainter()
{
    ui->axisRectPainter->setRectPosition(axisRect().mPositions);
}
