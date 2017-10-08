#include "tipdialog.h"
#include "ui_tipdialog.h"
#include "global/global_definitions.h"

TipDialog::TipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipDialog)
{
    ui->setupUi(this);

    ui->showTipCheckBox->setChecked(global::gIsBootTipShowingEnabled);

    connect(ui->nextPushButtom, SIGNAL(clicked()),
            this, SLOT(nextTip()));
    connect(ui->prevPushButtom, SIGNAL(clicked()),
            this, SLOT(prevTip()));
    connect(ui->closePushButtom, SIGNAL(clicked()),
            this, SLOT(reject()));
    connect(ui->showTipCheckBox, SIGNAL(clicked()),
            this, SLOT(setBootTipShowing()));
}

TipDialog::~TipDialog()
{
    delete ui;
}

void TipDialog::setTips(QStringList tips)
{
    mTipList = tips;
    mCurrentTip = 0;
    setCurrentTip(mCurrentTip);
}

void TipDialog::nextTip()
{
    mCurrentTip++;
    if (mCurrentTip == mTipList.size())
        mCurrentTip = 0;
    setCurrentTip(mCurrentTip);
}

void TipDialog::prevTip()
{
    mCurrentTip--;
    if (mCurrentTip < 0)
        mCurrentTip = mTipList.size() - 1;
    setCurrentTip(mCurrentTip);
}

void TipDialog::setBootTipShowing()
{
    global::gIsBootTipShowingEnabled = ui->showTipCheckBox->isChecked();
}

void TipDialog::setCurrentTip(int index)
{
    if (index >= 0 &&index < mTipList.size()) {
        ui->tipTextEdit->clear();
//        ui->tipTextEdit->setText(mTipList[index]);
        ui->tipTextEdit->setHtml(mTipList[index]);
    }
}
