#include "extendedcontrolwrapperdialog.h"
#include "ui_extendedcontrolwrapperdialog.h"

ExtendedControlWrapperDialog::ExtendedControlWrapperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtendedControlWrapperDialog)
{
    ui->setupUi(this);
}

ExtendedControlWrapperDialog::~ExtendedControlWrapperDialog()
{
    delete ui;
}

void ExtendedControlWrapperDialog::setWidget(QWidget *widget)
{
    mWidget = widget;
    ui->verticalLayout->addWidget(widget);
}

QWidget *ExtendedControlWrapperDialog::internalWidget()
{
    return mWidget;
}
