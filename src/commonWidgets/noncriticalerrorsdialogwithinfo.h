#ifndef NONCRITICALERRORSDIALOGWITHINFO_H
#define NONCRITICALERRORSDIALOGWITHINFO_H

#include <QDialog>

namespace Ui {
class NonCriticalErrorsDialogWithInfo;
}


/*!
 \brief Simple message dialog that can contain and show if user wants some extra information (details of noncritical errors).

*/
class NonCriticalErrorsDialogWithInfo : public QDialog
{
    Q_OBJECT

public:
    explicit NonCriticalErrorsDialogWithInfo(QWidget *parent = 0);
    ~NonCriticalErrorsDialogWithInfo();

    bool showItNextTime() const;
    void setDetails(const QStringList& details);
    void setMessage(const QString& message);

//    void setAcceptText(const QString& text);
//    void setRejectText(const QString& text);
    void showShowItNextTimeCheckBox(bool show);

public slots:
    void showDetails();
private:
    Ui::NonCriticalErrorsDialogWithInfo *ui;
};

#endif // NONCRITICALERRORSDIALOGWITHINFO_H
