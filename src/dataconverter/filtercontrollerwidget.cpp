#include "filtercontrollerwidget.h"

FilterControllerWidget::FilterControllerWidget(QWidget *parent) :
    QWidget(parent)
{
}

void FilterControllerWidget::setModel(std::shared_ptr<AbstractFilterContainer> model)
{
    mModel = model;

}
