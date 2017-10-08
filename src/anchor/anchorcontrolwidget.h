#ifndef ANCHORCONTROLWIDGET_H
#define ANCHORCONTROLWIDGET_H

#include <QWidget>
#include "anchor/anchor.h"


namespace Ui {
class AnchorControlWidget;
}

class AnchorControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnchorControlWidget(QWidget *parent = 0);
    ~AnchorControlWidget();

    void setAnchor(Anchor *anchor);

signals:
    void widgetIsHiding();
    void arrowedTextCreationRequest(double);

public slots:
    void treatNameChanging();
    void treatTimeChanging();
    void treatAnchorDeleting(QObject*);
    void treatArrowedTextCreationRequest();
    void treatSizeChanging();
    void treatStyleChanging();
    void treatBrushColorChanging(QColor);
    void treatBorderColorChanging(QColor);
    void treatBorderPenWidthChanging();

private:
    Ui::AnchorControlWidget *ui;
    Anchor* mAnchor;

    void initializeState();
};

#endif // ANCHORCONTROLWIDGET_H
