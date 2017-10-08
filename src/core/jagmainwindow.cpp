#include "jagmainwindow.h"
#include "ui_jagmainwindow.h"
#include "datasource/datachoicedialog.h"
#include "basic/basic.h"
#include <QFileDialog>
#include <QToolBar>
#include <QMessageBox>
#include "commonWidgets/preferencesdialog.h"
#include <QSettings>
#include "datasource/datasourcebasic.h"
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QUrl>
#include <QList>
#include <QSplashScreen>
#include <QPrinter>
#include <QPrintDialog>
#include <algorithm>
#include "global/global_definitions.h"
#include "commonWidgets/tipdialog.h"
#include <QTcpSocket>
#include <QLabel>
#include "datasource/datasourceadapter.h"
#include "event/eventsourceadapter.h"
#include "event/eventsource.h"
#include "datasource/simpledatasource.h"
#include "commonWidgets/aboutjag.h"
#include "datasource/datasourcecontroldialog.h"
#include <QToolButton>
#include "commonWidgets/noncriticalerrorsdialogwithinfo.h"
#include "tablePrinter/tableprinter.h"
#include <QMessageBox>
#include "utility/settings.h"
#include "controlWidgets/multimodelcontrolwidget.h"


JagMainWindow::JagMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JagMainWindow), mPreferencesDialog(NULL), mDockAnchorViewWidget(NULL), mIsMeasuringValueTableVisible(false),
    mDataSourceEditingDialog(NULL), mDefaultDataSourceDir(".")
{

    mListeningServer =  new QTcpServer(this);
    if (!mListeningServer->listen(QHostAddress("127.0.0.1"), global::gExternalDataSourcePortNumber)) {
        qWarning() << "Failed to start tcp server on port " << global::gExternalDataSourcePortNumber;
    } else {
        qDebug() << "Tcp server was started on port " << global::gExternalDataSourcePortNumber;
    }
    connect(mListeningServer, SIGNAL(newConnection()),
            this,  SLOT(slotNewConnection()));


    setAcceptDrops(true);
    ui->setupUi(this);




    ui->actionLoad_data_source->setText(tr("Load data source"));
    ui->actionEdit_data_sources->setText(tr("Edit data sources"));

    auto actMaps = actions();
    for (auto action : actMaps.values()) {
        mToolbarVisibleActionsMap[action->objectName()] = true;
    }




    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(treatTabCloseRequest(int)));
    connect(ui->tabWidget, SIGNAL(tabMoved(int, int)),
            this, SLOT(treatTabMotionRequest(int, int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(treatCurrentTabChanged(int)));


    connect(ui->actionLoad_data_source, SIGNAL(triggered()),
            this, SLOT(addDataSource()));
    connect(ui->actionEdit_data_sources, SIGNAL(triggered()),
            this, SLOT(editDataSources()));
    connect(ui->actionSave_global_command_source_as, SIGNAL(triggered()),
            this, SLOT(saveGlobalEventSourceAs()));
    ui->actionSave_global_command_source_as->setEnabled(false);
    connect(ui->actionSave_data_in_table, SIGNAL(triggered()),
            this, SLOT(saveDataInTable()));

    connect(ui->actionNew_plot, SIGNAL(triggered()),
            this, SLOT(addNewPlot()));
    connect(ui->actionAdd_plot_frame_horizontally, SIGNAL(triggered()),
            this, SLOT(addPlotFrameHorizontally()));
    connect(ui->actionAdd_vertical_plot_frame, SIGNAL(triggered()),
            this, SLOT(addPlotFrameVertically()));
    connect(ui->actionClone_horizontally, SIGNAL(triggered()),
            this, SLOT(clonePlotFrameHorizontally()));
    connect(ui->actionClone_vertically, SIGNAL(triggered()),
            this, SLOT(clonePlotFrameVertically()));
    connect(ui->actionAdd_graph, SIGNAL(triggered()),
            this, SLOT(addNewGraph()));
    connect(ui->actionAdd_event, SIGNAL(triggered()),
            this, SLOT(addNewEvent()));
    connect(ui->actionCombine_graphs, SIGNAL(triggered()),
            this, SLOT(combineGraphs()));
    connect(ui->actionSeparate_graphs, SIGNAL(triggered()),
            this, SLOT(separateGraphs()));
    connect(ui->actionIntellectual_layout, SIGNAL(triggered()),
            this, SLOT(composeIntellectually()));


    connect(ui->actionSave_as, SIGNAL(triggered()),
            this, SLOT(saveAs()));
    connect(ui->actionQuick_save, SIGNAL(triggered()),
            this, SLOT(quickSave()));
    connect(ui->actionCopy, SIGNAL(triggered()),
            this, SLOT(copyToClipBoard()));
    connect(ui->actionPrint, SIGNAL(triggered()),
            this, SLOT(print()));


    connect(ui->actionMeasuring, SIGNAL(triggered()),
            this, SLOT(setMeasuring()));
    connect(ui->actionArrow_with_text, SIGNAL(triggered()),
            this, SLOT(setArrowDrawingMode()));
    connect(ui->actionZoom_X, SIGNAL(triggered()),
            this, SLOT(setZoomX()));
    connect(ui->actionZoom_Y, SIGNAL(triggered()),
            this, SLOT(setZoomY()));
    connect(ui->actionZoom_XY, SIGNAL(triggered()),
            this, SLOT(setZoomXY()));
    connect(ui->actionAutoscale, SIGNAL(triggered()),
            this, SLOT(autoscale()));
    connect(ui->actionNormal_mode, SIGNAL(triggered()),
            this, SLOT(setNormalMode()));
    connect(ui->actionMotion, SIGNAL(triggered()),
            this, SLOT(setMotionMode()));
    connect(ui->actionDraw_level, SIGNAL(triggered()),
            this, SLOT(setLevelDrawing()));
    connect(ui->actionFast_secondary_processing, SIGNAL(triggered()),
            this, SLOT(setFastSecondaryProcessingMode()));
    connect(ui->actionPrevious_scope, SIGNAL(triggered()),
            this, SLOT(prevScope()));
    connect(ui->actionNext_scope, SIGNAL(triggered()),
            this, SLOT(nextScope()));

    connect(ui->actionShow_value_table, SIGNAL(toggled ( bool )),
            this, SLOT(showMeasuringTable(bool)));


    connect(ui->actionSecondary_processing, SIGNAL(triggered()),
            this, SLOT(secondaryProcessing()));
    connect(ui->actionAuto_units, SIGNAL(triggered()),
            this, SLOT(setAutoUnits()));
    connect(ui->actionCopy_names_to_clipboard, SIGNAL(triggered()),
            this, SLOT(copyNamesToInternalClipboard()));
    connect(ui->actionFull_screen, SIGNAL(triggered()),
            this, SLOT(toogleFullScreen()));


    connect(ui->actionPreferences, SIGNAL(triggered()),
            this, SLOT(setPreferences()));

    connect(ui->actionAbout_Qt, SIGNAL(triggered()),
            this, SLOT(aboutQt()));
    connect(ui->actionAbout_JaG, SIGNAL(triggered()),
            this, SLOT(aboutJag()));
    connect(ui->actionTips, SIGNAL(triggered()),
            this, SLOT(showHelpInformation()));

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(setPlotEditingEnabled()));

    setPlotEditingEnabled();

    ui->actionNormal_mode->setChecked(true);
    mStatusMessageLabel = new MessageLabel;
    statusBar()->insertWidget (0, mStatusMessageLabel);

    global::gGlobalDataSource = new SimpleDataSource();
    connect(global::gGlobalDataSource, SIGNAL(simpleDataSourceChanged()),
            this, SLOT(treatGlobalDataSourceChanging()));

    global::gGlobalEventSource = new EventSource();
    connect(global::gGlobalEventSource, SIGNAL(eventSourceChanged()),
            this, SLOT(treatGlobalEventSourceChanging()));

    loadSettings();

    mDockAnchorViewWidget = new DockAnchorViewWidget(this);
    mDockAnchorViewWidget->setVisible(mIsMeasuringValueTableVisible );
    ui->actionShow_value_table->setChecked(mIsMeasuringValueTableVisible);
    mDockAnchorViewWidget->setFloating(true);
    connect(mDockAnchorViewWidget, SIGNAL(anchorViewWidgetBecameInvisible()),
            this, SLOT(treatAnchorViewWidgetInvisibility()));


    mNewPlotButton = new QToolButton();
    mNewPlotButton->setDefaultAction(ui->actionNew_plot);
    //mNewPlotButton->setFixedSize(34, 34);
    //mNewPlotButton->setIconSize(QSize(30, 30));

    ui->tabWidget->setCornerWidget(mNewPlotButton, Qt::TopLeftCorner);
    mNewPlotButton->setFixedSize(34, 34);
    mNewPlotButton->setIconSize(QSize(30, 30));


    if (mDataSourceEditingDialog && mDataSourceEditingDialog->isVisible()) {
        mDataSourceEditingDialog->raise();
        mDataSourceEditingDialog->activateWindow();
    }

    mStatusMessageLabel->connectToJagMainWindow(this);

    setToolbarIcons();
}

JagMainWindow::~JagMainWindow()
{

    delete ui;
    if (mDataSourceVector.indexOf(global::gGlobalDataSource) == -1) {
        delete global::gGlobalDataSource;
    }
    // make deleteing of all dataSources (when to addDataSource(DataSource *dataSource) will be transitted only dataSources allocated with new
    foreach (auto dataSource, mDataSourceVector) {
        dataSource->setParent(NULL);
        delete dataSource;
    }
    foreach (auto eventSource, mEventSourceVector) {
        eventSource->setParent(NULL);
        delete eventSource;
    }

}

int JagMainWindow::numberOfDataSources() const
{
    return mDataSourceVector.size();
}

QMap<QString, QAction*> JagMainWindow::actions() const
{
    QMap<QString, QAction*> ret;

    ret["actionAdd_graph"] = ui->actionAdd_graph;
    ret["actionAdd_event"] = ui->actionAdd_event;
    ret["actionAdd_plot_frame_horizontally"] = ui->actionAdd_plot_frame_horizontally;
    ret["actionAdd_vertical_plot_frame"] = ui->actionAdd_vertical_plot_frame;
    ret["actionClone_horizontally"] = ui->actionClone_horizontally;
    ret["actionClone_vertically"] = ui->actionClone_vertically;
    ret["actionCombine_graphs"] = ui->actionCombine_graphs;
    ret["actionSeparate_graphs"] = ui->actionSeparate_graphs;
    ret["actionIntellectual_layout"] = ui->actionIntellectual_layout;


    ret["actionNormal_mode"] = ui->actionNormal_mode;
    ret["actionMeasuring"] = ui->actionMeasuring;
    ret["actionShow_value_table"] = ui->actionShow_value_table;
    ret["actionDraw_level"] = ui->actionDraw_level;
    ret["actionArrow_with_text"] = ui->actionArrow_with_text;
    ret["actionFast_secondary_processing"] = ui->actionFast_secondary_processing;
    ret["actionZoom_X"] = ui->actionZoom_X;
    ret["actionZoom_Y"] = ui->actionZoom_Y;
    ret["actionZoom_XY"] = ui->actionZoom_XY;
    ret["actionMotion"] = ui->actionMotion;
    ret["actionPrevious_scope"] = ui->actionPrevious_scope;
    ret["actionNext_scope"] = ui->actionNext_scope;
    ret["actionAutoscale"] = ui->actionAutoscale;


    ret["actionSecondary_processing"] = ui->actionSecondary_processing;
    ret["actionAuto_units"] = ui->actionAuto_units;
    ret["actionFull_screen"] = ui->actionFull_screen;

    ret["actionPreferences"] = ui->actionPreferences;
    ret["actionTips"] = ui->actionTips;

    return ret;

}

QMap<QString, bool> JagMainWindow::visibleActionsMap() const
{
    return mToolbarVisibleActionsMap;
}

void JagMainWindow::setActionsVisibilityMap(QMap<QString, bool> newMap)
{
    if (mToolbarVisibleActionsMap != newMap) {
        mToolbarVisibleActionsMap = newMap;
    }
}

//PlotStyle JagMainWindow::preferences() const
//{
//    return mPreferences;
//}

void JagMainWindow::addNewPlot()
{
    PlotShowEditWidget *newPlot = new PlotShowEditWidget(this);
    newPlot->setPreferences(PlotStyle::defaultPlotStyle());

    QVector<AbstractDataSourceAdapter*> dataSourceAdapterVector;
    for (auto dataSource : mDataSourceVector) {
        DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
        dataSourceAdapter->setDataSource(dataSource);
        dataSourceAdapterVector.push_back(dataSourceAdapter);
    }


    DataChoiceDialog *dataChoiseDialog = new DataChoiceDialog(newPlot, dataSourceAdapterVector/*mDataSourceVector*/);
    bool createEmptyPlot = false;
    // if there are no data sources then simply ask if user want to create empty plot
    if (mDataSourceVector.size() == 0) {
        if (QMessageBox::question(this,"", "There aren't any data sources. Do you want to create empty plot?",
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
            createEmptyPlot = true;
        }
    }
    if (createEmptyPlot || (dataChoiseDialog->exec()) == QDialog::Accepted) {
        mPlotVector.push_back(newPlot);
        ui->tabWidget->addTab(newPlot, QString("Plot ") + QString::number(mPlotVector.size()));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
        connect(newPlot, SIGNAL(infoStringEmitted(QString)),
                this, SLOT(showInfoString(QString)));
        connect(newPlot, SIGNAL(currentPlotFrameChanged()),
                this, SLOT(setCurrentPlotWidgetForAnchorViewWidget()));

        ui->actionNormal_mode->setChecked(true);
        ui->actionZoom_X->setChecked(false);
        ui->actionZoom_Y->setChecked(false);
        ui->actionZoom_XY->setChecked(false);
        ui->actionMeasuring->setChecked(false);

        setPlotEditingEnabled();
    } else {
        newPlot->setParent(NULL);
        delete newPlot;
    }
    dataChoiseDialog->setParent(NULL);
    delete dataChoiseDialog;

    for (auto dataSourceAdapter : dataSourceAdapterVector) {
        delete dataSourceAdapter;
    }


}

void JagMainWindow::addPlotFrameHorizontally()
{
    addNewPlotFrame(Qt::Horizontal);
}

void JagMainWindow::addPlotFrameVertically()
{
    addNewPlotFrame(Qt::Vertical);
}

void JagMainWindow::clonePlotFrameHorizontally()
{
    cloneCurrentPlotFrame(Qt::Horizontal);
}

void JagMainWindow::clonePlotFrameVertically()
{
    cloneCurrentPlotFrame(Qt::Vertical);
}

void JagMainWindow::addNewPlotFrame(Qt::Orientation orient)
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->addNewPlotFrame(orient);

        QVector<AbstractDataSourceAdapter*> dataSourceAdapterVector;
        for (auto dataSource : mDataSourceVector) {
            DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
            dataSourceAdapter->setDataSource(dataSource);
            dataSourceAdapterVector.push_back(dataSourceAdapter);
        }

        DataChoiceDialog *dataChoiseDialog = new DataChoiceDialog(mPlotVector[ui->tabWidget->currentIndex()], dataSourceAdapterVector/*mDataSourceVector*/);
        dataChoiseDialog->exec();

        for (auto dataSourceAdapter : dataSourceAdapterVector) {
            delete dataSourceAdapter;
        }

        dataChoiseDialog->setParent(NULL);
        delete dataChoiseDialog;
    }
}

void JagMainWindow::cloneCurrentPlotFrame(Qt::Orientation orient)
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->cloneCurrentPlotFrame(orient);
    }
}

void JagMainWindow::addNewGraph()
{
    if (ui->tabWidget->count() >= 1) {
//        DataChoiseDialog *dataChoiseDialog = new DataChoiseDialog(mPlotVector[ui->tabWidget->currentIndex()], mDataSourceVector);

        //        DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
        //        dataSourceAdapter->setDataSource(dataSource);
        QVector<AbstractDataSourceAdapter*> dataSourceAdapterVector;
        for (auto dataSource : mDataSourceVector) {
            DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
            dataSourceAdapter->setDataSource(dataSource);
            dataSourceAdapterVector.push_back(dataSourceAdapter);
        }

        DataChoiceDialog *dataChoiseDialog = new DataChoiceDialog(mPlotVector[ui->tabWidget->currentIndex()], dataSourceAdapterVector/*mDataSourceVector*/);
        dataChoiseDialog->exec();

        for (auto dataSourceAdapter : dataSourceAdapterVector) {
            delete dataSourceAdapter;
        }

        dataChoiseDialog->setParent(NULL);
        delete dataChoiseDialog;
    }
}

void JagMainWindow::addNewEvent()
{
    if (ui->tabWidget->count() >= 1) {
        QVector<AbstractDataSourceAdapter*> dataSourceAdapterVector;
//        for (auto dataSource : mDataSourceVector) {
//            DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
//            dataSourceAdapter->setDataSource(dataSource);
//            dataSourceAdapterVector.push_back(dataSourceAdapter);
//        }

        //temp
//        EventSource commandSource(QString("data.jef"));
//        EventSourceAdapter *eventSourceAdapter = new EventSourceAdapter();
//        eventSourceAdapter->setEventSource(&commandSource);
//        dataSourceAdapterVector.push_back(eventSourceAdapter);
        //temp


        for (auto eventSource : mEventSourceVector) {
            EventSourceAdapter *eventSourceAdapter = new EventSourceAdapter();
            eventSourceAdapter->setEventSource(eventSource);
            dataSourceAdapterVector.push_back(eventSourceAdapter);
        }


        DataChoiceDialog *dataChoiseDialog = new DataChoiceDialog(mPlotVector[ui->tabWidget->currentIndex()], dataSourceAdapterVector/*mDataSourceVector*/);
        dataChoiseDialog->exec();

        for (auto dataSourceAdapter : dataSourceAdapterVector) {
            delete dataSourceAdapter;
        }
    }
}

void JagMainWindow::addDataSource()
{
    //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//#ifdef __linux
    QFileDialog::Options option =  QFileDialog::DontUseNativeDialog;
//#else
//    QFileDialog::Option option = 0x0;
//#endif
    QStringList fileNamesList = QFileDialog::getOpenFileNames(
                             this,
                             "Select one or more files to open",
                             mDefaultDataSourceDir, QString(), NULL,
                             option
                                                             );
    foreach (QString fileName, fileNamesList) {
        addDataSource(fileName);
    }

}

void JagMainWindow::addDataSource(DataSource *dataSource)
{
    if (dataSource != NULL) {
        if (dataSource->isValid()) {
            connect(dataSource, SIGNAL(errorDuringLoading(QString)),
                    this, SLOT(treatErrorDuringDataSourceLoading(QString)));
            connect(dataSource, SIGNAL(nonCriticalErrorsDuringLoading()),
                    this, SLOT(treatNonCriticalErrorDuringDataSourceLoading()));

            mDataSourceVector.push_back(dataSource);
            mStatusMessageLabel->setMessage("<h3>Data source    "+ dataSource->getDataSourceName()  + "    was added <font color=green> successfully</font></h3>", 3000);
        } else {
            treatErrorDuringDataSourceLoading("Error during " + dataSource->getDataSourceName() + " loading");
        }
    }
}

void JagMainWindow::removeDataSource(DataSource *dataSource)
{
    if (dataSource != NULL) {
        int index = mDataSourceVector.indexOf(dataSource);
        if (dataSource == global::gGlobalDataSource) {
            if (index != -1) {
                mDataSourceEditingDialog->deleteDataSourceTab(dataSource);

                mDataSourceVector.remove(index);
                global::gGlobalDataSource->clearData();
                connect(global::gGlobalDataSource, SIGNAL(simpleDataSourceChanged()),
                        this, SLOT(treatGlobalDataSourceChanging()));

                return;
            }
        }
        if (index != -1) {
                mDataSourceEditingDialog->deleteDataSourceTab(dataSource);

                if (!dataSource->isFinished()) {
                    qDebug() << "Termination of an unfinished thread";
                    dataSource->terminate();
                    dataSource->wait();
                }
                mStatusMessageLabel->setMessage("<h3>Data source    "+ dataSource->getDataSourceName()  + "    was removed</h3>", 3000);
                dataSource->setParent(NULL);
                delete dataSource;
                mDataSourceVector.remove(index);
        }
    }
}

void JagMainWindow::addDataSource(QString fileName)
{
    QFileInfo file(fileName);
    if (file.exists()) {
        mDefaultDataSourceDir = file.absolutePath();
    }

    try {
        if (fileName.endsWith(".jef", Qt::CaseInsensitive)) {
            EventSource *eventSource = new EventSource(fileName);
            mEventSourceVector.push_back(eventSource);
        } else {
            auto dataSource = createDataSourceFromFileName(fileName);
            addDataSource(dataSource);

            std::shared_ptr<GraphStyle> style = dataSource->graphStyle();
            QPen pen = style->pen();
            pen.setWidthF(global::gDefaultPlotPreferences.mLineWidth);
            style->setPen(pen);
            QCPScatterStyle scatStyle = style->scatterStyle();
            scatStyle.setDecimation(global::gDefaultPlotPreferences.mScatterDecimation);
            scatStyle.setSize(global::gDefaultPlotPreferences.mScatterSize);
            style->setScatterStyle(scatStyle);
            style->setLineInterpolation(global::gDefaultPlotPreferences.mLineInterpolation);
            style->setSpecifiedProperty(false);

            emit newDataSourceCreated(dataSource);

            if (global::gShowDataSourceEditingDialog/* && DataSourceControlDialog::isDataSourceControlDialogActive == false*/) {
                editDataSources();
            }


            //DEBUG
//            static int i = 0;
//            if (i++ %2) {
//                QPen pen = style->pen();
//                pen.setWidthF(7);
//                style->setPen(pen);

//                QCPScatterStyle scatStyle = style->scatterStyle();
//                scatStyle.setDecimation(60);
//                scatStyle.setSize(20);
//                scatStyle.setShape(QCPScatterStyle::ssSquare);
//                style->setScatterStyle(scatStyle);
//                style->setSpecifiedProperty(true);
//            }
            //DEBUG

//            mDataSourceVector.push_back(dataSource);
//            mStatusMessageLabel->setMessage("<h3>Data source    "+ fileName + "    was loaded <font color=green> successfully</font></h3>", 3000);
        }
    } catch(...) {
        showCriticalMessage("Data source " + fileName + " can't be loaded.");
        mStatusMessageLabel->setMessage("<h3><font color=red>Data source    "+ fileName + "    was  not loaded </font></h3>", 3000);
    }

}

void JagMainWindow::editDataSources()
{

//    DataSourceControlDialog cw(mDataSourceVector);
//    connect(this, SIGNAL(newDataSourceCreated(DataSource*)),
//            &cw, SLOT(addDataSource(DataSource*)));
//    cw.exec();
//    disconnect(this, SIGNAL(newDataSourceCreated(DataSource*)),
//            &cw, SLOT(addDataSource(DataSource*)));


    if (mDataSourceEditingDialog == NULL) {
        mDataSourceEditingDialog = new DataSourceControlDialog(mDataSourceVector, this);
        connect(this, SIGNAL(newDataSourceCreated(DataSource*)),
                mDataSourceEditingDialog, SLOT(addDataSource(DataSource*)));
    }
//    mDataSourceEditingDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    mDataSourceEditingDialog->show();
    mDataSourceEditingDialog->raise();
    mDataSourceEditingDialog->activateWindow();
//    disconnect(this, SIGNAL(newDataSourceCreated(DataSource*)),
    //            cw, SLOT(addDataSource(DataSource*)));
}

void JagMainWindow::saveGlobalEventSourceAs()
{
    if (mEventSourceVector.indexOf(global::gGlobalEventSource) != -1) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                    global::gDefaultDirectory.endsWith("/") || global::gDefaultDirectory.endsWith("\\") ?
                                    global::gDefaultDirectory + "untitled.jef" : global::gDefaultDirectory + "/untitled.jef" ,
                                    tr("*.jef"));
        if (fileName != "") {
            global::gGlobalEventSource->saveAs(fileName);
        }
    }


}

void JagMainWindow::saveDataInTable()
{
        QVector<AbstractDataSourceAdapter*> dataSourceAdapterVector;
        for (auto dataSource : mDataSourceVector) {
            DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
            dataSourceAdapter->setDataSource(dataSource);
            dataSourceAdapterVector.push_back(dataSourceAdapter);
        }

        TablePrinter tablePrinter;
        DataChoiceDialog *dataChoiseDialog = new DataChoiceDialog(&tablePrinter, dataSourceAdapterVector);
        dataChoiseDialog->exec();

        for (auto dataSourceAdapter : dataSourceAdapterVector) {
            delete dataSourceAdapter;
        }
        tablePrinter.saveData();

}

void JagMainWindow::treatTabCloseRequest(int tabIndex)
{
    qDebug() << "Tab index to delete" << tabIndex;
    qDebug() << "Tabs quantity before deleting " << ui->tabWidget->count();
    qDebug() << "plots quantity before deleting" << mPlotVector.size();
    if (tabIndex < 0 || tabIndex >= mPlotVector.size()) {
        errorExit("Unexpected situation in %s\n", __FUNCTION__);
    }
    disconnect(mPlotVector[tabIndex], SIGNAL(infoStringEmitted(QString)),
            this, SLOT(showInfoString(QString)));
    mPlotVector[tabIndex]->setParent(NULL);  //this will automatically remove corresponding tab in tabWidget
    delete mPlotVector[tabIndex];
    mPlotVector.remove(tabIndex);

    mDockAnchorViewWidget->setPlotWidget(NULL);

    qDebug() << "Tabs quantity after deleting " << ui->tabWidget->count();
    qDebug() << "plots quantity after deleting" << mPlotVector.size();
}

void JagMainWindow::treatTabMotionRequest(int to , int from) // WTF? parameter order doesn't agree with documentation
{
    if (from < 0 || from >= mPlotVector.size()) {
        errorExit("Unexpected situation in %s\n", __FUNCTION__);
    }
    if (to < 0 || to >= mPlotVector.size()) {
        errorExit("Unexpected situation in %s\n", __FUNCTION__);
    }
    if (from > to) {
        auto plot = mPlotVector[from];
        mPlotVector.remove(from);
        mPlotVector.insert(to, plot);
    } else if (from < to ) {
        auto plot = mPlotVector[from];
        mPlotVector.remove(from);
        mPlotVector.insert(to - 1, plot);
    } else {
        errorExit("Unexpected situation in %s\n", __FUNCTION__);
    }
}

void JagMainWindow::treatCurrentTabChanged(int newTab)
{
    if ((ui->tabWidget->count()) && (newTab < ui->tabWidget->count())) {
       setStatesChecked(false);

       switch (mPlotVector[newTab]->plotWidgetState()) {
           case PlotWidget::PlotWidgetMode::Common:
               ui->actionNormal_mode->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::Measuring:
               ui->actionMeasuring->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::ZoomXY:
               ui->actionZoom_XY->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::ZoomX:
               ui->actionZoom_X->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::ZoomY:
               ui->actionZoom_Y->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::Motion:
               ui->actionMotion->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::ArrowDrawing:
               ui->actionArrow_with_text->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::FastSecondaryProcessing:
               ui->actionArrow_with_text->setChecked(true);
               break;
           case PlotWidget::PlotWidgetMode::LevelDrawing:
               ui->actionDraw_level->setChecked(true);
               break;
            //NOTE: Default clause is intentionally commented to raise errors in case of forgotten PlotWidget states
//           default:
//               ui->actionNormal_mode->setChecked(true);
//               break;
       }
//       mDockAnchorViewWidget->setPlotWidget(mPlotVector[ui->tabWidget->currentIndex()]->plotWidget());
       setCurrentPlotWidgetForAnchorViewWidget();
    }
}

void JagMainWindow::setCurrentPlotWidgetForAnchorViewWidget()
{
    if (ui->tabWidget->count() >= 1) {
        mDockAnchorViewWidget->setPlotWidget(mPlotVector[ui->tabWidget->currentIndex()]->plotWidget());
    }
}

void JagMainWindow::combineGraphs()
{
    if (ui->tabWidget->count() >= 1) {
       mPlotVector[ui->tabWidget->currentIndex()]->combineGraphs();
    }
}

void JagMainWindow::setZoomX()
{
    if (ui->tabWidget->count() >= 1) {
       setStatesChecked(false);
       ui->actionZoom_X->setChecked(true);
       mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::ZoomX);
    }
}

void JagMainWindow::setZoomY()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionZoom_Y->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::ZoomY);
    }
}

void JagMainWindow::setZoomXY()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionZoom_XY->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::ZoomXY);
    }
}

void JagMainWindow::setArrowDrawingMode()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionArrow_with_text->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::ArrowDrawing);
    }
}

void JagMainWindow::setLevelDrawing()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionDraw_level->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::LevelDrawing);
    }
}

void JagMainWindow::setNormalMode()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionNormal_mode->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::Common);
    }
}

void JagMainWindow::setMeasuring()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionMeasuring->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::Measuring);
    }
}

void JagMainWindow::setMotionMode()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionMotion->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::Motion);
    }
}

void JagMainWindow::setFastSecondaryProcessingMode()
{
    if (ui->tabWidget->count() >= 1) {
        setStatesChecked(false);
        ui->actionFast_secondary_processing->setChecked(true);
        mPlotVector[ui->tabWidget->currentIndex()]->setPlotWidgetState(PlotWidget::PlotWidgetMode::FastSecondaryProcessing);
    }
}

void JagMainWindow::setStatesChecked(bool checked)
{
    if (ui->tabWidget->count() >= 1) {
        ui->actionNormal_mode->setChecked(checked);
        ui->actionZoom_X->setChecked(checked);
        ui->actionZoom_Y->setChecked(checked);
        ui->actionZoom_XY->setChecked(checked);
        ui->actionDraw_level->setChecked(checked);
        ui->actionMeasuring->setChecked(checked);
        ui->actionMotion->setChecked(checked);
        ui->actionArrow_with_text->setChecked(checked);
        ui->actionFast_secondary_processing->setChecked(checked);
    }
}

void JagMainWindow::prevScope()
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->stepBackInHistory();
    }
}

void JagMainWindow::nextScope()
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->stepForwardInHistory();
    }
}

void JagMainWindow::showMeasuringTable(bool visibility)
{
    mIsMeasuringValueTableVisible = visibility;
    if (mIsMeasuringValueTableVisible)
        mDockAnchorViewWidget->show();
    else
        mDockAnchorViewWidget->hide();
}

void JagMainWindow::autoscale()
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->autoscale();
    }
}

void JagMainWindow::secondaryProcessing()
{
    if (ui->tabWidget->count() >= 1) {
        mPlotVector[ui->tabWidget->currentIndex()]->secondaryProcessing();
    }
}

void JagMainWindow::setAutoUnits()
{
    if (ui->tabWidget->count() >= 1) {
       mPlotVector[ui->tabWidget->currentIndex()]->setAutoUnits();
    }
}

void JagMainWindow::copyNamesToInternalClipboard()
{
//    if (ui->tabWidget->count() >= 1) {
//       QVector<QString> graphNames = mPlotVector[ui->tabWidget->currentIndex()]->getGraphNames();
//       for (int i = 0; i < graphNames.size(); ++ i)
//           graphNames[i] = global::getPrameterNameFromComplexName(graphNames[i]);
//       qSort(graphNames.begin(), graphNames.end());
//       auto it = std::unique(graphNames.begin(), graphNames.end());
//       graphNames.erase(it, graphNames.end());

//       if (graphNames.size() != 0) {
//           global::gParametersSearchClipboard = graphNames;
//           mStatusMessageLabel->setMessage("<h3><font color=green>Parameters'  names  were  copied  to  dataChoiseDialog  internal  clipboard</font></h3>", 3000);
//       } else {
//           mStatusMessageLabel->setMessage("<h3><font color=red>No parameter names to copy to the clippoard</font></h3>", 3000);
//       }
//    }

    if (ui->tabWidget->count() >= 1) {
       QVector<std::pair<QString, QString>> graphNames = mPlotVector[ui->tabWidget->currentIndex()]->currentPlotGraphNamesAndArgs();
       qSort(graphNames.begin(), graphNames.end());
       auto it = std::unique(graphNames.begin(), graphNames.end());
       graphNames.erase(it, graphNames.end());

       if (graphNames.size() != 0) {
           global::gParametersSearchClipboard.clear();
           global::gParametersSearchArgumentsClipboard.clear();
           for (auto item : graphNames) {
                global::gParametersSearchClipboard.push_back(item.first);
                global::gParametersSearchArgumentsClipboard.push_back(item.second);
           }
           mStatusMessageLabel->setMessage("<h3><font color=green>Parameters'  names  were  copied  to  dataChoiseDialog  internal  clipboard</font></h3>", 3000);
       } else {
           mStatusMessageLabel->setMessage("<h3><font color=red>No parameter names to copy to the clippoard</font></h3>", 3000);
       }
    }
}

void JagMainWindow::toogleFullScreen()
{
    if (ui->tabWidget->count() >= 1) {
       mPlotVector[ui->tabWidget->currentIndex()]->toogleFullScreen();
    }
}

void JagMainWindow::setPreferences()
{
    if (mPreferencesDialog == NULL) {
        mPreferencesDialog = new PreferencesDialog(this);
        connect(mPreferencesDialog, SIGNAL(preferenceSavingRequest()),
                this, SLOT(saveSettings()));
    }
    //if user have chosen apply or save/apply, update all plots and toolbar actions
    if (mPreferencesDialog->exec() == QDialog::Accepted) {
        for (auto plotShowWidget : mPlotVector) {
            plotShowWidget->installToolbarActions();
            plotShowWidget->updateAppearance();
        }
        setToolbarIcons();
    }

}

void JagMainWindow::aboutQt()
{
    QApplication::aboutQt () ;
    //    QMessageBox::aboutQt(this).exec();
}

void JagMainWindow::aboutJag()
{
    AboutJag aboutJag(this);
    aboutJag.exec();
}


void JagMainWindow::showHelpInformation()
{

    TipDialog tipDialog;
    QStringList tipsList;
    tipsList <<    "Useful shortcuts:<br><br>"
                   "press 1, 2 .. 9 to choose editing of 1, 2 .. 9 graph<br><br>"
                   "press Alt-1, Alt-2 .. Alt-9 to choose editing of 1, 2 .. 9 axis"

              << "You can move graphs, axes, join axes using drag & drop in plot"
                 " hierarchy widget. <br> Also you can move graphs to an existing or new axis by "
                 "dragging them in the main plotting widget."

              << "To add event or level lines to an existing graph use left and right button mouse clicks in level drawing mode."

              << "To add existing graph or event to global data or event source use context menu on "
                 "items in hierarchy widget."

              << "There are 3 quick means to set layout:<br>"
                 "Ctrl - -    to combine all graphs in one axis<br>"
                 "Ctrl - |    to put each graph in separate axis<br>"
                 "Ctrl - i    to group parameters with related names in one axis<br>"

              << "You can move current axes or current graph with Ctrl-Up and Ctrl-Down"

              << "JaG can load not only data sources but also \"event sources\" - ordinary csv files "
                 "with *.jef (jag event file) extension. Each string of such files must match a pattern: \"COMMAND_NAME, COMMAND_TIME, "
                 "[COMMAND_DESCRIPTION]\"."

              << "Useful shortcuts for axis and graph manipulation:<br><br>"
                 "Use \"-\" and \"=\" to shrink and stretch current axis<br>"
                 "r - to remove current axis or graph<br>"
                 "w - to increase current graph line width<br>"
                 "Shift + w - to decrease current graph line width<br>"
                 "m - to switch marker shape<br>"
                 "Shift + m - to switch marker shape in opposite direction<br>"
                 "c - to switch line color<br>"
                 "Shift + c - to switch line color in opposite direction<br>"
                 "d - increase marker decimation<br>"
                 "Shift + c - decrease marker decimation<br>"
                 "i - change line interpolation (only line and  step left are available<br>"
                 "Shift + i - change line interpolation in opposite direction<br>"

              << "In zoom modes use left button of your mouse to implement limited (by visible boundaries) zoom "
                 "and right button to implement unlimited zoom<br>"

              << "JaG can automatically set units to the parameters (you can use button in the toolbar or /plot/options/Auto dimension in the menubar)"
                 "if it was not explicitly specified in the data source.<br>"
                 "You should preliminarily set rules "
                 "in preferences so that JaG knows what dimension should be set to a particular parameter."

              << "To copy names of the graphs in the current plot to dataChoiseDialog internal clipboard (you can use it to quickly select the same parameters "
                 "from another data source) pres Ctrl-Shift-C"

              << "To measure x and y distance between two points you can use left mouse button in measuring mode"

              << "By default JaG considers that all command sourced files (*.jef) have UTF-8 encoding.<br>"
                 "If you use another encoding, you should explicitly mark it by placing a commentary "
                 "at the beginning of *.jef file. Example:<br>"
                 "<i>#encoding=Windows-1251</i>"

              << "To add an anchor to the current plot you can use right button click in measuring mode. Anchors are handy in measuring mode "
                 "if you enable data table in menu."

              << "You can copy graph properties from one graph and implement them to another. Use buttons "
                 "\"Copy prop.\" and \"Paste prop\" at the bottom of graph editing widget."

              << "To retrieve quickly certain bits from a parameter that can be considered as unsigned integer use context menu (choose graph in Plot hierarchy widget, "
                 "click right button and choose what bit you want to retrieve)."

              << "You can add data from text files with data in table form. <br>Format of table: <br>1 line - name of the parameters "
                 "(e.g. \"time x1 y2 z3\"). <br>2 line(optional) -format string (e.g %f %f %x %f) (available formats -%f for double values, "
                 "%x for unsigned values in hex form.) Without format string all data will be loaded as double. <br>3 and following lines - data.<br>You  shouldn't use different separators(\",\", \";\") in "
                 "such files."

              << "To insert into the footer text result of some external program use bash-like syntax: $( prog-name )."
                 "Example: to insert into the right footer string current date on Unix like OS set right footer string in preferences to \"Creation time $( /bin/time )\""
                  ;

    tipDialog.setTips(tipsList);
    tipDialog.exec();


}

void JagMainWindow::loadSettings()
{
    QSettings settings;
    QVariant varSetting;

    global::loadGlobalSettings(settings);

    if ((varSetting = settings.value("/Settings/gIsLatexMarkupEnabled")).isValid()) {
        QCP::gIsLatexMarkupEnabled = varSetting.toBool();
    }

    if (global::gIsJaGMainWindowInitialMoveEnabled)
        move(QPoint(global::gPreviousSessionXCoord, global::gPreviousSessionYCoord));

    //loading window size
    if ((varSetting = settings.value("/Settings/gPreviousSessionWidth")).isValid()) {
        global::gPreviousSessionWidth = varSetting.toInt();
        resize(global::gPreviousSessionWidth, height());
    } else {
        global::gPreviousSessionWidth = width();
    }
    if ((varSetting = settings.value("/Settings/gPreviousSessionHeight")).isValid()) {
        global::gPreviousSessionHeight = varSetting.toInt();
        resize(width(), global::gPreviousSessionHeight);
    } else {
        global::gPreviousSessionHeight = height();
    }

    PlotStyle::loadUserStylesFromSettings(settings);


    //fastSecondaryProcessing
    ::loadFromSettings(settings, "/Settings/gFastSecondaryProcessingVector", global::gFastSecondaryProcessingVector);

    //controlWidgets
    ::loadFromSettings(settings, "/Settings/controlWidgets/MultiModelControlWidget/sTabVisibilityVector", MultiModelControlWidget::sTabVisibilityVector, MultiModelControlWidget::sTabVisibilityVector.size());
    ::loadFromSettings(settings, "/Settings/controlWidgets/MultiModelControlWidget/sTabNamesVector"     , MultiModelControlWidget::sTabNamesVector, MultiModelControlWidget::sTabNamesVector.size());
    ::loadFromSettings(settings, "/Settings/controlWidgets/PlotControlWidget/sTabVisibilityVector"      , PlotControlWidget::sTabVisibilityVector, PlotControlWidget::sTabVisibilityVector.size());
    ::loadFromSettings(settings, "/Settings/controlWidgets/PlotControlWidget/sTabNamesVector"           , PlotControlWidget::sTabNamesVector, PlotControlWidget::sTabNamesVector.size());

    for (const QString &actionName : mToolbarVisibleActionsMap.keys()) {
        if ((varSetting = settings.value("/Settings/toolbarActions/" + actionName)).isValid())
            mToolbarVisibleActionsMap[actionName] = varSetting.toBool();
    }

}

void JagMainWindow::saveSettings()
{
    QSettings settings;

    PlotStyle::saveUserStylesInSettings(settings);

    settings.setValue("/Settings/gIsLatexMarkupEnabled", QCP::gIsLatexMarkupEnabled);

    for (const QString &actionName : mToolbarVisibleActionsMap.keys()) {
        settings.setValue("/Settings/toolbarActions/"+ actionName, mToolbarVisibleActionsMap[actionName]);
    }

    global::saveGlobalSettingsOnUserRequest(settings);

}

void JagMainWindow::showInfoString(QString info)
{
    mStatusMessageLabel->setMessage(info, 10000);
}

void JagMainWindow::slotNewConnection()
{
    qDebug() << "slotNewConnection";
    QTcpSocket * clientSocket = mListeningServer->nextPendingConnection();
    if (clientSocket) {
        mClientSocketVector.push_back(clientSocket);
        connect(clientSocket, SIGNAL(disconnected()),
                this, SLOT(treatSocketDisconnection()));
        connect(clientSocket, SIGNAL(readyRead()),
                this, SLOT(slotReadClient()));
    //    sendToClient(pClientSocket, "Server Response: Connected!");
    }
}

void JagMainWindow::slotReadClient()
{

    if (QTcpSocket * clientSocket = qobject_cast<QTcpSocket *>(sender())) {
        qDebug() << "slotReadClient in JagMainWindow";
        quint16 blockSize = 0;
        QDataStream in(clientSocket);
        in.setVersion(QDataStream::Qt_4_5);
        for (;;) {
            if (clientSocket->bytesAvailable()  < static_cast<qint64>(sizeof(quint16)))
                continue;
            in >> blockSize;
            break;
        }

        quint16 numberOfStrings = 0;
        for (;;) {
              if (clientSocket->bytesAvailable()  < blockSize)
                 continue;
              in >> numberOfStrings;
              for (int i = 0; i < numberOfStrings; ++i) {
                  QString dataSourceName;
                  in >> dataSourceName;
                  qDebug() << "string in message " << dataSourceName;
                  addDataSource(dataSourceName);
              }
              break;
        }

        //sending data to sender to close it
        QByteArray arrBlock;
        QDataStream outStream(&arrBlock, QIODevice::WriteOnly);
        outStream.setVersion(QDataStream::Qt_4_5);
        outStream << quint16(666);
        clientSocket->write(arrBlock);
        qDebug() << "Data send to client from server";
    }

}

void JagMainWindow::treatSocketDisconnection()
{
    if (QTcpSocket * clientSocket = qobject_cast<QTcpSocket *>(sender())) {
        int index = mClientSocketVector.indexOf(clientSocket);
        if (index != -1) {
            mClientSocketVector.remove(index);
            clientSocket->deleteLater();
        }
    }

}

void JagMainWindow::treatGlobalDataSourceChanging()
{
    if (mDataSourceVector.indexOf(global::gGlobalDataSource) == -1) {
        mDataSourceVector.push_back(global::gGlobalDataSource);
        disconnect(global::gGlobalDataSource, SIGNAL(simpleDataSourceChanged()),
                this, SLOT(treatGlobalDataSourceChanging()));
        global::gGlobalDataSource->setParent(NULL);
        global::gGlobalDataSource->setDataSourceName("Global");
        emit newDataSourceCreated(global::gGlobalDataSource);
    }
}

void JagMainWindow::treatGlobalEventSourceChanging()
{
    if (mEventSourceVector.indexOf(global::gGlobalEventSource) == -1) {
        mEventSourceVector.push_back(global::gGlobalEventSource);
        disconnect(global::gGlobalEventSource, SIGNAL(eventSourceChanged()),
                this, SLOT(treatGlobalEventSourceChanging()));
        global::gGlobalEventSource->setParent(NULL);
        global::gGlobalEventSource->setName("Global");
        ui->actionSave_global_command_source_as->setEnabled(true);
    }
}

void JagMainWindow::treatAnchorViewWidgetInvisibility()
{
    if (mIsMeasuringValueTableVisible) {
        mIsMeasuringValueTableVisible = false;
        mDockAnchorViewWidget->hide();
        ui->actionShow_value_table->setChecked(false);
    }
}

void JagMainWindow::treatErrorDuringDataSourceLoading(QString errorMessage)
{

    DataSource *dataSource = qobject_cast<DataSource *>(sender());
    if (dataSource) {
        errorMessage = "Error during " + dataSource->getDataSourceName() + " loading: " + errorMessage;
//        int index = -1;
//        if ((index = mDataSourceVector.indexOf(dataSource)) != -1) {
//            mDataSourceVector.remove(index);
//            mDataSourceEditingDialog->deleteDataSource(dataSource);
//        }
        if (mDataSourceVector.indexOf(dataSource) != -1) {
            removeDataSource(dataSource);
        }
    }
    showCriticalMessage(errorMessage);
}

void JagMainWindow::treatNonCriticalErrorDuringDataSourceLoading()
{
    DataSource *dataSource = qobject_cast<DataSource *>(sender());
    if (dataSource) {
        NonCriticalErrorsDialogWithInfo messageDialog;
        messageDialog.showShowItNextTimeCheckBox(false);
        messageDialog.setMessage(dataSource->getDataSourceName() + " contains incorrect data.\nDo you really want to continue loading?");
        messageDialog.setDetails(dataSource->errorsList());
//        messageDialog.setRejectText("No");
//        messageDialog.setAcceptText("Yes");
        if (messageDialog.exec() == QDialog::Rejected) {
//            int index = -1;
//            if ((index = mDataSourceVector.indexOf(dataSource)) != -1) {
//                mDataSourceVector.remove(index);
//                removeDataSource(dataSource);
//                mDataSourceEditingDialog->deleteDataSource(dataSource);
//            }
            if (mDataSourceVector.indexOf(dataSource) != -1) {
                removeDataSource(dataSource);
            }
        }
    }
}

void JagMainWindow::show()
{
    QMainWindow::show();
    if (mDataSourceEditingDialog && mDataSourceEditingDialog->isVisible()) {
        mDataSourceEditingDialog->raise();
        mDataSourceEditingDialog->activateWindow();
    }
}

void JagMainWindow::setPlotEditingEnabled()
{
    bool enabled = (ui->tabWidget->count() == 0 ? false : true);

    ui->actionAdd_graph->setEnabled(enabled);
    ui->actionAdd_event->setEnabled(enabled);
    ui->actionCombine_graphs->setEnabled(enabled);
    ui->actionSeparate_graphs->setEnabled(enabled);
    ui->actionIntellectual_layout->setEnabled(enabled);
    ui->actionAdd_plot_frame_horizontally->setEnabled(enabled);
    ui->actionAdd_vertical_plot_frame->setEnabled(enabled);
    ui->actionClone_horizontally->setEnabled(enabled);
    ui->actionClone_vertically->setEnabled(enabled);

    ui->actionSave_as->setEnabled(enabled);
    ui->actionQuick_save->setEnabled(enabled);
    ui->actionCopy->setEnabled(enabled);
    ui->actionPrint->setEnabled(enabled);

    ui->actionArrow_with_text->setEnabled(enabled);
    ui->actionMeasuring->setEnabled(enabled);
    ui->actionZoom_X->setEnabled(enabled);
    ui->actionZoom_Y->setEnabled(enabled);
    ui->actionZoom_XY->setEnabled(enabled);
    ui->actionAutoscale->setEnabled(enabled);
    ui->actionNormal_mode->setEnabled(enabled);
    ui->actionMotion->setEnabled(enabled);
    ui->actionNext_scope->setEnabled(enabled);
    ui->actionPrevious_scope->setEnabled(enabled);
    ui->actionDraw_level->setEnabled(enabled);
    ui->actionFast_secondary_processing->setEnabled(enabled);

    ui->actionShow_value_table->setEnabled(enabled);

    ui->actionSecondary_processing->setEnabled(enabled);
    ui->actionAuto_units->setEnabled(enabled);
    ui->actionCopy_names_to_clipboard->setEnabled(enabled);
    ui->actionFull_screen->setEnabled(enabled);


}

void JagMainWindow::setToolbarIcons()
{
    ui->actionAdd_graph->setIcon(global::getStyledIcon("plus"));
    ui->actionAdd_event->setIcon(global::getStyledIcon("addCommand"));
    ui->actionClone_horizontally->setIcon(global::getStyledIcon("clone_hor"));
    ui->actionClone_vertically->setIcon(global::getStyledIcon("clone_ver"));

    ui->actionAdd_plot_frame_horizontally->setIcon(global::getStyledIcon("new_frame_hor"));
    ui->actionAdd_vertical_plot_frame->setIcon(global::getStyledIcon("new_frame_ver"));
    ui->actionSecondary_processing->setIcon(global::getStyledIcon("function"));
    ui->actionFast_secondary_processing->setIcon(global::getStyledIcon("window_function"));
    ui->actionDraw_level->setIcon(global::getStyledIcon("level"));
    ui->actionArrow_with_text->setIcon(global::getStyledIcon("arrowedText"));

    ui->actionSeparate_graphs->setIcon(global::getStyledIcon("separate"));
    ui->actionCombine_graphs->setIcon(global::getStyledIcon("combine"));

    ui->actionIntellectual_layout->setIcon(global::getStyledIcon("idea"));
    ui->actionNormal_mode->setIcon(global::getStyledIcon("cursor"));

    ui->actionPrevious_scope->setIcon(global::getStyledIcon("undo"));
    ui->actionNext_scope->setIcon(global::getStyledIcon("redo"));
    ui->actionAuto_units->setIcon(global::getStyledIcon("weight"));
    ui->actionMeasuring->setIcon(global::getStyledIcon("measure"));

    ui->actionShow_value_table->setIcon(global::getStyledIcon("table"));
    ui->actionFull_screen->setIcon(global::getStyledIcon("full_window"));

    ui->actionPreferences->setIcon(global::getStyledIcon("settings"));
    mNewPlotButton->setIcon(global::getStyledIcon("new_frame"));

    ui->actionZoom_X->setIcon(global::getStyledIcon("zoom_horizontal"));
    ui->actionZoom_Y->setIcon(global::getStyledIcon("zoom_vertical"));

    ui->actionZoom_XY->setIcon(global::getStyledIcon("zoom_xy"));
    ui->actionAutoscale->setIcon(global::getStyledIcon("autoscale"));

    ui->actionMotion->setIcon(global::getStyledIcon("hand"));
    ui->actionTips->setIcon(global::getStyledIcon("help"));
}




void JagMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
//    qDebug() << "dragEnterEvent";
    if (event->mimeData()->hasFormat("text/uri-list")) {
//        qDebug() << "Accepted as uri";
        event->accept();
    }
//    if (event->mimeData()->hasFormat("text/plain")) {
//        qDebug() << "Accepted";
//        event->accept();
//    }
}

void JagMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
//    qDebug() << "dropEvent";
//    qDebug() << mimeData->text();

    if (mimeData->hasFormat("text/uri-list")) {
        QList<QUrl> urlList = mimeData->urls();
        foreach (QUrl url, urlList) {
            QString fileName = url.toLocalFile();
            qDebug() << "file name to load" << fileName;
            addDataSource(fileName);
        }
    }

    QMainWindow::dropEvent(event);
}

void JagMainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    //saving window sizes
    global::gPreviousSessionWidth  = width();
    global::gPreviousSessionHeight = height();
    global::gPreviousSessionXCoord = x();
    global::gPreviousSessionYCoord = y();

    //controlWidgets
    ::saveInSettings(settings, "/Settings/controlWidgets/MultiModelControlWidget/sTabVisibilityVector", MultiModelControlWidget::sTabVisibilityVector);
    ::saveInSettings(settings, "/Settings/controlWidgets/MultiModelControlWidget/sTabNamesVector"     , MultiModelControlWidget::sTabNamesVector);
    ::saveInSettings(settings, "/Settings/controlWidgets/PlotControlWidget/sTabVisibilityVector"      , PlotControlWidget::sTabVisibilityVector);
    ::saveInSettings(settings, "/Settings/controlWidgets/PlotControlWidget/sTabNamesVector"           , PlotControlWidget::sTabNamesVector);

    global::saveGlobalSettingsOnApplicClose(settings);

    //checking for unfinished threads
    for (DataSource* dataSource : mDataSourceVector) {
        if (!dataSource->isFinished()) {
            qDebug() << "Termination of an unfinished thread";
            dataSource->terminate();
            dataSource->wait();
        }
    }

    QMainWindow::closeEvent(event);
}

void JagMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->tabWidget->count() == 0) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Up) {
        mPlotVector[ui->tabWidget->currentIndex()]->moveCurrentObject(AxisModel::AxisMovement::Up);
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Down) {
        mPlotVector[ui->tabWidget->currentIndex()]->moveCurrentObject(AxisModel::AxisMovement::Down);
    } else if ( event->key() == Qt::Key_Minus) {
        mPlotVector[ui->tabWidget->currentIndex()]->stretchCurrentAxis(0.85);
    } else if ( event->key() == Qt::Key_Equal) {
        mPlotVector[ui->tabWidget->currentIndex()]->stretchCurrentAxis(1./0.85);
    } else if ( event->key() == Qt::Key_R) {
        mPlotVector[ui->tabWidget->currentIndex()]->deleteCurrentObject();
    } else if ( event->key() == Qt::Key_W) {
        if (event->modifiers() == Qt::ShiftModifier)
            mPlotVector[ui->tabWidget->currentIndex()]->increaseWidthOfCurrentGraph(1/1.15);
        else
            mPlotVector[ui->tabWidget->currentIndex()]->increaseWidthOfCurrentGraph(1.15);
    } else if ( event->key() == Qt::Key_M) {
        if (event->modifiers() == Qt::ShiftModifier)
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineScatterOfCurrentGraph(-1);
        else
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineScatterOfCurrentGraph(1);
    } else if ( event->key() == Qt::Key_C) {
        if (event->modifiers() == Qt::ShiftModifier)
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineColorOfCurrentGraph(-1);
        else
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineColorOfCurrentGraph(1);
    } else if ( event->key() == Qt::Key_I) {
        if (event->modifiers() == Qt::ShiftModifier)
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineInterpolationOfCurrentGraph(-1);
        else
            mPlotVector[ui->tabWidget->currentIndex()]->switchLineInterpolationOfCurrentGraph(1);
    } else if ( event->key() == Qt::Key_D) {
        if (event->modifiers() == Qt::ShiftModifier)
            mPlotVector[ui->tabWidget->currentIndex()]->increaseDecimationOfCurrentGraph(1./2.);
        else
            mPlotVector[ui->tabWidget->currentIndex()]->increaseDecimationOfCurrentGraph(2.);
    } else if (event->key() == Qt::Key_1) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(0);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(0);
        }
    } else if (event->key() == Qt::Key_2) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(1);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(1);
        }
    } else if (event->key() == Qt::Key_3) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(2);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(2);
        }
    } else if (event->key() == Qt::Key_4) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(3);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(3);
        }
    } else if (event->key() == Qt::Key_5) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(4);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(4);
        }
    } else if (event->key() == Qt::Key_6) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(5);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(5);
        }
    } else if (event->key() == Qt::Key_7) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(6);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(6);
        }
    } else if (event->key() == Qt::Key_8) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(7);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(7);
        }
    } else if (event->key() == Qt::Key_9) {
        if (ui->tabWidget->count() >= 1) {
            if (event->modifiers() == Qt::AltModifier)
               mPlotVector[ui->tabWidget->currentIndex()]->activateAxis(8);
            else
               mPlotVector[ui->tabWidget->currentIndex()]->activateGraph(8);
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void JagMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    emit widthChanged(width());
}

void JagMainWindow::separateGraphs()
{
    if (ui->tabWidget->count() >= 1) {
       mPlotVector[ui->tabWidget->currentIndex()]->separateGraphs();
    }
}

void JagMainWindow::composeIntellectually()
{
    if (ui->tabWidget->count() >= 1) {
       mPlotVector[ui->tabWidget->currentIndex()]->composeIntellectually();
    }
}

void JagMainWindow::saveAs()
{
    if (ui->tabWidget->count() >= 1) {
//        using global::gDefaultDirectory;
        static QString directoryName = global::gDefaultDirectory;

        QDir defaultDirectory(directoryName);
        bool dirExists = defaultDirectory.exists ();
        if (dirExists == false)
            dirExists = defaultDirectory.mkpath(directoryName);

        if (dirExists) {

            //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//#ifdef __linux
            QFileDialog::Option option =  QFileDialog::DontUseNativeDialog;
//#else
//            QFileDialog::Option option = 0x0;
//#endif
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                        directoryName.endsWith("/") || directoryName.endsWith("\\") ?
                                        directoryName + "untitled.png" : directoryName + "/untitled.png" ,
                                        tr("Images (*.png *.bmp *.jpg)"), NULL,  option);

            if (fileName.endsWith(".png", Qt::CaseInsensitive)) {
                mPlotVector[ui->tabWidget->currentIndex()]->savePng(fileName);
                mStatusMessageLabel->setMessage("<h3><font color=green> Plot was saved as " + fileName  + "</font></h3>", 3000);
            } else if (fileName.endsWith(".bmp", Qt::CaseInsensitive)) {
                mPlotVector[ui->tabWidget->currentIndex()]->saveBmp(fileName);
                mStatusMessageLabel->setMessage("<h3><font color=green> Plot was saved as " + fileName  + "</font></h3>", 3000);
            } else if (fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
                mPlotVector[ui->tabWidget->currentIndex()]->saveJpg(fileName);
                mStatusMessageLabel->setMessage("<h3><font color=green> Plot was saved as " + fileName  + "</font></h3>", 3000);
            } else {
                mStatusMessageLabel->setMessage("<h3><font color=red> Can't save plot as " + fileName  + "</font></h3>", 3000);
            }

            QFileInfo file(fileName);
            if (file.exists()) {
                directoryName = file.absolutePath();
            }
        }

    }
}

void JagMainWindow::quickSave()
{

    if (ui->tabWidget->count() >= 1) {
        using global::gDefaultDirectory;
        using global::gApplicationStartTime;
        static int quickSavesPlotNumber = 0;

        static QString quickSaveDirectoryPath
                = gDefaultDirectory.endsWith("/") || gDefaultDirectory.endsWith("\\") ?
                    gDefaultDirectory + gApplicationStartTime + "/" : gDefaultDirectory + "/" + gApplicationStartTime + "/";

        QDir quickSaveDirectory(quickSaveDirectoryPath);
        bool dirExists = quickSaveDirectory.exists ();
        if (dirExists == false)
            dirExists = quickSaveDirectory.mkpath(quickSaveDirectoryPath);

        if (dirExists) {
            QString fileName = quickSaveDirectoryPath + "plot_" + QString::number(quickSavesPlotNumber) + ".png" ;

            mPlotVector[ui->tabWidget->currentIndex()]->savePng(fileName);
            quickSavesPlotNumber++;
            mStatusMessageLabel->setMessage("<h3><font color=blue> Plot was saved as " + fileName  + "</font></h3>", 3000);

        }

    }
}

void JagMainWindow::copyToClipBoard()
{
//    QClipboard* pcb = QApplication::clipboard();
    if (ui->tabWidget->count() >= 1) {
       QApplication::clipboard()->setPixmap(mPlotVector[ui->tabWidget->currentIndex()]->toPixmap());
       mStatusMessageLabel->setMessage("<h3><font color=blue> Plot was saved to the clipboard</font></h3>", 3000);

    }
}

void JagMainWindow::print()
{
    if (ui->tabWidget->count() >= 1) {
        QPrinter printer;
        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec() == QDialog::Accepted) {
//            QPainter painter(&printer);
//            QPixmap picture = mPlotVector[ui->tabWidget->currentIndex()]->toPixmap();
//            double ratio = std::max(
//                                    double(picture.height()) / printer.height(),
//                                    double(picture.width()) / printer.width()
//                                    );
//            picture = mPlotVector[ui->tabWidget->currentIndex()]->toPixmap(picture.width()/ratio, picture.height()/ratio);
//            painter.drawPixmap(0, 0, picture, 0, 0, picture.width(), picture.height());

            QCPPainter painter(&printer);
//            mPlotVector[ui->tabWidget->currentIndex()]->toPainter(&painter);

            QPixmap picture = mPlotVector[ui->tabWidget->currentIndex()]->toPixmap();
            double ratio = std::max(
                                    double(picture.height()) / printer.height(),
                                    double(picture.width()) / printer.width()
                                    );

            mPlotVector[ui->tabWidget->currentIndex()]->toPainter(&painter, picture.width()/ratio, picture.height()/ratio);
//            picture = mPlotVector[ui->tabWidget->currentIndex()]->toPixmap(picture.width()/ratio, picture.height()/ratio);
//            painter.drawPixmap(0, 0, picture, 0, 0, picture.width(), picture.height());


        }
    }
}





