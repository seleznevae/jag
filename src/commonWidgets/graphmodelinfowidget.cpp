#include "commonWidgets/graphmodelinfowidget.h"

GraphModelInfoWidget::GraphModelInfoWidget(QWidget *parent) :
    QTextEdit(parent), model(NULL)
{
}

void GraphModelInfoWidget::setModel(GraphModel *newModel)
{
    model = newModel;
    connectWithModel();
    update();
}

void GraphModelInfoWidget::update()
{
    if (model != NULL) {
        QString text = "Name = " + model->getName();
        text += "\nwidth = " + QString::number(model->getPen().width());
        setText(text);
    }
}

void GraphModelInfoWidget::connectWithModel()
{
    connect(model, SIGNAL(appearanceChanged()), this, SLOT(update()));
    connect(model, SIGNAL(dataChanged()), this, SLOT(update()));
}
