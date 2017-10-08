#include "arrowedtextprefseditingwidget.h"
#include "ui_arrowedtextprefseditingwidget.h"

ArrowedTextPrefsEditingWidget::ArrowedTextPrefsEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArrowedTextPrefsEditingWidget)
{
    ui->setupUi(this);

    ui->arrowEndingWidget->addItem("None",        QCPLineEnding::esNone);
    ui->arrowEndingWidget->addItem("FlatArrow",   QCPLineEnding::esFlatArrow);
    ui->arrowEndingWidget->addItem("SpikeArrow",  QCPLineEnding::esSpikeArrow);
    ui->arrowEndingWidget->addItem("LineArrow",   QCPLineEnding::esLineArrow);
    ui->arrowEndingWidget->addItem("Disc",        QCPLineEnding::esDisc);
    ui->arrowEndingWidget->addItem("Square",      QCPLineEnding::esSquare);
    ui->arrowEndingWidget->addItem("Diamond",     QCPLineEnding::esDiamond);
    ui->arrowEndingWidget->addItem("Bar",         QCPLineEnding::esBar);
    ui->arrowEndingWidget->addItem("HalfBar",     QCPLineEnding::esHalfBar);
    ui->arrowEndingWidget->addItem("SkewedBar",   QCPLineEnding::esSkewedBar);
}

ArrowedTextPrefsEditingWidget::~ArrowedTextPrefsEditingWidget()
{
    delete ui;
}

void ArrowedTextPrefsEditingWidget::setArrowedTextPrefs(const style::ArrowedTextPref &prefs)
{
    disconnectAll();
    ui->labelPrefEditingWidget->setLabelPrefs(prefs.mLabelPrefs);
    ui->backgroundColorWidget->setColor(prefs.mBackgroundColor);
    ui->penEditingWidget->setPen(prefs.mPen);
    ui->arrowEndingWidget->setCurrentValue(prefs.mArrowEndingStyle);
    connectAll();
}

style::ArrowedTextPref ArrowedTextPrefsEditingWidget::arrowedTextPrefs() const
{
    style::ArrowedTextPref result;
    result.mLabelPrefs      = ui->labelPrefEditingWidget->labelPrefs();
    result.mBackgroundColor = ui->backgroundColorWidget->color();
    result.mPen             = ui->penEditingWidget->pen();
    result.mArrowEndingStyle= ui->arrowEndingWidget->currentValue<QCPLineEnding::EndingStyle>();
    return result;
}

void ArrowedTextPrefsEditingWidget::connectAll()
{
    connect(ui->labelPrefEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SLOT(treatArrowedTextPrefsChanging()));
    connect(ui->backgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatArrowedTextPrefsChanging()));
    connect(ui->penEditingWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatArrowedTextPrefsChanging()));
    connect(ui->arrowEndingWidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatArrowedTextPrefsChanging()));
}

void ArrowedTextPrefsEditingWidget::disconnectAll()
{
    disconnect(ui->labelPrefEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SLOT(treatArrowedTextPrefsChanging()));
    disconnect(ui->backgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatArrowedTextPrefsChanging()));
    disconnect(ui->penEditingWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatArrowedTextPrefsChanging()));
    disconnect(ui->arrowEndingWidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatArrowedTextPrefsChanging()));
}

void ArrowedTextPrefsEditingWidget::treatArrowedTextPrefsChanging()
{
    emit arrowedTextPrefChanged();
}
