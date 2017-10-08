#include "tilecontroller.h"
#include "ui_tilecontroller.h"
#include <QLabel>
TileController::TileController(WidgetTileManager*man, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TileController), tileManager(man)
{
    ui->setupUi(this);
}

TileController::~TileController()
{
    delete ui;
}

void TileController::on_addPushButton_clicked()
{
    static int k = 5;
    QLabel *lab = new QLabel(QString::number(k));
    lab->setFrameShadow(QFrame::Raised);
    lab->setFrameShape(QFrame::Box);

//    lab->setStyleSheet("QLabel { color: white;  background:   rgb(6, 150, 224);    border-top-left-radius: 9px;"
//                                   "border-top-right-radius: 9px;"
//                                   "font: bold ;"
//                                   "font-size: 16px;  }");

    lab->setStyleSheet("QLabel { color: white;  background:   rgb(116, 150, 224);    border-top-left-radius: 9px;"
                                   "border-top-right-radius: 9px;"
                                   "font: bold ;"
                                   "font-size: 16px;  }");


    tileManager->addWidget(lab);
    k++;

}

void TileController::on_removePushButton_clicked()
{
    tileManager->removeWidget(tileManager->currentWidget());
}
