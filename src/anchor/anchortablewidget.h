#ifndef ANCHOTTABLEWIDGET_H
#define ANCHOTTABLEWIDGET_H

#include <QTableWidget>
#include "core/plotwidget.h"

class AnchorTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit AnchorTableWidget(QWidget *parent = 0);

    void setPlotWidget(const PlotWidget*);
    bool isColumnWidthFixed() const;

signals:

public slots:
    void initializeState(double currentTime = NAN);
    void setColumnWidthFixed(bool);

protected:
    virtual void paintEvent(QPaintEvent *event);


private:
    const PlotWidget *mPlotWidget;
    bool mIsColumnsWidthFixed;


};

#endif // ANCHOTTABLEWIDGET_H
