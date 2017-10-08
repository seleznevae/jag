#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class TipDialog;
}

/*!
 \brief TipDialog is  a dialog to show small tips to a user.

 User can view a current tip and go forward or backward through a list of tips.
*/
class TipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TipDialog(QWidget *parent = 0);
    ~TipDialog();

    void setTips(QStringList tips);

public slots:
    void nextTip();
    void prevTip();

private slots:
    void setBootTipShowing();
private:
    Ui::TipDialog *ui;
    QStringList mTipList;
    int mCurrentTip;
    void setCurrentTip(int index);
};

#endif // TIPDIALOG_H
