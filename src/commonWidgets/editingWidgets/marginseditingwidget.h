#ifndef MARGINSEDITINGWIDGET_H
#define MARGINSEDITINGWIDGET_H

#include <QWidget>

namespace Ui {
class MarginsEditingWidget;
}

class MarginsEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarginsEditingWidget(QWidget *parent = 0);
    ~MarginsEditingWidget();

    void setMargins(const QMargins&);
    QMargins margins() const;
private slots:
    void treatMarginsChanging();
signals:
    void marginsEdited(QMargins);

private:
    Ui::MarginsEditingWidget *ui;

    void connectAll();
    void disconnectAll();
};

#endif // MARGINSEDITINGWIDGET_H
