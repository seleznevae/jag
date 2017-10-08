#ifndef WINDOWFILTERCONTROLLERWIDGET_H
#define WINDOWFILTERCONTROLLERWIDGET_H

#include <QWidget>
#include "dataconverter/filtercontrollerwidget.h"
#include "dataconverter/windowfilter.h"

namespace Ui {
class WindowFilterControllerWidget;
}

class WindowFilterControllerWidget : public FilterControllerWidget
{
    Q_OBJECT

public:
    explicit WindowFilterControllerWidget(QWidget *parent = 0);
    ~WindowFilterControllerWidget();

    virtual
    void setFilter(std::shared_ptr<DataConverter> filter);

public slots:
    void treatBeginTimeChanging();
    void treatEndTimeChanging();
    void treatWindowWidthChanging(int width);

private:
    Ui::WindowFilterControllerWidget *ui;
    std::shared_ptr<WindowFilter> mFilter;

    void initializeState();
};

#endif // WINDOWFILTERCONTROLLERWIDGET_H
