#ifndef EXTENDEDCONTROLWRAPPERDIALOG_H
#define EXTENDEDCONTROLWRAPPERDIALOG_H

#include <QDialog>

namespace Ui {
class ExtendedControlWrapperDialog;
}

class ExtendedControlWrapperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtendedControlWrapperDialog(QWidget *parent = 0);
    ~ExtendedControlWrapperDialog();

    void setWidget(QWidget *widget);
    QWidget* internalWidget();

private:
    Ui::ExtendedControlWrapperDialog *ui;
    QWidget *mWidget;
};

#endif // EXTENDEDCONTROLWRAPPERDIALOG_H
