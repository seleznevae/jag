#ifndef DATACHOISEWIDGET_H
#define DATACHOISEWIDGET_H

#include <QTreeWidget>
#include "datasource.h"
#include "datasource/abstractdatasourceadapter.h"
#include "core/graphcontainer.h"


class DataChoiceWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DataChoiceWidget(QWidget *parent = 0);

//    void setDataSource(DataSource *);
    void setDataSource(AbstractDataSourceAdapter *);
    void setShowEditWidget(GraphContainer*);

    QVector<QString> getCheckedItemsNames() const;
    QVector<QString> getCheckedItemsArgumentsNames() const;

//    DataSource * dataSource() const;
    AbstractDataSourceAdapter * dataSource() const;
    int currentParameterNumber() const;

signals:
    void checkedItemsListChanged();

public slots:
    void updateData();
    void addCheckedDataToShowEditWidget();
    void activateItemByRegExString(QString regexName);
    void treatItemClick(QTreeWidgetItem* item, int column);
    void setFavouriteSearchEnabled(bool);
    void setHateSearchEnabled(bool);
    void nextSearch(QString regexName);
    void prevSearch(QString regexName);
    void checkCurrentItem();
    void checkItemsByName(QVector<QString>, QVector<QString> = QVector<QString>());
    void clearAllCheckings();

    void expandItem(const QModelIndex &);
    void deleteParameterFromCheckList(int index);
    void setCurrentParameterNumber(int index = -1);



protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
//    DataSource *mDataSource;
    AbstractDataSourceAdapter *mDataSource;
    QVector<std::tuple<QString, int>> mDataVector;
    QVector<QVector<QTreeWidgetItem*>> mItemVector;
    QVector<QVector<QString>> mDisplayNamesVector;
    QVector<QString> mDisplayDescriptionVector;

    std::vector<std::tuple<QString, int, int>> mDataVectorMap;
    GraphContainer* mShowEditWidget;
    QVector<int>   validNamesIdsVector;
    bool mFavouriteSearchEnabled;
    bool mHateSearchEnabled;
    QString mPreviouslyFoundItem;

    QVector<std::tuple<QString, int, int>> mCheckedParameters;
    QVector<std::tuple<QString, int, int>> mCheckedParametersArguments;

    bool mWasLeftButtonPressed; //false - rigth button was pressed
    int mCurrentParameter; // number of current parameter(current parameter - parameter for which argument will be set with right mouse button
};

#endif // DATACHOISEWIDGET_H
