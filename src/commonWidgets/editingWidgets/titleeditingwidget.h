#ifndef TITLEEDITINGWIDGET_H
#define TITLEEDITINGWIDGET_H

#include <QWidget>
#include "plotStyle/plotstyle.h"
namespace Ui {
class TitleEditingWidget;
}

class TitleEditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TitleEditingWidget(QWidget *parent = 0);
    ~TitleEditingWidget();


    void setTitleProperties(const style::Label &title);
    void setTitle(const QString &title);

    style::Label titleProperties() const;
    QString title() const;
signals:
    void titleChanged();
    void titlePropertiesChanged();

private:
    Ui::TitleEditingWidget *ui;


    void connectAll();
    void disconnectAll();
};

#endif // TITLEEDITINGWIDGET_H
