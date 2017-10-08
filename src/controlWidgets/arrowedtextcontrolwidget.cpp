#include "arrowedtextcontrolwidget.h"
#include "ui_arrowedtextcontrolwidget.h"
#include <QDoubleValidator>
#include "global/global_definitions.h"
#include <QColorDialog>

ArrowedTextControlWidget::ArrowedTextControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::ArrowedTextControlWidget), mArrowedText(NULL)
{
    ui->setupUi(this);

    layout()->setSpacing(0);
    layout()->setMargin(0);

    ui->penChoiceWidget->setHeader("");

    ui->arrowStyleComboBox->addItem("None",        QCPLineEnding::esNone);
    ui->arrowStyleComboBox->addItem("FlatArrow",   QCPLineEnding::esFlatArrow);
    ui->arrowStyleComboBox->addItem("SpikeArrow",  QCPLineEnding::esSpikeArrow);
    ui->arrowStyleComboBox->addItem("LineArrow",   QCPLineEnding::esLineArrow);
    ui->arrowStyleComboBox->addItem("Disc",        QCPLineEnding::esDisc);
    ui->arrowStyleComboBox->addItem("Square",      QCPLineEnding::esSquare);
    ui->arrowStyleComboBox->addItem("Diamond",     QCPLineEnding::esDiamond);
    ui->arrowStyleComboBox->addItem("Bar",         QCPLineEnding::esBar);
    ui->arrowStyleComboBox->addItem("HalfBar",     QCPLineEnding::esHalfBar);
    ui->arrowStyleComboBox->addItem("SkewedBar",   QCPLineEnding::esSkewedBar);

    connect(ui->nameLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatNameChanging()));
    connect(ui->textXCoordLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatTextXCoordChanging()));
    connect(ui->textYCoordLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatTextYCoordChanging()));
    connect(ui->arrowXCoordLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatArrowXCoordChanging()));
    connect(ui->arrowYCoordLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatArrowYCoordChanging()));

    connect(ui->penChoiceWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatPenChanging()));

//    connect(ui->penWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
//            this, SLOT(treatLineWidthChanging()));
    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
//    ui->penWidthDoubleSpinBox->setKeyboardTracking(false);

    connect(ui->backgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBackgroundColorChanging(QColor)));
    connect(ui->arrowStyleComboBox, SIGNAL(valueActivated(boost::any)),
                this, SLOT(treatArrowStyleChanging()));

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->textXCoordLineEdit->setValidator(doubleValidator);
    ui->textYCoordLineEdit->setValidator(doubleValidator);
    ui->arrowXCoordLineEdit->setValidator(doubleValidator);
    ui->arrowYCoordLineEdit->setValidator(doubleValidator);

}

void ArrowedTextControlWidget::setArrowedText(ArrowedText *arrowedText)
{
    if (mArrowedText != arrowedText) {
        mArrowedText = arrowedText;
        initializeState();
    }
}

ArrowedTextControlWidget::~ArrowedTextControlWidget()
{
    delete ui;
}

void ArrowedTextControlWidget::setExtendedMode(bool)
{

}

void ArrowedTextControlWidget::treatNameChanging()
{
    if (mArrowedText) {
        mArrowedText->setText(ui->nameLineEdit->text());
        ui->nameLineEdit->clearFocus();
    }
}

void ArrowedTextControlWidget::treatTextXCoordChanging()
{
    if (mArrowedText) {
        auto textCenter = mArrowedText->textCenter();
        textCenter.setX(ui->textXCoordLineEdit->text().toDouble());
        mArrowedText->setTextCenter(textCenter);
        ui->textXCoordLineEdit->clearFocus();
    }
}

void ArrowedTextControlWidget::treatTextYCoordChanging()
{
    if (mArrowedText) {
        auto textCenter = mArrowedText->textCenter();
        textCenter.setY(ui->textYCoordLineEdit->text().toDouble());
        mArrowedText->setTextCenter(textCenter);
        ui->textYCoordLineEdit->clearFocus();
    }
}

void ArrowedTextControlWidget::treatArrowXCoordChanging()
{
    if (mArrowedText) {
        auto arrowFinish = mArrowedText->arrowFinish();
        arrowFinish.setX(ui->arrowXCoordLineEdit->text().toDouble());
        mArrowedText->setArrowFinish(arrowFinish);
        ui->textYCoordLineEdit->clearFocus();
    }
}

void ArrowedTextControlWidget::treatArrowYCoordChanging()
{
    if (mArrowedText) {
        auto arrowFinish = mArrowedText->arrowFinish();
        arrowFinish.setY(ui->arrowYCoordLineEdit->text().toDouble());
        mArrowedText->setArrowFinish(arrowFinish);
        ui->textYCoordLineEdit->clearFocus();
    }
}

void ArrowedTextControlWidget::treatPenChanging()
{
    if (mArrowedText) {
        mArrowedText->setPen(ui->penChoiceWidget->pen());
        ui->penChoiceWidget->clearFocus();
    }
}

//void ArrowedTextControlWidget::treatLineWidthChanging()
//{
//    if (mArrowedText) {
//        mArrowedText->setLineWidthF(ui->penWidthDoubleSpinBox->value());
//        ui->penWidthDoubleSpinBox->clearFocus();
//    }
//}

void ArrowedTextControlWidget::treatBackgroundColorChanging(QColor color)
{
    if (mArrowedText != NULL) {
        ui->backgroundColorWidget->clearFocus();
        mArrowedText->setBackgroundColor(color);
    }
}

void ArrowedTextControlWidget::treatArrowedTextDeleting(QObject *arrowedText)
{
    if (mArrowedText != NULL) {
        if (arrowedText && arrowedText == mArrowedText) {
            mArrowedText = NULL;
            hide();
            emit widgetIsHiding();
        }
    }
}

void ArrowedTextControlWidget::treatArrowStyleChanging()
{
    if (mArrowedText) {
        ui->arrowStyleComboBox->clearFocus();
        mArrowedText->setArrowStyle(ui->arrowStyleComboBox->currentValue<QCPLineEnding::EndingStyle>());
    }
}

void ArrowedTextControlWidget::initializeState()
{
    if (mArrowedText) {
        connect(mArrowedText, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatArrowedTextDeleting(QObject*)));

        ui->nameLineEdit->setText(mArrowedText->text());
        auto textCenter = mArrowedText->textCenter();
        ui->textXCoordLineEdit->setText(QString::number(textCenter.x()));
        ui->textYCoordLineEdit->setText(QString::number(textCenter.y()));

        auto arrowPoint = mArrowedText->arrowFinish();
        ui->arrowXCoordLineEdit->setText(QString::number(arrowPoint.x()));
        ui->arrowYCoordLineEdit->setText(QString::number(arrowPoint.y()));

//        ui->penWidthDoubleSpinBox->setValue(mArrowedText->lineWidthF());
        ui->penChoiceWidget->setPen(mArrowedText->pen());

        ui->backgroundColorWidget->setColor(mArrowedText->backgroundColor());
        ui->arrowStyleComboBox->setCurrentValue(mArrowedText->arrowStyle());
    }
}
