#ifndef FILTERCONTROLLERWIDGET_H
#define FILTERCONTROLLERWIDGET_H

#include <QWidget>
#include "dataconverter/dataconverter.h"
#include <memory>
#include "core/graphmodel.h"
#include "dataconverter/abstractfiltercontainer.h"

class FilterControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterControllerWidget(QWidget *parent = 0);



    virtual
    void setFilter(std::shared_ptr<DataConverter> filter) = 0;

    void setModel(std::shared_ptr<AbstractFilterContainer> model);

signals:

public slots:

protected:
    std::shared_ptr<AbstractFilterContainer> mModel;


};

#endif // FILTERCONTROLLERWIDGET_H
