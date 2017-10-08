#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "global/global_definitions.h"
#include <QDialog>
#include "plotStyle/plotstyle.h"
#include <QVector>

namespace Ui {
class PreferencesDialog;
}

class QCheckBox;

/*!
 \brief Dialog that can is used to show and set manually basic JaG settings.

*/
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

signals:
    void preferenceSavingRequest();

public slots:
    void okRequest();
    void cancelRequest();
    void okAndSaveRequest();
    void setChooseDefaultDirectory();
    void setDefaultPreferences();

    void errorNumberEnabilityChanged();
private:
    Ui::PreferencesDialog *ui;
//    QVector<PlotStyle> mUserPlotStyle;
    QVector<QCheckBox*> mToolbarActionsCheckBoxesVector;

    void initializeStateFromPreferences();

    void applyChangesToPreferences();
};

#endif // PREFERENCESDIALOG_H
