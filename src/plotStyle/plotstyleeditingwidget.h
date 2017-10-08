#ifndef PLOTSTYLEEDITINGWIDGET_H
#define PLOTSTYLEEDITINGWIDGET_H

#include <QWidget>
#include "plotstyle.h"

namespace Ui {
class PlotStyleEditingWidget;
}

class PlotStyleEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotStyleEditingWidget(QWidget *parent = 0);
    ~PlotStyleEditingWidget();

    void setPlotStyle(const PlotStyle &style);
    PlotStyle plotStyle() const;

signals:
    void styleChanged();
private:
    Ui::PlotStyleEditingWidget *ui;
    QString mPlotStyleName;

    void connetAll();
    void disconnetAll();

};

#endif // PLOTSTYLEEDITINGWIDGET_H
