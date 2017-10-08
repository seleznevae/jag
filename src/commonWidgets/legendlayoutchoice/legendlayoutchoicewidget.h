#ifndef LEGENDLAYOUTCHOICEWIDGET_H
#define LEGENDLAYOUTCHOICEWIDGET_H

#include <QWidget>
#include "core/plotlayoutmodel.h"
#include <tuple>

class QPushButton;

namespace Ui {
class LegendLayoutChoiceWidget;
}

class LegendLayoutChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LegendLayoutChoiceWidget(QWidget *parent = 0);
    ~LegendLayoutChoiceWidget();

    void setCurrentLegendLocation(style::LegendLocation);
    style::LegendLocation currentLocation() const;

public slots:
    void treatButtonClicked();

signals:
    void currentLegendLocationChanged(style::LegendLocation);

private:
    Ui::LegendLayoutChoiceWidget *ui;
    QVector<std::tuple<style::LegendLocation, QPushButton*>> mLegendLocationMap;
    QPushButton *mCurrentButton;

    void setEverythingChecked(bool checked);
    void setCurrentButton(QPushButton*);
};

#endif // LEGENDLAYOUTCHOICEWIDGET_H
