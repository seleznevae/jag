#ifndef LABELPREFSEDITINGWIDGET_H
#define LABELPREFSEDITINGWIDGET_H

#include <QWidget>
#include "../plotStyle/plotstyle.h"

namespace Ui {
class LabelPrefsEditingWidget;
}

class LabelPrefsEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LabelPrefsEditingWidget(QWidget *parent = 0);
    ~LabelPrefsEditingWidget();

    void setLabelPrefs(const style::Label&);
    style::Label labelPrefs() const;

signals:
    void labelPrefsEdited();
private slots:
    void treatPrefsChanging();
private:
    Ui::LabelPrefsEditingWidget *ui;

    void connectAll();
    void disconnectAll();
};

#endif // LABELPREFSEDITINGWIDGET_H
