#include "plotcontrolwidget.h"
#include "ui_plotcontrolwidget.h"
#include <QDoubleValidator>
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"


QVector<bool> PlotControlWidget::sTabVisibilityVector = {true, true, true, true, true, true, true, true, true, true, true, true, true};
QVector<QString> PlotControlWidget::sTabNamesVector = {"Margins", "Legend", "Y", "X", "Title", "AxisRect", "Grid", "SubGrid", "Tick", "SubTick", "Misc", "Colors", "Axes pref."};

PlotControlWidget::PlotControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::PlotControlWidget)
{
    ui->setupUi(this);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout_3->setSpacing(0);
    ui->verticalLayout_3->setMargin(0);
    ui->verticalLayout_4->setSpacing(0);
    ui->verticalLayout_4->setMargin(0);
    ui->verticalLayout_5->setSpacing(0);
    ui->verticalLayout_5->setMargin(0);
    ui->verticalLayout_6->setSpacing(0);
    ui->verticalLayout_6->setMargin(0);
    ui->verticalLayout_8->setSpacing(0);
    ui->verticalLayout_8->setMargin(0);


    layout()->setSpacing(0);
    layout()->setMargin(0);



    mFastenToolButtonVector << ui->fastenToolButton_0;
    mFastenToolButtonVector << ui->fastenToolButton_1;
    mFastenToolButtonVector << ui->fastenToolButton_2;
    mFastenToolButtonVector << ui->fastenToolButton_3;
    mFastenToolButtonVector << ui->fastenToolButton_4;
    mFastenToolButtonVector << ui->fastenToolButton_5;
    mFastenToolButtonVector << ui->fastenToolButton_6;
    mFastenToolButtonVector << ui->fastenToolButton_7;
    mFastenToolButtonVector << ui->fastenToolButton_8;
    mFastenToolButtonVector << ui->fastenToolButton_9;
    mFastenToolButtonVector << ui->fastenToolButton_10;
    mFastenToolButtonVector << ui->fastenToolButton_11;
    mFastenToolButtonVector << ui->fastenToolButton_12;
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        mFastenToolButtonVector[i]->setChecked(sTabVisibilityVector[i]);
        connect(mFastenToolButtonVector[i], SIGNAL(toggled(bool)),
                this, SLOT(treatFastenToolButtonPressed()));
    }
    mTabsVector << ui->tab_0;
    mTabsVector << ui->tab_1;
    mTabsVector << ui->tab_2;
    mTabsVector << ui->tab_3;
    mTabsVector << ui->tab_4;
    mTabsVector << ui->tab_5;
    mTabsVector << ui->tab_6;
    mTabsVector << ui->tab_7;
    mTabsVector << ui->tab_8;
    mTabsVector << ui->tab_9;
    mTabsVector << ui->tab_10;
    mTabsVector << ui->tab_11;
    mTabsVector << ui->tab_12;

    for (auto tab : mTabsVector) {
        tab->layout()->setSpacing(0);
        tab->layout()->setMargin(0);
    }


    ui->xGridWidget->setHead("X grid");
    ui->yGridWidget->setHead("Y grid");
    ui->xSubGridWidget->setHead("X sub grid");
    ui->ySubGridWidget->setHead("Y sub grid");

    ui->scaleTypeComboBox->addItem("Linear", PlotLayoutModel::ScaleType::Linear);
    ui->scaleTypeComboBox->addItem("Logarithmic", PlotLayoutModel::ScaleType::Logarithmic);

    ui->timeFormatComboBox->addItem("Number", QCPAxis::ltNumber);
    ui->timeFormatComboBox->addItem("DateTime", QCPAxis::ltDateTime);

    ui->subTickEditingWidget->setSubTickCountVisibility(false);

    connect(ui->xLowMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatXLowLimitRequest()));
    connect(ui->xHighMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatXHighLimitRequest()));
    connect(ui->xLabelLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatXLabelChanging()));
    connect(ui->titleEditingWidget, SIGNAL(titleChanged()),
            this, SLOT(treatTitleChanging()));
    connect(ui->titleEditingWidget, SIGNAL(titlePropertiesChanged()),
            this, SLOT(treatTitlePropertiesChanging()));

    connect(ui->scaleTypeComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatScaleTypeChanging()));
    connect(ui->timeFormatComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatTimeAxisTypeChanging()));
    connect(ui->xTickRotationDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatXTickRotationChanging()));
    connect(ui->eventLabelRotationDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatCommandLabelRotationChanging()));

    connect(ui->legendLayoutComboBox, SIGNAL(currentLegendLocationChanged(style::LegendLocation)),
            this, SLOT(treatLegendLayoutChanging()));

    connect(ui->legendFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatLegendFontSizeChanging()));
    connect(ui->yLabelFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatUnitsFontSizeChanging()));
    connect(ui->xLabelFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatXLabelFontSizeChanging()));
    connect(ui->xAxisFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatXAxisFontSizeChanging()));
    connect(ui->yAxisFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatYAxisFontSizeChanging()));
//    connect(ui->titleFontSizeSpinBox, SIGNAL(valueChanged(int)),
//            this, SLOT(treatTitleFontSizeChanging()));
    connect(ui->eventFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatEventFontSizeChanging()));
    connect(ui->arrowedTextFontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatArrowedTextFontSizeChanging()));

    connect(ui->legendFrameVisibilityCheckBox, SIGNAL(toggled ( bool )),
            this, SLOT(treatLegendFrameVisibilityChanging()));
    connect(ui->legendVisibilityCheckBox, SIGNAL(toggled ( bool )),
            this, SLOT(treatLegendVisibilityChanging()));
    connect(ui->unitsVisibilityCheckBox, SIGNAL(toggled ( bool )),
            this, SLOT(treatUnitsVisibilityChanging()));

    connect(ui->defaultPushButton, SIGNAL(clicked(bool)),
            this, SLOT(initiliazeLayoutPreferencesFromDefault()));


    connect(ui->xGridWidget, SIGNAL(gridChanged(style::Grid)),
            this, SLOT(treatXGridChanging()));
    connect(ui->yGridWidget, SIGNAL(gridChanged(style::Grid)),
            this, SLOT(treatYGridChanging()));
    connect(ui->xSubGridWidget, SIGNAL(gridChanged(style::Grid)),
            this, SLOT(treatXSubGridChanging()));
    connect(ui->ySubGridWidget, SIGNAL(gridChanged(style::Grid)),
            this, SLOT(treatYSubGridChanging()));
    connect(ui->axisRectEditingWidget, SIGNAL(axisRectPropertiesChanged()),
            this, SLOT(treatAxisRectChanging()));
    connect(ui->tickEditingWidget, SIGNAL(tickChanged()),
            this, SLOT(treatTickChanging()));
    connect(ui->subTickEditingWidget, SIGNAL(tickChanged()),
            this, SLOT(treatSubTickChanging()));
    connect(ui->colorSetEdititngWidget, SIGNAL(colorPrefChangedByUser()),
            this, SLOT(treatColorPrefChanging()));

    connect(ui->xAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SLOT(treatXAxisPrefChanging()));
    connect(ui->yAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SLOT(treatYAxisPrefChanging()));

    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
    ui->xTickRotationDoubleSpinBox->setKeyboardTracking(false);
    ui->eventLabelRotationDoubleSpinBox->setKeyboardTracking(false);
    ui->legendFontSizeSpinBox->setKeyboardTracking(false);
    ui->yLabelFontSizeSpinBox->setKeyboardTracking(false);
    ui->xLabelFontSizeSpinBox->setKeyboardTracking(false);
    ui->xAxisFontSizeSpinBox->setKeyboardTracking(false);
//    ui->titleFontSizeSpinBox->setKeyboardTracking(false);
    ui->eventFontSizeSpinBox->setKeyboardTracking(false);
    ui->arrowedTextFontSizeSpinBox->setKeyboardTracking(false);

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->xLowMarginLineEdit->setValidator(doubleValidator);
    ui->xHighMarginLineEdit->setValidator(doubleValidator);

}

PlotControlWidget::~PlotControlWidget()
{
    delete ui;
}

void PlotControlWidget::setLayoutModel(PlotLayoutModel *layoutModel)
{
    mLayoutModel = layoutModel;
    initializeState();
}

void PlotControlWidget::setExtendedMode(bool isModeExtended)
{
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        mFastenToolButtonVector[i]->setVisible(isModeExtended);
    }

    int limit = ui->tabWidget->count();
    for (int i = 0; i < limit; ++i)
        ui->tabWidget->removeTab(0);
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        ui->tabWidget->insertTab(i, mTabsVector[i], sTabNamesVector[i]);

    }

    if (isModeExtended == false) {
        for (int i = sTabVisibilityVector.size() - 1; i >= 0; --i) {
            if (sTabVisibilityVector[i] == false)
                ui->tabWidget->removeTab(i);
        }
    }
}

void PlotControlWidget::initializeState()
{
    if (mLayoutModel != NULL) {

        auto xRange = mLayoutModel->getXRange();

        ui->titleEditingWidget->setTitle(mLayoutModel->title());
        ui->titleEditingWidget->setTitleProperties(mLayoutModel->titleProperties());

        ui->xLowMarginLineEdit->setText(QString::number(std::get<0>(xRange), 'g', global::gDoubleToStringConversionPrecision));
        ui->xHighMarginLineEdit->setText(QString::number(std::get<1>(xRange), 'g', global::gDoubleToStringConversionPrecision));

        ui->legendVisibilityCheckBox->setCheckState(mLayoutModel->isLegendVisible() ?  Qt::Checked : Qt::Unchecked);
        ui->unitsVisibilityCheckBox->setCheckState(mLayoutModel->areUnitsVisible() ?  Qt::Checked : Qt::Unchecked);
        ui->legendFontSizeSpinBox->setValue(mLayoutModel->legendFont().pointSize());
        ui->yLabelFontSizeSpinBox->setValue(mLayoutModel->yLabelFont().pointSize());
        ui->xLabelLineEdit->setText(mLayoutModel->xLabel());
        ui->xLabelFontSizeSpinBox->setValue(mLayoutModel->xLabelFont().pointSize());
        ui->xAxisFontSizeSpinBox->setValue(mLayoutModel->xAxisFont().pointSize());
        ui->yAxisFontSizeSpinBox->setValue(mLayoutModel->yAxisFont().pointSize());


//        ui->titleFontSizeSpinBox->setValue(mLayoutModel->titleFont().pointSize());

        ui->eventFontSizeSpinBox->setValue(mLayoutModel->commandFont().pointSize());
        ui->arrowedTextFontSizeSpinBox->setValue(mLayoutModel->arrowedTextFont().pointSize());

        ui->xTickRotationDoubleSpinBox->setValue(mLayoutModel->xTickRotation());
        ui->eventLabelRotationDoubleSpinBox->setValue(mLayoutModel->commandLabelRotation());

        ui->timeFormatComboBox->setCurrentValue(mLayoutModel->timeAxisType());
        ui->scaleTypeComboBox->setCurrentValue(mLayoutModel->scaleType());

//        ui->legendLayoutWidget->setCurrentValue(mLayoutModel->legendLayout());
        ui->legendLayoutComboBox->setLegendLocation(mLayoutModel->legendLayout());
        ui->legendFrameVisibilityCheckBox->setChecked(mLayoutModel->isLegendFrameVisible());

        ui->xGridWidget->setGrid(mLayoutModel->xGrid());
        ui->yGridWidget->setGrid(mLayoutModel->yGrid());
        ui->xSubGridWidget->setGrid(mLayoutModel->xSubGrid());
        ui->ySubGridWidget->setGrid(mLayoutModel->ySubGrid());
        ui->axisRectEditingWidget->setAxisRect(mLayoutModel->axisRect());


        PlotStyle pref = mLayoutModel->plotStyle();
        ui->tickEditingWidget->setTick(pref.mTickProperties);
        ui->subTickEditingWidget->setSubTick(pref.mSubTickProperties);
        ui->colorSetEdititngWidget->setColorPref(pref.mColorPref);

        ui->xAxisPrefWidget->setAxisPref(pref.mXAxisPref);
        ui->yAxisPrefWidget->setAxisPref(pref.mYAxisPref);


//        connect(ui->legendFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setLegendFontSize(int)));
//        connect(ui->dimensionFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setDimensionFontSize(int)));
//        connect(ui->xLabelFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setXLabelFontSize(int)));
//        connect(ui->xAxisFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setXAxisFontSize(int)));
//        connect(ui->yAxisFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setYAxisFontSize(int)));
//        connect(ui->titleFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setTitleFontSize(int)));
//        connect(ui->eventFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setTimeEventFontSize(int)));
//        connect(ui->arrowedTextFontSizeSpinBox, SIGNAL(valueChanged(int)),
//                mLayoutModel, SLOT(setArrowedTextFontSize(int)));

//        connect(ui->legendFrameVisibilityCheckBox, SIGNAL(toggled(bool)),
//                mLayoutModel, SLOT(setLegendFrameVisibility(bool)));

//        void layoutModelChanged();
//        void layoutAppearanceChanged();
        connect(mLayoutModel, SIGNAL(layoutAppearanceChanged()),
                this, SLOT(updateRanges()));
        connect(mLayoutModel, SIGNAL(layoutModelChanged()),
                this, SLOT(updateRanges()));
    }
}

void PlotControlWidget::treatXLowLimitRequest()
{
//    ui->xLowMarginLineEdit->clearFocus();
//    ui->xHighMarginLineEdit->clearFocus();
//    emit settingXRangeRequest(
//                ui->xLowMarginLineEdit->text().toDouble(),
//                ui->xHighMarginLineEdit->text().toDouble()
//                );
    if (mLayoutModel != NULL) {
        ui->xLowMarginLineEdit->clearFocus();
        ui->xHighMarginLineEdit->clearFocus();
        mLayoutModel->setXRangeWrapper(ui->xLowMarginLineEdit->text().toDouble(), ui->xHighMarginLineEdit->text().toDouble());
    }
}


void PlotControlWidget::treatXHighLimitRequest()
{
//    ui->xLowMarginLineEdit->clearFocus();
//    ui->xHighMarginLineEdit->clearFocus();
//    emit settingXRangeRequest(
//                ui->xLowMarginLineEdit->text().toDouble(),
//                ui->xHighMarginLineEdit->text().toDouble()
//                );

    if (mLayoutModel != NULL) {
        ui->xLowMarginLineEdit->clearFocus();
        ui->xHighMarginLineEdit->clearFocus();
        mLayoutModel->setXRangeWrapper(ui->xLowMarginLineEdit->text().toDouble(), ui->xHighMarginLineEdit->text().toDouble());
    }
}

void PlotControlWidget::treatXLabelChanging()
{
    ui->xLabelLineEdit->clearFocus();
    mLayoutModel->setXLabel(ui->xLabelLineEdit->text());
}

void PlotControlWidget::treatTitleChanging()
{
    ui->titleEditingWidget->clearFocus();
    if (mLayoutModel != NULL) {
        QString title = ui->titleEditingWidget->title();
        title.replace(QString("\\n"), QString("\n"));
        mLayoutModel->setTitle(title);
    }

}

void PlotControlWidget::treatTitlePropertiesChanging()
{
    ui->titleEditingWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setTitleProperties(ui->titleEditingWidget->titleProperties());
    }
}

void PlotControlWidget::treatScaleTypeChanging()
{
    ui->scaleTypeComboBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setScaleType(ui->scaleTypeComboBox->currentValue<PlotLayoutModel::ScaleType>());
    }
}

void PlotControlWidget::treatTimeAxisTypeChanging()
{
    ui->timeFormatComboBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setTimeAxisType(ui->timeFormatComboBox->currentValue<QCPAxis::LabelType>());

    }
}

void PlotControlWidget::setMinWidth(int newMinWidth)
{
    setMinimumWidth(newMinWidth);
}

void PlotControlWidget::treatXTickRotationChanging()
{
    ui->xTickRotationDoubleSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setXTickRotaion(ui->xTickRotationDoubleSpinBox->value());
    }
}

void PlotControlWidget::treatCommandLabelRotationChanging()
{
    ui->eventLabelRotationDoubleSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setTimeEventLabelRotaion(ui->eventLabelRotationDoubleSpinBox->value());
    }
}

void PlotControlWidget::treatLegendLayoutChanging()
{
    ui->legendLayoutComboBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setLegendLayout(ui->legendLayoutComboBox->legendLocation());
    }
}

void PlotControlWidget::treatLegendFontSizeChanging()
{
    ui->legendFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setLegendFontSize(ui->legendFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatUnitsFontSizeChanging()
{
    ui->yLabelFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setYLabelFontSize(ui->yLabelFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatXLabelFontSizeChanging()
{
    ui->xLabelFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setXLabelFontSize(ui->xLabelFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatXAxisFontSizeChanging()
{
    ui->xAxisFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setXAxisFontSize(ui->xAxisFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatYAxisFontSizeChanging()
{
    ui->yAxisFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setYAxisFontSize(ui->yAxisFontSizeSpinBox->value());
    }
}

//void PlotControlWidget::treatTitleFontSizeChanging()
//{
//    ui->titleFontSizeSpinBox->clearFocus();
//    if (mLayoutModel != NULL) {
//        mLayoutModel->setTitleFontSize(ui->titleFontSizeSpinBox->value());
//    }
//}

void PlotControlWidget::treatEventFontSizeChanging()
{
    ui->eventFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setTimeEventFontSize(ui->eventFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatArrowedTextFontSizeChanging()
{
    ui->arrowedTextFontSizeSpinBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setArrowedTextFontSize(ui->arrowedTextFontSizeSpinBox->value());
    }
}

void PlotControlWidget::treatLegendFrameVisibilityChanging()
{
    ui->legendFrameVisibilityCheckBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setLegendFrameVisibility(ui->legendFrameVisibilityCheckBox->isChecked());
    }

}

void PlotControlWidget::treatLegendVisibilityChanging()
{
    ui->legendVisibilityCheckBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setLegendVisibility(ui->legendVisibilityCheckBox->isChecked());
    }

}

void PlotControlWidget::treatUnitsVisibilityChanging()
{
    ui->unitsVisibilityCheckBox->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setDimensionVisibility(ui->unitsVisibilityCheckBox->isChecked());
    }
}

void PlotControlWidget::treatXGridChanging()
{
    ui->xGridWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setXGrid(ui->xGridWidget->grid());
    }
}

void PlotControlWidget::treatYGridChanging()
{
    ui->yGridWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setYGrid(ui->yGridWidget->grid());
    }
}

void PlotControlWidget::treatXSubGridChanging()
{
    ui->xSubGridWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setXSubGrid(ui->xSubGridWidget->grid());
    }
}

void PlotControlWidget::treatYSubGridChanging()
{
    ui->ySubGridWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setYSubGrid(ui->ySubGridWidget->grid());
    }
}

void PlotControlWidget::treatAxisRectChanging()
{
    ui->axisRectEditingWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setAxisRect(ui->axisRectEditingWidget->axisRect());
    }
}

void PlotControlWidget::treatTickChanging()
{
    ui->tickEditingWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setTick(ui->tickEditingWidget->tick());
    }
}

void PlotControlWidget::treatSubTickChanging()
{
    ui->subTickEditingWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setSubTick(ui->subTickEditingWidget->tick());
    }
}

void PlotControlWidget::treatColorPrefChanging()
{
    ui->colorSetEdititngWidget->clearFocus();
    if (mLayoutModel != NULL) {
        mLayoutModel->setColorPref(ui->colorSetEdititngWidget->colorPref());
    }
}

void PlotControlWidget::treatXAxisPrefChanging()
{
    ui->xAxisPrefWidget->clearFocus();
    if (mLayoutModel != NULL) {
        auto pref = mLayoutModel->plotStyle();
        pref.mXAxisPref = ui->xAxisPrefWidget->axisPref();
        mLayoutModel->setPlotStyle(pref);
    }
}

void PlotControlWidget::treatYAxisPrefChanging()
{
    ui->yAxisPrefWidget->clearFocus();
    if (mLayoutModel != NULL) {
        auto pref = mLayoutModel->plotStyle();
        pref.mYAxisPref = ui->yAxisPrefWidget->axisPref();
        mLayoutModel->setPlotStyle(pref);
    }
}

void PlotControlWidget::updateRanges()
{
    if (mLayoutModel) {
        auto xRange = mLayoutModel->getXRange();
        ui->xLowMarginLineEdit->setText(QString::number(std::get<0>(xRange), 'g', global::gDoubleToStringConversionPrecision));
        ui->xHighMarginLineEdit->setText(QString::number(std::get<1>(xRange), 'g', global::gDoubleToStringConversionPrecision));
    }
}

void PlotControlWidget::treatFastenToolButtonPressed()
{
    FastenToolButton *button = qobject_cast<FastenToolButton*>(sender());
    if (button == nullptr)
        return;
    for(int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        if (button == mFastenToolButtonVector[i]) {
            sTabVisibilityVector[i] = mFastenToolButtonVector[i]->isChecked();
        }
    }
}

void PlotControlWidget::initiliazeLayoutPreferencesFromDefault()
{
    if (mLayoutModel) {
        mLayoutModel->setPlotStyle(PlotStyle::defaultPlotStyle());
        initializeState();
    }
}

