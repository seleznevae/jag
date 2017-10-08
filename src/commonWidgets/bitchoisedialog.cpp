#include "bitchoisedialog.h"
#include "ui_bitchoisedialog.h"

BitChoiseDialog::BitChoiseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitChoiseDialog)
{
    ui->setupUi(this);

    for (int i = 0; i < 32; ++i) {
        QCheckBox *checkBox = new QCheckBox(QString::number(i) + tr(" bit"), this);
        ui->bitVerticalLayout->addWidget(checkBox);
        mCheckBoxVector.push_back(checkBox);
    }
}

BitChoiseDialog::~BitChoiseDialog()
{
    delete ui;
}

QVector<int> BitChoiseDialog::checkedBits() const
{
    QVector<int> ret;
    for (int i = 0; i < mCheckBoxVector.size(); ++i) {
        if (mCheckBoxVector[i]->isChecked()) {
            ret.push_back(i);
        }
    }
    return ret;
}

bool BitChoiseDialog::shouldCombineBits() const
{
    return ui->combineConsecutiveBitsCheckBox->isChecked();
}
