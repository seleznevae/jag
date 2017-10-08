#include <QApplication>
#include <algorithm>

#include <QLabel>
#include "widgettilemanager/widgettilemanager.h"
#include "tilecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Utils::FancyMainWindow w;
    WidgetTileManager wtm;
    wtm.show();

    wtm.enableMoving(true);

    QLabel *lab1 = new QLabel("11111111");
    lab1->setFrameShadow(QFrame::Raised);
    lab1->setFrameShape(QFrame::Box);
    wtm.addWidget(lab1);

//    QLabel *lab2 = new QLabel("22222222");
//    lab2->setFrameShadow(QFrame::Raised);
//    lab2->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab2);

//    QLabel *lab3 = new QLabel("33333333");
//    lab3->setFrameShadow(QFrame::Raised);
//    lab3->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab3);

//    QLabel *lab4 = new QLabel("44444444");
//    lab4->setFrameShadow(QFrame::Raised);
//    lab4->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab4);

//    QLabel *lab5 = new QLabel("55555555");
//    lab5->setFrameShadow(QFrame::Raised);
//    lab5->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab5);

//    QLabel *lab6 = new QLabel("66666666");
//    lab6->setFrameShadow(QFrame::Raised);
//    lab6->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab6);

//    QLabel *lab7 = new QLabel("77777777");
//    lab7->setFrameShadow(QFrame::Raised);
//    lab7->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab7);

//    QLabel *lab8 = new QLabel("88888888");
//    lab8->setFrameShadow(QFrame::Raised);
//    lab8->setFrameShape(QFrame::Box);
//    wtm.addWidget(lab8);


    wtm.setCurrentWidget(lab1);
    wtm.markCurrentWidget(true);

    TileController controller(&wtm);
    controller.show();

//    wtm.removeWidget(lab2);

    return a.exec();
}
