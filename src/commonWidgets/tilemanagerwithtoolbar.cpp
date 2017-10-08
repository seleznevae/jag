#include "tilemanagerwithtoolbar.h"
#include "plotwidgetwithtoolbar.h"
#include "widgettilemanager/widgettilemanager.h"
#include <QToolBar>
#include <QVBoxLayout>
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"
#include <QAction>

TileManagerWithToolbar::TileManagerWithToolbar(QWidget *parent)
    : QMainWindow(parent), QFullScreenAdapter(this), mPlotToolBar(nullptr),
      mZoomToolBar(nullptr), mProcessingToolBar(nullptr), mOptionsToolBar(nullptr),
      mTileManager(nullptr)
{


//    setWindowFlags(Qt::Widget);

//    auto actions_map = global::gJagMainWindowPointer->actions();

//    using namespace global::toolbar;

//    int k = 0;
//    QToolBar *plotToolBar = new QToolBar;
//    if (gAddGraphAvailable)               { plotToolBar->addAction(actions_map["actionAdd_graph"]); k++; }
//    if (gAddEventAvailable)               { plotToolBar->addAction(actions_map["actionAdd_event"]); k++; }
//    if (gAddHorizontalPlotFrameAvailable) { plotToolBar->addAction(actions_map["actionAdd_plot_frame_horizontally"]); k++; }
//    if (gAddVerticalPlotFrameAvailable)   { plotToolBar->addAction(actions_map["actionAdd_vertical_plot_frame"]); k++; }
//    if (gCombineGraphsAvailable)          { plotToolBar->addAction(actions_map["actionCombine_graphs"]); k++; }
//    if (gSeparateGraphsAvailable)         { plotToolBar->addAction(actions_map["actionSeparate_graphs"]); k++; }
//    if (gIntellectualLayoutAvailable)     { plotToolBar->addAction(actions_map["actionIntellectual_layout"]); k++; }
//    if (k)
//        addToolBar(plotToolBar);

//    k = 0;
//    QToolBar *zoomToolBar = new QToolBar;
//    if (gNormalModeAvailable)              { zoomToolBar->addAction(actions_map["actionNormal_mode"]); k++; }
//    if (gMeasuringAvailable)               { zoomToolBar->addAction(actions_map["actionMeasuring"]); k++; }
//    if (gShowValueTableAvailable)          { zoomToolBar->addAction(actions_map["actionShow_value_table"]); k++; }
//    if (gDrawLevelAvailable)               { zoomToolBar->addAction(actions_map["actionDraw_level"]); k++; }
//    if (gArrowWithTextAvailable)           { zoomToolBar->addAction(actions_map["actionArrow_with_text"]); k++; }
//    if (gFastSecondaryProcessingAvailable) { zoomToolBar->addAction(actions_map["actionFast_secondary_processing"]); k++; }
//    if (gZoomXAvailable)                   { zoomToolBar->addAction(actions_map["actionZoom_X"]); k++; }
//    if (gZoomYAvailable)                   { zoomToolBar->addAction(actions_map["actionZoom_Y"]); k++; }
//    if (gZoomXYAvailable)                  { zoomToolBar->addAction(actions_map["actionZoom_XY"]); k++; }
//    if (gMotionAvailable)                  { zoomToolBar->addAction(actions_map["actionMotion"]); k++; }
//    if (gPreviousScopeAvailable)           { zoomToolBar->addAction(actions_map["actionPrevious_scope"]); k++; }
//    if (gNextScopeAvailable)               { zoomToolBar->addAction(actions_map["actionNext_scope"]); k++; }
//    if (gAutoscaleAvailable)               { zoomToolBar->addAction(actions_map["actionAutoscale"]); k++; }
//    if (k)
//        addToolBar(zoomToolBar);

//    k = 0;
//    QToolBar *processingToolBar = new QToolBar;
//    if (gSecondaryProcessingAvailable)   { processingToolBar->addAction(actions_map["actionSecondary_processing"]); k++; }
//    if (gSetUnitsAutomaticallyAvailable) { processingToolBar->addAction(actions_map["actionAuto_units"]); k++; }
//    if (gFullScreenAvailable)            { processingToolBar->addAction(actions_map["actionFull_screen"]); k++; }
//    addToolBar(processingToolBar);

//    k = 0;
//    QToolBar *optionsToolBar = new QToolBar;
//    if (gPreferencesAvailable)   { optionsToolBar->addAction(actions_map["actionPreferences"]);  k++; }
//    if (k)
//        addToolBar(optionsToolBar);

//    mTileManager = new WidgetTileManager;
//    setCentralWidget(mTileManager);


    setWindowFlags(Qt::Widget);

    installToolbarActions();


    mTileManager = new WidgetTileManager;
    setCentralWidget(mTileManager);
}

void TileManagerWithToolbar::installToolbarActions()
{

    if (mPlotToolBar)
        removeToolBar(mPlotToolBar);
    if (mZoomToolBar)
        removeToolBar(mZoomToolBar);
    if (mProcessingToolBar)
        removeToolBar(mProcessingToolBar);
    if (mOptionsToolBar)
        removeToolBar(mOptionsToolBar);


    auto actions_map = global::gJagMainWindowPointer->actions();

    int counter = 0;
    QMap<QString, bool> visibleActions = global::gJagMainWindowPointer->visibleActionsMap();
    auto addAction = [&visibleActions, &actions_map, &counter](QToolBar *toolBar, const QString &actionName) {
       if (visibleActions[actions_map[actionName]->objectName()]) {
           toolBar->addAction(actions_map[actionName]);
           counter++;
       }
    };

    mPlotToolBar = new QToolBar;
    counter = 0;
    addAction(mPlotToolBar, "actionAdd_graph");
    addAction(mPlotToolBar, "actionAdd_event");
    addAction(mPlotToolBar, "actionAdd_plot_frame_horizontally");
    addAction(mPlotToolBar, "actionAdd_vertical_plot_frame");
    addAction(mPlotToolBar, "actionClone_horizontally");
    addAction(mPlotToolBar, "actionClone_vertically");
    addAction(mPlotToolBar, "actionCombine_graphs");
    addAction(mPlotToolBar, "actionSeparate_graphs");
    addAction(mPlotToolBar, "actionIntellectual_layout");
    if (counter)
        addToolBar(mPlotToolBar);

    counter = 0;
    mZoomToolBar = new QToolBar;
    addAction(mZoomToolBar, "actionNormal_mode");
    addAction(mZoomToolBar, "actionMeasuring");
    addAction(mZoomToolBar, "actionShow_value_table");
    addAction(mZoomToolBar, "actionDraw_level");
    addAction(mZoomToolBar, "actionArrow_with_text");
    addAction(mZoomToolBar, "actionFast_secondary_processing");
    addAction(mZoomToolBar, "actionZoom_X");
    addAction(mZoomToolBar, "actionZoom_Y");
    addAction(mZoomToolBar, "actionZoom_XY");
    addAction(mZoomToolBar, "actionMotion");
    addAction(mZoomToolBar, "actionPrevious_scope");
    addAction(mZoomToolBar, "actionNext_scope");
    addAction(mZoomToolBar, "actionAutoscale");
    if (counter)
        addToolBar(mZoomToolBar);

    counter = 0;
    mProcessingToolBar = new QToolBar;
    addAction(mProcessingToolBar, "actionSecondary_processing");
    addAction(mProcessingToolBar, "actionAuto_units");
    addAction(mProcessingToolBar, "actionFull_screen");
    if (counter)
        addToolBar(mProcessingToolBar);

    counter = 0;
    mOptionsToolBar = new QToolBar;
    addAction(mOptionsToolBar, "actionPreferences");
    addAction(mOptionsToolBar, "actionTips");
    if (counter)
        addToolBar(mOptionsToolBar);

}






