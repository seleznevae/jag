#include "marginseditingwidget.h"
#include "ui_marginseditingwidget.h"

MarginsEditingWidget::MarginsEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarginsEditingWidget)
{
    ui->setupUi(this);
    connectAll();
}

MarginsEditingWidget::~MarginsEditingWidget()
{
    delete ui;
}

void MarginsEditingWidget::setMargins(const QMargins &margins)
{
    disconnectAll();
    ui->leftMarginSpinBox->setValue(margins.left());
    ui->topMarginSpinBox->setValue(margins.top());
    ui->rightMarginSpinBox->setValue(margins.right());
    ui->bottomMarginSpinBox->setValue(margins.bottom());
    connectAll();
}

QMargins MarginsEditingWidget::margins() const
{
    QMargins margins(ui->leftMarginSpinBox->value(),
                     ui->topMarginSpinBox->value(),
                     ui->rightMarginSpinBox->value(),
                     ui->bottomMarginSpinBox->value());
    return margins;
}

void MarginsEditingWidget::treatMarginsChanging()
{
    emit marginsEdited(margins());
}

void MarginsEditingWidget::connectAll()
{
    connect(ui->leftMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    connect(ui->topMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    connect(ui->rightMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    connect(ui->bottomMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
}

void MarginsEditingWidget::disconnectAll()
{
    disconnect(ui->leftMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    disconnect(ui->topMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    disconnect(ui->rightMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
    disconnect(ui->bottomMarginSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatMarginsChanging()));
}
