#ifndef GRAPHMODELINFOWIDGET_H
#define GRAPHMODELINFOWIDGET_H

#include <QTextEdit>
#include "core/graphmodel.h"
#include "core/plotlayoutmodel.h"

class GraphModelInfoWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit GraphModelInfoWidget(QWidget *parent = 0);
    void setModel(GraphModel *newModel);

signals:

public slots:
    void update();


private:
    GraphModel *model;

    void connectWithModel();

};

#endif // GRAPHMODELINFOWIDGET_H
