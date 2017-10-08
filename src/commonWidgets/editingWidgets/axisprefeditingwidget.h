#ifndef AXISPREFEDITINGWIDGET_H
#define AXISPREFEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class AxisPrefEditingWidget;
}

class AxisPrefEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AxisPrefEditingWidget(QWidget *parent = 0);
    ~AxisPrefEditingWidget();

    void setAxisPref(const style::AxisPref& pref);
    style::AxisPref axisPref();

signals:
    void axisPrefChanged();
private:
    Ui::AxisPrefEditingWidget *ui;

    void connectAll();
    void disconnectAll();

private slots:
    void treatAxisPrefChanging();
};

#endif // AXISPREFEDITINGWIDGET_H
