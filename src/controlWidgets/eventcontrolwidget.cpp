#include "eventcontrolwidget.h"
#include "ui_eventcontrolwidget.h"
#include "global/global_definitions.h"
#include <QColorDialog>
#include <QDoubleValidator>

EventControlWidget::EventControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::EventControlWidget), mEvent(NULL)
{
    ui->setupUi(this);

    layout()->setSpacing(0);
    layout()->setMargin(0);

    ui->brushStyleComboBox->addItem("NoBrush", Qt::NoBrush);
    ui->brushStyleComboBox->addItem("Solid", Qt::SolidPattern);
    ui->brushStyleComboBox->addItem("Dense1", Qt::Dense1Pattern);
    ui->brushStyleComboBox->addItem("Dense2", Qt::Dense2Pattern);
    ui->brushStyleComboBox->addItem("Dense3", Qt::Dense3Pattern);
    ui->brushStyleComboBox->addItem("Dense4", Qt::Dense4Pattern);
    ui->brushStyleComboBox->addItem("Dense5", Qt::Dense5Pattern);
    ui->brushStyleComboBox->addItem("Dense6", Qt::Dense6Pattern);
    ui->brushStyleComboBox->addItem("Dense7", Qt::Dense7Pattern);
    ui->brushStyleComboBox->addItem("Hor", Qt::HorPattern);
    ui->brushStyleComboBox->addItem("Ver", Qt::VerPattern);
    ui->brushStyleComboBox->addItem("Cross", Qt::CrossPattern);
    ui->brushStyleComboBox->addItem("BDiag", Qt::BDiagPattern);
    ui->brushStyleComboBox->addItem("FDiag", Qt::FDiagPattern);
    ui->brushStyleComboBox->addItem("DiagCross", Qt::DiagCrossPattern);

    ui->brushBasisComboBox->addItem("Left Basis", TimeEvent::Left);
    ui->brushBasisComboBox->addItem("Right Basis", TimeEvent::Right);


    ui->penStyleComboBox->addItem("Solid", Qt::SolidLine);
    ui->penStyleComboBox->addItem("Dash", Qt::DashLine);
    ui->penStyleComboBox->addItem("Dot", Qt::DotLine);
    ui->penStyleComboBox->addItem("DashDot", Qt::DashDotLine);
    ui->penStyleComboBox->addItem("DashDotDot", Qt::DashDotDotLine);

    connect(ui->nameLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatNameChanging()));
    connect(ui->timeLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatTimeChanging()));
    connect(ui->descriptionLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatDescriptionChanging()));
    connect(ui->labelDeviationLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatLabelDeviationChanging()));
    connect(ui->brushColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBrushColorChanging(QColor)));
    connect(ui->brushStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushStyleChanging()));
    connect(ui->brushBasisComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushBasisChanging()));
    connect(ui->brushDurationLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatBrushDurationChanging()));
    connect(ui->addArrowedTextPushButton, SIGNAL(clicked()),
            this, SLOT(treatArrowedTextCreationRequest()));

    connect(ui->penWidthSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatPenWidthChanging()));
    connect(ui->penStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatPenStyleChanging()));
    connect(ui->penColorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatPenColorChanging()));


    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->labelDeviationLineEdit->setValidator(doubleValidator);
    ui->timeLineEdit->setValidator(doubleValidator);
    ui->brushDurationLineEdit->setValidator(doubleValidator);

}

EventControlWidget::~EventControlWidget()
{
    delete ui;
}

void EventControlWidget::setTimeEvent(TimeEvent *newEvent)
{
    if (mEvent != newEvent) {
        mEvent = newEvent;
        initializeState();
    }
}

void EventControlWidget::setExtendedMode(bool)
{

}

void EventControlWidget::treatNameChanging()
{
    if (mEvent) {
        mEvent->setName(ui->nameLineEdit->text());
        ui->nameLineEdit->clearFocus();
    }
}

void EventControlWidget::treatTimeChanging()
{
    if (mEvent) {
        mEvent->setEventTime(ui->timeLineEdit->text().toDouble());
        ui->timeLineEdit->clearFocus();
    }
}

void EventControlWidget::treatDescriptionChanging()
{
    if (mEvent) {
        mEvent->setDescription(ui->descriptionLineEdit->text());
        ui->descriptionLineEdit->clearFocus();
    }
}

void EventControlWidget::treatEventDeleting(QObject* event)
{

    if (mEvent != NULL) {
        if (event && event == mEvent) {
            mEvent = NULL;
            hide();
            emit widgetIsHiding();
        }
    }
}

void EventControlWidget::treatLabelDeviationChanging()
{
    if (mEvent) {
        mEvent->setLabelDeviation(ui->labelDeviationLineEdit->text().toDouble());
        ui->labelDeviationLineEdit->clearFocus();
    }
}

void EventControlWidget::treatPenWidthChanging()
{
    if (mEvent) {
        QPen pen = mEvent->pen();
        pen.setWidthF(ui->penWidthSpinBox->value());
        mEvent->setPen(pen);
        ui->penWidthSpinBox->clearFocus();
    }
}

void EventControlWidget::treatPenStyleChanging()
{
    if (mEvent) {
        QPen pen = mEvent->pen();
        pen.setStyle(ui->penStyleComboBox->currentValue<Qt::PenStyle>());
        mEvent->setPen(pen);
        ui->penStyleComboBox->clearFocus();
    }
}

void EventControlWidget::treatPenColorChanging()
{
    if (mEvent) {
        QPen pen = mEvent->pen();
        pen.setColor(ui->penColorChoiceWidget->color());
        mEvent->setPen(pen);
        ui->penColorChoiceWidget->clearFocus();
    }
}

void EventControlWidget::treatBrushColorChanging(QColor color)
{
    if (mEvent != NULL) {
        ui->brushColorWidget->clearFocus();
        QBrush brush = mEvent->brush();
        brush.setColor(color);
        mEvent->setBrush(brush);
    }
}

void EventControlWidget::treatBrushStyleChanging()
{
    if (mEvent) {
        ui->brushStyleComboBox->clearFocus();
        QBrush brush = mEvent->brush();
        brush.setStyle(ui->brushStyleComboBox->currentValue<Qt::BrushStyle>());
        mEvent->setBrush(brush);

    }
}

void EventControlWidget::treatBrushBasisChanging()
{
    if (mEvent) {
        ui->brushBasisComboBox->clearFocus();
        mEvent->setBrushBasis(ui->brushBasisComboBox->currentValue<TimeEvent::BrushBasis>());
    }
}

void EventControlWidget::treatBrushDurationChanging()
{
    if (mEvent) {
        ui->brushDurationLineEdit->clearFocus();
        mEvent->setBrushDuration(ui->brushDurationLineEdit->text().toDouble());
    }
}

void EventControlWidget::treatArrowedTextCreationRequest()
{
    if (mEvent) {
        emit arrowedTextCreationRequest(mEvent->eventTime());
    }
}

void EventControlWidget::initializeState()
{
    if (mEvent) {
        ui->nameLineEdit->setText(mEvent->name());
        ui->timeLineEdit->setText(QString::number(mEvent->eventTime()));
        ui->descriptionLineEdit->setText(mEvent->description());
        connect(mEvent, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatEventDeleting(QObject*)));

        ui->labelDeviationLineEdit->setText(QString::number(mEvent->labelDeviation()));
        QBrush brush = mEvent->brush();

        ui->brushColorWidget->setColor(brush.color());

        ui->brushBasisComboBox->setCurrentValue(mEvent->brushBasis());
        ui->brushStyleComboBox->setCurrentValue(brush.style());

        ui->brushDurationLineEdit->setText(QString::number(mEvent->brushDuration()));

        QPen pen = mEvent->pen();
        ui->penWidthSpinBox->setValue(pen.widthF());
        ui->penStyleComboBox->setCurrentValue(pen.style());
        ui->penColorChoiceWidget->setColor(pen.color());
    }
}
