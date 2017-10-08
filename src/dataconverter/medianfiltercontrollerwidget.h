#ifndef MEDIANFILTERCONTROLLERWIDGET_H
#define MEDIANFILTERCONTROLLERWIDGET_H

#include <QWidget>
#include "dataconverter/filtercontrollerwidget.h"
#include "dataconverter/medianwindowfilter.h"

namespace Ui {
class MedianFilterControllerWidget;
}

class MedianFilterControllerWidget : public FilterControllerWidget
{
    Q_OBJECT

public:
    explicit MedianFilterControllerWidget(QWidget *parent = 0);
    ~MedianFilterControllerWidget();

    virtual
    void setFilter(std::shared_ptr<DataConverter> filter);

public slots:
    void treatWindowWidthChanging(int width);
    void treatBeginTimeChanging();
    void treatEndTimeChanging();

private:
    Ui::MedianFilterControllerWidget *ui;
    std::shared_ptr<MedianWindowFilter> mFilter;

    void initializeState();
};

#endif // MEDIANFILTERCONTROLLERWIDGET_H
