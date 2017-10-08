#include "datachoicedialog.h"
#include "ui_datachoicedialog.h"
#include <QSplashScreen>
#include "global/global_definitions.h"
#include <QMessageBox>
#include "commonWidgets/tipdialog.h"
#include <QStringList>
#include "datasource/datasourceadapter.h"
#include <QInputDialog>
#include <QToolTip>
#include "commonWidgets/comboboxwithtips.h"

using namespace global;

DataChoiceDialog::DataChoiceDialog(GraphContainer* plotShowEditWidget, QVector<AbstractDataSourceAdapter/*DataSource*/*> dataSourceVector, QWidget *parent) :
    QDialog(parent),  ui(new Ui::DataChoiseDialog),
    mPlotShowEditWidget(plotShowEditWidget), mFindTimerId(0), mTimerCounter(0), mCheckedItemsFirstColWidth(-1)
{
    ui->setupUi(this);
    ui->erasingTimerProgressBar->setValue(100);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->autoDataFilteringCheckBox->setChecked(gAutoParametersFilteringDuringSearchIsEnabled);
    ui->deletionTimerCheckBox->setChecked(gFindStringErasingTimerEnabled);
    ui->descriptionSearchCheckBox->setChecked(gSearchByDescriptionIsEnabled);

    foreach (auto dataSource, dataSourceVector) {
//        DataChoiseWidget* dataChoiceWidget = new DataChoiseWidget(this);
        DataChoiceWidgetWrapper* dataChoiceWidget = new DataChoiceWidgetWrapper(this);
//        DataChoiseWidgetDecorator* dataChoiceWidget = new DataChoiseWidgetDecorator(this);

        //temp
//        dataChoiceWidget->setDataSource(dataSource);
        //temp


//        DataSourceAdapter *dataSourceAdapter = new DataSourceAdapter();
//        dataSourceAdapter->setDataSource(dataSource);
        dataChoiceWidget->setDataSource(dataSource);


        dataChoiceWidget->setShowEditWidget(mPlotShowEditWidget);
        mDataChoiseWidget.push_back(dataChoiceWidget);
//        ui->tabWidget->addTab(dataChoiceWidget, getFileNameFromPath(dataSource->getDataSourceName()));
        ui->tabWidget->addTab(dataChoiceWidget, dataSource->getDataSourceAlias());

//        connect(ui->findLineEdit, SIGNAL(textChanged(QString)),
//                dataChoiceWidget, SLOT(activateItemByRegExString(QString)));
        connect(dataChoiceWidget, SIGNAL(checkedItemsListChanged()),
             this, SLOT(updateCheckedItemsLists()));

    }

//    ui->savedVariablesComboBox->addItems(gPreviouslySavedParametersNames);
    initializeSavedVariablesComboBoxWithTips();


    connect(ui->findLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(activateItemByRegExString(QString)));
    connect(ui->findLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(findStringTimerStart()));

    connect(ui->autoDataFilteringCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(setAutoParametersFiltrationDuringSearch(bool)));
    connect(ui->showExtraInfoCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(treatExtraWidgetsAccessibilityChanging(bool)));
    connect(ui->deletionTimerCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(treatDeletionTimerEnablingChanging(bool)));
    connect(ui->descriptionSearchCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(treatDescriptionSearchChanging(bool)));
    connect(ui->clearSearchPatternAfterCheckedItemsChangingCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(treatSearchStringClearingAfterItemCheckingChanging(bool)));
    connect(ui->dialogConfigCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(treatDialogConfigEnablingChanging(bool)));


    connect(ui->favouritePushButton, SIGNAL(clicked()),
            this, SLOT(favouriteButtonPushed()));
    connect(ui->hatePushButton, SIGNAL(clicked()),
            this, SLOT(hateButtonPushed()));
    connect(ui->nextPushButton, SIGNAL(clicked()),
            this, SLOT(nextButtonPushed()));
    connect(ui->prevPushButton, SIGNAL(clicked()),
            this, SLOT(prevButtonPushed()));
    connect(ui->eraseAllPushButton, SIGNAL(clicked()),
            this, SLOT(eraseAllButtonPushed()));
    connect(ui->helpPushButton, SIGNAL(clicked()),
            this, SLOT(showHelpInformation()));

    connect(ui->loadPushButton, SIGNAL(clicked()),
            this, SLOT(loadSavedParameters()));
    connect(ui->savePushButton, SIGNAL(clicked()),
            this, SLOT(saveParameterList()));
    connect(ui->deletePushButton, SIGNAL(clicked()),
            this, SLOT(deleteParameterList()));


//    connect(ui->checkedItemsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
//            this, SLOT(deleteClickedItemFromCheckedParameters(QTreeWidgetItem*,int)));
    connect(ui->checkedItemsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(treatItemClickedInCheckedParameters(QTreeWidgetItem*,int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(setCurrentCheckedItem()));


    if (gDataDialogWidth > 0 && gDataDialogHeight > 0) {
        resize(gDataDialogWidth, gDataDialogHeight);
    }

    ui->showExtraInfoCheckBox->setCheckState(gAreExtraWidgetsVisibleInDataChoiseDialog ? Qt::Checked : Qt::Unchecked );
    ui->clearSearchPatternAfterCheckedItemsChangingCheckBox->setChecked(global::gSearchStringClearingAfterCheckedParamsListChangingEnabled);
    ui->dialogConfigCheckBox->setChecked(global::gDataChoiceDialogConfigsEnabled);
    treatDialogConfigEnablingChanging(global::gDataChoiceDialogConfigsEnabled);
    updateExtraWidgetsState();
//    updateCheckedItemsLists();
//    updateBufferLists();


    ui->findLineEdit->setFocus();

    ui->checkedItemsTreeWidget->setFocusProxy(this);
    ui->savedVariablesComboBox->setFocusProxy(this);
    ui->bufferTreeWidget->setFocusProxy(this);

    ui->erasingTimerProgressBar->setValue(0);
}

DataChoiceDialog::~DataChoiceDialog()
{
    global::gDataDialogWidth = width();
    global::gDataDialogHeight = height();
    global::gAutoParametersFilteringDuringSearchPattern = ".*";
    delete ui;
}

void DataChoiceDialog::accept()
{
    hide();
    foreach (auto dataChoiceWidget, mDataChoiseWidget) {
        dataChoiceWidget->addCheckedDataToShowEditWidget();
    }
    QDialog::accept();
//    gAutoParametersFilteringDuringSearchPattern = ".*";
}

void DataChoiceDialog::favouriteButtonPushed()
{
    ui->findLineEdit->setFocus();
    foreach (auto dataChoiceWidget, mDataChoiseWidget) {
        dataChoiceWidget->setFavouriteSearchEnabled(ui->favouritePushButton->isChecked() ? true : false);
    }
}

void DataChoiceDialog::hateButtonPushed()
{
    ui->findLineEdit->setFocus();
    foreach (auto dataChoiceWidget, mDataChoiseWidget) {
        dataChoiceWidget->setHateSearchEnabled(ui->hatePushButton->isChecked() ? true : false);
    }
}

void DataChoiceDialog::nextButtonPushed()
{
    ui->findLineEdit->setFocus();
    if (ui->tabWidget->count() >= 1) {
       mDataChoiseWidget[ui->tabWidget->currentIndex()]->nextSearch(ui->findLineEdit->text());
    }
}

void DataChoiceDialog::prevButtonPushed()
{
    ui->findLineEdit->setFocus();
    if (ui->tabWidget->count() >= 1) {
       mDataChoiseWidget[ui->tabWidget->currentIndex()]->prevSearch(ui->findLineEdit->text());
    }
}

void DataChoiceDialog::eraseAllButtonPushed()
{
    ui->findLineEdit->setFocus();
    ui->findLineEdit->clear();
}

void DataChoiceDialog::showHelpInformation()
{
    ui->findLineEdit->setFocus();
//    QMessageBox::information(this, "Useful shortcuts",
//                             "Ctrl-n         -  next match\n"
//                             "Ctrl-p         -  previous match\n"
//                             "Ctrl-u, Esc    -  clear search string\n"
//                             "Ctrl-h         -  delete previous symbol in search string\n"
//                             "Ctrl-Tab       -  next data source\n"
//                             "Ctrl-l         -  enable loved parameters filtering\n"
//                             "Ctrl-Shift-l   -  enable hated parameters filtering\n"
//                             "Ctrl-Enter, Ctrl-Space     -  check current item\n"
//                             "Ctrl-C     -  copy names of the checked parameters to the clipboard\n"
//                             "Ctrl-V     -  check parameters with names from the clipboard\n"
//                             "Ctrl-Z     -  clear all checkings\n"
//                             );

    TipDialog tipDialog;
    QStringList tipsList;
    tipsList <<    "Useful shortcuts:<br><br> "
                   "Ctrl-n | Down  -  next match<br>"
                   "Ctrl-p | Up    -  previous match<br>"
                   "Ctrl-u, Esc    -  clear search string<br>"
                   "Ctrl-h         -  delete previous symbol in search string<br>"
                   "Ctrl-Tab       -  next data source<br>"
                   "Ctrl-l         -  enable loved parameters filtering<br>"
                   "Ctrl-Shift-l   -  enable hated parameters filtering<br>"
                   "Ctrl-Enter, Ctrl-Space     -  check current item<br>"
                   "Ctrl-C     -  copy names of the checked parameters to the clipboard<br>"
                   "Ctrl-V     -  check parameters with names from the clipboard<br>"
                   "Ctrl-Z     -  clear all checkings<br>"
                   "Ctrl-F     -  begin new generalized search (erase search string and paste \".*\" into the search string<br>"
                   "Ctrl-A     -  switch autofiltering mode<br>"
                   "Ctrl-T     -  turn on/off deletion timer<br>"
              << "You can quickly delete checked parameter from checked list by clicking on it in widget for checked parameters.";

    tipDialog.setTips(tipsList);
    tipDialog.exec();
}

void DataChoiceDialog::activateItemByRegExString(QString newPattern)
{
    global::gAutoParametersFilteringDuringSearchPattern = newPattern + ".*";
    foreach (auto dataChoiceWidget, mDataChoiseWidget) {
        dataChoiceWidget->activateItemByRegExString(newPattern);
    }
}

void DataChoiceDialog::setAutoParametersFiltrationDuringSearch(bool newValue)
{
    if (gAutoParametersFilteringDuringSearchIsEnabled != newValue) {
        gAutoParametersFilteringDuringSearchIsEnabled = newValue;
        if (gAutoParametersFilteringDuringSearchIsEnabled)
            gAutoParametersFilteringDuringSearchPattern = ui->findLineEdit->text() + ".*";
        foreach (auto dataChoiceWidget, mDataChoiseWidget) {
            dataChoiceWidget->updateData();
        }
        activateItemByRegExString(ui->findLineEdit->text());
    }
}

void DataChoiceDialog::updateCheckedItemsLists()
{
//    QVector<QVector<QTreeWidgetItem*>> mCheckParametersItemVector;
    mCheckParametersItemVector.clear();

    ui->checkedItemsTreeWidget->clear();
    if (gAreExtraWidgetsVisibleInDataChoiseDialog) {

//        ui->checkedItemsTreeWidget->setHeaderLabel(tr("Checked parameters"));
        ui->checkedItemsTreeWidget->setHeaderLabels(QStringList {tr("Checked parameters"), "Arguments"});

        QFontMetrics metrics(qApp->font());
        mCheckedItemsFirstColWidth = metrics.width(tr("Checked parameters"));

        foreach (auto dataChoiceWidget, mDataChoiseWidget) {
            mCheckParametersItemVector.push_back(QVector<QTreeWidgetItem*>());
            QVector<QString> checkedItems = dataChoiceWidget->getCheckedItemsNames();
            QVector<QString> checkedItemsArguments = dataChoiceWidget->getCheckedItemsArgumentsNames();
            if (checkedItems.size() != 0) {
                QTreeWidgetItem *dataSourceItem = new QTreeWidgetItem();
//                QString dataSourceName = dataChoiceWidget->dataSource()->getDataSourceName();
//                dataSourceItem->setText(0, dataSourceName);
                QString dataSourceAlias = dataChoiceWidget->dataSource()->getDataSourceAlias();
                dataSourceItem->setText(0, dataSourceAlias);
                ui->checkedItemsTreeWidget->addTopLevelItem(dataSourceItem);
//                for (auto parameterName : checkedItems) {
//                    QTreeWidgetItem *checkedParameterItem = new QTreeWidgetItem(dataSourceItem, QStringList(parameterName));
//                    mCheckParametersItemVector.back().push_back(checkedParameterItem);
//                }
                for (int i = 0; i < checkedItems.size(); ++i) {
                    QStringList list;
                    list << checkedItems[i];
                    list << (checkedItemsArguments[i] == "" ? "std" : checkedItemsArguments[i]);
                    mCheckedItemsFirstColWidth = std::max(mCheckedItemsFirstColWidth, metrics.width(checkedItems[i]) + 60);
                    QTreeWidgetItem *checkedParameterItem = new QTreeWidgetItem(dataSourceItem, list);
                    mCheckParametersItemVector.back().push_back(checkedParameterItem);
                }
            }
        }
        ui->checkedItemsTreeWidget->expandAll();
        mCheckedItemsFirstColWidth += 15;
        ui->checkedItemsTreeWidget->header()->resizeSection(0, mCheckedItemsFirstColWidth);

        setCurrentCheckedItem();
    }

    if (global::gSearchStringClearingAfterCheckedParamsListChangingEnabled) {
        eraseAllButtonPushed();
    }
}

void DataChoiceDialog::updateBufferLists()
{
    ui->bufferTreeWidget->clear();
    if (gAreExtraWidgetsVisibleInDataChoiseDialog) {
        int maxParameterNameWidth = std::accumulate(gParametersSearchClipboard.begin(), gParametersSearchClipboard.end(),
                                                    QString(tr("Parameters in clipboard")).size(), [](const int &max, const QString& name) { return std::max(max, name.size());});
        maxParameterNameWidth += 5;

//        ui->bufferTreeWidget->setHeaderLabel(tr("Internal clipboard"));
        ui->bufferTreeWidget->setHeaderLabels(QStringList{tr("Parameters in clipboard"), tr("Arguments")});
//        foreach (auto parameterName, gParametersSearchClipboard) {
//                QTreeWidgetItem *dataSourceItem = new QTreeWidgetItem();
//                dataSourceItem->setText(0, parameterName);
//                ui->bufferTreeWidget->addTopLevelItem(dataSourceItem);
//        }

        for (int i = 0; i < gParametersSearchClipboard.size(); ++i) {
            QString parameterName = gParametersSearchClipboard[i];
            QTreeWidgetItem *dataSourceItem = new QTreeWidgetItem();
            dataSourceItem->setText(0, parameterName);
            if (i < gParametersSearchArgumentsClipboard.size())
                dataSourceItem->setText(1, gParametersSearchArgumentsClipboard[i]);
            ui->bufferTreeWidget->addTopLevelItem(dataSourceItem);
        }

        ui->bufferTreeWidget->header()->resizeSection(0, QFontMetrics(qApp->font()).width(QString(maxParameterNameWidth, 'x')));
    }
}

void DataChoiceDialog::treatExtraWidgetsAccessibilityChanging(bool accessibility)
{
    if (gAreExtraWidgetsVisibleInDataChoiseDialog != accessibility) {
        gAreExtraWidgetsVisibleInDataChoiseDialog = accessibility;
        updateExtraWidgetsState();
    }
}

void DataChoiceDialog::treatDeletionTimerEnablingChanging(bool enabled)
{

    if (gFindStringErasingTimerEnabled != enabled) {
        gFindStringErasingTimerEnabled = enabled;
        if (mFindTimerId)
            killTimer(mFindTimerId);

        findStringTimerStart();
    }
}

void DataChoiceDialog::treatDescriptionSearchChanging(bool newValue)
{
    if (gSearchByDescriptionIsEnabled != newValue) {
        gSearchByDescriptionIsEnabled = newValue;
        foreach (auto dataChoiceWidget, mDataChoiseWidget) {
            dataChoiceWidget->updateData();
        }
        activateItemByRegExString(ui->findLineEdit->text());
    }
}

void DataChoiceDialog::treatSearchStringClearingAfterItemCheckingChanging(bool newValue)
{
    if (gSearchStringClearingAfterCheckedParamsListChangingEnabled != newValue) {
        gSearchStringClearingAfterCheckedParamsListChangingEnabled = newValue;
    }
}

void DataChoiceDialog::treatDialogConfigEnablingChanging(bool enabled)
{
    global::gDataChoiceDialogConfigsEnabled = enabled;
    if (ui->dialogConfigCheckBox->isChecked() != enabled) {
        ui->dialogConfigCheckBox->setChecked(enabled);
    }
    if (enabled)
        ui->dialogConfigGroupBox->show();
    else
        ui->dialogConfigGroupBox->hide();
}

void DataChoiceDialog::updateExtraWidgetsState()
{
    if (gAreExtraWidgetsVisibleInDataChoiseDialog) {
        ui->bufferTreeWidget->show();
        updateBufferLists();
        ui->checkedItemsTreeWidget->show();
        updateCheckedItemsLists();
    } else {
        ui->bufferTreeWidget->hide();
        ui->checkedItemsTreeWidget->hide();
    }
}

void DataChoiceDialog::findStringTimerStart()
{


    if (gFindStringErasingTimerEnabled) {
        if (ui->findLineEdit->text() != ""
                && ui->findLineEdit->text() != ".*") {
            if (mFindTimerId)
                killTimer(mFindTimerId);
            mTimerCounter = 0;
            mFindTimerId = startTimer(gFindStringErasingTimerInterval*1000./cTimerDevisor);
            ui->erasingTimerProgressBar->setValue(100);
        }
    } else {
        mFindTimerId = 0;
        mTimerCounter = 0;
        ui->erasingTimerProgressBar->setValue(0);
    }
}

void DataChoiceDialog::loadSavedParameters()
{
    eraseAllButtonPushed();
    if (ui->tabWidget->count() >= 1) {
        if (ui->savedVariablesComboBox->count() > 0) {
            QVector<QString> bufVector;
            QVector<QString> bufArgsVector;
//            foreach (QString var , gPreviouslySavedParametersNames[ui->savedVariablesComboBox->currentText()]) {
//                bufVector.push_back(var);
//            }
            for (auto var : gPreviouslySavedParametersNames[ui->savedVariablesComboBox->currentText()].first) {
                bufVector.push_back(var);
            }
            for (auto var : gPreviouslySavedParametersNames[ui->savedVariablesComboBox->currentText()].second) {
                bufArgsVector.push_back(var);
            }
            mDataChoiseWidget[ui->tabWidget->currentIndex()]->checkItemsByName(bufVector, bufArgsVector);
            ui->savedVariablesComboBox->clearFocus();
            ui->loadPushButton->clearFocus();
        }
    }

}

void DataChoiceDialog::saveParameterList()
{

    QStringList checkedItemsList;
    QStringList checkedItemsArgsList;
    foreach (auto dataChoiceWidget, mDataChoiseWidget) {
        QVector<QString> checkedItems = dataChoiceWidget->getCheckedItemsNames();
        QVector<QString> checkedItemsArgs = dataChoiceWidget->getCheckedItemsArgumentsNames();
        foreach (auto var, checkedItems) {
            checkedItemsList << var;
        }
        foreach (auto var, checkedItemsArgs) {
            checkedItemsArgsList << var;
        }
    }
    if (checkedItemsList.size() != 0) {
        QString newVariableListName = QInputDialog::getText(this, "Choose parameter list name",
                                                     "Choose parameter list name");
        if (newVariableListName == "")
            return;
        if (gPreviouslySavedParametersNames.keys().indexOf(newVariableListName) != -1) {
            QMessageBox::warning(this, "Warning", "Parameter list with such name already exists. "
                                 "Can't save parameter list with such name.");
            return;
        }
        gPreviouslySavedParametersNames.insert(newVariableListName, std::make_pair(checkedItemsList, checkedItemsArgsList));
        ui->savedVariablesComboBox->clear();

//        ui->savedVariablesComboBox->addItems(gPreviouslySavedParametersNames);
        initializeSavedVariablesComboBoxWithTips();

        ui->savedVariablesComboBox->clearFocus();
        ui->savePushButton->clearFocus();
    }

}

void DataChoiceDialog::deleteParameterList()
{
    if (ui->savedVariablesComboBox->count() > 0) {
        gPreviouslySavedParametersNames.remove(ui->savedVariablesComboBox->currentText());
        ui->savedVariablesComboBox->clear();
//        ui->savedVariablesComboBox->addItems(gPreviouslySavedParametersNames.keys());
//        ui->savedVariablesComboBox->addItems(gPreviouslySavedParametersNames);
        initializeSavedVariablesComboBoxWithTips();
        ui->savedVariablesComboBox->clearFocus();
        ui->deletePushButton->clearFocus();
    }
}

//void DataChoiceDialog::deleteClickedItemFromCheckedParameters(QTreeWidgetItem *item, int /*column*/)
//{
//    for (int i = 0; i < mCheckParametersItemVector.size(); ++i) {
//        int index = mCheckParametersItemVector[i].indexOf(item);
//        if (index != -1) {
//            mDataChoiseWidget[i]->deleteParameterFromCheckList(index);
//        }
//    }
//}

void DataChoiceDialog::treatItemClickedInCheckedParameters(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (ui->checkedItemsTreeWidget->buttonReleaseXCoord() <= mCheckedItemsFirstColWidth) {
        for (int i = 0; i < mCheckParametersItemVector.size(); ++i) {
            int index = mCheckParametersItemVector[i].indexOf(item);
            if (index != -1) {
                mDataChoiseWidget[i]->deleteParameterFromCheckList(index);
                break;
            }
        }
    } else {
        for (int i = 0; i < mCheckParametersItemVector.size(); ++i) {
            int index = mCheckParametersItemVector[i].indexOf(item);
            if (index != -1 && i < ui->tabWidget->count()) {
                ui->tabWidget->setCurrentIndex(i);
                mDataChoiseWidget[i]->setCurrentParameterNumber(index);
                break;
            }
        }
    }
}



void DataChoiceDialog::keyPressEvent(QKeyEvent *event)
{

    if (event->modifiers() == Qt::NoModifier &&
            event->key() == Qt::Key_Return) {
            //push ok button
            ui->buttonBox->button(QDialogButtonBox::Ok)->click();
    } else if (event->modifiers() == Qt::NoModifier &&
         event->key() == Qt::Key_Up) {
            prevButtonPushed();
    } else if (event->modifiers() == Qt::NoModifier &&
         event->key() == Qt::Key_Down) {
            nextButtonPushed();
    } else if (event->modifiers() == Qt::NoModifier &&
              event->key() == Qt::Key_Left) {
                  ui->findLineEdit->cursorBackward(false, 1);
    } else if (event->modifiers() == Qt::NoModifier &&
               event->key() == Qt::Key_Right) {
                   ui->findLineEdit->cursorForward(false, 1);
    } else if (event->modifiers() == Qt::NoModifier &&
               event->key() == Qt::Key_Home) {
                   ui->findLineEdit->setCursorPosition(0);
    } else if (event->modifiers() == Qt::NoModifier &&
               event->key() == Qt::Key_End) {
                   ui->findLineEdit->setCursorPosition(ui->findLineEdit->text().size());
    } else if (event->modifiers() == Qt::ControlModifier &&
               event->key() == Qt::Key_Tab) {
                //switching data sources
               if (ui->tabWidget->count() >= 1) {
                   int newIndex = ui->tabWidget->currentIndex();
                   newIndex++;
                   if (newIndex == ui->tabWidget->count())
                       newIndex = 0;
                   ui->tabWidget->setCurrentIndex(newIndex);
               }
    } else if ((event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_H)
                || (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Backspace)){
            //delete previous symbol
            QString lineEditContent = ui->findLineEdit->text();
            int newCursorPosition = 0;

            QString selectedText = ui->findLineEdit->selectedText();
            if (selectedText.size() == 0) {
                int	cursorPosition = ui->findLineEdit->cursorPosition();
                if (lineEditContent.size() != 0 && (cursorPosition - 1 >= 0)) {
                    lineEditContent.remove(cursorPosition - 1, 1);
                }
                newCursorPosition = cursorPosition - 1;
            } else {
                int selectionStart = ui->findLineEdit->selectionStart();
                lineEditContent.remove(selectionStart, selectedText.size());
                newCursorPosition = selectionStart;
            }
            ui->findLineEdit->setText(lineEditContent);
            ui->findLineEdit->setFocus();
            ui->findLineEdit->setCursorPosition(newCursorPosition);

    } else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Delete){
           //delete next symbol
           QString lineEditContent = ui->findLineEdit->text();
           int newCursorPosition = 0;

           QString selectedText = ui->findLineEdit->selectedText();
           if (selectedText.size() == 0) {
               int	cursorPosition = ui->findLineEdit->cursorPosition();
               if (lineEditContent.size() != 0 && (cursorPosition < lineEditContent.size())) {
                   lineEditContent.remove(cursorPosition , 1);
               }
               newCursorPosition = cursorPosition;
           } else {
               int selectionStart = ui->findLineEdit->selectionStart();
               lineEditContent.remove(selectionStart, selectedText.size());
               newCursorPosition = selectionStart;
           }
           ui->findLineEdit->setText(lineEditContent);
           ui->findLineEdit->setFocus();
           ui->findLineEdit->setCursorPosition(newCursorPosition);
    } else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Escape){
                    //erase string completely
                  eraseAllButtonPushed();
    } else if (event->modifiers() == Qt::NoModifier ||
            event->modifiers() == Qt::ShiftModifier) {
                QString currentText = ui->findLineEdit->text();
                int	cursorPosition = ui->findLineEdit->cursorPosition();
                currentText.insert(cursorPosition, event->text());
//                ui->findLineEdit->setText(ui->findLineEdit->text() + event->text());
                ui->findLineEdit->setText(currentText);
                ui->findLineEdit->setFocus();
                ui->findLineEdit->setCursorPosition(cursorPosition + 1);
    } else if ((event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Return)
                || (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Space)){
                //checking current item
                if (ui->tabWidget->count() >= 1) {
                   mDataChoiseWidget[ui->tabWidget->currentIndex()]->checkCurrentItem();
                }
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C){
               //copy item names to internal "clipboard"
               if (ui->tabWidget->count() >= 1) {
                  gParametersSearchClipboard = mDataChoiseWidget[ui->tabWidget->currentIndex()]->getCheckedItemsNames();
                  gParametersSearchArgumentsClipboard= mDataChoiseWidget[ui->tabWidget->currentIndex()]->getCheckedItemsArgumentsNames();
                  updateBufferLists();
               }

    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V){
        //check items with names from "clipboard"
        if (ui->tabWidget->count() >= 1) {
           mDataChoiseWidget[ui->tabWidget->currentIndex()]->checkItemsByName(gParametersSearchClipboard, gParametersSearchArgumentsClipboard);
        }

    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z){
        //clearing all checkings
        if (ui->tabWidget->count() >= 1) {
            foreach (auto dataChoiceWidget, mDataChoiseWidget)
                dataChoiceWidget->clearAllCheckings();
        }

    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F){
        //began new search
        ui->findLineEdit->setText(".*");

    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_T){
        //turn on|off deletion timer
        ui->deletionTimerCheckBox->setChecked(!ui->deletionTimerCheckBox->isChecked());
        treatDeletionTimerEnablingChanging(ui->deletionTimerCheckBox->isChecked());

    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A){
        //switch autofiltering mode
        ui->autoDataFilteringCheckBox->setChecked(!ui->autoDataFilteringCheckBox->isChecked());
        setAutoParametersFiltrationDuringSearch(ui->autoDataFilteringCheckBox->isChecked());

    } else {
        QDialog::keyPressEvent(event);
    }
}

void DataChoiceDialog::timerEvent(QTimerEvent *event)
{

    if (mFindTimerId) {
        mTimerCounter++;
        if (mTimerCounter < cTimerDevisor) {
            ui->erasingTimerProgressBar->setValue(100.*(double)(cTimerDevisor - mTimerCounter)/cTimerDevisor);

        } else {
            killTimer(mFindTimerId);
            mTimerCounter = 0;
            mFindTimerId = 0;
            if (ui->findLineEdit->text().startsWith(".*"))
                ui->findLineEdit->setText(".*");
            else
                ui->findLineEdit->setText("");
            ui->erasingTimerProgressBar->setValue(0);
        }
    }
    QDialog::timerEvent(event);
}

void DataChoiceDialog::setCurrentCheckedItem()
{
    int curTabIndex = ui->tabWidget->currentIndex();
    if (curTabIndex < 0 || curTabIndex >= mCheckParametersItemVector.size())
        return;
    if (mCheckParametersItemVector[curTabIndex].size() != 0) {
        ui->checkedItemsTreeWidget->setCurrentItem(mCheckParametersItemVector[curTabIndex][mDataChoiseWidget[curTabIndex]->currentParameterNumber()]);
    }
}

void DataChoiceDialog::initializeSavedVariablesComboBoxWithTips()
{
    QMap<QString, QStringList> mapForSavedParams;
    for (auto key : gPreviouslySavedParametersNames.keys()) {
        auto item = gPreviouslySavedParametersNames[key];
        if (item.first.size() != item.second.size()) {
            mapForSavedParams[key] = item.first;
        } else {
            QStringList newList;
            for (int i = 0; i < item.first.size(); ++i) {
                QString newName;
                newName += item.first[i];
                if (item.second[i] != "") {
                    newName += " (" + item.second[i] + ")";
                }
                newList << newName;
            }
            mapForSavedParams[key] = newList;
        }
    }
    ui->savedVariablesComboBox->addItems(mapForSavedParams);
}


