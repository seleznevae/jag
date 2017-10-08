#include "penchoicewidget.h"
#include "ui_penchoicewidget.h"
#include "commonWidgets/anycombobox.h"
#include <QDebug>
#include "global/global_definitions.h"

PenChoiceWidget::PenChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PenChoiceWidget)
{
    ui->setupUi(this);
    layout()->setSpacing(0);
    layout()->setMargin(0);
//    ui->gridLayout->setSpacing(0);
//    ui->gridLayout->setMargin(0);
    ui->horizontalLayout->setSpacing(0);
    ui->horizontalLayout->setMargin(0);

    ui->lineStyleWidget->addItem(global::getStyledIcon("ls_solid"       ), Qt::SolidLine);
    ui->lineStyleWidget->addItem(global::getStyledIcon("ls_dash"        ), Qt::DashLine);
    ui->lineStyleWidget->addItem(global::getStyledIcon("ls_dot"         ), Qt::DotLine);
    ui->lineStyleWidget->addItem(global::getStyledIcon("ls_dash_dot"    ), Qt::DashDotLine);
    ui->lineStyleWidget->addItem(global::getStyledIcon("ls_dash_dot_dot"), Qt::DashDotDotLine);

    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
    ui->penWidthDoubleSpinBox->setKeyboardTracking(false);
}

PenChoiceWidget::~PenChoiceWidget()
{
    delete ui;
}

void PenChoiceWidget::setPen(const QPen &pen)
{
    disconnectSlots();
    ui->penWidthDoubleSpinBox->setValue(pen.widthF());
    ui->colorChoiceWidget->setColor(pen.color());
    ui->lineStyleWidget->setCurrentValue(pen.style());
    connectSlots();
}

QPen PenChoiceWidget::pen() const
{
    QPen ret;
    ret.setWidthF(ui->penWidthDoubleSpinBox->value());
    ret.setColor(ui->colorChoiceWidget->color());
    ret.setStyle(ui->lineStyleWidget->currentValue<Qt::PenStyle>());
    return ret;
}

void PenChoiceWidget::setHeader(const QString &header)
{
//    ui->widthLabel->setText(header + " line width");
//    ui->colorLabel->setText(header + " color");
//    ui->styleLabel->setText(header + " style");

    ui->label->setText(header);
}

void PenChoiceWidget::enableStyleSetting(bool enabled)
{
//    ui->styleLabel->setVisible(enabled);
    ui->lineStyleWidget->setVisible(enabled);
}

void PenChoiceWidget::connectSlots()
{
    connect(ui->penWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatPenChanging()));
    connect(ui->colorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatPenChanging()));
    connect(ui->lineStyleWidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatPenChanging()));
}

void PenChoiceWidget::disconnectSlots()
{
    disconnect(ui->penWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatPenChanging()));
    disconnect(ui->colorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatPenChanging()));
    disconnect(ui->lineStyleWidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatPenChanging()));
}

void PenChoiceWidget::treatPenChanging()
{
    QPen penToEmit = pen();
    emit penChanged(penToEmit);
}
