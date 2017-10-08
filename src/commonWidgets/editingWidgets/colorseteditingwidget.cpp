#include "colorseteditingwidget.h"
#include "ui_colorseteditingwidget.h"

ColorSetEditingWidget::ColorSetEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSetEditingWidget)
{
    ui->setupUi(this);

    layout()->setMargin(0);
    layout()->setSpacing(0);

    ui->graphsColorListChoiceWidget->setHeader("<p align=\"center\"><b>Graph line color</b></p>",
                                               "<p align=\"center\"><b>Graph brush color</b></p>");


    ui->axesBackgroundBrushStylewidget->addItem("NoBrush", Qt::NoBrush);
    ui->axesBackgroundBrushStylewidget->addItem("Solid", Qt::SolidPattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense1", Qt::Dense1Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense2", Qt::Dense2Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense3", Qt::Dense3Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense4", Qt::Dense4Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense5", Qt::Dense5Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense6", Qt::Dense6Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Dense7", Qt::Dense7Pattern);
    ui->axesBackgroundBrushStylewidget->addItem("Hor", Qt::HorPattern);
    ui->axesBackgroundBrushStylewidget->addItem("Ver", Qt::VerPattern);
    ui->axesBackgroundBrushStylewidget->addItem("Cross", Qt::CrossPattern);
    ui->axesBackgroundBrushStylewidget->addItem("BDiag", Qt::BDiagPattern);
    ui->axesBackgroundBrushStylewidget->addItem("FDiag", Qt::FDiagPattern);
    ui->axesBackgroundBrushStylewidget->addItem("DiagCross", Qt::DiagCrossPattern);
    ui->axesBackgroundBrushStylewidget->addItem("TexturePattern", Qt::TexturePattern);

    enableColorListWidget(false);
    connectAll();

}

ColorSetEditingWidget::~ColorSetEditingWidget()
{
    delete ui;
}

void ColorSetEditingWidget::setColorPref(const style::ColorPref &pref)
{
    disconnectAll();
    ui->backgroundColorWidget->setColor(pref.mBackgroundColor);
    ui->axesBackgroundColorWidget->setColor(pref.mAxesBackgroundBrush.color());
    ui->axesBackgroundBrushStylewidget->setCurrentValue(pref.mAxesBackgroundBrush.style());

    ui->grafPrimitivesColorWidget->setColor(pref.mGraficalPrimitivesColor);
    ui->fontColorWidget->setColor(pref.mFontColor);
    ui->graphsColorListChoiceWidget->setColorLists(pref.mGraphColorList, pref.mBrushColorList);
    connectAll();
}

style::ColorPref ColorSetEditingWidget::colorPref() const
{
    style::ColorPref ret;
    ret.mBackgroundColor = ui->backgroundColorWidget->color();
    ret.mAxesBackgroundBrush.setColor(ui->axesBackgroundColorWidget->color());
    ret.mAxesBackgroundBrush.setStyle(ui->axesBackgroundBrushStylewidget->currentValue<Qt::BrushStyle>());

    ret.mGraficalPrimitivesColor = ui->grafPrimitivesColorWidget->color();
    ret.mFontColor= ui->fontColorWidget->color();
    std::tie(ret.mGraphColorList, ret.mBrushColorList) = ui->graphsColorListChoiceWidget->colorLists();
    return ret;
}

void ColorSetEditingWidget::enableColorListWidget(bool enabled)
{
    mIsColorListWidgetEnabled = enabled;
    if (enabled)
        ui->graphsColorListChoiceWidget->show();
    else
        ui->graphsColorListChoiceWidget->hide();
}

void ColorSetEditingWidget::treatColorPrefChanging()
{
    emit colorPrefChangedByUser();
}

void ColorSetEditingWidget::connectAll()
{
    connect(ui->backgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    connect(ui->axesBackgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    connect(ui->axesBackgroundBrushStylewidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatColorPrefChanging()));
    connect(ui->grafPrimitivesColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    connect(ui->fontColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    connect(ui->graphsColorListChoiceWidget, SIGNAL(colorListsChanged()),
            this, SLOT(treatColorPrefChanging()));
}

void ColorSetEditingWidget::disconnectAll()
{
    disconnect(ui->backgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    disconnect(ui->axesBackgroundColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    disconnect(ui->axesBackgroundBrushStylewidget, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatColorPrefChanging()));
    disconnect(ui->grafPrimitivesColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    disconnect(ui->fontColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatColorPrefChanging()));
    disconnect(ui->graphsColorListChoiceWidget, SIGNAL(colorListsChanged()),
            this, SLOT(treatColorPrefChanging()));
}


