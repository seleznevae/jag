#ifndef CLUSTERFILTERCONTROLLERWIDGET_H
#define CLUSTERFILTERCONTROLLERWIDGET_H

#include <QWidget>
#include "dataconverter/filtercontrollerwidget.h"
#include "dataconverter/clusterfilter.h"

namespace Ui {
class ClusterFilterControllerWidget;
}

class ClusterFilterControllerWidget : public FilterControllerWidget
{
    Q_OBJECT

public:
    explicit ClusterFilterControllerWidget(QWidget *parent = 0);
    ~ClusterFilterControllerWidget();

    virtual
    void setFilter(std::shared_ptr<DataConverter> filter);


public slots:
    void treatBeginTimeChanging();
    void treatEndTimeChanging();
    void treatWindowWidthChanging();
    void treatThresholdChanging();
    void treatNumberOfConsecErrorsChanging();
    void treatPolicyChanging();

    void initializeState();

private slots:
    void connectAll();
    void disconnectAll();

private:
    Ui::ClusterFilterControllerWidget *ui;
    std::shared_ptr<ClusterFilter> mFilter;


};

#endif // CLUSTERFILTERCONTROLLERWIDGET_H




