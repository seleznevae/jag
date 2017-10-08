#include "plotwidgetwithtoolbar.h"
#include "core/plotwidget.h"
#include <QToolBar>
#include <QVBoxLayout>
#include "global/global_definitions.h"
#include "core/jagmainwindow.h"

PlotWidgetWithToolbar::PlotWidgetWithToolbar(QWidget *parent) :
    QMainWindow(parent), QFullScreenAdapter(this)
{
//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->setSpacing(0);
//    setLayout(layout);

//    mToolBar = new QToolBar;
//    layout->addWidget(mToolBar);

//    auto actions_map = global::gJagMainWindowPointer->actions();

//    mToolBar->addAction(actions_map["actionAdd_graph"]);
//    mToolBar->addAction(actions_map["actionAdd_event"]);
//    mToolBar->addAction(actions_map["actionCombine_graphs"]);
//    mToolBar->addAction(actions_map["actionSeparate_graphs"]);
//    mToolBar->addAction(actions_map["actionIntellectual_layout"]);

//    mToolBar->addAction(actions_map["actionNormal_mode"]);
//    mToolBar->addAction(actions_map["actionMeasuring"]);
//    mToolBar->addAction(actions_map["actionShow_value_table"]);
//    mToolBar->addAction(actions_map["actionDraw_level"]);
//    mToolBar->addAction(actions_map["actionArrow_with_text"]);
//    mToolBar->addAction(actions_map["actionFast_secondary_processing"]);
//    mToolBar->addAction(actions_map["actionZoom_X"]);
//    mToolBar->addAction(actions_map["actionZoom_Y"]);
//    mToolBar->addAction(actions_map["actionZoom_XY"]);
//    mToolBar->addAction(actions_map["actionMotion"]);
//    mToolBar->addAction(actions_map["actionPrevious_scope"]);
//    mToolBar->addAction(actions_map["actionNext_scope"]);
//    mToolBar->addAction(actions_map["actionAutoscale"]);

//    mToolBar->addAction(actions_map["actionSecondary_processing"]);
//    mToolBar->addAction(actions_map["actionAuto_units"]);
//    mToolBar->addAction(actions_map["actionFull_screen"]);

////        mToolBar->addAction(action);

//    mPlotWidget = new PlotWidget;
//    layout->addWidget(mPlotWidget);

    ///////////////////////////////

    //This class is supposed to be deleted soon

//    setWindowFlags(Qt::Widget);

//    auto actions_map = global::gJagMainWindowPointer->actions();

//    QToolBar *plotToolBar = new QToolBar;
//    plotToolBar->addAction(actions_map["actionAdd_graph"]);
//    plotToolBar->addAction(actions_map["actionAdd_event"]);
//    plotToolBar->addAction(actions_map["actionCombine_graphs"]);
//    plotToolBar->addAction(actions_map["actionSeparate_graphs"]);
//    plotToolBar->addAction(actions_map["actionIntellectual_layout"]);
//    addToolBar(plotToolBar);

//    QToolBar *zoomToolBar = new QToolBar;
//    zoomToolBar->addAction(actions_map["actionNormal_mode"]);
//    zoomToolBar->addAction(actions_map["actionMeasuring"]);
//    zoomToolBar->addAction(actions_map["actionShow_value_table"]);
//    zoomToolBar->addAction(actions_map["actionDraw_level"]);
//    zoomToolBar->addAction(actions_map["actionArrow_with_text"]);
//    zoomToolBar->addAction(actions_map["actionFast_secondary_processing"]);
//    zoomToolBar->addAction(actions_map["actionZoom_X"]);
//    zoomToolBar->addAction(actions_map["actionZoom_Y"]);
//    zoomToolBar->addAction(actions_map["actionZoom_XY"]);
//    zoomToolBar->addAction(actions_map["actionMotion"]);
//    zoomToolBar->addAction(actions_map["actionPrevious_scope"]);
//    zoomToolBar->addAction(actions_map["actionNext_scope"]);
//    zoomToolBar->addAction(actions_map["actionAutoscale"]);
//    addToolBar(zoomToolBar);

//    QToolBar *processingToolBar = new QToolBar;
//    processingToolBar->addAction(actions_map["actionSecondary_processing"]);
//    processingToolBar->addAction(actions_map["actionAuto_units"]);
//    processingToolBar->addAction(actions_map["actionFull_screen"]);
//    addToolBar(processingToolBar);

//    QToolBar *optionsToolBar = new QToolBar;
//    optionsToolBar->addAction(actions_map["actionPreferences"]);
//    addToolBar(optionsToolBar);

//    mPlotWidget = new PlotWidget;
//    setCentralWidget(mPlotWidget);
}
