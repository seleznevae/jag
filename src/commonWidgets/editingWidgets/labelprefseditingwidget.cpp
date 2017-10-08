#include "labelprefseditingwidget.h"
#include "ui_labelprefseditingwidget.h"

LabelPrefsEditingWidget::LabelPrefsEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabelPrefsEditingWidget)
{
    ui->setupUi(this);

    connectAll();
}

LabelPrefsEditingWidget::~LabelPrefsEditingWidget()
{
    delete ui;
}

void LabelPrefsEditingWidget::setLabelPrefs(const style::Label &labelPref)
{
    disconnectAll();
    ui->fontWidget->setFont(labelPref.mFont);
    ui->colorWidget->setColor(labelPref.mColor);
    connectAll();
}

style::Label LabelPrefsEditingWidget::labelPrefs() const
{
    style::Label ret;
    ret.mFont = ui->fontWidget->font();
    ret.mColor = ui->colorWidget->color();
    return ret;
}

void LabelPrefsEditingWidget::treatPrefsChanging()
{
    emit labelPrefsEdited();
}

void LabelPrefsEditingWidget::connectAll()
{
    connect(ui->fontWidget, SIGNAL(fontEdited(QFont)),
            this, SLOT(treatPrefsChanging()));
    connect(ui->colorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatPrefsChanging()));
}

void LabelPrefsEditingWidget::disconnectAll()
{
    disconnect(ui->fontWidget, SIGNAL(fontEdited(QFont)),
            this, SLOT(treatPrefsChanging()));
    disconnect(ui->colorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatPrefsChanging()));
}
