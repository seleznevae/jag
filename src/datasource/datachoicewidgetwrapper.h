#ifndef DATACHOISEWIDGETWRAPPER_H
#define DATACHOISEWIDGETWRAPPER_H

#include <QWidget>
#include "datasource/datachoicewidget.h"
#include <datasource/abstractdatasourceadapter.h>
#include "core/graphcontainer.h"

namespace Ui {
class DataChoiceWidgetWrapper;
}

class DataChoiceWidgetWrapper : public QWidget
{
    Q_OBJECT

public:
    explicit DataChoiceWidgetWrapper(QWidget *parent = 0);
    ~DataChoiceWidgetWrapper();


    void setDataSource(AbstractDataSourceAdapter *);
    void setShowEditWidget(GraphContainer*);

    QVector<QString> getCheckedItemsNames() const;
    QVector<QString> getCheckedItemsArgumentsNames() const;

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
    void timerEvent(QTimerEvent *event);

private:
    Ui::DataChoiceWidgetWrapper *ui;
    int mTimerId;
    AbstractDataSourceAdapter *mDataSource;
};

#endif // DATACHOISEWIDGETWRAPPER_H

/////////////////////////////////////////////////////////////////


