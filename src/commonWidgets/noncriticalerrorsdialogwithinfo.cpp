#include "noncriticalerrorsdialogwithinfo.h"
#include "ui_noncriticalerrorsdialogwithinfo.h"
#include <QPushButton>

NonCriticalErrorsDialogWithInfo::NonCriticalErrorsDialogWithInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NonCriticalErrorsDialogWithInfo)
{
    ui->setupUi(this);
    ui->detailsTextEdit->hide();

    connect(ui->detailsCheckBox, SIGNAL(clicked()),
            this, SLOT(showDetails()));

    auto yesButton = ui->acceptRejectButtonBox->button(QDialogButtonBox::Yes);
    connect(yesButton, SIGNAL(clicked()),
                this, SLOT(accept()));
    yesButton->setDefault(true);
    auto noButton = ui->acceptRejectButtonBox->button(QDialogButtonBox::No);
    connect(noButton, SIGNAL(clicked()),
                this, SLOT(reject()));
    noButton->setDefault(false);
//    connect(ui->acceptedPushButton, SIGNAL(clicked()),
//            this, SLOT(accept()));
//    connect(ui->rejectPushButton, SIGNAL(clicked()),
//            this, SLOT(reject()));




    QStyle *style = this->style() ? this->style() : QApplication::style();
    int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, this);
    QIcon tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
    ui->messageIconLabel->setPixmap(tmpIcon.pixmap(iconSize, iconSize));
//    switch (icon) {
//    case QMessageBox::Information:
//    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, mb);
//    break;
//    case QMessageBox::Warning:
//    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, mb);
//    break;
//    case QMessageBox::Critical:
//    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, mb);
//    break;
//    case QMessageBox::Question:
//    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mb);
//    default:
//    break;
//    }
//    if (!tmpIcon.isNull())
//    return tmpIcon.pixmap(iconSize, iconSize);
//    return QPixmap();
    updateGeometry ();
}

NonCriticalErrorsDialogWithInfo::~NonCriticalErrorsDialogWithInfo()
{
    delete ui;
}

bool NonCriticalErrorsDialogWithInfo::showItNextTime() const
{
    return !ui->showCheckBox->isChecked();
}

void NonCriticalErrorsDialogWithInfo::setDetails(const QStringList &details)
{

    ui->detailsTextEdit->clear();
    if (!details.isEmpty()) {
        ui->detailsTextEdit->setText("<ol><li>" + details.join("</li><li>") + "</ol>");
    }
}

void NonCriticalErrorsDialogWithInfo::setMessage(const QString &message)
{
    ui->messageLabel->setText(message);
}

//void NonCriticalErrorsDialogWithInfo::setAcceptText(const QString &text)
//{
//    ui->acceptedPushButton->setText(text);
//}

//void NonCriticalErrorsDialogWithInfo::setRejectText(const QString &text)
//{
//    ui->rejectPushButton->setText(text);
//}

void NonCriticalErrorsDialogWithInfo::showShowItNextTimeCheckBox(bool show)
{
    ui->showCheckBox->setVisible(show);
}

void NonCriticalErrorsDialogWithInfo::showDetails()
{
    if (ui->detailsCheckBox->isChecked()) {
        ui->detailsTextEdit->show();
    } else {
        ui->detailsTextEdit->hide();
    }
}


