#ifndef DATACHOISEDIALOG_H
#define DATACHOISEDIALOG_H

#include <QDialog>
#include "datasource/datasource.h"
#include "datasource/datachoicewidget.h"
#include "abstractdatasourceadapter.h"
#include "datasource/datachoicewidgetwrapper.h"
#include "core/graphcontainer.h"

namespace Ui {
class DataChoiseDialog;
}

class DataChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataChoiceDialog(GraphContainer* plotShowEditWidget, QVector</*DataSource*/AbstractDataSourceAdapter*> dataSourceVector, QWidget *parent = 0);
    ~DataChoiceDialog();

//    virtual void keyPressEvent ( QKeyEvent * event );


signals:


public slots:
    virtual void accept();
    void favouriteButtonPushed();
    void hateButtonPushed();
    void nextButtonPushed();
    void prevButtonPushed();
    void eraseAllButtonPushed();
    void showHelpInformation();
    void activateItemByRegExString(QString);
    void setAutoParametersFiltrationDuringSearch(bool);
    void updateCheckedItemsLists();
    void updateBufferLists();
    void treatExtraWidgetsAccessibilityChanging(bool);
    void treatDeletionTimerEnablingChanging(bool);
    void treatDescriptionSearchChanging(bool);
    void treatSearchStringClearingAfterItemCheckingChanging(bool);
    void treatDialogConfigEnablingChanging(bool);
    void updateExtraWidgetsState();
    void findStringTimerStart();

    void loadSavedParameters();
    void saveParameterList();
    void deleteParameterList();

//    void deleteClickedItemFromCheckedParameters(QTreeWidgetItem * item, int column);
    void treatItemClickedInCheckedParameters(QTreeWidgetItem * item, int column);



protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void timerEvent(QTimerEvent * event);

private slots:
    void setCurrentCheckedItem();

private:
    Ui::DataChoiseDialog *ui;
//    QVector<DataChoiseWidget*> mDataChoiseWidget;
    QVector<DataChoiceWidgetWrapper*> mDataChoiseWidget;
    GraphContainer* mPlotShowEditWidget;

    int mFindTimerId;
    const int cTimerDevisor = 100;
    int mTimerCounter;

    int mCheckedItemsFirstColWidth;

    QVector<QVector<QTreeWidgetItem*>> mCheckParametersItemVector;  //0 - element(may be empty) corresponds checked item of 0-th data source() /*!< TODO */

    void initializeSavedVariablesComboBoxWithTips();
};

#endif // DATACHOISEDIALOG_H
