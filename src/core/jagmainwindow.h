#ifndef JAGMAINWINDOW_H
#define JAGMAINWINDOW_H

#include <QMainWindow>
#include "datasource/datasource.h"
#include "commonWidgets/plotshoweditwidget.h"
#include "plotStyle/plotstyle.h"
#include "commonWidgets/preferencesdialog.h"
#include <QTcpServer>
#include "basic/messagelabel.h"
#include "event/timeevent.h"
#include "anchor/dockanchorviewwidget.h"
#include "datasource/datasourcecontroldialog.h"


class EventSource;
class DummyWidget;

class MessageLabel;
namespace Ui {
class JagMainWindow;
}

/*!
 \brief JagMainWindow is  a  JaG main window class.

 JagMainWindow contains menubar, toolbar, statusbar and tab widget with PlotShowEditWidget objects that contains plots and control widgets.
 JagMainWindow directs all user requests for adding new graphs to an existing plot, mode changes, plot savings etc. to the corresponding PlotShowEditWidget.
 Also JagMainWindow is a class that actually creates DataSource objects, owns them and deletes them if necessary. Also JagMainWindow loads
 all user settings and save them.

*/
class JagMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JagMainWindow(QWidget *parent = 0);
    ~JagMainWindow();

    int numberOfDataSources() const;


    QMap<QString, QAction*> actions() const;
    QMap<QString, bool> visibleActionsMap() const;
    void setActionsVisibilityMap(QMap<QString, bool>);

//    PlotStyle preferences() const;

public slots:
    void addNewPlot();
    void addPlotFrameHorizontally();
    void addPlotFrameVertically();
    void clonePlotFrameHorizontally();
    void clonePlotFrameVertically();
    void addNewPlotFrame(Qt::Orientation orient = Qt::Horizontal);
    void cloneCurrentPlotFrame(Qt::Orientation orient = Qt::Horizontal);
    void addNewGraph();
    void addNewEvent();

    void addDataSource();
    void addDataSource(DataSource*);  //temporaly measure later should be deleted. JaGMainWindow should create and delete DataSource by itself
    void removeDataSource(DataSource*);
    void addDataSource(QString fileName);

    void editDataSources();
    void saveGlobalEventSourceAs();
    void saveDataInTable();

    void treatTabCloseRequest(int tabIndex);
    void treatTabMotionRequest(int from, int to);
    void treatCurrentTabChanged(int newTab);
    void setCurrentPlotWidgetForAnchorViewWidget();

    void combineGraphs();
    void separateGraphs();
    void composeIntellectually();

    void saveAs();
    void quickSave();
    void copyToClipBoard();
    void print();

    void setZoomX();
    void setZoomY();
    void setZoomXY();
    void setArrowDrawingMode();
    void setLevelDrawing();
    void setNormalMode();
    void setMeasuring();
    void setMotionMode();
    void setFastSecondaryProcessingMode();
    void setStatesChecked(bool);
    void prevScope();
    void nextScope();

    void showMeasuringTable(bool);

    void autoscale();

    void secondaryProcessing();
    void setAutoUnits();
    void copyNamesToInternalClipboard();
    void toogleFullScreen();

    void setPreferences();

    void aboutQt();
    void aboutJag();
    void showHelpInformation();

    void loadSettings();
    void saveSettings();

    void showInfoString(QString);


    void slotNewConnection();
    void slotReadClient();
    void treatSocketDisconnection();

    void treatGlobalDataSourceChanging();
    void treatGlobalEventSourceChanging();

    void treatAnchorViewWidgetInvisibility();

    void treatErrorDuringDataSourceLoading(QString errorMessage);
    void treatNonCriticalErrorDuringDataSourceLoading();

    void show();

    void setPlotEditingEnabled();

    void setToolbarIcons();

    friend class DummyWidget;
signals:
    void newDataSourceCreated(DataSource*);
    void widthChanged(int);

protected:
    void virtual dragEnterEvent(QDragEnterEvent* event);
    void virtual dropEvent(QDropEvent * event);

    virtual void closeEvent(QCloseEvent * event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::JagMainWindow *ui;
    QVector<DataSource*> mDataSourceVector;
    QVector<EventSource*> mEventSourceVector;
    QVector<PlotShowEditWidget*> mPlotVector;

//    PlotStyle mPreferences;

    PreferencesDialog *mPreferencesDialog;

    QTcpServer *mListeningServer;
//    QTcpSocket *mClientSocket;
    QVector<QTcpSocket *> mClientSocketVector;
    MessageLabel *mStatusMessageLabel;
    DockAnchorViewWidget *mDockAnchorViewWidget;
    QToolButton * mNewPlotButton;

    bool mIsMeasuringValueTableVisible;

    DataSourceControlDialog *mDataSourceEditingDialog;
    QString mDefaultDataSourceDir;

    QMap<QString, bool> mToolbarVisibleActionsMap;

};

#endif // JAGMAINWINDOW_H
