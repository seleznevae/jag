#include "lineeditwithunicodedialog.h"
#include "ui_lineeditwithunicodedialog.h"
#include "commonWidgets/texteditdialog.h"

LineEditWithUnicodeDialog::LineEditWithUnicodeDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineEditWithUnicodeDialog)
{
    ui->setupUi(this);
    ui->horizontalLayout->setSpacing(0);
    ui->horizontalLayout->setMargin(0);

    connect(ui->lineEdit, SIGNAL(editingFinished()),
            this, SIGNAL(editingFinished()));
    connect(ui->toolButton, SIGNAL(clicked(bool)),
            this, SLOT(showUnicodeDialog()));
}

LineEditWithUnicodeDialog::~LineEditWithUnicodeDialog()
{
    delete ui;
}

void LineEditWithUnicodeDialog::setFont(const QFont &newFont)
{
    mFont = newFont;
//    ui->lineEdit->setFont(newFont);
}

void LineEditWithUnicodeDialog::clearFocus()
{
    ui->lineEdit->clearFocus();
    ui->toolButton->clearFocus();
}

QString LineEditWithUnicodeDialog::text() const
{
    return ui->lineEdit->text();
}

QFont LineEditWithUnicodeDialog::font() const
{
    return mFont;
}

void LineEditWithUnicodeDialog::setText(const QString &text)
{
    ui->lineEdit->setText(text);
}

void LineEditWithUnicodeDialog::showUnicodeDialog()
{
    TextEditDialog *dial = new TextEditDialog(qApp->font(), this);
    dial->setText(ui->lineEdit->text());
    dial->setFont(mFont);
    if (dial->exec() == QDialog::Accepted) {
        ui->lineEdit->setText(dial->text());
        emit editingFinished();
    }
    mFont = dial->font();
    dial->setParent(nullptr);
    delete dial;
}
