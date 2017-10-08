#ifndef LEGENDEDITINGWIDGET_H
#define LEGENDEDITINGWIDGET_H

#include <QWidget>
#include "../plotStyle/plotstyle.h"

namespace Ui {
class LegendEditingWidget;
}

class LegendEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LegendEditingWidget(QWidget *parent = 0);
    ~LegendEditingWidget();

    void setLegendProperties(style::LegendPref);
    style::LegendPref legendProperties() const;
signals:
    void legendPropertiesEdited(const style::LegendPref&);

private slots:
    void treatLegendPropertiesEditing();

private:
    Ui::LegendEditingWidget *ui;

    void connectAll();
    void disconnectAll();
};

#endif // LEGENDEDITINGWIDGET_H
