#ifndef GRIDEDITINGWIDGET_H
#define GRIDEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"






namespace Ui {
class GridEditingWidget;
}

class GridEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GridEditingWidget(QWidget *parent = 0);
    ~GridEditingWidget();

    void setHead(const QString& head);
    void setGrid(style::Grid newGrid);
    style::Grid grid() const;
signals:
    void gridChanged(style::Grid);
private:
    Ui::GridEditingWidget *ui;
    style::Grid mGrid;

    void connectAll();
    void disconnectAll();

private slots:
    void treatPropertiesChanging();
};

#endif // GRIDEDITINGWIDGET_H
