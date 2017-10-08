#include "grideditingwidget.h"
#include "ui_grideditingwidget.h"


GridEditingWidget::GridEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridEditingWidget)
{
    ui->setupUi(this);

    layout()->setSpacing(0);
    layout()->setMargin(0);
}

GridEditingWidget::~GridEditingWidget()
{
    delete ui;
}

void GridEditingWidget::setHead(const QString &head)
{
    ui->visibilityCheckBox->setText(head + " visible");
    ui->penChoiceWidget->setHeader(head);
}

void GridEditingWidget::setGrid(style::Grid newGrid)
{
    disconnectAll();
    ui->penChoiceWidget->setPen(newGrid.mPen);
    ui->visibilityCheckBox->setChecked(newGrid.mIsVisible);
    connectAll();
}

style::Grid GridEditingWidget::grid() const
{
    style::Grid newGrid;
    newGrid.mIsVisible = ui->visibilityCheckBox->isChecked();
    newGrid.mPen = ui->penChoiceWidget->pen();
    return newGrid;
}

void GridEditingWidget::connectAll()
{
    connect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatPropertiesChanging()));
    connect(ui->visibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatPropertiesChanging()));

}

void GridEditingWidget::disconnectAll()
{
    disconnect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatPropertiesChanging()));
    disconnect(ui->visibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatPropertiesChanging()));
}

void GridEditingWidget::treatPropertiesChanging()
{
    emit gridChanged(grid());
}


