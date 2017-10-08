#include "tickeditingwidget.h"
#include "ui_tickeditingwidget.h"
#include "algorithm"

TickEditingWidget::TickEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TickEditingWidget)
{
    ui->setupUi(this);


    for (QLayout *lay : QVector<QLayout*>{layout(), ui->horizontalLayout, ui->horizontalLayout_3}) {
        lay->setSpacing(0);
        lay->setMargin(0);
    }

    ui->penChoiceWidget->setHeader("Tick pen");
    ui->penChoiceWidget->enableStyleSetting(false);

}

TickEditingWidget::~TickEditingWidget()
{
    delete ui;
}

style::MainTick TickEditingWidget::tick() const
{
    style::MainTick ret;
    ret.mPen          = ui->penChoiceWidget->pen();
    ret.mLengthIn     = ui->lengthInDoubleSpinBox->value();
    ret.mLengthOut    = ui->lengthOutDoubleSpinBox->value();
    ret.mSubTickCount = ui->subTickCountSpinBox->value();
    ret.mPositions    = ui->positionChoiceWidget->position();
    ret.mTicksOnInternalAxisRectEnabled = ui->internalTicksEnabledCheckBox->isChecked();

    return ret;
}

void TickEditingWidget::setTick(const style::MainTick &tick)
{
    disconnectAll();
    setSubTickCountVisibility(true);
    ui->subTickCountSpinBox->setValue(tick.mSubTickCount);
    setWidgetPropertiesFromTickPrefs(tick);
    ui->positionChoiceWidget->setPositionType(PositionType::posTick);
    connectAll();
}

void TickEditingWidget::setSubTick(const style::Tick &tick)
{
    disconnectAll();
    setSubTickCountVisibility(false);
    setWidgetPropertiesFromTickPrefs(tick);
    ui->positionChoiceWidget->setPositionType(PositionType::posSubTick);
    connectAll();
}

void TickEditingWidget::setSubTickCountVisibility(bool visible)
{
    ui->subTickCountLabel  ->setVisible(visible);
    ui->subTickCountSpinBox->setVisible(visible);
    ui->penChoiceWidget->setHeader(QString(visible ? "Tick" : "Subtick") + " pen");
}

void TickEditingWidget::connectAll()
{

    connect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SIGNAL(tickChanged()));
    connect(ui->lengthInDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SIGNAL(tickChanged()));
    connect(ui->lengthOutDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SIGNAL(tickChanged()));
    connect(ui->subTickCountSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(tickChanged()));
    connect(ui->positionChoiceWidget, SIGNAL(positionChangedByUser()),
            this, SIGNAL(tickChanged()));

}

void TickEditingWidget::disconnectAll()
{
    disconnect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SIGNAL(tickChanged()));
    disconnect(ui->lengthInDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SIGNAL(tickChanged()));
    disconnect(ui->lengthOutDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SIGNAL(tickChanged()));
    disconnect(ui->subTickCountSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(tickChanged()));
    disconnect(ui->positionChoiceWidget, SIGNAL(positionChangedByUser()),
               this, SIGNAL(tickChanged()));
}

void TickEditingWidget::setWidgetPropertiesFromTickPrefs(const style::Tick &tick)
{
    ui->penChoiceWidget->setPen(tick.mPen);
    ui->lengthInDoubleSpinBox->setValue(tick.mLengthIn);
    ui->lengthOutDoubleSpinBox->setValue(tick.mLengthOut);
    ui->positionChoiceWidget->setPosition(tick.mPositions);
    ui->internalTicksEnabledCheckBox->setChecked(tick.mTicksOnInternalAxisRectEnabled);
}
