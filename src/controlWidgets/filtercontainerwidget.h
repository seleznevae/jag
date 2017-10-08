#ifndef FILTERCONTAINERWIDGET_H
#define FILTERCONTAINERWIDGET_H

#include <QWidget>
#include "core/graphmodel.h"
#include "dataconverter/filtercontrollerwidget.h"
#include <QPushButton>
#include "dataconverter/abstractfiltercontainer.h"

namespace Ui {
class FilterContainerWidget;
}


/*!
 \brief FilterContainerWidget is  a widget to show FilterControllerWidgets and buttons to create and delete them.

*/
class FilterContainerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterContainerWidget(QWidget *parent = 0);
    ~FilterContainerWidget();

    void setModel(std::shared_ptr<AbstractFilterContainer> model);
public slots:
    void clearModel();
    void addFilter();
    void treatKillRequest();

private:
    Ui::FilterContainerWidget *ui;
    std::shared_ptr<AbstractFilterContainer> mModel;

    QVector<std::shared_ptr<DataConverter>> mFilterVector;
    QVector<FilterControllerWidget*> mFilterControllerWidgetVector;
    QVector<QPushButton*> mKillButtonVector;

    void initializeState();
    void clearState();
    FilterControllerWidget* createFilterControlWidget(std::shared_ptr<DataConverter>);

    void addFilterAndKillWidget(std::shared_ptr<DataConverter> widget);
};

#endif // FILTERCONTAINERWIDGET_H
