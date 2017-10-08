#include "legendeditingwidget.h"
#include "ui_legendeditingwidget.h"

LegendEditingWidget::LegendEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegendEditingWidget)
{
    ui->setupUi(this);
    connectAll();
}

LegendEditingWidget::~LegendEditingWidget()
{
    delete ui;
}

void LegendEditingWidget::setLegendProperties(style::LegendPref pref)
{
    disconnectAll();
    ui->legendVisibilityCheckBox->setChecked(pref.mIsLegendVisible);
    ui->legendFontWidget->setFont(pref.mLegendFont);
    ui->legendFrameVisiblityCheckBox->setChecked(pref.mIsLegendFrameVisible);
    ui->legendPosiotionComboBox->setLegendLocation(pref.mLocation);
    ui->legendFrameCornerRadiusSpinBox->setValue(pref.mLegendFrameCornerRadius);
    ui->frameShadowVisibilityCheckBox->setChecked(pref.mIsLegendFrameShadowEnabled);
    connectAll();
}

style::LegendPref LegendEditingWidget::legendProperties() const
{
    style::LegendPref ret;
    ret.mIsLegendVisible = ui->legendVisibilityCheckBox->isChecked();
    ret.mLegendFont = ui->legendFontWidget->font();
    ret.mIsLegendFrameVisible = ui->legendFrameVisiblityCheckBox->isChecked();
    ret.mLocation = ui->legendPosiotionComboBox->legendLocation();
    ret.mLegendFrameCornerRadius = ui->legendFrameCornerRadiusSpinBox->value();
    ret.mIsLegendFrameShadowEnabled = ui->frameShadowVisibilityCheckBox->isChecked();

    return ret;
}

void LegendEditingWidget::treatLegendPropertiesEditing()
{
    emit legendPropertiesEdited(legendProperties());
}

void LegendEditingWidget::connectAll()
{
    connect(ui->legendVisibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
    connect(ui->legendFontWidget, SIGNAL(fontEdited(QFont)),
            this, SLOT(treatLegendPropertiesEditing()));
    connect(ui->legendFrameVisiblityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
    connect(ui->legendPosiotionComboBox, SIGNAL(currentLegendLocationChanged(style::LegendLocation)),
            this, SLOT(treatLegendPropertiesEditing()));
    connect(ui->legendFrameCornerRadiusSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatLegendPropertiesEditing()));
    connect(ui->frameShadowVisibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
}

void LegendEditingWidget::disconnectAll()
{
    disconnect(ui->legendVisibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
    disconnect(ui->legendFontWidget, SIGNAL(fontEdited(QFont)),
            this, SLOT(treatLegendPropertiesEditing()));
    disconnect(ui->legendFrameVisiblityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
    disconnect(ui->legendPosiotionComboBox, SIGNAL(currentLegendLocationChanged(style::LegendLocation)),
            this, SLOT(treatLegendPropertiesEditing()));
    disconnect(ui->legendFrameCornerRadiusSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatLegendPropertiesEditing()));
    disconnect(ui->frameShadowVisibilityCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLegendPropertiesEditing()));
}
