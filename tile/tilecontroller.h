#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include <QWidget>
#include "widgettilemanager/widgettilemanager.h"

namespace Ui {
class TileController;
}

class TileController : public QWidget
{
    Q_OBJECT

public:
    explicit TileController(WidgetTileManager*, QWidget *parent = 0);
    ~TileController();

private slots:
    void on_addPushButton_clicked();

    void on_removePushButton_clicked();

private:
    Ui::TileController *ui;
    WidgetTileManager *tileManager;
};

#endif // TILECONTROLLER_H
