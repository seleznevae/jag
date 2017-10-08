#ifndef MARKEDWIDGET_H
#define MARKEDWIDGET_H

#include <QWidget>

class MarkedWidget : public QWidget
{
    Q_OBJECT
public:

    enum class MarkingType { Frame,
                             Corner,
                             None
                           };
    explicit MarkedWidget(QWidget *parent = 0);

    QColor markingColor() const { return mMarkingColor; }
    MarkingType markingType() const { return mMarkingType; }

    void setMarkingColor(const QColor& color);
    void setMarkingType(MarkingType type);
signals:

private:
    void paintEvent(QPaintEvent *event);

public slots:

private:
    MarkingType mMarkingType;
    QColor mMarkingColor;
};

#endif // MARKEDWIDGET_H
