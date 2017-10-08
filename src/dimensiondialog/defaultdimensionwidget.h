#ifndef DEFAULTDIMENSIONWIDGET_H
#define DEFAULTDIMENSIONWIDGET_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class DefaultDimensionWidget;
}

class DefaultDimensionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultDimensionWidget(QWidget *parent = 0);
    ~DefaultDimensionWidget();

public slots:
    void applyChangesToGlobalLists();
    void activateParameterByUnitsIndex(const QModelIndex & index);
    void activateUnitByParameterIndex(const QModelIndex & index);
    void addPair();
    void deleteCurrentRow();

private:
    Ui::DefaultDimensionWidget *ui;

    QStringListModel  *mParameterListModel;
    QStringListModel  *mUnitsListModel;
};

#endif // DEFAULTDIMENSIONWIDGET_H
