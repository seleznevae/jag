#ifndef POSITIONCHOICEWIDGET_H
#define POSITIONCHOICEWIDGET_H

#include <QWidget>
#include "utility/utility.h"

namespace Ui {
class PositionChoiceWidget;
}

class PositionPainter;

enum class PositionType {
    posAxisRect,
    posTick,
    posSubTick
};

class PositionChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PositionChoiceWidget(QWidget *parent = 0);
    ~PositionChoiceWidget();

    void setPosition(utility::Positions position);
    utility::Positions position() const;
    void setPositionType(PositionType type);

signals:
    void positionChangedByUser();

private slots:
    void updatePositionPainter();

private:
    Ui::PositionChoiceWidget *ui;
    PositionPainter *mPositionPainter;

};

#endif // POSITIONCHOICEWIDGET_H
