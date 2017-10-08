#include "global/global_definitions.h"
#include <QApplication>
#include "core/jagmainwindow.h"
#include <QProcess>
#include <QTextCodec>
#include <QSettings>
#include "utility/settings.h"


namespace global {

QVector<QString> gFavouriteParameters;
QVector<QString> gHateParameters;

int gPreviousSessionWidth;
int gPreviousSessionHeight;

int gPreviousSessionXCoord = 10;
int gPreviousSessionYCoord = 10;
bool gIsJaGMainWindowInitialMoveEnabled;

int gPreviousSecProcDialogWidth = 400;
int gPreviousSecProcDialogHeight = 600;


int gDataDialogWidth = -1;
int gDataDialogHeight = -1;

QVector<QString> gParametersSearchClipboard;
QVector<QString> gParametersSearchArgumentsClipboard;


QString gDefaultDirectory;


QString gApplicationStartTime;

QString gLeftFooterString;
QString gCentralFooterString;
QString gRightFooterString;
bool gIsFooterVisible;
QFont gFooterFont;

QStringList gPluginsPathList;
QVector<PluginLibrary> gPlugins;


PlotPreferences gDefaultPlotPreferences;

QString convertFooterString(const QString &footer)
{
    QString ret(footer);

    QRegExp regExp("\\$\\((.+)\\)");
    regExp.setMinimal(true); // make it non greedy
    int index = -1;
    while (index = regExp.indexIn(ret), index != -1 && regExp.captureCount() > 0) {// && regExp.captureCount() > 1) {
        QStringList matchedList = regExp.capturedTexts();
        if (matchedList.size() < 2)
            break;
        QString externApplName = matchedList[1];
        QProcess proc;
        proc.start(externApplName);
        proc.waitForFinished();
        QByteArray out = proc.readAllStandardOutput();
        QString procOut = QTextCodec::codecForLocale()->toUnicode(out);
        ret.replace(index, matchedList[0].size(), procOut);
    }
    ret.replace("\n", " ");

    return ret;
}

QVector<const GraphModel*> gCurrGraphVector;

QVector<QString> gPrevSecondProcesFunctionVector;

static bool doHaveEqualSubstring(const QString &str1, const QString &str2, int subStrLen, Qt::CaseSensitivity cs = Qt::CaseSensitive)
{
    if (subStrLen <= 0)
        return false;
    if (str1 == str2)
        return true;
    const QString &s1 = (str1.size() < str2.size()) ? str1 : str2;
    const QString &s2 = (str1.size() < str2.size()) ? str2 : str1;
    int limit = s1.size() - (subStrLen - 1);
    for (int i = 0; i < limit; ++i) {
        if (s2.contains(s1.mid(i, subStrLen), cs))
            return true;
    }
    return false;
}


bool areNamesRelated(const QString &name1, const QString &name2)
{

//    if (getPrameterNameFromComplexName(name1) == getPrameterNameFromComplexName(name2))
    if (doHaveEqualSubstring(getPrameterNameFromComplexName(name1), getPrameterNameFromComplexName(name2),
                             global::gCharNumToMatchForRelatedGraphs, Qt::CaseInsensitive))
        return true;
    else
        return false;
}

bool gWheelZoomEnabled;
QString gDimensionlessParameterUnitLabel;

QStringList gParametersForDimensionList;
QStringList gUnitsForUnitList;

QVector<FastSecondaryProcessingRequest> gFastSecondaryProcessingVector;

QString gParameterNameSeparator = "::";

bool gIsShadowInZoomModesEnabled;
QColor gZoomShadowColor;

QString getPrameterNameFromComplexName(const QString &complexName)
{
    int	index = complexName.lastIndexOf(global::gParameterNameSeparator);
    if (index == -1)
        return complexName;
    QString ret = complexName;
    ret.remove(0, index + global::gParameterNameSeparator.size());
    return ret;
}

std::pair<QString,QString> getPrameterAndArgNameFromComplexName(const QString &complexName)
{
    QString name = getPrameterNameFromComplexName(complexName);
    QRegExp regExpForArgum("(.*) \\((.+)\\)$");
    int ind = regExpForArgum.indexIn(name);
    auto captList = regExpForArgum.capturedTexts();
    if (ind != -1 && captList.size() == 3) {
        return std::make_pair(captList[1], captList[2]);
    } else {
        return std::make_pair(name, "");
    }
}


bool gIsAutoNanTo0ConversionEnabled = false;

quint16 gExternalDataSourcePortNumber = 33333;
bool gAutoParametersFilteringDuringSearchIsEnabled = false;
QString gAutoParametersFilteringDuringSearchPattern = ".*";


QString getFileNameFromPath(const QString &fileName)
{
    QString outFileName = fileName;
    int index;
    if ((index = outFileName.lastIndexOf("/")) != -1) {
        outFileName.remove(0, index + 1);
    }
    if ((index = outFileName.lastIndexOf("\\")) != -1) {
        outFileName.remove(0, index + 1);
    }
    return outFileName;
}

bool gIsBootTipShowingEnabled = true;
bool gIsExtraSecondaryProcessingFunctionsEnabled = true;

JagMainWindow *gJagMainWindowPointer = NULL;

bool gAutoDimFillOnlyForEmpty;

int gDoubleToStringConversionPrecision;

QList<QColor> gColorSceme = {
                            Qt::blue,
                            Qt::green,
                            Qt::red,
                            Qt::magenta,
                            Qt::black,
                            Qt::darkGreen,
                            Qt::darkYellow,
                            Qt::darkMagenta,
                            Qt::darkCyan,
                            Qt::darkRed,
                            Qt::darkBlue,
                            Qt::cyan,
                            Qt::yellow,
                            Qt::darkGray,
                            Qt::gray,
                            Qt::lightGray,
                            Qt::white,
//                            QColor(0,0,0,0)
                            };



bool gAreExtraWidgetsVisibleInDataChoiseDialog = true;
//double gEventLineWidth;

int gTracerSize;


SimpleDataSource *gGlobalDataSource = NULL;
EventSource *gGlobalEventSource = NULL;

bool gFindStringErasingTimerEnabled = true;
double gFindStringErasingTimerInterval;


//QMap<QString, QStringList> gPreviouslySavedParametersNames;  //QMap<stringList name, stringList of saved names>
QMap<QString, std::pair<QStringList, QStringList>> gPreviouslySavedParametersNames;  //QMap<stringlist name, std::pair<stringlist of saved names, stringlist of saved args names>>


double gXTickSpacing;
double gYTickSpacing;

int gGraphCaptureDistance;


bool gShowDataSourceLoadingProgress;

bool gSearchByDescriptionIsEnabled = true;
bool gSearchStringClearingAfterCheckedParamsListChangingEnabled = false;
bool gDataChoiceDialogConfigsEnabled = false;


std::shared_ptr<GraphModel> gGlobalGraphModel = NULL;

bool gIsDimensionCopyable;
bool gIsLineColorCopyable;
bool gIsLineWidthCopyable;
bool gIsLineStyleCopyable;
bool gIsInterpolationCopyable ;
bool gIsScatterShapeCopyable;
bool gIsScatterSizeCopyable ;
bool gIsScatterDecimationCopyable ;
bool gIsBrushStyleCopyable;
bool gIsBrushColorCopyable;
bool gIsBrushBasisCopyable;
bool gIsTransformationCopyable;
bool gIsFiltersCopyable;
bool gIsStringRepresentationCopyable;

bool gShowDataSourceEditingDialog;

double gWidthDeviationForExternalPlotting;


QString gApplicationGUIStyle;

bool gIsDataSourceCheckingStrict;


int gErrorsListCapacity;


int gCharNumToMatchForRelatedGraphs;

double gLineWidthDeviationForExternalPlotting;
bool gRussianLanguage;

ParamNamingPolicy gParameterNamingPolicy;

//QColor gDefaultPlotBackgroundColor;
//QColor gDefaultGridColor;
//QColor gDefaultSubGridColor;
//QColor gDefaultAxisRectColor;
//QColor gDefaultPlotFontColor;
//QColor gDefaultPlotGraphicalPrimitivesColor;

ColorScheme gDefaultColorScheme;

QCPAxis::TickEvaluationPolicy gXTickEvaluationPolicy;
QCPAxis::TickEvaluationPolicy gYTickEvaluationPolicy;
bool gIsEdgeXTicksModificationEnabled;
bool gIsEdgeYTicksModificationEnabled;

//QList<QColor> gGraphColorList;
//QList<QColor> gBrushColorList;


bool gDrawValuesInMeasuringMode;

RangeEvaluationPolicy gXRangeEvaluationPolicy;
RangeEvaluationPolicy gYRangeEvaluationPolicy;
RangeEvaluationPolicy gXZoomRangeEvaluationPolicy;
RangeEvaluationPolicy gYZoomRangeEvaluationPolicy;

ColorScheme& ColorScheme::operator=(Scheme scheme) {
    switch (scheme) {
        case Scheme::System:
            qApp->setStyleSheet("");
            break;
        case Scheme::DarkScheme:
            {
            QFile f(":qdarkstyle/style.qss");
            if (!f.exists())
            {
                printf("Unable to set stylesheet, file not found\n");
            }
            else
            {
                f.open(QFile::ReadOnly | QFile::Text);
                QTextStream ts(&f);
                qApp->setStyleSheet(ts.readAll());
            }
            }
            break;
        case Scheme::DarkOrangeScheme:
            {
            QFile f(":darkorange/darkorange.qss");
            if (!f.exists())
            {
                printf("Unable to set stylesheet, file not found\n");
            }
            else
            {
                f.open(QFile::ReadOnly | QFile::Text);
                QTextStream ts(&f);
                qApp->setStyleSheet(ts.readAll());
            }
            }
            break;
    }
    mScheme = scheme;
    return *this;
}

QIcon getStyledIcon(const QString &iconPath)
{
    QString prefix_color_part;
    switch (global::gIconColorTheme) {
        case IconColorTheme::color:  prefix_color_part = "color";
                                     break;
        case IconColorTheme::black:  prefix_color_part = "black";
                                     break;
        case IconColorTheme::gray:   prefix_color_part = "gray";
                                     break;
        case IconColorTheme::blue:   prefix_color_part = "blue";
                                     break;
        case IconColorTheme::orange: prefix_color_part = "orange";
                                     break;
    }

    QString prefix = ":/styles/" + prefix_color_part + "/" + prefix_color_part + "_";
    return QIcon(prefix + iconPath);
}
IconColorTheme gIconColorTheme;







}


void global::setDefaultPreferences()
{

    using namespace global;
    gIsJaGMainWindowInitialMoveEnabled = false;

#ifdef __linux
gDefaultDirectory = "/";
#else
gDefaultDirectory = "C:/";
#endif

    gWheelZoomEnabled = true;

    gDimensionlessParameterUnitLabel = "";

    gAutoDimFillOnlyForEmpty = true;

    gDoubleToStringConversionPrecision = 6;

//    gEventLineWidth = 3.0;

    gTracerSize = 10;

    gFindStringErasingTimerInterval = 4.;

    gXTickSpacing = 1.5;
    gYTickSpacing = 1.5;

    gGraphCaptureDistance = 6;

    gShowDataSourceLoadingProgress = true;

    gCharNumToMatchForRelatedGraphs = 4;

    gIsFooterVisible = false;

    gIsDimensionCopyable = false;
    gIsLineColorCopyable = false;
    gIsLineWidthCopyable = true;
    gIsLineStyleCopyable = true;
    gIsInterpolationCopyable  = true;
    gIsScatterShapeCopyable = true;
    gIsScatterSizeCopyable  = true;
    gIsScatterDecimationCopyable  = true;
    gIsBrushStyleCopyable = false;
    gIsBrushColorCopyable =  false;
    gIsBrushBasisCopyable =  false;
    gIsTransformationCopyable = true;
    gIsFiltersCopyable = true;
    gIsStringRepresentationCopyable = false;

    gShowDataSourceEditingDialog = true;

    gWidthDeviationForExternalPlotting = 0.4;
    gLineWidthDeviationForExternalPlotting = 0.6;

    gXTickEvaluationPolicy = QCPAxis::preserveReadability;
    gYTickEvaluationPolicy = QCPAxis::preserveSpacing;
    gIsEdgeXTicksModificationEnabled = true;
    gIsEdgeYTicksModificationEnabled = true;

    gXRangeEvaluationPolicy = RangeEvaluationPolicy::PreserveReadability;
    gYRangeEvaluationPolicy = RangeEvaluationPolicy::PreserveReadability;
    gXZoomRangeEvaluationPolicy = RangeEvaluationPolicy::PreserveReadability;
    gYZoomRangeEvaluationPolicy = RangeEvaluationPolicy::PreserveReadability;

    gLeftFooterString = "Left footer";
    gCentralFooterString = "Central footer";
    gRightFooterString = "Right footer";

    gIsShadowInZoomModesEnabled = true;
    gZoomShadowColor = QColor(150, 150, 150, 100);

#ifdef __linux
gApplicationGUIStyle = "GTK+";
#else
gApplicationGUIStyle = "Windows";
#endif

    gIsDataSourceCheckingStrict = false;

    gErrorsListCapacity = 50;
    gRussianLanguage = false;

    gDrawValuesInMeasuringMode = true;

    gParameterNamingPolicy = ParamNamingPolicy::NeverAddAlias;


    gDefaultColorScheme = ColorScheme::Scheme::DarkScheme;
    gIconColorTheme = IconColorTheme::gray;

    if (global::gJagMainWindowPointer) {
        QMap<QString, bool> visibilityActionsMap = gJagMainWindowPointer->visibleActionsMap();
        for (auto actionName : visibilityActionsMap.keys()) {
            visibilityActionsMap[actionName] = true;
        }
        global::gJagMainWindowPointer->setActionsVisibilityMap(visibilityActionsMap);
    }

    QCP::gIsLatexMarkupEnabled = true;

    gFastSecondaryProcessingVector.clear();
    gFastSecondaryProcessingVector << FastSecondaryProcessingRequest{"fftPS( g1 )", FastSecondaryProcessingRequest::NewGraphLocation::NewRightPlotFrame};
    gFastSecondaryProcessingVector << FastSecondaryProcessingRequest{"abs (g1)", FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame};
}


std::tuple<double, double>
global::recountRangeWithRespectToPolicy (const std::tuple<double, double> range, const RangeEvaluationPolicy policy)
{
    if (policy == global::RangeEvaluationPolicy::ExactMatching) {
        return range;
    } else {
        auto yMin = std::get<0>(range);
        auto yMax = std::get<1>(range);
        if (yMax == yMin)
            return range;
        double delta = yMax - yMin;
        double magnitudeFactor = std::pow(10.0, std::floor(std::log10(delta))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
        double yMagnitudeFactor = magnitudeFactor;
        double newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
        while (std::abs(newYMax - yMax)/delta > 0.1) {
            yMagnitudeFactor /= 10;
            newYMax = std::ceil(yMax/yMagnitudeFactor)*yMagnitudeFactor;
        }
        yMagnitudeFactor = magnitudeFactor;
        double newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
        while (std::abs(newYMin - yMin)/delta > 0.1) {
            yMagnitudeFactor /= 10;
            newYMin = std::floor(yMin/yMagnitudeFactor)*yMagnitudeFactor;
        }
        return std::tuple<double, double>(newYMin, newYMax);
    }
}








void global::loadGlobalSettings(QSettings& settings)
{
    QVariant varSetting;

    ::loadFromSettings(settings, "/Settings/gDoubleToStringConversionPrecision", global::gDoubleToStringConversionPrecision);

    ::loadFromSettings(settings, "/Settings/gAreExtraWidgetsVisibleInDataChoiseDialog", global::gAreExtraWidgetsVisibleInDataChoiseDialog);

    ::loadFromSettings(settings, "/Settings/gIsJaGMainWindowInitialMoveEnabled", global::gIsJaGMainWindowInitialMoveEnabled);
    ::loadFromSettings(settings, "/Settings/gPreviousSessionXCoord", global::gPreviousSessionXCoord);
    ::loadFromSettings(settings, "/Settings/gPreviousSessionYCoord", global::gPreviousSessionYCoord);

    ::loadFromSettings(settings, "/Settings/gPreviousSecProcDialogWidth", global::gPreviousSecProcDialogWidth);
    ::loadFromSettings(settings, "/Settings/gPreviousSecProcDialogHeight", global::gPreviousSecProcDialogHeight);

    ::loadFromSettings(settings, "/Settings/gPrevSecondProcesFunctionList", global::gPrevSecondProcesFunctionVector);

    ::loadFromSettings(settings, "/Settings/gCharNumToMatchForRelatedGraphs", global::gCharNumToMatchForRelatedGraphs);

    ::loadFromSettings(settings, "/Settings/gDataDialogWidth", global::gDataDialogWidth);
    ::loadFromSettings(settings, "/Settings/gDataDialogHeight", global::gDataDialogHeight);
    ::loadFromSettings(settings, "/Settings/gAutoParametersFilteringDuringSearchIsEnabled", global::gAutoParametersFilteringDuringSearchIsEnabled);

//    ::loadFromSettings(settings, "/Settings/gEventLineWidth", global::gEventLineWidth);

    if ((varSetting = settings.value("/Settings/gApplicationGUIStyle")).isValid()) {
        global::gApplicationGUIStyle = varSetting.toString();
        QApplication::setStyle(global::gApplicationGUIStyle);
    }

    ::loadFromSettings(settings, "/Settings/gFindStringErasingTimerEnabled", global::gFindStringErasingTimerEnabled);
    ::loadFromSettings(settings, "/Settings/gFindStringErasingTimerInterval", global::gFindStringErasingTimerInterval);

    ::loadFromSettings(settings, "/Settings/gSearchByDescriptionIsEnabled", global::gSearchByDescriptionIsEnabled);

    ::loadFromSettings(settings, "/Settings/gAutoDimFillOnlyForEmpty", global::gAutoDimFillOnlyForEmpty);
    ::loadFromSettings(settings, "/Settings/gDimensionlessParameterUnitLabel", global::gDimensionlessParameterUnitLabel);

    ::loadFromSettings(settings, "/Settings/gUnitsForUnitList", global::gUnitsForUnitList);
    ::loadFromSettings(settings, "/Settings/gParametersForDimensionList", global::gParametersForDimensionList);

    ::loadFromSettings(settings, "/Settings/gDefaultDirectory", global::gDefaultDirectory);

    ::loadFromSettings(settings, "/Settings/gIsBootTipShowingEnabled", global::gIsBootTipShowingEnabled);

    ::loadFromSettings(settings, "/Settings/gIsExtraSecondaryProcessingFunctionsEnabled", global::gIsExtraSecondaryProcessingFunctionsEnabled);

    ::loadFromSettings(settings, "/Settings/gParameterNamingPolicy", global::gParameterNamingPolicy);

    ::loadFromSettings(settings, "/Settings/gXTickEvaluationPolicy", global::gXTickEvaluationPolicy);
    ::loadFromSettings(settings, "/Settings/gYTickEvaluationPolicy", global::gYTickEvaluationPolicy);

    ::loadFromSettings(settings, "/Settings/gIsEdgeXTicksModificationEnabled", global::gIsEdgeXTicksModificationEnabled);
    ::loadFromSettings(settings, "/Settings/gIsEdgeYTicksModificationEnabled", global::gIsEdgeYTicksModificationEnabled);

    ::loadFromSettings(settings, "/Settings/gXRangeEvaluationPolicy", global::gXRangeEvaluationPolicy);
    ::loadFromSettings(settings, "/Settings/gYRangeEvaluationPolicy", global::gYRangeEvaluationPolicy);
    ::loadFromSettings(settings, "/Settings/gXZoomRangeEvaluationPolicy", global::gXZoomRangeEvaluationPolicy);
    ::loadFromSettings(settings, "/Settings/gYZoomRangeEvaluationPolicy", global::gYZoomRangeEvaluationPolicy);


    ::loadFromSettings(settings, "/Settings/gIsFooterVisible", global::gIsFooterVisible);
    ::loadFromSettings(settings, "/Settings/gLeftFooterString", global::gLeftFooterString);
    ::loadFromSettings(settings, "/Settings/gCentralFooterString", global::gCentralFooterString);
    ::loadFromSettings(settings, "/Settings/gRightFooterString", global::gRightFooterString);
    ::loadFromSettings(settings, "/Settings/gFooterFont", global::gFooterFont);

    if ((varSetting = settings.value("/Settings/gDefaultColorScheme")).isValid()) {
        global::gDefaultColorScheme = static_cast<global::ColorScheme::Scheme>(varSetting.toInt());
    }

    ::loadFromSettings(settings, "/Settings/gIconColorTheme", global::gIconColorTheme);

    ::loadFromSettings(settings, "/Settings/gWheelZoomEnabled", global::gWheelZoomEnabled);

    ::loadFromSettings(settings, "/Settings/gIsDataSourceCheckingStrict", global::gIsDataSourceCheckingStrict);

    ::loadFromSettings(settings, "/Settings/gIsAutoNanTo0ConversionEnabled", global::gIsAutoNanTo0ConversionEnabled);

    ::loadFromSettings(settings, "/Settings/gXTickSpacing", global::gXTickSpacing);
    ::loadFromSettings(settings, "/Settings/gYTickSpacing", global::gYTickSpacing);

    ::loadFromSettings(settings, "/Settings/gTracerSize", global::gTracerSize);

    ::loadFromSettings(settings, "/Settings/gGraphCaptureDistance", global::gGraphCaptureDistance);

    ::loadFromSettings(settings, "/Settings/gShowDataSourceLoadingProgress", global::gShowDataSourceLoadingProgress);

    ::loadFromSettings(settings, "/Settings/gDrawValuesInMeasuringMode", global::gDrawValuesInMeasuringMode);

    ::loadFromSettings(settings, "/Settings/gErrorsListCapacity", global::gErrorsListCapacity);

    ::loadFromSettings(settings, "/Settings/gLineWidthDeviationForExternalPlotting", global::gLineWidthDeviationForExternalPlotting);


    ::loadFromSettings(settings, "/Settings/gWidthDeviationForExternalPlotting", global::gWidthDeviationForExternalPlotting);

    ::loadFromSettings(settings, "/Settings/gIsDimensionCopyable", global::gIsDimensionCopyable);
    ::loadFromSettings(settings, "/Settings/gIsLineColorCopyable", global::gIsLineColorCopyable);
    ::loadFromSettings(settings, "/Settings/gIsLineWidthCopyable", global::gIsLineWidthCopyable);
    ::loadFromSettings(settings, "/Settings/gIsLineStyleCopyable", global::gIsLineStyleCopyable);
    ::loadFromSettings(settings, "/Settings/gIsInterpolationCopyable", global::gIsInterpolationCopyable);
    ::loadFromSettings(settings, "/Settings/gIsScatterShapeCopyable", global::gIsScatterShapeCopyable);
    ::loadFromSettings(settings, "/Settings/gIsScatterSizeCopyable", global::gIsScatterSizeCopyable);
    ::loadFromSettings(settings, "/Settings/gIsScatterDecimationCopyable", global::gIsScatterDecimationCopyable);
    ::loadFromSettings(settings, "/Settings/gIsBrushStyleCopyable", global::gIsBrushStyleCopyable);
    ::loadFromSettings(settings, "/Settings/gIsBrushColorCopyable", global::gIsBrushColorCopyable);
    ::loadFromSettings(settings, "/Settings/gIsBrushBasisCopyable", global::gIsBrushBasisCopyable);
    ::loadFromSettings(settings, "/Settings/gIsTransformationCopyable", global::gIsTransformationCopyable);
    ::loadFromSettings(settings, "/Settings/gIsFiltersCopyable", global::gIsFiltersCopyable);
    ::loadFromSettings(settings, "/Settings/gIsStringRepresentationCopyable", global::gIsStringRepresentationCopyable);

    ::loadFromSettings(settings, "/Settings/gShowDataSourceEditingDialog", global::gShowDataSourceEditingDialog);

//    ::loadFromSettings(settings, "/Settings/gDefaultPlotBackgroundColor", global::gDefaultPlotBackgroundColor);
//    ::loadFromSettings(settings, "/Settings/gDefaultPlotFontColor", global::gDefaultPlotFontColor);
//    ::loadFromSettings(settings, "/Settings/gDefaultPlotGraphicalPrimitivesColor", global::gDefaultPlotGraphicalPrimitivesColor);

    //data choice dialog properties
    ::loadFromSettings(settings, "/Settings/gSearchStringClearingAfterCheckedParamsListChangingEnabled", global::gSearchStringClearingAfterCheckedParamsListChangingEnabled);
    ::loadFromSettings(settings, "/Settings/gDataChoiceDialogConfigsEnabled", global::gDataChoiceDialogConfigsEnabled);

    //zoom properties
    ::loadFromSettings(settings, "/Settings/gZoomShadowColor", global::gZoomShadowColor);
    ::loadFromSettings(settings, "/Settings/gIsShadowInZoomModesEnabled", global::gIsShadowInZoomModesEnabled);

    ::loadFromSettings(settings, "/Settings/gFavouriteParameters", global::gFavouriteParameters);
    ::loadFromSettings(settings, "/Settings/gHateParameters", global::gHateParameters);

    ::loadFromSettings(settings, "/Settings/gPluginsPathList", global::gPluginsPathList);
    for (const auto &pluginPath : gPluginsPathList)
        gPlugins << PluginLibrary(pluginPath);

    PlotPreferences::loadFromSettings(settings, "/Settings/gDefaultPlotPreferences", global::gDefaultPlotPreferences);

    if ((varSetting = settings.value("/Settings/gPreviouslySavedParametersNames")).isValid()) {
        global::gPreviouslySavedParametersNames.clear();
        QMap<QString, QVariant>	bufMap = varSetting.toMap();
        foreach (QString key, bufMap.keys()) {
            QVariant bufStringList = bufMap[key];
            global::gPreviouslySavedParametersNames.insert(key, std::pair<QStringList, QStringList>(bufStringList.toStringList(), QStringList()));
        }
    }
    if ((varSetting = settings.value("/Settings/gPreviouslySavedParametersArgsNames")).isValid()) {
        QMap<QString, QVariant>	bufMap = varSetting.toMap();
        foreach (QString key, bufMap.keys()) {
            QVariant bufStringList = bufMap[key];
            if (global::gPreviouslySavedParametersNames.contains(key)) {
                global::gPreviouslySavedParametersNames[key].second = bufStringList.toStringList();
            }
        }
    }
}



void global::saveGlobalSettingsOnUserRequest(QSettings &settings)
{
    using namespace global;

    settings.setValue("/Settings/gRussianLanguage", gRussianLanguage);

    settings.setValue("/Settings/gDoubleToStringConversionPrecision", gDoubleToStringConversionPrecision);

    settings.setValue("/Settings/gIsFooterVisible", gIsFooterVisible);
    settings.setValue("/Settings/gLeftFooterString", gLeftFooterString);
    settings.setValue("/Settings/gCentralFooterString", gCentralFooterString);
    settings.setValue("/Settings/gRightFooterString", gRightFooterString);
    settings.setValue("/Settings/gFooterFont", gFooterFont);
    settings.setValue("/Settings/gFooterFontSize", gFooterFont.pointSize());

    settings.setValue("/Settings/gDefaultColorScheme", static_cast<int>(gDefaultColorScheme.scheme()));


    settings.setValue("/Settings/gXTickEvaluationPolicy", static_cast<int>(gXTickEvaluationPolicy));
    settings.setValue("/Settings/gYTickEvaluationPolicy", static_cast<int>(gYTickEvaluationPolicy));

    settings.setValue("/Settings/gIsEdgeXTicksModificationEnabled", gIsEdgeXTicksModificationEnabled);
    settings.setValue("/Settings/mIsEdgeYTicksModificationEnabled", gIsEdgeYTicksModificationEnabled);

    settings.setValue("/Settings/gXRangeEvaluationPolicy", static_cast<int>(gXRangeEvaluationPolicy));
    settings.setValue("/Settings/gYRangeEvaluationPolicy", static_cast<int>(gYRangeEvaluationPolicy));
    settings.setValue("/Settings/gXZoomRangeEvaluationPolicy", static_cast<int>(gXZoomRangeEvaluationPolicy));
    settings.setValue("/Settings/gYZoomRangeEvaluationPolicy", static_cast<int>(gYZoomRangeEvaluationPolicy));

    settings.setValue("/Settings/gIconColorTheme", static_cast<int>(gIconColorTheme));

    settings.setValue("/Settings/gWheelZoomEnabled", gWheelZoomEnabled);

    settings.setValue("/Settings/gApplicationGUIStyle", gApplicationGUIStyle);

    settings.setValue("/Settings/gFindStringErasingTimerInterval", gFindStringErasingTimerInterval);

    settings.setValue("/Settings/gIsJaGMainWindowInitialMoveEnabled", gIsJaGMainWindowInitialMoveEnabled);

    settings.setValue("/Settings/gGraphCaptureDistance", gGraphCaptureDistance);

    settings.setValue("/Settings/gAutoDimFillOnlyForEmpty", gAutoDimFillOnlyForEmpty);

    settings.setValue("/Settings/gDrawValuesInMeasuringMode", gDrawValuesInMeasuringMode);

    settings.setValue("/Settings/gCharNumToMatchForRelatedGraphs", gCharNumToMatchForRelatedGraphs);

    settings.setValue("/Settings/gIsDimensionCopyable", gIsDimensionCopyable);
    settings.setValue("/Settings/gIsLineColorCopyable", gIsLineColorCopyable);
    settings.setValue("/Settings/gIsLineWidthCopyable", gIsLineWidthCopyable);
    settings.setValue("/Settings/gIsLineStyleCopyable", gIsLineStyleCopyable);
    settings.setValue("/Settings/gIsInterpolationCopyable", gIsInterpolationCopyable);
    settings.setValue("/Settings/gIsScatterShapeCopyable", gIsScatterShapeCopyable);
    settings.setValue("/Settings/gIsScatterSizeCopyable", gIsScatterSizeCopyable);
    settings.setValue("/Settings/gIsScatterDecimationCopyable", gIsScatterDecimationCopyable);
    settings.setValue("/Settings/gIsBrushStyleCopyable", gIsBrushStyleCopyable);
    settings.setValue("/Settings/gIsBrushColorCopyable", gIsBrushColorCopyable);
    settings.setValue("/Settings/gIsBrushBasisCopyable", gIsBrushBasisCopyable);
    settings.setValue("/Settings/gIsTransformationCopyable", gIsTransformationCopyable);
    settings.setValue("/Settings/gIsFiltersCopyable", gIsFiltersCopyable);
    settings.setValue("/Settings/gIsStringRepresentationCopyable", gIsStringRepresentationCopyable);

    settings.setValue("/Settings/gShowDataSourceEditingDialog", gShowDataSourceEditingDialog);

    settings.setValue("/Settings/gLineWidthDeviationForExternalPlotting", gLineWidthDeviationForExternalPlotting);

    settings.setValue("/Settings/gXTickSpacing", gXTickSpacing);
    settings.setValue("/Settings/gYTickSpacing", gYTickSpacing);

    settings.setValue("/Settings/gErrorsListCapacity", gErrorsListCapacity);

    settings.setValue("/Settings/gShowDataSourceLoadingProgress", gShowDataSourceLoadingProgress);

    settings.setValue("/Settings/gIsDataSourceCheckingStrict", gIsDataSourceCheckingStrict);

    settings.setValue("/Settings/gWidthDeviationForExternalPlotting", gWidthDeviationForExternalPlotting);


    QList<QVariant> pluginsPathList;
    foreach (auto elem, gPluginsPathList)
        pluginsPathList.push_back(elem);
    settings.setValue("/Settings/gPluginsPathList", pluginsPathList);

    //zoom properties
    settings.setValue("/Settings/gZoomShadowColor", gZoomShadowColor);
    settings.setValue("/Settings/gIsShadowInZoomModesEnabled", gIsShadowInZoomModesEnabled);

    //fastSecondaryProcessing
    ::saveInSettings(settings, "/Settings/gFastSecondaryProcessingVector", global::gFastSecondaryProcessingVector);

    settings.setValue("/Settings/gDimensionlessParameterUnitLabel", gDimensionlessParameterUnitLabel);
    QList<QVariant> parameterList;
    foreach (auto elem, gParametersForDimensionList)
        parameterList.push_back(elem);
    settings.setValue("/Settings/gParametersForDimensionList", parameterList);
    QList<QVariant> unitsList;
    foreach (auto elem, gUnitsForUnitList)
        unitsList.push_back(elem);
    settings.setValue("/Settings/gUnitsForUnitList", unitsList);

    ::saveInSettings(settings, "/Settings/gFavouriteParameters", global::gFavouriteParameters);
    ::saveInSettings(settings, "/Settings/gHateParameters", global::gHateParameters);

    PlotPreferences::saveInSettings(settings, "/Settings/gDefaultPlotPreferences", global::gDefaultPlotPreferences);

    settings.setValue("/Settings/gDefaultDirectory", gDefaultDirectory);

    settings.setValue("/Settings/gParameterNamingPolicy", static_cast<int>(global::gParameterNamingPolicy));

}








void global::saveGlobalSettingsOnApplicClose(QSettings& settings)
{
    using namespace global;

    settings.setValue("/Settings/gPreviousSessionWidth", global::gPreviousSessionWidth);
    settings.setValue("/Settings/gPreviousSessionHeight", global::gPreviousSessionHeight);

    settings.setValue("/Settings/gPreviousSecProcDialogWidth", gPreviousSecProcDialogWidth);
    settings.setValue("/Settings/gPreviousSecProcDialogHeight", gPreviousSecProcDialogHeight);

    settings.setValue("/Settings/gPreviousSessionXCoord", gPreviousSessionXCoord);
    settings.setValue("/Settings/gPreviousSessionYCoord", gPreviousSessionYCoord);



    settings.setValue("/Settings/gIsBootTipShowingEnabled", gIsBootTipShowingEnabled);

    //data choice dialog properties
    settings.setValue("/Settings/gDataDialogWidth", gDataDialogWidth);
    settings.setValue("/Settings/gDataDialogHeight", gDataDialogHeight);
    settings.setValue("/Settings/gAutoParametersFilteringDuringSearchIsEnabled", gAutoParametersFilteringDuringSearchIsEnabled);
    settings.setValue("/Settings/gAreExtraWidgetsVisibleInDataChoiseDialog", gAreExtraWidgetsVisibleInDataChoiseDialog);
    settings.setValue("/Settings/gSearchStringClearingAfterCheckedParamsListChangingEnabled", gSearchStringClearingAfterCheckedParamsListChangingEnabled);
    settings.setValue("/Settings/gDataChoiceDialogConfigsEnabled", gDataChoiceDialogConfigsEnabled);
    settings.setValue("/Settings/gSearchByDescriptionIsEnabled", gSearchByDescriptionIsEnabled);

    settings.setValue("/Settings/gIsAutoNanTo0ConversionEnabled", gIsAutoNanTo0ConversionEnabled);

    settings.setValue("/Settings/gTracerSize", gTracerSize);

//    settings.setValue("/Settings/gEventLineWidth", gEventLineWidth);
    settings.setValue("/Settings/gIsExtraSecondaryProcessingFunctionsEnabled", gIsExtraSecondaryProcessingFunctionsEnabled);

    settings.setValue("/Settings/gFindStringErasingTimerEnabled", gFindStringErasingTimerEnabled);

    QList<QVariant> prevFunctionsList;
    foreach (auto elem, gPrevSecondProcesFunctionVector)
        prevFunctionsList.push_back(elem);
    settings.setValue("/Settings/gPrevSecondProcesFunctionList", prevFunctionsList);

    //    QMap<QString, QVariant>	bufMap;
    //    foreach(QString key, gPreviouslySavedParametersNames.keys()) {
    //        bufMap.insert(key, QVariant(gPreviouslySavedParametersNames[key]));
    //    }
    //    settings.setValue("/Settings/gPreviouslySavedParametersNames", bufMap);
        QMap<QString, QVariant>	bufMap;
        foreach(QString key, gPreviouslySavedParametersNames.keys()) {
            bufMap.insert(key, QVariant(gPreviouslySavedParametersNames[key].first));
        }
        settings.setValue("/Settings/gPreviouslySavedParametersNames", bufMap);
        bufMap.clear();
        foreach(QString key, gPreviouslySavedParametersNames.keys()) {
            bufMap.insert(key, QVariant(gPreviouslySavedParametersNames[key].second));
        }
        settings.setValue("/Settings/gPreviouslySavedParametersArgsNames", bufMap);
}
