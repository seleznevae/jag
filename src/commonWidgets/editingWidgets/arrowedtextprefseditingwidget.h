#ifndef ARROWEDTEXTPREFSEDITINGWIDGET_H
#define ARROWEDTEXTPREFSEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"

namespace Ui {
class ArrowedTextPrefsEditingWidget;
}

class ArrowedTextPrefsEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArrowedTextPrefsEditingWidget(QWidget *parent = 0);
    ~ArrowedTextPrefsEditingWidget();

    void setArrowedTextPrefs(const style::ArrowedTextPref& prefs);
    style::ArrowedTextPref arrowedTextPrefs() const;

signals:
    void arrowedTextPrefChanged();

private:
    Ui::ArrowedTextPrefsEditingWidget *ui;

    void connectAll();
    void disconnectAll();

private slots:
    void treatArrowedTextPrefsChanging();
};

#endif // ARROWEDTEXTPREFSEDITINGWIDGET_H


