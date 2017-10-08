#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QFrame>

class PopupWidget : public QFrame
{
    Q_OBJECT
public:
    explicit PopupWidget(QWidget *parent = nullptr);

    void setWidget(QWidget *widget);
    QWidget* widget() const noexcept { return mWidget; }

    void show(QPoint coord);
private:
    QWidget *mWidget;
};

#endif // POPUPWIDGET_H
