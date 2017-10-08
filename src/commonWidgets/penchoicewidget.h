#ifndef PENCHOICEWIDGET_H
#define PENCHOICEWIDGET_H

#include <QWidget>
#include <QPen>

namespace Ui {
class PenChoiceWidget;
}

class PenChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PenChoiceWidget(QWidget *parent = 0);
    ~PenChoiceWidget();

    void setPen(const QPen &pen);
    QPen pen() const;
    void setHeader(const QString &header);

    void enableStyleSetting(bool enabled);
signals:
    void penChanged(QPen);
private:
    Ui::PenChoiceWidget *ui;

    void connectSlots();
    void disconnectSlots();

private slots:
    void treatPenChanging();
};

#endif // PENCHOICEWIDGET_H
