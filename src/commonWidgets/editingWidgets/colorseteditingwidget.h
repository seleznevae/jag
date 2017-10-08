#ifndef COLORSETEDITINGWIDGET_H
#define COLORSETEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class ColorSetEditingWidget;
}

class ColorSetEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSetEditingWidget(QWidget *parent = 0);
    ~ColorSetEditingWidget();

    void setColorPref(const style::ColorPref &pref);
    style::ColorPref colorPref() const;

    void enableColorListWidget(bool enabled);
signals:
    void colorPrefChangedByUser();
private slots:
    void treatColorPrefChanging();
private:
    Ui::ColorSetEditingWidget *ui;
    bool mIsColorListWidgetEnabled;

    void connectAll();
    void disconnectAll();
};

#endif // COLORSETEDITINGWIDGET_H
