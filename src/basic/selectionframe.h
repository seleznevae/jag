#ifndef SELECTIONFRAME_H
#define SELECTIONFRAME_H

#include <QRubberBand>

class SelectionFrame : public QRubberBand
{
    Q_OBJECT
public:

    explicit SelectionFrame(Shape shape, QWidget *parent);

    void paintEvent(QPaintEvent *event);
    ~SelectionFrame();
signals:

public slots:

private:
    QColor border_color;
    QColor background_color;
};

#endif // SELECTIONFRAME_H
