#include "colorchoisewidget.h"
#include "ui_colorchoisewidget.h"
#include "global/global_definitions.h"
#include <QColorDialog>
#include <cstdlib>

ColorChoiseWidget::ColorChoiseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorChoiseWidget)
{
    ui->setupUi(this);

    ui->colorButton->setScheme(&global::gColorSceme);
    ui->colorButton->setCellSize (24);
    //show arrow
//    ui->colorButton->setPopupMode(QToolButton::MenuButtonPopup );
//    ui->colorButton->setPickModeRight (QSint::ColorButton::PM_COLORDIALOG);

    connect(ui->colorDialogToolButton, SIGNAL(clicked()),
            this, SLOT(showColorDialog()));
    connect(ui->colorButton, SIGNAL(colorChanged(QColor)),
            this, SIGNAL(currentColorChanged(QColor)));
}

ColorChoiseWidget::~ColorChoiseWidget()
{
    delete ui;
}

void ColorChoiseWidget::addColorToScheme(const QColor& newColor)
{
    if (global::gColorSceme.indexOf(newColor) == -1) {
        global::gColorSceme << newColor;
        ui->colorButton->setScheme(&global::gColorSceme);
    }
}

void ColorChoiseWidget::setColor(const QColor &color)
{
    addColorToScheme(color);
    if (color != ui->colorButton->color()) {
        ui->colorButton->setColor(color);
        emit currentColorChanged(color);
    }
}

QColor ColorChoiseWidget::color() const
{
    return ui->colorButton->color();
}

void ColorChoiseWidget::showColorDialog()
{
    QColor newColor = QColorDialog::getColor(ui->colorButton->color(), this, "Choose color", QColorDialog::ShowAlphaChannel);
    if (newColor.isValid()) {
        setColor(newColor);
    }
    ui->colorDialogToolButton->clearFocus();
}
