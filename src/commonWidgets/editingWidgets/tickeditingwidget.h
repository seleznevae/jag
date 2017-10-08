#ifndef TICKEDITINGWIDGET_H
#define TICKEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class TickEditingWidget;
}


class TickEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TickEditingWidget(QWidget *parent = 0);
    ~TickEditingWidget();

    style::MainTick tick() const;
    void setTick(const style::MainTick &tick);
    void setSubTick(const style::Tick &tick);

    void setSubTickCountVisibility(bool visible);
signals:
    void tickChanged();

private:
    Ui::TickEditingWidget *ui;
//    style::Tick mTick;

    void connectAll();
    void disconnectAll();
    void setWidgetPropertiesFromTickPrefs(const style::Tick &tick);
};

#endif // TICKEDITINGWIDGET_H
