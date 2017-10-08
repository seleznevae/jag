#ifndef MARGINFILTERCONTROLLERWIDGET_H
#define MARGINFILTERCONTROLLERWIDGET_H

#include <QWidget>
#include "dataconverter/filtercontrollerwidget.h"
#include "dataconverter/borderfilter.h"
#include <QButtonGroup>

namespace Ui {
class MarginFilterControllerWidget;
}

class MarginFilterControllerWidget : public FilterControllerWidget
{
    Q_OBJECT

public:
    explicit MarginFilterControllerWidget(QWidget *parent = 0);
    ~MarginFilterControllerWidget();

    virtual
    void setFilter(std::shared_ptr<DataConverter> filter);

public slots:
    void setXLowMargin();
    void setXHighMargin();
    void setYLowMargin();
    void setYHighMargin();
    void setXFilteringType(bool);
    void setYFilteringType(bool);
    void setFilteringLogic(bool);
private:
    Ui::MarginFilterControllerWidget *ui;
    std::shared_ptr<BorderFilter> mFilter;

    QButtonGroup *mXbuttonGroup;
    QButtonGroup *mYbuttonGroup;
    QButtonGroup *mLogicButtonGroup;

    void initializeState();

    void connectAll();
    void disconnectAll();
};

#endif // MARGINFILTERCONTROLLERWIDGET_H
