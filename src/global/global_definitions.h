#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <QVector>
#include <QList>
#include <QColor>
#include <QString>
#include "core/graphmodel.h"
#include "datasource/simpledatasource.h"
#include "event/timeevent.h"
#include <QMap>
#include <core/plotlayoutmodel.h>
#include <tuple>
#include "core/plotpreferences.h"
#include "utility/enum.h"
#include "plugins/pluginlibrary.h"

class JagMainWindow;
class EventSource;
class QSettings;

namespace global {

void loadGlobalSettings(QSettings& settings);
void saveGlobalSettingsOnUserRequest(QSettings& settings);
void saveGlobalSettingsOnApplicClose(QSettings& settings);

extern QVector<QString> gFavouriteParameters;
extern QVector<QString> gHateParameters;

extern int gPreviousSessionWidth;
extern int gPreviousSessionHeight;

extern int gPreviousSecProcDialogWidth;
extern int gPreviousSecProcDialogHeight;

extern int gPreviousSessionXCoord;
extern int gPreviousSessionYCoord;
extern bool gIsJaGMainWindowInitialMoveEnabled;

extern int gDataDialogWidth;
extern int gDataDialogHeight;
extern QVector<QString> gParametersSearchClipboard;
extern QVector<QString> gParametersSearchArgumentsClipboard;
extern bool gAutoParametersFilteringDuringSearchIsEnabled;
extern QString gAutoParametersFilteringDuringSearchPattern;
extern bool gSearchStringClearingAfterCheckedParamsListChangingEnabled;
extern bool gDataChoiceDialogConfigsEnabled;

extern QString gDefaultDirectory;
extern QString gApplicationStartTime;

bool areNamesRelated(const QString &name1, const QString &name2);


extern QVector<QString> gPrevSecondProcesFunctionVector;
const int gPrevSecondProcesFunctionVectorMaxSize = 8;
extern QVector<const GraphModel*> gCurrGraphVector;

extern bool gWheelZoomEnabled;
extern QString gDimensionlessParameterUnitLabel;

extern QStringList gParametersForDimensionList;
extern QStringList gUnitsForUnitList;

extern QString gParameterNameSeparator;
QString getPrameterNameFromComplexName(const QString &complexName);
std::pair<QString,QString> getPrameterAndArgNameFromComplexName(const QString &complexName);
QString getFileNameFromPath(const QString &complexName);

extern bool gIsAutoNanTo0ConversionEnabled;

extern quint16 gExternalDataSourcePortNumber;

extern bool gIsBootTipShowingEnabled;

extern JagMainWindow *gJagMainWindowPointer;
extern int gDoubleToStringConversionPrecision;

extern QList<QColor> gColorSceme;

extern QStringList gPluginsPathList;
extern QVector<PluginLibrary> gPlugins;

extern bool gAreExtraWidgetsVisibleInDataChoiseDialog;

extern bool gIsShadowInZoomModesEnabled;
extern QColor gZoomShadowColor;

extern int gTracerSize;
extern double gEventLineWidth;

extern SimpleDataSource *gGlobalDataSource;
extern EventSource *gGlobalEventSource;

extern bool gIsExtraSecondaryProcessingFunctionsEnabled;
extern bool gFindStringErasingTimerEnabled;
extern double gFindStringErasingTimerInterval;
extern QMap<QString, std::pair<QStringList, QStringList>> gPreviouslySavedParametersNames;  //QMap<stringlist name, std::pair<stringlist of saved names, stringlist of saved args names>>

extern double gXTickSpacing;
extern double gYTickSpacing;

extern int gGraphCaptureDistance;

extern QString gApplicationGUIStyle;

extern bool gShowDataSourceLoadingProgress;
extern bool gSearchByDescriptionIsEnabled;

extern std::shared_ptr<GraphModel> gGlobalGraphModel;

extern bool gAutoDimFillOnlyForEmpty;

extern bool gIsDimensionCopyable;
extern bool gIsLineColorCopyable;
extern bool gIsLineWidthCopyable;
extern bool gIsLineStyleCopyable;
extern bool gIsInterpolationCopyable;
extern bool gIsScatterShapeCopyable;
extern bool gIsScatterSizeCopyable;
extern bool gIsScatterDecimationCopyable;
extern bool gIsBrushStyleCopyable;
extern bool gIsBrushColorCopyable;
extern bool gIsBrushBasisCopyable;
extern bool gIsTransformationCopyable;
extern bool gIsFiltersCopyable;
extern bool gIsStringRepresentationCopyable;

extern bool gShowDataSourceEditingDialog;

extern double gWidthDeviationForExternalPlotting;

extern bool gIsDataSourceCheckingStrict;
extern int gErrorsListCapacity;


extern double gLineWidthDeviationForExternalPlotting;
extern bool gRussianLanguage;

enum class ParamNamingPolicy {
                                AlwaysAddAlias,
                                AddAliasForMultSources,
                                NeverAddAlias,
                            };
extern ParamNamingPolicy gParameterNamingPolicy;


extern bool gIsFooterVisible;
extern QString gLeftFooterString;
extern QString gCentralFooterString;
extern QString gRightFooterString;
extern QFont gFooterFont;

QString convertFooterString(const QString &);






class ColorScheme {
public:
    enum class Scheme {
        System,
        DarkScheme,
        DarkOrangeScheme,
    };

    ColorScheme(): mScheme(Scheme::System) { }
    ColorScheme& operator=(Scheme);
    Scheme scheme() const { return mScheme; }

private:
    Scheme mScheme;


};

struct FastSecondaryProcessingRequest
{
    enum class NewGraphLocation { CurrentPlotFrame,
                                  NewRightPlotFrame,
                                  NewBottomPlotFrame
                                };
    QString mFunctionString;
    NewGraphLocation mNewGraphLocation;
};

extern QIcon getStyledIcon(const QString& iconPath);


enum class IconColorTheme {
    color  = 0x00,
    black  = 0x01,
    gray   = 0x02,
    blue   = 0x03,
    orange = 0x04
};
extern IconColorTheme gIconColorTheme;


extern QVector<FastSecondaryProcessingRequest> gFastSecondaryProcessingVector;

extern ColorScheme gDefaultColorScheme;
extern PlotPreferences gDefaultPlotPreferences;

void setDefaultPreferences();


extern bool gDrawValuesInMeasuringMode;
extern int gCharNumToMatchForRelatedGraphs;

extern QCPAxis::TickEvaluationPolicy gXTickEvaluationPolicy;
extern QCPAxis::TickEvaluationPolicy gYTickEvaluationPolicy;
extern bool gIsEdgeXTicksModificationEnabled;
extern bool gIsEdgeYTicksModificationEnabled;

enum class RangeEvaluationPolicy { ExactMatching,
                                   PreserveReadability
                                 };

extern RangeEvaluationPolicy gXRangeEvaluationPolicy;
extern RangeEvaluationPolicy gYRangeEvaluationPolicy;
extern RangeEvaluationPolicy gXZoomRangeEvaluationPolicy;
extern RangeEvaluationPolicy gYZoomRangeEvaluationPolicy;
std::tuple<double, double> recountRangeWithRespectToPolicy(const std::tuple<double, double> range, const RangeEvaluationPolicy policy);

}




#endif // GLOBAL_DEFINITIONS_H
