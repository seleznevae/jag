#ifndef PLOTSTYLECHOOSINGWIDGET_H
#define PLOTSTYLECHOOSINGWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "plotStyle/plotstyle.h"
#include "core/plotlayoutmodel.h"

namespace Ui {
class PlotStyleChoosingWidget;
}

class ArrowedText;

class PlotStyleChoosingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotStyleChoosingWidget(QWidget *parent = 0);
    ~PlotStyleChoosingWidget();

//    void setPlotStyle(const PlotStyle& style);
    PlotStyle plotStyle();
    void applyChanges();
private:
    Ui::PlotStyleChoosingWidget *ui;
    PlotLayoutModel mLayoutModel;

    QVector<PlotStyle> mDefaultStyles;
    QVector<PlotStyle> mUserDefinedStyles;
    QVector<QListWidgetItem*> mDefaultStylesItems;
    QVector<QListWidgetItem*> mUserDefinedStylesItems;
    QString mDefaultStyleName;
    ArrowedText *mArrowedText;

    void setCurrentStyle(const PlotStyle& style);
    void markDefaultPlotStyle();

private slots:
    void treatStyleModification();

    void addNewCustomPlotStyle();
    void removeUserPlotStyle();
    void treatCurrentItemChanging();
    void changeDefaultPlotStyle();

    void connectAll();
    void disconnectAll();

    void createPlotWithStyle(const PlotStyle&);
};

#endif // PLOTSTYLECHOOSINGWIDGET_H
