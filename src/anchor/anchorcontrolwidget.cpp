#include "anchorcontrolwidget.h"
#include "ui_anchorcontrolwidget.h"
#include <QString>
#include "commonWidgets/anycombobox.h"

AnchorControlWidget::AnchorControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnchorControlWidget), mAnchor(NULL)
{
    ui->setupUi(this);

    layout()->setSpacing(0);
    layout()->setMargin(0);

    ui->styleComboBox->addItem("None",      QCPItemTracer::tsNone);
    ui->styleComboBox->addItem("Plus",      QCPItemTracer::tsPlus);
    ui->styleComboBox->addItem("Crosshair", QCPItemTracer::tsCrosshair);
    ui->styleComboBox->addItem("Circle",    QCPItemTracer::tsCircle);
    ui->styleComboBox->addItem("Square",    QCPItemTracer::tsSquare);

    connect(ui->nameLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatNameChanging()));
    connect(ui->timeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatTimeChanging()));
    connect(ui->addArrowedTextPushButton, SIGNAL(clicked()),
            this, SLOT(treatArrowedTextCreationRequest()));
    connect(ui->sizeDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatSizeChanging()));
    connect(ui->styleComboBox, SIGNAL(valueActivated(boost::any)),
                this, SLOT(treatStyleChanging()));
    connect(ui->brushColorChoiseWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBrushColorChanging(QColor)));
    connect(ui->borderColorChoiseWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBorderColorChanging(QColor)));
    connect(ui->borderWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatBorderPenWidthChanging()));


}

AnchorControlWidget::~AnchorControlWidget()
{
    delete ui;
}

void AnchorControlWidget::setAnchor(Anchor *anchor)
{
    if (mAnchor != anchor) {
        mAnchor = anchor;
        initializeState();
    }
}

void AnchorControlWidget::treatNameChanging()
{
    if (mAnchor) {
        ui->nameLineEdit->clearFocus();
        mAnchor->setName(ui->nameLineEdit->text());
    }
}

void AnchorControlWidget::treatTimeChanging()
{
    if (mAnchor) {
        ui->timeLineEdit->clearFocus();
        mAnchor->setTime(ui->timeLineEdit->text().toDouble());
    }
}

void AnchorControlWidget::treatAnchorDeleting(QObject* anchor)
{
   if (mAnchor != NULL) {
        if (anchor && anchor == mAnchor) {
            mAnchor = NULL;
            hide();
            emit widgetIsHiding();
        }
    }
}

void AnchorControlWidget::treatArrowedTextCreationRequest()
{
    if (mAnchor) {
        emit arrowedTextCreationRequest(mAnchor->time());
    }
}

void AnchorControlWidget::treatSizeChanging()
{
    if (mAnchor) {
        ui->sizeDoubleSpinBox->clearFocus();
        mAnchor->setSize(ui->sizeDoubleSpinBox->value());
    }
}

void AnchorControlWidget::treatStyleChanging()
{
    if (mAnchor) {
        ui->styleComboBox->clearFocus();
        mAnchor->setStyle(ui->styleComboBox->currentValue<QCPItemTracer::TracerStyle>());
    }
}

void AnchorControlWidget::treatBrushColorChanging(QColor color)
{
    if (mAnchor) {
        mAnchor->setBrushColor(color);
    }
}

void AnchorControlWidget::treatBorderColorChanging(QColor color)
{
    if (mAnchor) {
        mAnchor->setBorderPenColor(color);
    }
}

void AnchorControlWidget::treatBorderPenWidthChanging()
{
    if (mAnchor) {
        mAnchor->setBorderPenWidth(ui->borderWidthDoubleSpinBox->value());
    }
}

void AnchorControlWidget::initializeState()
{
    if (mAnchor) {
        ui->nameLineEdit->setText(mAnchor->name());
        ui->timeLineEdit->setText(QString::number(mAnchor->time()));
        ui->sizeDoubleSpinBox->setValue(mAnchor->size());
        ui->styleComboBox->setCurrentValue(mAnchor->style());
        ui->brushColorChoiseWidget->setColor(mAnchor->brushColor());
        ui->borderWidthDoubleSpinBox->setValue(mAnchor->borderPen().widthF());
        ui->borderColorChoiseWidget->setColor(mAnchor->borderPen().color());

        connect(mAnchor, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatAnchorDeleting(QObject*)));
    }
}
