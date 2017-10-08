#ifndef MULTIMODELCONTROLWIDGETWRAPPER_H
#define MULTIMODELCONTROLWIDGETWRAPPER_H


#include "controlWidgets/multimodelcontrolwidget.h"
#include "controlwrappers/controlwidgetwrapper.h"




class MultiModelControlWidgetWrapper: public ControlWidgetWrapper
{
    Q_OBJECT
public:
    explicit MultiModelControlWidgetWrapper(QWidget *parent=0);
    ~MultiModelControlWidgetWrapper() { }

    void setModelVector(QVector<GraphModel*> newModelVector);
    void setLayoutModel(PlotLayoutModel *model);

public slots:
    void initializeStateFromModels();

    void treatModelDeleting();


private:
    MultiModelControlWidget *mControlWidgetCopy;
};

#endif // MULTIMODELCONTROLWIDGETWRAPPER_H




