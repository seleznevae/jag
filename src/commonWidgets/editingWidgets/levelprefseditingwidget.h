#ifndef LEVELPREFSEDITINGWIDGET_H
#define LEVELPREFSEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class LevelPrefsEditingWidget;
}

class LevelPrefsEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelPrefsEditingWidget(QWidget *parent = 0);
    ~LevelPrefsEditingWidget();

    void setLevelPrefs(const style::LevelPref &pref);
    style::LevelPref levelPrefs() const;

signals:
    void levelPrefsChanged(style::LevelPref);
private:
    Ui::LevelPrefsEditingWidget *ui;

    void connectAll();
    void disconnectAll();
private slots:
    void treatPropertiesChanging();
};

#endif // LEVELPREFSEDITINGWIDGET_H


//class GridEditingWidget : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit GridEditingWidget(QWidget *parent = 0);
//    ~GridEditingWidget();

//    void setHead(const QString& head);
//    void setGrid(style::Grid newGrid);
//    style::Grid grid() const;
//signals:
//    void gridChanged(style::Grid);
//private:
//    Ui::GridEditingWidget *ui;
//    style::Grid mGrid;

//    void connectAll();
//    void disconnectAll();

//private slots:
//    void treatPropertiesChanging();
//};
