#ifndef COLORCHOISEWIDGET_H
#define COLORCHOISEWIDGET_H

#include <QWidget>

namespace Ui {
class ColorChoiseWidget;
}

class ColorChoiseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorChoiseWidget(QWidget *parent = 0);
    ~ColorChoiseWidget();


    void setColor(const QColor&);
    QColor color() const;

signals:
    void currentColorChanged(QColor);

protected:
    void addColorToScheme(const QColor&);
protected slots:
    void showColorDialog();

private:
    Ui::ColorChoiseWidget *ui;
};

#endif // COLORCHOISEWIDGET_H
