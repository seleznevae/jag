#include "titleeditingwidget.h"
#include "ui_titleeditingwidget.h"
#include <QPen>

TitleEditingWidget::TitleEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleEditingWidget)
{
    ui->setupUi(this);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);

    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
    ui->titleFontSizeSpinBox->setKeyboardTracking(false);
}

TitleEditingWidget::~TitleEditingWidget()
{
    delete ui;
}

void TitleEditingWidget::setTitleProperties(const style::Label &titleProps)
{
    disconnectAll();
    ui->titleColorWidget->setColor(titleProps.mColor);
    ui->titleFontSizeSpinBox->setValue(titleProps.mFont.pointSize());
    connectAll();
}

void TitleEditingWidget::setTitle(const QString &title)
{
    disconnectAll();
    ui->titleEditWidget->setText(title);
    connectAll();
}

style::Label TitleEditingWidget::titleProperties() const
{
    style::Label ret;
    ret.mFont = ui->titleEditWidget->font();
    ret.mFont.setPointSize(ui->titleFontSizeSpinBox->value());
    ret.mColor = ui->titleColorWidget->color();
    return ret;
}

QString TitleEditingWidget::title() const
{
    return ui->titleEditWidget->text();
}

void TitleEditingWidget::connectAll()
{
    connect(ui->titleColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SIGNAL(titlePropertiesChanged()));
    connect(ui->titleFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(titlePropertiesChanged()));
    connect(ui->titleEditWidget, SIGNAL(editingFinished()),
            this, SIGNAL(titleChanged()));
    connect(ui->titleEditWidget, SIGNAL(editingFinished()),
            this, SIGNAL(titlePropertiesChanged()));
}

void TitleEditingWidget::disconnectAll()
{
    disconnect(ui->titleColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SIGNAL(titlePropertiesChanged()));
    disconnect(ui->titleFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(titlePropertiesChanged()));
    disconnect(ui->titleEditWidget, SIGNAL(editingFinished()),
            this, SIGNAL(titleChanged()));
    disconnect(ui->titleEditWidget, SIGNAL(editingFinished()),
            this, SIGNAL(titlePropertiesChanged()));
}


