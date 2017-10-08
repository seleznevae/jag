#ifndef BITCHOISEDIALOG_H
#define BITCHOISEDIALOG_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class BitChoiseDialog;
}


/*!
 \brief Simple dialog in which user can choose which bits he would like to be retrieved from some unsigned parameter.

*/
class BitChoiseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitChoiseDialog(QWidget *parent = 0);
    ~BitChoiseDialog();

    QVector<int> checkedBits() const;
    bool shouldCombineBits() const;
private:
    Ui::BitChoiseDialog *ui;
    QVector<QCheckBox*> mCheckBoxVector;
};

#endif // BITCHOISEDIALOG_H
