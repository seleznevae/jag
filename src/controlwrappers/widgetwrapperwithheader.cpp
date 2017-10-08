#include "widgetwrapperwithheader.h"
#include "ui_widgetwrapperwithheader.h"
#include "global/global_definitions.h"
#include "controlwrappers/extendedcontrolwrapperdialog.h"
#include "core/jagmainwindow.h"


WidgetWrapperWithHeader::WidgetWrapperWithHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetWrapperWithHeader)
{
    ui->setupUi(this);

    connect(ui->headerPushButton, SIGNAL(clicked(bool)),
            this, SLOT(showExtendedDialog()));
}

WidgetWrapperWithHeader::~WidgetWrapperWithHeader()
{
    delete ui;
}

void WidgetWrapperWithHeader::setHeaderText(const QString &header)
{
//    ui->headerLabel->setText(header);
    ui->headerPushButton->setText(header);
}

void WidgetWrapperWithHeader::setHeader(bool visible)
{
    if (visible) {
//        ui->headerLabel->show();
        ui->headerPushButton->show();
    } else {
//        ui->headerLabel->hide();
        ui->headerPushButton->hide();
    }
}

void WidgetWrapperWithHeader::setExpandable(bool)
{

}

void WidgetWrapperWithHeader::addWidget(QWidget *widget)
{
    mWidget = widget;
    ui->verticalLayout->addWidget(widget);
    ControlWidget* controlWidget = qobject_cast<ControlWidget*>(mWidget);
    if (controlWidget)
        controlWidget->setExtendedMode(false);
}

void WidgetWrapperWithHeader::paintEvent(QPaintEvent *event)
{
    switch (global::gDefaultColorScheme.scheme()) {
        case global::ColorScheme::Scheme::System:
//            ui->headerLabel->setStyleSheet("QLabel { color: white;  background:   rgb(6, 150, 224);    border-top-left-radius: 9px;"
//                               "border-top-right-radius: 9px;"
//                               "font: bold ;"
//                               "font-size: 16px;  }");
            ui->headerPushButton->setStyleSheet("QPushButton { color: white;  background:   rgb(6, 150, 224);    border-top-left-radius: 9px;"
                               "border-top-right-radius: 9px;"
                               "font: bold ;"
                               "padding: 3px;"
                                                "border: 1px solid navy;"
                               "font-size: 16px;  }");
            break;
//            "border: 1px solid navy;"//#8f8f91;"
//            "border-color: navy;"
//            "border-color: beige;"
//            "border-style: outset;"
        case global::ColorScheme::Scheme::DarkScheme:
//            ui->headerLabel->setStyleSheet("QLabel { color: white;  background:   rgb(6, 150, 224);    border-top-left-radius: 9px;"
//                               "border-top-right-radius: 9px;"
//                               "color:  #333333;"
//                               "font: bold ;"
//                               "font-size: 16px;  }");
            ui->headerPushButton->setStyleSheet("QPushButton { color: white;  background:   rgb(6, 150, 224);    border-top-left-radius: 9px;"
                               "border-top-right-radius: 9px;"
                               "color:  #333333;"
                               "font: bold ;"
                               "font-size: 16px;  }"
                                            );
            break;
        case global::ColorScheme::Scheme::DarkOrangeScheme:
//            ui->headerLabel->setStyleSheet("QLabel { color: white;  background:   #ffa02f;    border-top-left-radius: 9px;"
//                               "border-top-right-radius: 9px;"
//                               "color:  #444444;"
//                               "font: bold ;"
//                               "font-size: 16px;  }");
            ui->headerPushButton->setStyleSheet("QPushButton { color: white;  background:   #ffa02f;    border-top-left-radius: 9px;"
                               "border-top-right-radius: 9px;"
                               "color:  #444444;"
                               "font: bold ;"
                               "font-size: 16px;  }");
            break;
    }

    QWidget::paintEvent(event);
}

void WidgetWrapperWithHeader::showExtendedDialog()
{
    ui->headerPushButton->clearFocus();
    ExtendedControlWrapperDialog extDialog(global::gJagMainWindowPointer);
    extDialog.setWidget(mWidget);
    ControlWidget* controlWidget = qobject_cast<ControlWidget*>(mWidget);
    if (controlWidget)
        controlWidget->setExtendedMode(true);
    extDialog.exec();
    addWidget(mWidget);
    if (controlWidget)
        controlWidget->setExtendedMode(false);
    extDialog.setParent(nullptr);
}


