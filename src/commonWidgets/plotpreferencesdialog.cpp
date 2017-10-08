#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "global/global_definitions.h"
#include <QFileDialog>
#include <QStyleFactory>
#include <QMessageBox>
#include "commonWidgets/anycombobox.h"
#include "core/jagmainwindow.h"
#include "core/plotpreferences.h"
PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    ui->languageComboBox->addItem("English");
    ui->languageComboBox->addItem("Russian");

    ui->lineInterpolationComboBox->addItem("Line"       , GraphModel::LineStyle::lsLine);
    ui->lineInterpolationComboBox->addItem("Smooth"     , GraphModel::LineStyle::lsSmooth);
    ui->lineInterpolationComboBox->addItem("Step Left"  , GraphModel::LineStyle::lsStepLeft);
    ui->lineInterpolationComboBox->addItem("Step Right" , GraphModel::LineStyle::lsStepRight);
    ui->lineInterpolationComboBox->addItem("Step Center", GraphModel::LineStyle::lsStepCenter);
    ui->lineInterpolationComboBox->addItem("Impulse"    , GraphModel::LineStyle::lsImpulse);
    ui->lineInterpolationComboBox->addItem("None"       , GraphModel::LineStyle::lsNone);

    ui->parameterNamingPolicyComboBox->addItem("Always add alias", global::ParamNamingPolicy::AlwaysAddAlias);
    ui->parameterNamingPolicyComboBox->addItem("Add alias for multiple sources", global::ParamNamingPolicy::AddAliasForMultSources);
    ui->parameterNamingPolicyComboBox->addItem("Never add alias", global::ParamNamingPolicy::NeverAddAlias);

    ui->colorSchemeWidget->addItem("System",               global::ColorScheme::Scheme::System);
    ui->colorSchemeWidget->addItem("Dark scheme",          global::ColorScheme::Scheme::DarkScheme);
    ui->colorSchemeWidget->addItem("Dark orange scheme",   global::ColorScheme::Scheme::DarkOrangeScheme);

    ui->xTickEvaluationPolicyComboBox->addItem("Preserve spacing",     QCPAxis::preserveSpacing);
    ui->xTickEvaluationPolicyComboBox->addItem("Preserve readability", QCPAxis::preserveReadability);

    ui->yTickEvaluationPolicyComboBox->addItem("Preserve spacing",     QCPAxis::preserveSpacing);
    ui->yTickEvaluationPolicyComboBox->addItem("Preserve readability", QCPAxis::preserveReadability);

    ui->iconColorChoiceWidget->addItem("color",      global::IconColorTheme::color);
    ui->iconColorChoiceWidget->addItem("black",      global::IconColorTheme::black);
    ui->iconColorChoiceWidget->addItem("gray",       global::IconColorTheme::gray);
    ui->iconColorChoiceWidget->addItem("blue",       global::IconColorTheme::blue);
    ui->iconColorChoiceWidget->addItem("orange",     global::IconColorTheme::orange);

    ui->xRangeEvaluationPolicyComboBox->addItem("Exact Matching", global::RangeEvaluationPolicy::ExactMatching);
    ui->xRangeEvaluationPolicyComboBox->addItem("Preserve Readability", global::RangeEvaluationPolicy::PreserveReadability);
    ui->yRangeEvaluationPolicyComboBox->addItem("Exact Matching", global::RangeEvaluationPolicy::ExactMatching);
    ui->yRangeEvaluationPolicyComboBox->addItem("Preserve Readability", global::RangeEvaluationPolicy::PreserveReadability);
    ui->xZoomRangeEvaluationPolicyComboBox->addItem("Exact Matching", global::RangeEvaluationPolicy::ExactMatching);
    ui->xZoomRangeEvaluationPolicyComboBox->addItem("Preserve Readability", global::RangeEvaluationPolicy::PreserveReadability);
    ui->yZoomRangeEvaluationPolicyComboBox->addItem("Exact Matching", global::RangeEvaluationPolicy::ExactMatching);
    ui->yZoomRangeEvaluationPolicyComboBox->addItem("Preserve Readability", global::RangeEvaluationPolicy::PreserveReadability);

//    ui->legendLocationWidget->addItem("Left",              PlotLayoutModel::LegendLocation::Left);
//    ui->legendLocationWidget->addItem("Right",             PlotLayoutModel::LegendLocation::Right);
//    ui->legendLocationWidget->addItem("TopSeparate",       PlotLayoutModel::LegendLocation::TopSeparate);
//    ui->legendLocationWidget->addItem("TopCombined",       PlotLayoutModel::LegendLocation::TopCombined);
//    ui->legendLocationWidget->addItem("BottomSeparate",    PlotLayoutModel::LegendLocation::BottomSeparate);
//    ui->legendLocationWidget->addItem("BottomCombined",    PlotLayoutModel::LegendLocation::BottomCombined);
//    ui->legendLocationWidget->addItem("Floating",          PlotLayoutModel::LegendLocation::Floating);


//    ui->graphsColorListChoiceWidget->setHeader("Graph line color", "Graph brush color");



    QStyleFactory styleFactory;
    ui->guiStyleComboBox->addItems(styleFactory.keys());

    initializeStateFromPreferences();

    connect(ui->applyButton, SIGNAL(clicked()),
            this, SLOT(okRequest()));
    connect(ui->cancelButton, SIGNAL(clicked()),
            this, SLOT(cancelRequest()));
    connect(ui->applyAndSaveButton, SIGNAL(clicked()),
            this, SLOT(okAndSaveRequest()));
    connect(ui->defaultDirectoryPushButton, SIGNAL(clicked()),
            this, SLOT(setChooseDefaultDirectory()));
    connect(ui->defaultPushButton, SIGNAL(clicked()),
            this, SLOT(setDefaultPreferences()));

    connect(ui->dataSourceStrictCheckingCheckBox, SIGNAL(clicked()),
            this, SLOT(errorNumberEnabilityChanged()));



}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::okRequest()
{
    setResult(QDialog::Accepted);
    applyChangesToPreferences();
    hide();
}

void PreferencesDialog::cancelRequest()
{
    initializeStateFromPreferences();
    hide();
}

void PreferencesDialog::okAndSaveRequest()
{
    okRequest();
    emit preferenceSavingRequest();
}

void PreferencesDialog::setChooseDefaultDirectory()
{
    //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//#ifdef __linux
    QFileDialog::Options option =  QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly;
//#else
//    QFileDialog::Option option = 0x0;
//#endif
    QString newDir = QFileDialog::getExistingDirectory(this,
                                                       "Choose default directory for JaG",
                                                       ui->defaultDirectoryLineEdit->text(),
                                                       option
                                                       );
    if (newDir != "")
        ui->defaultDirectoryLineEdit->setText(newDir);
}

void PreferencesDialog::setDefaultPreferences()
{
//    *mPlotStyle = PlotStyle();
    global::setDefaultPreferences();
    initializeStateFromPreferences();
}

void PreferencesDialog::errorNumberEnabilityChanged()
{
    ui->errorsNumberSpinBox->setEnabled(!ui->dataSourceStrictCheckingCheckBox->isChecked());
}

void PreferencesDialog::initializeStateFromPreferences()
{
    for (auto checkBox : mToolbarActionsCheckBoxesVector) {
        checkBox->setParent(nullptr);
        delete checkBox;
    }
    mToolbarActionsCheckBoxesVector.clear();

    using namespace global;
    ui->languageComboBox->setCurrentIndex(gRussianLanguage ? 1 : 0);

    for (int i = 0; i < ui->guiStyleComboBox->count(); ++i) {
        if (gApplicationGUIStyle == ui->guiStyleComboBox->itemText(i)) {
            ui->guiStyleComboBox->setCurrentIndex(i);
            break;
        }
    }

    ui->xTickSpacingDoubleSpinBox->setValue(gXTickSpacing);
    ui->yTickSpacingDoubleSpinBox->setValue(gYTickSpacing);

    ui->graphWidthIncreaseDoubleSpinBox->setValue(gLineWidthDeviationForExternalPlotting);

    ui->doubleToStringPrecisonSpinBox->setValue(gDoubleToStringConversionPrecision);
    ui->lineWidthDoubleSpinBox->setValue(global::gDefaultPlotPreferences.mLineWidth);

    ui->scatterSizeDoubleSpinBox->setValue(global::gDefaultPlotPreferences.mScatterSize);
    ui->scatterDecimationSpinBox->setValue(global::gDefaultPlotPreferences.mScatterDecimation);

    ui->erasingTimerDoubleSpinBox->setValue(gFindStringErasingTimerInterval);

    ui->graphCaptureDistanceSpinBox->setValue(gGraphCaptureDistance);


    ui->lineInterpolationComboBox->setCurrentValue(global::gDefaultPlotPreferences.mLineInterpolation);

    ui->parameterNamingPolicyComboBox->setCurrentValue(global::gParameterNamingPolicy);

    ui->iconColorChoiceWidget->setCurrentValue(global::gIconColorTheme);

    ui->xTickEvaluationPolicyComboBox->setCurrentValue(global::gXTickEvaluationPolicy);
    ui->yTickEvaluationPolicyComboBox->setCurrentValue(global::gYTickEvaluationPolicy);



    ui->xRangeEvaluationPolicyComboBox->setCurrentValue(global::gXRangeEvaluationPolicy);
    ui->yRangeEvaluationPolicyComboBox->setCurrentValue(global::gYRangeEvaluationPolicy);
    ui->xZoomRangeEvaluationPolicyComboBox->setCurrentValue(global::gXZoomRangeEvaluationPolicy);
    ui->yZoomRangeEvaluationPolicyComboBox->setCurrentValue(global::gYZoomRangeEvaluationPolicy);
    ui->isEdgeXTicksModificationEnabledCheckBox->setChecked(global::gIsEdgeXTicksModificationEnabled);
    ui->isEdgeYTicksModificationEnabledCheckBox->setChecked(global::gIsEdgeYTicksModificationEnabled);

    ui->fastSecProcRequestVectorEditorWidget->setFastSecProcRequests(global::gFastSecondaryProcessingVector);

    ui->leftFooterLineEdit->setText(global::gLeftFooterString);
    ui->centralFooterLineEdit->setText(global::gCentralFooterString);
    ui->rightFooterLineEdit->setText(global::gRightFooterString);
    ui->isFooterVisibleCheckBox->setChecked(global::gIsFooterVisible);
    ui->footerFontComboBox->setCurrentFont(global::gFooterFont);
    ui->footerFontSizeSpinBox->setValue(global::gFooterFont.pointSize());


    ui->xLabelLineEdit->setText(global::gDefaultPlotPreferences.mXLabelString);


    ui->restorePositionCheckBox->setChecked(gIsJaGMainWindowInitialMoveEnabled);

    ui->unitsVisibilityCheckBox->setCheckState(global::gDefaultPlotPreferences.mUnitsVisibility ?  Qt::Checked : Qt::Unchecked);



//    ui->plotStyleChoosingWidget->setPlotStyle(*mPlotStyle);

    ui->pluginsPathListEditingWidget->setPluginsPathList(global::gPluginsPathList);


    ui->wheelZoomEnableCheckBox->setCheckState(gWheelZoomEnabled ?  Qt::Checked : Qt::Unchecked);

    ui->isZommShadowEnabledCheckBox->setChecked(gIsShadowInZoomModesEnabled);
    ui->zoomShadowColorWidget->setColor(gZoomShadowColor);

    ui->favouritePlainTextEdit->clear();
    foreach  (QString favourite, gFavouriteParameters)
        ui->favouritePlainTextEdit->appendPlainText(favourite);
    ui->hatePlainTextEdit->clear();
    foreach  (QString hate, gHateParameters)
        ui->hatePlainTextEdit->appendPlainText(hate);

    ui->defaultDirectoryLineEdit->setText(gDefaultDirectory);

    ui->dimensionlessUnitLineEdit->setText(gDimensionlessParameterUnitLabel);

    ui->tracerSizeSpinBox->setValue(gTracerSize);
//    ui->eventLineWidthDoubleSpinBox->setValue(gEventLineWidth);

    ui->progressShowingCheckBox->setChecked(gShowDataSourceLoadingProgress);

    ui->numCharForRelatedGraphsSpinBox->setValue(gCharNumToMatchForRelatedGraphs);

    ui->dataSourceStrictCheckingCheckBox->setChecked(gIsDataSourceCheckingStrict);
    if (gIsDataSourceCheckingStrict) {
        ui->errorsNumberSpinBox->setEnabled(false);
    }

    ui->latexMarkupCheckBox->setChecked(QCP::gIsLatexMarkupEnabled);
//    ui->legendFrameCornerRadiusSpinBox->setValue(QCP::gLegendFrameCornerRadius);
//    ui->legendFrameShadowCheckBox->setChecked(QCP::gIsLegendFrameShadowEnabled);

    //copyable graph properties
    ui->isDimensionCopyableCheckBox->setChecked(gIsDimensionCopyable);
    ui->isLineColorCopyableCheckBox->setChecked(gIsLineColorCopyable);
    ui->isLineWidthCopyableCheckBox->setChecked(gIsLineWidthCopyable);
    ui->isLineStyleCopyableCheckBox->setChecked(gIsLineStyleCopyable);
    ui->isInterpolationCopyableCheckBox->setChecked(gIsInterpolationCopyable);
    ui->isStringRepresentationCheckBox->setChecked(gIsStringRepresentationCopyable);

    ui->isScatterShapeCopyableCheckBox->setChecked(gIsScatterShapeCopyable);
    ui->isScatterSizeCopyableCheckBox->setChecked(gIsScatterSizeCopyable);
    ui->isScatterDecimationCopyableCheckBox->setChecked(gIsScatterDecimationCopyable);

    ui->isBrushStyleCopyableCheckBox->setChecked(gIsBrushStyleCopyable);
    ui->isBrushColorCopyableCheckBox->setChecked(gIsBrushColorCopyable);
    ui->isBrushBasisCopyableCheckBox->setChecked(gIsBrushBasisCopyable);

    ui->isTransformationCopyableCheckBox->setChecked(gIsTransformationCopyable);
    ui->isFiltersCopyableCheckBox->setChecked(gIsFiltersCopyable);

    ui->showDataSourceEditingDialogCheckBox->setChecked(gShowDataSourceEditingDialog);

    ui->objectWidthIncreaseDoubleSpinBox->setValue(gWidthDeviationForExternalPlotting);


    ui->autoDimensionForEmptyCheckBox->setChecked(gAutoDimFillOnlyForEmpty);
    ui->errorsNumberSpinBox->setValue(gErrorsListCapacity);

    ui->colorSchemeWidget->setCurrentValue(global::gDefaultColorScheme.scheme());


    ui->drawValuesInMeasuringModeCheckBox->setChecked(global::gDrawValuesInMeasuringMode);




    //toolBar actions
    auto actions_map = global::gJagMainWindowPointer->actions();
    QMap<QString, bool> visibleActions = global::gJagMainWindowPointer->visibleActionsMap();
    ui->toolBarActionsWidget->setLayout(new QVBoxLayout());
    for (const auto & actName : actions_map.keys()) {
        QCheckBox *newCheckBox = new QCheckBox(actions_map[actName]->text());
        ui->toolBarActionsWidget->layout()->addWidget(newCheckBox);
        newCheckBox->setChecked(visibleActions[actName]);
        newCheckBox->setIcon(actions_map[actName]->icon());
        mToolbarActionsCheckBoxesVector.push_back(newCheckBox);
    }
}

void PreferencesDialog::applyChangesToPreferences()
{
    using namespace global;
    if ((gRussianLanguage && ui->languageComboBox->currentIndex() == 0)
            || (!gRussianLanguage && ui->languageComboBox->currentIndex() == 1)) {
        QMessageBox::information(this, "Information",
                                 "You have changed application language. To make changes affect you should close JaG."
                                 );
    }

    gRussianLanguage = (ui->languageComboBox->currentIndex() == 1 ? true : false);


    gDoubleToStringConversionPrecision = ui->doubleToStringPrecisonSpinBox->value();

    global::gParameterNamingPolicy = ui->parameterNamingPolicyComboBox->currentValue<global::ParamNamingPolicy>();

    QCP::gIsLatexMarkupEnabled = ui->latexMarkupCheckBox->isChecked();

    gApplicationGUIStyle = ui->guiStyleComboBox->currentText();
    QApplication::setStyle(gApplicationGUIStyle);

    global::gDefaultPlotPreferences.mLineWidth = ui->lineWidthDoubleSpinBox->value();

    global::gDefaultPlotPreferences.mLineInterpolation = ui->lineInterpolationComboBox->currentValue<GraphModel::LineStyle>();

    global::gDefaultPlotPreferences.mScatterSize = ui->scatterSizeDoubleSpinBox->value();
    global::gDefaultPlotPreferences.mScatterDecimation = ui->scatterDecimationSpinBox->value();

    global::gDefaultPlotPreferences.mXLabelString = ui->xLabelLineEdit->text();


    global::gIconColorTheme = ui->iconColorChoiceWidget->currentValue<global::IconColorTheme>();

    global::gDefaultPlotPreferences.mUnitsVisibility = ui->unitsVisibilityCheckBox->isChecked() ? true : false ;




//    *mPlotStyle = ui->plotStyleChoosingWidget->plotStyle();



    gIsJaGMainWindowInitialMoveEnabled = ui->restorePositionCheckBox->isChecked();

    global::gLeftFooterString = ui->leftFooterLineEdit->text();
    global::gCentralFooterString = ui->centralFooterLineEdit->text();
    global::gRightFooterString = ui->rightFooterLineEdit->text();
    global::gIsFooterVisible = ui->isFooterVisibleCheckBox->isChecked();
    global::gFooterFont = QFont(ui->footerFontComboBox->currentFont());
    global::gFooterFont.setPointSize(ui->footerFontSizeSpinBox->value());


    gXTickSpacing = ui->xTickSpacingDoubleSpinBox->value();
    gYTickSpacing = ui->yTickSpacingDoubleSpinBox->value();

    global::gFastSecondaryProcessingVector = ui->fastSecProcRequestVectorEditorWidget->fastSecProcRequests();

    gIsShadowInZoomModesEnabled =  ui->isZommShadowEnabledCheckBox->isChecked();
    gZoomShadowColor =  ui->zoomShadowColorWidget->color();

    gWheelZoomEnabled = ui->wheelZoomEnableCheckBox->isChecked() ? true : false ;

    gTracerSize = ui->tracerSizeSpinBox->value();

    gGraphCaptureDistance = ui->graphCaptureDistanceSpinBox->value();

    gShowDataSourceLoadingProgress = ui->progressShowingCheckBox->isChecked();

    gFindStringErasingTimerInterval = ui->erasingTimerDoubleSpinBox->value();

    gIsDataSourceCheckingStrict = ui->dataSourceStrictCheckingCheckBox->isChecked();
    gErrorsListCapacity = ui->errorsNumberSpinBox->value();

    gLineWidthDeviationForExternalPlotting = ui->graphWidthIncreaseDoubleSpinBox->value();

    gDrawValuesInMeasuringMode = ui->drawValuesInMeasuringModeCheckBox->isChecked();

    gCharNumToMatchForRelatedGraphs = ui->numCharForRelatedGraphsSpinBox->value();

    gXTickEvaluationPolicy = ui->xTickEvaluationPolicyComboBox->currentValue<QCPAxis::TickEvaluationPolicy>();
    gYTickEvaluationPolicy = ui->yTickEvaluationPolicyComboBox->currentValue<QCPAxis::TickEvaluationPolicy>();

    gXRangeEvaluationPolicy = ui->xRangeEvaluationPolicyComboBox->currentValue<global::RangeEvaluationPolicy>();
    gYRangeEvaluationPolicy = ui->yRangeEvaluationPolicyComboBox->currentValue<global::RangeEvaluationPolicy>();
    gXZoomRangeEvaluationPolicy = ui->xZoomRangeEvaluationPolicyComboBox->currentValue<global::RangeEvaluationPolicy>();
    gYZoomRangeEvaluationPolicy = ui->yZoomRangeEvaluationPolicyComboBox->currentValue<global::RangeEvaluationPolicy>();

    gIsEdgeXTicksModificationEnabled = ui->isEdgeXTicksModificationEnabledCheckBox->isChecked();
    gIsEdgeYTicksModificationEnabled = ui->isEdgeYTicksModificationEnabledCheckBox->isChecked();


    global::gPluginsPathList = ui->pluginsPathListEditingWidget->pluginsPathList();

    QString plainText = ui->favouritePlainTextEdit->toPlainText();
    gFavouriteParameters.clear();
    foreach (QString favourite, plainText.split('\n')) {
        gFavouriteParameters.push_back(favourite);
    }

    QString plainHateText = ui->hatePlainTextEdit->toPlainText();
    gHateParameters.clear();
    foreach (QString hate, plainHateText.split('\n')) {
        gHateParameters.push_back(hate);
    }

    //copyable graph properties
    gIsDimensionCopyable = ui->isDimensionCopyableCheckBox->isChecked();
    gIsLineColorCopyable = ui->isLineColorCopyableCheckBox->isChecked();
    gIsLineWidthCopyable = ui->isLineWidthCopyableCheckBox->isChecked();
    gIsLineStyleCopyable = ui->isLineStyleCopyableCheckBox->isChecked();
    gIsInterpolationCopyable = ui->isInterpolationCopyableCheckBox->isChecked();
    gIsStringRepresentationCopyable = ui->isStringRepresentationCheckBox->isChecked();

    gIsScatterShapeCopyable = ui->isScatterShapeCopyableCheckBox->isChecked();
    gIsScatterSizeCopyable = ui->isScatterSizeCopyableCheckBox->isChecked();
    gIsScatterDecimationCopyable = ui->isScatterDecimationCopyableCheckBox->isChecked();

    gIsBrushStyleCopyable = ui->isBrushStyleCopyableCheckBox->isChecked();
    gIsBrushColorCopyable = ui->isBrushColorCopyableCheckBox->isChecked();
    gIsBrushBasisCopyable = ui->isBrushBasisCopyableCheckBox->isChecked();

    gIsTransformationCopyable = ui->isTransformationCopyableCheckBox->isChecked();
    gIsFiltersCopyable = ui->isFiltersCopyableCheckBox->isChecked();

    gShowDataSourceEditingDialog = ui->showDataSourceEditingDialogCheckBox->isChecked();

    gDefaultDirectory = ui->defaultDirectoryLineEdit->text();

    gDimensionlessParameterUnitLabel = ui->dimensionlessUnitLineEdit->text();

    gWidthDeviationForExternalPlotting = ui->objectWidthIncreaseDoubleSpinBox->value();


    gAutoDimFillOnlyForEmpty = ui->autoDimensionForEmptyCheckBox->isChecked();


    global::gDefaultColorScheme = ui->colorSchemeWidget->currentValue<global::ColorScheme::Scheme>();


    ui->plotStyleChoosingWidget->applyChanges();

    auto actions_map = global::gJagMainWindowPointer->actions();
    QMap<QString, bool> visibleActions = global::gJagMainWindowPointer->visibleActionsMap();
    for (const auto & actName : actions_map.keys()) {
        for (auto checkBox : mToolbarActionsCheckBoxesVector) {
            if (checkBox->text() == actions_map[actName]->text()) {
                visibleActions[actName] = checkBox->isChecked();
                break;
            }
        }
    }
    global::gJagMainWindowPointer->setActionsVisibilityMap(visibleActions);

    ui->defaultDimensionWidget->applyChangesToGlobalLists();

}


