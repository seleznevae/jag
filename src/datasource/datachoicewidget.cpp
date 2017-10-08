#include "datachoicewidget.h"
#include <Qt>
#include <QDebug>
#include "global/global_definitions.h"
#include <QEventLoop>
#include <QSplashScreen>
#include "event/eventsourceadapter.h"
#include "basic/basic.h"
#include "commonWidgets/noncriticalerrorsdialogwithinfo.h"



//static
//QString stripDirectoryFromFileName(const QString& fileName);

DataChoiceWidget::DataChoiceWidget(QWidget *parent) :
    QTreeWidget(parent), mDataSource(NULL), mShowEditWidget(NULL), mFavouriteSearchEnabled(false), mHateSearchEnabled(false), mWasLeftButtonPressed(true),
    mCurrentParameter(-1)
{
    //for fast checking
    setAlternatingRowColors(true);

     connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
             this, SLOT(treatItemClick(QTreeWidgetItem *, int)));

     //to expand top level items by one click
     connect(this, SIGNAL(clicked(const QModelIndex &)),
         this, SLOT(expandItem(const QModelIndex &)));


     setContextMenuPolicy(Qt::CustomContextMenu);

}

//void DataChoiseWidget::setDataSource(DataSource *dataSource)
void DataChoiceWidget::setDataSource(AbstractDataSourceAdapter *dataSource)
{
    if (dataSource != mDataSource) {
        mDataSource = dataSource;
        updateData();
    }
}

void DataChoiceWidget::setShowEditWidget(GraphContainer *widget)
{
    mShowEditWidget = widget;
}

QVector<QString> DataChoiceWidget::getCheckedItemsNames() const
{
//    QVector<QString> checkedParametersNames;
//    for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
//        for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
//            if (mItemVector[primaryIndex][secondaryIndex]->checkState(0) == Qt::Checked) {
//                checkedParametersNames.push_back(mDisplayNamesVector[primaryIndex][secondaryIndex]);
//            }
//        }
//    }
//    return checkedParametersNames;

    QVector<QString> checkedItemsNames;
    for (const auto & item : mCheckedParameters) {
        checkedItemsNames.push_back(std::get<0>(item));
    }
    return checkedItemsNames;
}

QVector<QString> DataChoiceWidget::getCheckedItemsArgumentsNames() const
{
    QVector<QString> checkedItemsArgumentsNames;
    for (const auto & item : mCheckedParametersArguments) {
//        if (std::get<1>(item) == -1)
//            checkedItemsArgumentsNames.push_back("std");
//        else
            checkedItemsArgumentsNames.push_back(std::get<0>(item));
    }
    return checkedItemsArgumentsNames;
}

//DataSource *DataChoiseWidget::dataSource() const
AbstractDataSourceAdapter *DataChoiceWidget::dataSource() const
{
    return mDataSource;
}

int DataChoiceWidget::currentParameterNumber() const
{
    if (mCheckedParametersArguments.size() == 0) {
        return -1;
    }
    if (mCurrentParameter < 0 || mCurrentParameter >= mCheckedParametersArguments.size())
        return mCheckedParametersArguments.size() - 1;
    return mCurrentParameter;
}



void DataChoiceWidget::updateData()
{
    using namespace global;
    setCurrentParameterNumber();
    if (mDataSource != NULL) {
        clear();
        mDataVector.clear();
        validNamesIdsVector.clear();
        mDataVectorMap.clear();
        mDataVector.clear();
        mDisplayNamesVector.clear();
        mDisplayDescriptionVector.clear();
        mItemVector.clear();


//        setHeaderLabel("Parameters in " + mDataSource->getDataSourceName());
        QStringList headerStringList;
//        headerStringList << ("Parameters in " + mDataSource->getDataSourceName());
        headerStringList << tr("Parameter") ;

        headerStringList << tr("Description");


        setHeaderLabels(headerStringList);

        mDataVector = mDataSource->getDataList();

        //mDataVectorMap - is used for data sortring (by name)
        for (int i = 0; i < mDataVector.size(); ++i)
            mDataVectorMap.push_back(std::tuple<QString, int, int> (std::get<0>(mDataVector[i]), std::get<1>(mDataVector[i]), i));
        std::sort(mDataVectorMap.begin(), mDataVectorMap.end(),
                  [](const std::tuple<QString, int, int> &arg1, const std::tuple<QString, int, int> &arg2)
                        {
                            return std::get<0>(arg1).toLower() < std::get<0>(arg2).toLower();
                        }
                  );

        ////without sorting
//        for (int dataIndex = 0; dataIndex < mDataVector.size(); ++dataIndex) {
//            if (std::get<1>(mDataVector[dataIndex]) == 1) {
//                QString tempString =  QString(std::get<0>(mDataVector[dataIndex]));
//                QTreeWidgetItem *mDataItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tempString));
//                mDisplayNamesVector.push_back(QVector<QString> ({tempString}));
//                mDataItem->setFlags(mDataItem->flags() | Qt::ItemIsUserCheckable);
//                mDataItem->setCheckState(0,Qt::Unchecked);
//                addTopLevelItem(mDataItem);
//                mItemVector.push_back(QVector<QTreeWidgetItem *> ({mDataItem}));
//            } else {
//                QTreeWidgetItem *mDataItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(std::get<0>(mDataVector[dataIndex]))));
//                addTopLevelItem(mDataItem);
//                QVector<QTreeWidgetItem *> buferVector;
//                QVector<QString> buferStringVector;
//                for (int secondaryIndex = 0; secondaryIndex < std::get<1>(mDataVector[dataIndex]) ; ++secondaryIndex) {
//                    QString tempString =  QString(std::get<0>(mDataVector[dataIndex]))
//                            +QString("[%1]").arg(QString::number(secondaryIndex));
//                    QTreeWidgetItem* mDataSubItem = new QTreeWidgetItem(mDataItem, QStringList(tempString));
//                    mDataSubItem->setFlags(mDataSubItem->flags() | Qt::ItemIsUserCheckable);
//                    mDataSubItem->setCheckState(0,Qt::Unchecked);
//                    buferVector.push_back(mDataSubItem);
//                    buferStringVector.push_back(tempString);
//                }
//                mItemVector.push_back(buferVector);
//                mDisplayNamesVector.push_back(buferStringVector);
//            }
//        }

        ////with sorting
        for (int dataIndex = 0; dataIndex < static_cast<int>(mDataVectorMap.size()); ++dataIndex) {

            //favourite filter
//            if (mFavouriteSearchEnabled) {
//                bool isValid = false;
//                foreach (QString pattern, gFavouriteParameters) {
//                    QRegExp regExp(pattern);
//                    if (regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))) {
//                        validNamesIdsVector.push_back(dataIndex);
//                        isValid = true;
//                        break;
//                    }
//                }
//                if (!isValid)
//                    continue;
//            } else
//                validNamesIdsVector.push_back(dataIndex);

            //filtering by search string
            if (gAutoParametersFilteringDuringSearchIsEnabled) {
                QRegExp regExp(gAutoParametersFilteringDuringSearchPattern);
                regExp.setCaseSensitivity(Qt::CaseInsensitive);
//                if (!regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex]))))
//                    continue;
                if (!gSearchByDescriptionIsEnabled && !regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex]))))
                    continue;
                if (gSearchByDescriptionIsEnabled && !regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))
                        && !regExp.exactMatch(mDataSource->getDescription(std::get<2>(mDataVectorMap[dataIndex]))))
                    continue;
            }

            //favourite and hate filters
            if (mFavouriteSearchEnabled && !mHateSearchEnabled) {
                bool isValid = false;
                foreach (QString pattern, gFavouriteParameters) {
                    QRegExp regExp(pattern);
                    if (regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))) {
                        validNamesIdsVector.push_back(dataIndex);
                        isValid = true;
                        break;
                    }
                }
                if (!isValid)
                    continue;
            } else if (!mFavouriteSearchEnabled && mHateSearchEnabled) {
                bool isValid = true;
                foreach (QString pattern, gHateParameters) {
                    QRegExp regExp(pattern);
                    if (regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid)
                    validNamesIdsVector.push_back(dataIndex);
                else
                    continue;
            } else if (mFavouriteSearchEnabled && mHateSearchEnabled) {
                bool isValid = false;
                foreach (QString pattern, gFavouriteParameters) {
                    QRegExp regExp(pattern);
                    if (regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))) {
                        isValid = true;
                        break;
                    }
                }
                if (!isValid)
                    continue;
                foreach (QString pattern, gHateParameters) {
                    QRegExp regExp(pattern);
                    if (regExp.exactMatch(QString(std::get<0>(mDataVectorMap[dataIndex])))) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid)
                    validNamesIdsVector.push_back(dataIndex);
                else
                    continue;
            } else
                validNamesIdsVector.push_back(dataIndex);

            if (std::get<1>(mDataVectorMap[dataIndex]) == 1) {
                QString tempString =  QString(std::get<0>(mDataVectorMap[dataIndex]));
                QStringList tempQStringList;
                tempQStringList << tempString;


                tempQStringList << mDataSource->getDescription(std::get<2>(mDataVectorMap[dataIndex]));


                QTreeWidgetItem *mDataItem = new QTreeWidgetItem((QTreeWidget*)0, tempQStringList);
                mDisplayNamesVector.push_back(QVector<QString> ({tempString}));

                mDisplayDescriptionVector.push_back(mDataSource->getDescription(std::get<2>(mDataVectorMap[dataIndex])));
                //desabling users checks because it will be checked programmatically
                mDataItem->setFlags(mDataItem->flags() & ~Qt::ItemIsUserCheckable);
                mDataItem->setCheckState(0,Qt::Unchecked);
                addTopLevelItem(mDataItem);
                mItemVector.push_back(QVector<QTreeWidgetItem *> ({mDataItem}));
                //check item if parameter is already  in check list
                if (mCheckedParameters.indexOf(std::tuple<QString, int, int>(tempString,std::get<2>(mDataVectorMap[dataIndex]), 0)) != -1) {
                    mDataItem->setCheckState(0, Qt::Checked);
                }
                //set item current if it was current previusly
                if (tempString == mPreviouslyFoundItem)
                    setCurrentItem(mDataItem);
            } else {

                QString tempString =  QString(std::get<0>(mDataVectorMap[dataIndex]));
                QStringList tempQStringList;
                tempQStringList << tempString;
                tempQStringList << mDataSource->getDescription(std::get<2>(mDataVectorMap[dataIndex]));

                QTreeWidgetItem *mDataItem = new QTreeWidgetItem((QTreeWidget*)0, tempQStringList);
                addTopLevelItem(mDataItem);
                QVector<QTreeWidgetItem *> buferVector;
                QVector<QString> buferStringVector;
                for (int secondaryIndex = 0; secondaryIndex < std::get<1>(mDataVectorMap[dataIndex]) ; ++secondaryIndex) {
                    QString tempString =  QString(std::get<0>(mDataVectorMap[dataIndex]))
                            +QString("[%1]").arg(QString::number(secondaryIndex));
                    QTreeWidgetItem* mDataSubItem = new QTreeWidgetItem(mDataItem, QStringList(tempString));
                    mDataSubItem->setFlags(mDataSubItem->flags() & ~Qt::ItemIsUserCheckable);
                    mDataSubItem->setCheckState(0,Qt::Unchecked);
                    buferVector.push_back(mDataSubItem);
                    buferStringVector.push_back(tempString);
                    //check item if parameter is already  in check list
                    if (mCheckedParameters.indexOf(std::tuple<QString, int, int>(tempString,std::get<2>(mDataVectorMap[dataIndex]), secondaryIndex)) != -1) {
                        mDataSubItem->setCheckState(0, Qt::Checked);
                    }
                    //set item current if it was current previusly
                    if (tempString == mPreviouslyFoundItem)
                        setCurrentItem(mDataItem);
                }
                mItemVector.push_back(buferVector);
                mDisplayNamesVector.push_back(buferStringVector);
                mDisplayDescriptionVector.push_back(mDataSource->getDescription(std::get<2>(mDataVectorMap[dataIndex])));
            }
        }

        QFont myFont = QApplication::font();
        QFontMetrics fm(myFont);
        setColumnWidth(0, mDataSource->getMaxNameLength()*fm.width("W") + 60);
    }
}

void DataChoiceWidget::addCheckedDataToShowEditWidget()
{
    ////without sorting
//    if (mShowEditWidget != NULL) {
//        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
//            for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
//                if (mItemVector[primaryIndex][secondaryIndex]->checkState(0) == Qt::Checked) {
//                    auto data = mDataSource->getDataByIndex(primaryIndex, secondaryIndex);
//                    mShowEditWidget->addNewGraph(std::get<0>(data), std::get<1>(data),
//                                                 stripDirectoryFromFileName(mDataSource->getDataSourceName())
//                                                 + "::" + mDisplayNamesVector[primaryIndex][secondaryIndex]);

//                }
//            }
//        }
//    }



//    ////with sorting
//    if (mShowEditWidget != NULL) {
//        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
//            for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
//                if (mItemVector[primaryIndex][secondaryIndex]->checkState(0) == Qt::Checked) {
////                    auto data = mDataSource->getDataByIndex(std::get<2>(mDataVectorMap[primaryIndex]), secondaryIndex);

////                    if (!mDataSource->isDataReady()) {
////                        QWidget widget;
////                        QEventLoop eventLoop;
////                        widget.show();
////                        widget.raise();
////                        while (!mDataSource->isDataReady()) {
////                            eventLoop.processEvents( );
////                        }
////                    }
//                    if (!mDataSource->isDataReady()) {
//                        QPixmap pixmap(":/wait");
//                        QSplashScreen *splashScreen = new QSplashScreen(pixmap,  Qt::WindowStaysOnTopHint);
//                        splashScreen->show();
//                        while (!mDataSource->isDataReady()) {
//                            qApp->processEvents();
//                        }
//                        delete splashScreen;
//                    }

//                    auto data = mDataSource->getDataByIndex(std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), secondaryIndex);
//                    mShowEditWidget->addNewGraph(std::get<0>(data), std::get<1>(data),
//                                                 stripDirectoryFromFileName(mDataSource->getDataSourceName())
//                                                 + "::" + mDisplayNamesVector[primaryIndex][secondaryIndex]);

//                }
//            }
//        }
//    }


    //no need in following sruff if there're no checked parameters
    if (mCheckedParameters.empty())
        return;

    if (!mDataSource->isDataReady()) {
        QPixmap pixmap(":/wait");
        QSplashScreen *splashScreen = new QSplashScreen(pixmap,  Qt::WindowStaysOnTopHint);
        splashScreen->show();
        while (!mDataSource->isDataReady() && mDataSource->isDataSourceValid() ) {
            qApp->processEvents();
        }
        delete splashScreen;
    }
    if (!mDataSource->isDataSourceValid()) {
        showCriticalMessage(tr("Can't add data from ") + mDataSource->getDataSourceName());
        return;
    }
    if (!mDataSource->isDataCorrect()) {
//        showCriticalMessage("Can't add data from " + mDataSource->getDataSourceName() + "they are not correct");
        NonCriticalErrorsDialogWithInfo messageDialog;
        messageDialog.setMessage(mDataSource->getDataSourceName() + tr(" contains incorrect data.\nDo you really want to continue?"));
        messageDialog.setDetails(mDataSource->errorsList());
//        messageDialog.setRejectText("Discard");
//        messageDialog.setAcceptText("Continue");
        auto result = messageDialog.exec();
        if (messageDialog.showItNextTime() == false) {
            mDataSource->setCorrectness(true);
        }
        if (result == QDialog::Rejected) {
            return;
        }

    }

//    for (auto checkedParameter : mCheckedParameters) {
//        mDataSource->addDataToPlotEdit(mShowEditWidget, std::get<1>(checkedParameter), std::get<2>(checkedParameter));
//    }


    for (int i = 0; i < mCheckedParameters.size(); ++i) {
        auto checkedParameter = mCheckedParameters[i];
        auto checkedParameterArgument = mCheckedParametersArguments[i];

        if (std::get<1>(checkedParameterArgument) < 0) {
            mDataSource->addDataToPlotEdit(mShowEditWidget, std::get<1>(checkedParameter), std::get<2>(checkedParameter));
        } else {
            mDataSource->addCurveDataToPlotEdit(mShowEditWidget, std::get<1>(checkedParameterArgument), std::get<2>(checkedParameterArgument),
                                           std::get<1>(checkedParameter), std::get<2>(checkedParameter));
        }
    }

}

void DataChoiceWidget::activateItemByRegExString(QString regexName)
{
    if (global::gAutoParametersFilteringDuringSearchIsEnabled)
        updateData(); // to filter data by search string regexp

    QRegExp regExp(regexName + ".*");
    if (regExp.exactMatch(mPreviouslyFoundItem))
        return;

    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    for (int itemIndex = 0; itemIndex < mDisplayNamesVector.size(); ++itemIndex) {
        QString name = mDisplayNamesVector[itemIndex][0];
        QString description = mDisplayDescriptionVector[itemIndex];
//        if (regExp.indexIn(name) != -1) {
//        if (regExp.exactMatch(name)) {
        if (regExp.exactMatch(name) || (global::gSearchByDescriptionIsEnabled && regExp.exactMatch(description))) {
            setCurrentItem(mItemVector[itemIndex][0]);
            scrollToItem (mItemVector[itemIndex][0], QAbstractItemView::PositionAtCenter);  //to scroll widget, so that item would be at center
            break;
        }
    }
    if (currentItem())
        mPreviouslyFoundItem = currentItem()->text(0);



}

void DataChoiceWidget::treatItemClick(QTreeWidgetItem *item, int /*column*/)
{

    //checking if clicked item is low level (ignore clicking on top level items - essential for vector parameters)
    bool found = false;
    foreach (auto itemsVector, mItemVector) {
        if (itemsVector.indexOf(item) != -1) {
            found = true;
            break;
        }
    }
    if (found) {
        if (mWasLeftButtonPressed) {
            if (item->checkState(0) == Qt::Checked)
                item->setCheckState(0, Qt::Unchecked);
            else if (item->checkState(0) == Qt::Unchecked)
                item->setCheckState(0, Qt::Checked);
        }

        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
            if (mItemVector[primaryIndex].indexOf(item) != -1) {
                int secondaryIndex = mItemVector[primaryIndex].indexOf(item);
                std::tuple<QString, int, int> element (item->text(0), //current showing name
                                                        std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), //original primary index
                                                        secondaryIndex //secondary index
                                                        );
                if (mWasLeftButtonPressed) {
                    if (item->checkState(0) == Qt::Checked) {
                        if (mCheckedParameters.indexOf(element) == -1) {
                            mCheckedParameters.push_back(element);
                            mCheckedParametersArguments.push_back(std::tuple<QString, int, int>("", -1, 0));
//                            mCurrentParameter = mCheckedParameters.size() - 1;
                            setCurrentParameterNumber();
                        }
                    } else if (item->checkState(0) == Qt::Unchecked) {
                        int index = mCheckedParameters.indexOf(element);
                        if (index != -1) {
                            mCheckedParameters.remove(index);
                            mCheckedParametersArguments.remove(index);
//                            mCurrentParameter = mCheckedParameters.size() - 1;
                            setCurrentParameterNumber();
                        }
                    }
                } else {
                    if (mCheckedParametersArguments.size() > 0) {
//                        mCheckedParametersArguments.back() = element;
                        mCheckedParametersArguments[currentParameterNumber()] = element;
                    }
                }
                break;
            }
        }
        if (currentItem())
            mPreviouslyFoundItem = currentItem()->text(0);

        emit checkedItemsListChanged();

    }
    clearFocus();
}

void DataChoiceWidget::setFavouriteSearchEnabled(bool newState)
{
        if (mFavouriteSearchEnabled != newState) {
            mFavouriteSearchEnabled = newState;
            updateData();
        }
}

void DataChoiceWidget::setHateSearchEnabled(bool newState)
{
    if (mHateSearchEnabled != newState) {
        mHateSearchEnabled = newState;
        updateData();
    }
}

void DataChoiceWidget::nextSearch(QString regexName)
{
    QRegExp regExp(regexName + ".*");
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    auto current = currentItem();
    int currentItemIndex = 0;
    if (current != NULL) {
        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
            if (mItemVector[primaryIndex].indexOf(current) != -1) {
                currentItemIndex = primaryIndex;
                break;
            }
        }
    }
    for (int itemIndex = currentItemIndex + 1; itemIndex != currentItemIndex; ++itemIndex) {
        if (itemIndex == mDisplayNamesVector.size())
            itemIndex = 0;
        QString name = mDisplayNamesVector[itemIndex][0];
        QString description = mDisplayDescriptionVector[itemIndex];
//        if (regExp.exactMatch(name)) {
        if (regExp.exactMatch(name) || (global::gSearchByDescriptionIsEnabled && regExp.exactMatch(description))) {
            setCurrentItem(mItemVector[itemIndex][0]);
            break;
        }
    }
    if (currentItem())
        mPreviouslyFoundItem = currentItem()->text(0);



}

void DataChoiceWidget::prevSearch(QString regexName)
{
    QRegExp regExp(regexName + ".*");
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    auto current = currentItem();
    int currentItemIndex = 0;
    if (current != NULL) {
        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
            if (mItemVector[primaryIndex].indexOf(current) != -1) {
                currentItemIndex = primaryIndex;
                break;
            }
        }
    }
    for (int itemIndex = currentItemIndex - 1; itemIndex != currentItemIndex; --itemIndex) {
        if (itemIndex < 0)
            itemIndex = mDisplayNamesVector.size() - 1;
        QString name = mDisplayNamesVector[itemIndex][0];
        QString description = mDisplayDescriptionVector[itemIndex];
//        if (regExp.exactMatch(name)) {
        if (regExp.exactMatch(name) || (global::gSearchByDescriptionIsEnabled && regExp.exactMatch(description))) {
            setCurrentItem(mItemVector[itemIndex][0]);
            break;
        }
    }
    if (currentItem())
        mPreviouslyFoundItem = currentItem()->text(0);
}

void DataChoiceWidget::checkCurrentItem()
{

    QTreeWidgetItem *currentActivatedItem = currentItem();
    if (currentActivatedItem) {
        bool found = false;
        foreach (auto itemsVector, mItemVector) {
            if (itemsVector.indexOf(currentActivatedItem) != -1) {
                found = true;
                break;
            }
        }
        if (found) {
            if (currentActivatedItem->checkState(0) == Qt::Checked)
                currentActivatedItem->setCheckState(0, Qt::Unchecked);
            else if (currentActivatedItem->checkState(0) == Qt::Unchecked)
                currentActivatedItem->setCheckState(0, Qt::Checked);

            for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
                if (mItemVector[primaryIndex].indexOf(currentActivatedItem) != -1) {
                    int secondaryIndex = mItemVector[primaryIndex].indexOf(currentActivatedItem);
                    std::tuple<QString, int, int> element (currentActivatedItem->text(0), //current showing name
                                                            std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), //original primary index
                                                            secondaryIndex //secondary index
                                                            );
                    if (currentActivatedItem->checkState(0) == Qt::Checked) {
                        if (mCheckedParameters.indexOf(element) == -1) {
                            mCheckedParameters.push_back(element);
                            mCheckedParametersArguments.push_back(std::tuple<QString, int, int>("", -1, 0));
//                            mCurrentParameter = mCheckedParameters.size() - 1;
                            setCurrentParameterNumber();
                        }
                    } else if (currentActivatedItem->checkState(0) == Qt::Unchecked) {
                        int index = mCheckedParameters.indexOf(element);
                        if (index != -1) {
                            mCheckedParameters.remove(index);
                            mCheckedParametersArguments.remove(index);
//                            mCurrentParameter = mCheckedParameters.size() - 1;
                            setCurrentParameterNumber();
                        }
                    }
                    break;
                }

            }
            if (currentItem())
                mPreviouslyFoundItem = currentItem()->text(0);

            emit checkedItemsListChanged();

        }
    }

    clearFocus();


}

void DataChoiceWidget::checkItemsByName(QVector<QString> parameterNames, QVector<QString> parameterArgumentsNames)
{
//    if (parameterNames.size() != 0) {
//        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
//            for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
//                if (parameterNames.indexOf(mDisplayNamesVector[primaryIndex][secondaryIndex]) != -1) {
//                    if (mItemVector[primaryIndex][secondaryIndex]->checkState(0) == Qt::Unchecked) {
//                        std::tuple<QString, int, int> element (mItemVector[primaryIndex][secondaryIndex]->text(0), //current showing name
//                                                                std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), //original primary index
//                                                                secondaryIndex //secondary index
//                                                                );
//                        if (mCheckedParameters.indexOf(element) == -1) {
//                            mCheckedParameters.push_back(element);
//                            mCheckedParametersArguments.push_back(std::tuple<QString, int, int>("", -1, 0));
////                            mCurrentParameter = mCheckedParameters.size() - 1;
//                            setCurrentParameterNumber();
//                        }
//                    }
//                    mItemVector[primaryIndex][secondaryIndex]->setCheckState(0, Qt::Checked);
//                }
//            }
//        }
//    }
//    emit checkedItemsListChanged();
//    Q_UNUSED(parameterArgumentsNames);


    bool isArgumentSearchNecessary = parameterArgumentsNames.size() == parameterNames.size();

    if (parameterNames.size() != 0) {
        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
            for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
                int index = parameterNames.indexOf(mDisplayNamesVector[primaryIndex][secondaryIndex]);
                if (index != -1) {
                    std::tuple<QString, int, int> element (mItemVector[primaryIndex][secondaryIndex]->text(0), //current showing name
                                                            std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), //original primary index
                                                            secondaryIndex //secondary index
                                                            );
                    std::tuple<QString, int, int> argElement = std::tuple<QString, int, int>("", -1, 0);
                    if (isArgumentSearchNecessary && parameterArgumentsNames[index] != "") {
                        for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
                            for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
                                if (mDisplayNamesVector[primaryIndex][secondaryIndex] == parameterArgumentsNames[index]) {
                                    argElement =  std::tuple<QString, int, int>(mItemVector[primaryIndex][secondaryIndex]->text(0), //current showing name
                                                                                                    std::get<2>(mDataVectorMap[validNamesIdsVector[primaryIndex]]), //original primary index
                                                                                                    secondaryIndex //secondary index
                                                                                                    );
                                }
                            }
                        }
                    }
                    int index2 = mCheckedParameters.indexOf(element);
                    if (index2 == -1 || (index2 < mCheckedParametersArguments.size() && mCheckedParametersArguments[index2] != argElement)) {
                        mCheckedParameters.push_back(element);
                        mCheckedParametersArguments.push_back(argElement);
//                        mCurrentParameter = mCheckedParameters.size() - 1;
                        setCurrentParameterNumber();
                    }
                    mItemVector[primaryIndex][secondaryIndex]->setCheckState(0, Qt::Checked);
                }
            }
        }
    }

    emit checkedItemsListChanged();
}

void DataChoiceWidget::clearAllCheckings()
{
    for (int primaryIndex = 0; primaryIndex < mItemVector.size(); ++primaryIndex) {
        for (int secondaryIndex = 0; secondaryIndex < mItemVector[primaryIndex].size(); ++secondaryIndex) {
            mItemVector[primaryIndex][secondaryIndex]->setCheckState(0, Qt::Unchecked);
        }
    }
    mCheckedParameters.clear();
    mCheckedParametersArguments.clear();
    emit checkedItemsListChanged();
}

void DataChoiceWidget::expandItem(const QModelIndex &index)
{
    isExpanded(index)? collapse(index) : expand(index);
}

void DataChoiceWidget::deleteParameterFromCheckList(int index)
{
    if (index >= 0 && index < mCheckedParameters.size()) {
        mCheckedParameters.remove(index);
        mCheckedParametersArguments.remove(index);
    }
    updateData();
    emit checkedItemsListChanged();
}

void DataChoiceWidget::setCurrentParameterNumber(int index)
{
    if (index == -1) {
        mCurrentParameter = mCheckedParameters.size() - 1;
    } else {
        mCurrentParameter = index;
    }
}

void DataChoiceWidget::keyPressEvent(QKeyEvent *event)
{
    if (event) {
        event->ignore();
    }
}

void DataChoiceWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mWasLeftButtonPressed = event->button() & Qt::LeftButton;
    QTreeWidget::mouseReleaseEvent(event);
}

//QString stripDirectoryFromFileName(const QString& fileName) {
//    QString outFileName = fileName;
//    int index;
//    if ((index = outFileName.lastIndexOf("/")) != -1) {
//        outFileName.remove(0, index + 1);
//    }
//    if ((index = outFileName.lastIndexOf("\\")) != -1) {
//        outFileName.remove(0, index + 1);
//    }
//    return outFileName;
//}
