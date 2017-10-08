#ifndef AXISRECTEDITINGWIDGET_H
#define AXISRECTEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class AxisRectEditingWidget;
}

class AxisRectEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AxisRectEditingWidget(QWidget *parent = 0);
    ~AxisRectEditingWidget();

    void setAxisRect(const style::AxisRect &axisRect);
    style::AxisRect axisRect() const;

signals:
    void axisRectPropertiesChanged();
private:
    Ui::AxisRectEditingWidget *ui;

    void connectAll();
    void disconnectAll();
private slots:
    void updateAxisRectPainter();
};

#endif // AXISRECTEDITINGWIDGET_H
