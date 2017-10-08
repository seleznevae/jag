#ifndef COLORMAPEVALUATOR_H
#define COLORMAPEVALUATOR_H

#include <QObject>
#include <QColor>

class ColorMapEvaluator : public QObject
{
    Q_OBJECT
public:
    explicit ColorMapEvaluator(QObject *parent = 0);

    virtual QColor getColorForPoint(double x, double y) = 0;

signals:

public slots:
};

#endif // COLORMAPEVALUATOR_H
