#include "plotshoweditwidget.h"
#include "ui_plotshoweditwidget.h"
#include "secondaryprocessing/datacontainer.h"
#include "secondaryprocessing/secondaryprocessing.h"
#include "secondaryprocessing/secondaryprocessingdialog.h"
#include "global/global_definitions.h"
#include <QMessageBox>
#include <QToolBar>
#include "commonWidgets/widgettilemanager/widgettilemanager.h"
#include "commonWidgets/fastsecondaryprocessingdialog.h"


PlotShowEditWidget::PlotShowEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotShowEditWidget), mCurrentLayoutModel(nullptr),
    mCurrentPlot(nullptr), isControlsVisible(true)
{
    ui->setupUi(this);

    ui->tileManagerWithToolbar->mTileManager->markCurrentWidget(true);

    ui->plotHierarchyWidget->setPlotShowEditWidget(this);

//    QVector<PlotLayoutModel*> mLayoutModelVector;
//    QVector<PlotWidgetWithToolbar*> mPlotVector;

//    PlotLayoutModel* newLayout = new PlotLayoutModel;
//    PlotWidgetWithToolbar *newPlot = new PlotWidgetWithToolbar;
//    mLayoutModelVector.push_back(newLayout);
//    mPlotVector.push_back(newPlot);

//    newPlot->mPlotWidget->setLayoutModel(newLayout);
//    ui->plotTileManager->addWidget(newPlot);

//    makeCurrentPair(newLayout, newPlot);

    addNewPlotFrame();

    setPlotWidgetState(PlotWidget::PlotWidgetMode::Common);

//    ui->plotHierarchyWidget->setPlotLayoutModel(newLayout);
//    ui->plotControlWidget->setLayoutModel(newLayout);



//    ui->plotWithToolsWidget->mPlotWidget->setLayoutModel(&mLayoutModel);
//    ui->plotHierarchyWidget->setPlotLayoutModel(&mLayoutModel);
//    ui->plotControlWidget->setLayoutModel(&mLayoutModel);

    /*
//    QToolBar *tb = new QToolBar();
//    tb->addWidget(new QLabel("asdfsadf"));
//    ui->splitter_2->addWidget(tb);

//        QToolBar *tb = new QToolBar();
//        ui->splitter_2->addWidget(tb);
//    auto actions = global::gJagMainWindowPointer->actions();
//    for (auto action : actions)
//        tb->addAction(action);
*/

    connect(ui->plotHierarchyWidget, SIGNAL(graphActivated(GraphModel*)),
            this, SLOT(treatGraphEditRequest(GraphModel *)));
    connect(ui->plotHierarchyWidget, SIGNAL(graphVectorActivated(QVector<GraphModel*>)),
            this, SLOT(treatGraphVectorEditRequest(QVector<GraphModel*>)));
    connect(ui->plotHierarchyWidget, SIGNAL(axesVectorActivated(QVector<AxisModel*> )),
            this, SLOT(treatAxisEditRequest(QVector<AxisModel*> )));
    connect(ui->plotHierarchyWidget, SIGNAL(timeEventActivated(TimeEvent*)),
            this, SLOT(treatCommandEditRequest(TimeEvent *)));
    connect(ui->plotHierarchyWidget, SIGNAL(anchorActivated(Anchor*)),
            this, SLOT(treatAnchorEditRequest(Anchor*)));
    connect(ui->plotHierarchyWidget, SIGNAL(arrowedTextActivated(ArrowedText*)),
            this, SLOT(treatArrowedTextEditRequest(ArrowedText*)));
    connect(ui->plotHierarchyWidget, SIGNAL(horizontalLevelActivated(HorizontalLevel*)),
            this, SLOT(treatHorizontalLevelEditRequest(HorizontalLevel*)));

//    connect(ui->plotControlWidget, SIGNAL(settingXRangeRequest(double, double)),
//            &mLayoutModel, SLOT(setXRangeWrapper(double,double)));
//    connect(&mLayoutModel, SIGNAL(layoutModelChanged()),
//            ui->plotControlWidget, SLOT(initializeState()));


//    connect(ui->plotControlWidget, SIGNAL(legendChangingVisibilityRequest(bool)),
//            &mLayoutModel, SLOT(setLegendVisibility(bool)));
//    connect(ui->plotControlWidget, SIGNAL(dimensionChangingVisibilityRequest(bool)),
//            &mLayoutModel, SLOT(setDimensionVisibility(bool)));

//    connect(ui->eventControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
//            &mLayoutModel, SLOT(addArrowedTextByTime(double)));
//    connect(ui->anchorControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
//            &mLayoutModel, SLOT(addArrowedTextByTime(double)));

//    connect(ui->plotWithToolsWidget->mPlotWidget, SIGNAL(currentCoordinates(double, double)),
//            this, SLOT(emitInfoString(double,double)));

    connect(ui->plotHierarchyWidget, SIGNAL(minimumWidth(int)),
            ui->plotControlWidget, SLOT(setMinWidth(int)));

    connect(ui->tileManagerWithToolbar->mTileManager, SIGNAL(currentWidgetChanged(QWidget*)),
            this, SLOT(treatCurrentPlotFrameChanging(QWidget*)));

//    ui->multiModelControlWidget->setLayoutModel(&mLayoutModel);

    ui->axisControlWidget->hide();
    ui->eventControlWidget->hide();
    ui->anchorControlWidget->hide();
    ui->multiModelControlWidget->hide();
    ui->arrowedTextControlwidget->hide();
    ui->horizontalLevelControlWidget->hide();
}

PlotShowEditWidget::~PlotShowEditWidget()
{
    for (auto layout : mLayoutModelVector) {
        delete layout;
    }
    for (auto plot : mPlotWidgetVector) {
        delete plot;
    }
    delete ui;
}

QPixmap PlotShowEditWidget::toPixmap()
{
//    return ui->plotWithToolsWidget->mPlotWidget->toPixmap();
    if (mCurrentPlot) {
        if (mPlotWidgetVector.size() == 1 && !global::gIsFooterVisible) {
            return mCurrentPlot->toPixmap();
        } else {

            int extraHeightForFooter = global::gIsFooterVisible ? QFontMetrics(global::gFooterFont).height() + QFontMetrics(global::gFooterFont).descent(): 0;

//            QPixmap result(ui->tileManagerWithToolbar->mTileManager->width(), ui->tileManagerWithToolbar->mTileManager->height());
            QPixmap result(ui->tileManagerWithToolbar->mTileManager->width(), ui->tileManagerWithToolbar->mTileManager->height()+(extraHeightForFooter));
            //TODO: replace global::gDefaultPlotBackgroundColor to something better
            result.fill(mCurrentLayoutModel->plotStyle().mColorPref.mBackgroundColor);
            QCPPainter painter;
            painter.begin(&result);
            int mainPictureHeight = 0;
            int mainPictureWidth = 0;
            for (auto plot : mPlotWidgetVector) {
//                painter.drawPixmap(ui->tileManagerWithToolbar->mTileManager->mapFromGlobal(plot->mapToGlobal(QPoint(0,0))), plot->toPixmap());
                QPoint point = ui->tileManagerWithToolbar->mTileManager->mapFromGlobal(plot->mapToGlobal(QPoint(0,0)));
                QPixmap pixmap = plot->toPixmap();
                painter.drawPixmap(point, pixmap);
                mainPictureWidth = std::max(mainPictureWidth, point.x() + pixmap.width());
                mainPictureHeight = std::max(mainPictureHeight, point.y() + pixmap.height());

            }

            if (global::gIsFooterVisible) {
                QString leftFooter = global::convertFooterString(global::gLeftFooterString);
                QString centralFooter = global::convertFooterString(global::gCentralFooterString);
                QString rightFooter = global::convertFooterString(global::gRightFooterString);

                painter.setFont(global::gFooterFont);
                painter.drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignLeft | Qt::AlignVCenter, leftFooter);
                painter.drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignCenter | Qt::AlignVCenter, centralFooter);
                painter.drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignRight | Qt::AlignVCenter, rightFooter);
            }

            painter.end();
            return result;
        }
    } else {
        return QPixmap();
    }
}

QPixmap PlotShowEditWidget::toPixmap(int width, int height)
{
    if (mCurrentPlot) {
        return mCurrentPlot->toPixmap(width, height);
    } else {
        return QPixmap();
    }
//    return ui->plotWithToolsWidget->mPlotWidget->toPixmap(width, height);
}

void PlotShowEditWidget::toPainter(QCPPainter *painter, int width, int height)
{
    if (mCurrentPlot) {
        if (mPlotWidgetVector.size() == 1 && !global::gIsFooterVisible) {
            mCurrentPlot->toPainter(painter, width, height);
        } else {
            double ratio = double(width) / ui->tileManagerWithToolbar->mTileManager->width();
            int extraHeightForFooter = global::gIsFooterVisible ? QFontMetrics(global::gFooterFont).height() + QFontMetrics(global::gFooterFont).descent(): 0;
            int mainPictureHeight = 0;
            int mainPictureWidth = 0;
            for (auto plot : mPlotWidgetVector) {
//                qDebug() << ui->tileManagerWithToolbar->mTileManager->mapFromGlobal(plot->mapToGlobal(QPoint(0,0)))*ratio;
//                painter->drawPixmap(ui->tileManagerWithToolbar->mTileManager->mapFromGlobal(plot->mapToGlobal(QPoint(0,0)))*ratio, plot->toPixmap(plot->width()*ratio, plot->height()*ratio));
                QPoint point = ui->tileManagerWithToolbar->mTileManager->mapFromGlobal(plot->mapToGlobal(QPoint(0,0)))*ratio;
                QPixmap pixmap = plot->toPixmap(plot->width()*ratio, plot->height()*ratio);
                painter->drawPixmap(point, pixmap);
                mainPictureWidth = std::max(mainPictureWidth, point.x() + pixmap.width());
                mainPictureHeight = std::max(mainPictureHeight, point.y() + pixmap.height());
            }

            if (global::gIsFooterVisible) {
                QString leftFooter = global::convertFooterString(global::gLeftFooterString);
                QString centralFooter = global::convertFooterString(global::gCentralFooterString);
                QString rightFooter = global::convertFooterString(global::gRightFooterString);

                painter->setFont(global::gFooterFont);
                painter->drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignLeft | Qt::AlignVCenter, leftFooter);
                painter->drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignCenter | Qt::AlignVCenter, centralFooter);
                painter->drawText(QRect(0, mainPictureHeight, mainPictureWidth, extraHeightForFooter),  Qt::AlignRight | Qt::AlignVCenter, rightFooter);
            }


        }
    }
    return;
}

PlotWidget::PlotWidgetMode PlotShowEditWidget::plotWidgetState() const
{
    if (mCurrentPlot) {
        return mCurrentPlot->plotWidgetState();
    } else {
        return PlotWidget::PlotWidgetMode();
    }
//    return ui->plotWithToolsWidget->mPlotWidget->plotWidgetState();
}

const PlotWidget *PlotShowEditWidget::plotWidget() const
{
    if (mCurrentPlot) {
        return mCurrentPlot;
    } else {
        return nullptr;
    }
//    return ui->plotWithToolsWidget->mPlotWidget;
}

GraphModel* PlotShowEditWidget::addNewGraph(const QVector<double> &x, const QVector<double> &y, const QString &name)
{
    if (mCurrentLayoutModel) {
        return mCurrentLayoutModel->addPlot(x, y, name);;
    } else {
        return nullptr;
    }
//    return mLayoutModel.addPlot(x, y, name);
}

void PlotShowEditWidget::addNewEvent(const TimeEvent *event)
{
    if (mCurrentLayoutModel) {
        mCurrentLayoutModel->addCommand(event);
    }
    return;
    //    mLayoutModel.addCommand(event);
}

void PlotShowEditWidget::addNewPlotFrame(Qt::Orientation orientation)
{
    PlotLayoutModel* newLayout = new PlotLayoutModel;
    addNewLayout(newLayout, orientation);

//    PlotWidget *newPlot = new PlotWidget;
//    mLayoutModelVector.push_back(newLayout);
//    mPlotWidgetVector.push_back(newPlot);

//    newPlot->setLayoutModel(newLayout);

////    if (mCurrentPlot) {
////        newPlot->setGeometry(mCurrentPlot->geometry());

////    }
//    ui->tileManagerWithToolbar->mTileManager->addWidget(newPlot, orientation);
//    makeCurrentPair(newLayout, newPlot);
//    newLayout->setPreferences(mPreferences);

}

void PlotShowEditWidget::cloneCurrentPlotFrame(Qt::Orientation orientation)
{
    if (mLayoutModelVector.size() == 0 || mCurrentLayoutModel == nullptr)
        return;

    PlotLayoutModel* newLayout = mCurrentLayoutModel->clone();
    addNewLayout(newLayout, orientation);

    mCurrentLayoutModel->fixLayoutChanging();
}

void PlotShowEditWidget::removeLayoutModel(PlotLayoutModel *layoutModel)
{
    int index = mLayoutModelVector.indexOf(layoutModel);
    if (index >= 0 && index < mLayoutModelVector.size()) {
        ui->tileManagerWithToolbar->mTileManager->removeWidget(mPlotWidgetVector[index]);
        delete mLayoutModelVector[index];
        mPlotWidgetVector.remove(index);
        mLayoutModelVector.remove(index);
    }
}

void PlotShowEditWidget::treatGraphEditRequest(GraphModel *model)
{

    setControlWidgetsVisible(false);
    ui->multiModelControlWidget->show();
    ui->multiModelControlWidget->setModelVector(QVector<GraphModel *>{model});
}

void PlotShowEditWidget::treatGraphVectorEditRequest(QVector<GraphModel *>graphVector)
{


    setControlWidgetsVisible(false);
    ui->multiModelControlWidget->show();
    ui->multiModelControlWidget->setModelVector(graphVector);

}

void PlotShowEditWidget::treatAxisEditRequest(QVector<AxisModel*> models)
{

    setControlWidgetsVisible(false);
    ui->axisControlWidget->show();
    ui->axisControlWidget->setAxisModels(models);
}

void PlotShowEditWidget::treatCommandEditRequest(TimeEvent *command)
{

    setControlWidgetsVisible(false);
    ui->eventControlWidget->show();
    ui->eventControlWidget->setTimeEvent(command);
}

void PlotShowEditWidget::treatAnchorEditRequest(Anchor *anchor)
{

    setControlWidgetsVisible(false);
    ui->anchorControlWidget->show();
    ui->anchorControlWidget->setAnchor(anchor);

}

void PlotShowEditWidget::treatArrowedTextEditRequest(ArrowedText *ArrowedText)
{

    setControlWidgetsVisible(false);
    ui->arrowedTextControlwidget->show();
    ui->arrowedTextControlwidget->setArrowedText(ArrowedText);
}

void PlotShowEditWidget::treatHorizontalLevelEditRequest(HorizontalLevel *level)
{
    setControlWidgetsVisible(false);
    ui->horizontalLevelControlWidget->show();
    ui->horizontalLevelControlWidget->setHorizontalLevel(level);
}

void PlotShowEditWidget::treatFastSecondaryProcessingRequest(double start, double finish, int indexOfVisibleAxis)
{
    PlotWidget *widget = qobject_cast<PlotWidget*>(sender());
    int index = mPlotWidgetVector.indexOf(widget);
    if (index >= 0) {
        //recieving a vector of GraphModels to deal with
        PlotLayoutModel *currentLayoutModel = mLayoutModelVector[index];
        QVector<AxisModel*> visibleAxes = currentLayoutModel->getVisibleAxisModelVector();
        if (indexOfVisibleAxis < 0 || indexOfVisibleAxis >= visibleAxes.size())
            return;
        AxisModel* axisModel = visibleAxes[indexOfVisibleAxis];
        QVector<const GraphModel*> graphs = axisModel->getFunctionModelVector();
        QVector<const GraphModel*> visibleGraphs;
        std::copy_if(graphs.begin(), graphs.end(), std::back_inserter(visibleGraphs),
                     [](const GraphModel* graph) { return graph->isVisible();});


        //recieving FastSecondRequest
        global::FastSecondaryProcessingRequest fastSeqProcRequest;
        if (global::gFastSecondaryProcessingVector.size() == 0) {
            return;
        } else {
            FastSecondaryProcessingDialog dial;
            if (dial.exec() == QDialog::Accepted) {
                fastSeqProcRequest = dial.request();
            } else {
                return;
            }
        }

        //implementing secondary processing on each graph
        DataContainer::setConstraints(start, finish);
        QVector<DataContainer> resultDataContainers;
        QVector<QString> newGraphsNames;
        std::string functionString = fastSeqProcRequest.mFunctionString.toStdString();
        for (int i = 0; i < visibleGraphs.size(); ++i) {
            global::gCurrGraphVector.clear();
            global::gCurrGraphVector.push_back(visibleGraphs[i]);
            try {
                auto beg = functionString.begin();
                DataContainer result;
                if (parse_func_string_and_count(beg, functionString.end(), result) && beg == functionString.end()) {
                    resultDataContainers.push_back(result);
                    //creating name for new graphs
                    QString newGraphName = fastSeqProcRequest.mFunctionString;
                    newGraphName.replace(QRegExp("g[0123456789]*"), visibleGraphs[i]->getName());
                    newGraphsNames.push_back(newGraphName);
                }
            } catch (...) {
                ;
            }

            global::gCurrGraphVector.clear();
        }
        DataContainer::setDefaultConstraints();


        //adding results to the plot
        if (resultDataContainers.size() == 0)
            return;
        bool copyPropsOfGraphsEnabled = resultDataContainers.size() == visibleGraphs.size() ? true : false;
        auto copyPropOfGraphs = [copyPropsOfGraphsEnabled] (GraphModel* newGraph, const GraphModel *oldGraph){
            if (copyPropsOfGraphsEnabled == false)
                return;
            newGraph->setColor(oldGraph->getPen().color());
            newGraph->setWidth(oldGraph->getPen().widthF());
        };
        if (fastSeqProcRequest.mNewGraphLocation == global::FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame) {
            for (int i = 0; i < resultDataContainers.size(); ++i) {
                GraphModel *newGraph = new GraphModel;
                newGraph->setOriginData(resultDataContainers[i].mXData, resultDataContainers[i].mYData);
                newGraph->setName(newGraphsNames[i]);
                //TODO:: set properties of newGraph
                if (resultDataContainers[i].mType == DataContainer::GraphType::Function)
                    newGraph->setGraphType(GraphModel::GraphType::Function);
                else
                    newGraph->setGraphType(GraphModel::GraphType::Curve);
                copyPropOfGraphs(newGraph, visibleGraphs[i]);
                if (i == 0) {
                    currentLayoutModel->moveExternalGraphToNewAxis(newGraph, 1);
                } else {
                    currentLayoutModel->addExternalGraphToAxis(newGraph, currentLayoutModel->getAxisModelVector().back());
                }
            }
        } else {
            auto newLocation = fastSeqProcRequest.mNewGraphLocation == global::FastSecondaryProcessingRequest::NewGraphLocation::NewRightPlotFrame ?
                        Qt::Horizontal : Qt::Vertical;
            addNewPlotFrame(newLocation);
            for (int i = 0; i < resultDataContainers.size(); ++i) {
                auto newGraph = mCurrentLayoutModel->addPlot(resultDataContainers[i].mXData, resultDataContainers[i].mYData, "");
                newGraph->setName(newGraphsNames[i]);
                if (resultDataContainers[i].mType == DataContainer::GraphType::Curve)
                    newGraph->setGraphType(GraphModel::GraphType::Curve);
                copyPropOfGraphs(newGraph, visibleGraphs[i]);
                mCurrentLayoutModel->setLayoutState(PlotLayoutModel::LayoutState::AllInOne);
            }
        }
    }
}

void PlotShowEditWidget::treatCurrentPlotFrameChanging(QWidget *currentWidget)
{
    if (currentWidget) {
        if (PlotWidget *currentPlot = qobject_cast<PlotWidget*>(currentWidget)) {
            int index = mPlotWidgetVector.indexOf(currentPlot);
            if (index >= 0 && index < mPlotWidgetVector.size()) {
                if (mCurrentPlot != currentPlot) {
                    setControlWidgetsVisible(false);
                    mCurrentPlot = currentPlot;
                    mCurrentLayoutModel = mLayoutModelVector[index];
                    makeCurrentPair(mCurrentLayoutModel, mCurrentPlot);
                    emit currentPlotFrameChanged();
                }
            }
        }
    }
}


void PlotShowEditWidget::autoscale()
{
    if (mCurrentLayoutModel) {
        mCurrentLayoutModel->autoscaleXRange();
        mCurrentLayoutModel->autoscaleYRange();
    }
    return;

//    mLayoutModel.autoscaleXRange();
//    mLayoutModel.autoscaleYRange();
}

void PlotShowEditWidget::setPlotWidgetState(PlotWidget::PlotWidgetMode state)
{
    if (mCurrentLayoutModel) {
        for (auto plot : mPlotWidgetVector)
            plot->setPlotWidgetState(state);
    }
    if (state == PlotWidget::PlotWidgetMode::Common) {
        ui->tileManagerWithToolbar->mTileManager->enableMoving(true);
    } else {
        ui->tileManagerWithToolbar->mTileManager->enableMoving(false);
    }
    return;

//    ui->plotWithToolsWidget->mPlotWidget->setPlotWidgetState(state);
}

void PlotShowEditWidget::combineGraphs()
{
    if (mCurrentLayoutModel) {
        mCurrentLayoutModel->setLayoutState(PlotLayoutModel::LayoutState::AllInOne);
    }
    return;
//    mLayoutModel.setLayoutState(PlotLayoutModel::LayoutState::AllInOne);
}

void PlotShowEditWidget::separateGraphs()
{
    if (mCurrentLayoutModel) {
        mCurrentLayoutModel->setLayoutState(PlotLayoutModel::LayoutState::EachSeparate);
    }
    return;
//    mLayoutModel.setLayoutState(PlotLayoutModel::LayoutState::EachSeparate);
}

void PlotShowEditWidget::composeIntellectually()
{
    if (mCurrentLayoutModel) {
        mCurrentLayoutModel->composeIntellectually();
    }
    return;
//    mLayoutModel.composeIntellectually();
}

void PlotShowEditWidget::moveCurrentObject(AxisModel::AxisMovement motion)
{
    if (mCurrentLayoutModel) {
        AxisModel* currentAxis = NULL;
        GraphModel* currentGraph = NULL;
        if ((currentAxis = ui->plotHierarchyWidget->currentAxis()) != NULL) {
            mCurrentLayoutModel->moveAxisModel(currentAxis, motion);
        } else if ((currentGraph = ui->plotHierarchyWidget->currentGraph()) != NULL) {
            GraphModel::GraphMovement graphMotion = (motion == AxisModel::AxisMovement::Up) ?
                        GraphModel::GraphMovement::LongUp : GraphModel::GraphMovement::LongDown;
            mCurrentLayoutModel->moveGraphModel(currentGraph, graphMotion);
        }
    }
    return;

//    AxisModel* currentAxis = NULL;
//    GraphModel* currentGraph = NULL;
//    if ((currentAxis = ui->plotHierarchyWidget->currentAxis()) != NULL) {
//        mLayoutModel.moveAxisModel(currentAxis, motion);
//    } else if ((currentGraph = ui->plotHierarchyWidget->currentGraph()) != NULL) {
//        GraphModel::GraphMovement graphMotion = (motion == AxisModel::AxisMovement::Up) ?
//                    GraphModel::GraphMovement::LongUp : GraphModel::GraphMovement::LongDown;
//        mLayoutModel.moveGraphModel(currentGraph, graphMotion);
//    }
}

void PlotShowEditWidget::stretchCurrentAxis(double factor)
{
    AxisModel* currentAxis = NULL;
    if ((currentAxis = ui->plotHierarchyWidget->currentAxis()) != NULL) {
        double currentStretch = currentAxis->stretchFactor();
        currentAxis->setStretchFactor(currentStretch*factor);
        ui->axisControlWidget->initializeState();
    }
}

void PlotShowEditWidget::increaseDecimationOfCurrentGraph(double factor)
{

//    GraphModel* currentGraph = NULL;
//    if ((currentGraph = ui->plotHierarchyWidget->currentGraph()) != NULL) {
//        QCPScatterStyle scatter = currentGraph->getScatterStyle();
//        int currentDecimation = scatter.decimation();
//        currentDecimation *= factor;
//        if (currentDecimation <= 0)
//            currentDecimation = 1;
//        currentGraph->setScatterDecimation(currentDecimation);
//        ui->modelControlWidget->setModel(currentGraph);
//    }

    auto selectedGraphs = ui->plotHierarchyWidget->selectedGraphs();
    for (auto graph : selectedGraphs) {
        QCPScatterStyle scatter = graph->getScatterStyle();
        int currentDecimation = scatter.decimation();
        currentDecimation *= factor;
        if (currentDecimation <= 0)
            currentDecimation = 1;
        graph->setScatterDecimation(currentDecimation);
//        ui->modelControlWidget->setModel(graph);
        ui->multiModelControlWidget->setModelVector({graph});
    }
}

void PlotShowEditWidget::deleteCurrentObject()
{
    if (mCurrentLayoutModel) {
        AxisModel* currentAxis = NULL;
        GraphModel* currentGraph = NULL;
        if ((currentAxis = ui->plotHierarchyWidget->currentAxis()) != NULL) {
            mCurrentLayoutModel->removeAxisModel(currentAxis);
        } else if ((currentGraph = ui->plotHierarchyWidget->currentGraph()) != NULL) {
            mCurrentLayoutModel->removeGraphModel(currentGraph);
        }
    }
    return;

//    AxisModel* currentAxis = NULL;
//    GraphModel* currentGraph = NULL;
//    if ((currentAxis = ui->plotHierarchyWidget->currentAxis()) != NULL) {
//        mLayoutModel.removeAxisModel(currentAxis);
//    } else if ((currentGraph = ui->plotHierarchyWidget->currentGraph()) != NULL) {
//        mLayoutModel.removeGraphModel(currentGraph);
//    }
}

void PlotShowEditWidget::increaseWidthOfCurrentGraph(double factor)
{

    auto selectedGraphs = ui->plotHierarchyWidget->selectedGraphs();
    if (selectedGraphs.size() > 0) {
        for (auto graph : selectedGraphs) {
            auto pen = graph->getPen();
            pen.setWidthF(pen.widthF() * factor);
            graph->setPen(pen);
    //        ui->modelControlWidget->initializeStateFromModel();
        }
        ui->multiModelControlWidget->initializeStateFromModels();
    }

}

void PlotShowEditWidget::switchLineScatterOfCurrentGraph(int deviation)
{

    auto selectedGraphs = ui->plotHierarchyWidget->selectedGraphs();
    for (auto graph : selectedGraphs) {
        QCPScatterStyle scatterStyle = graph->getScatterStyle();
        QCPScatterStyle::ScatterShape scatterShape = scatterStyle.shape();
        QVector<QCPScatterStyle::ScatterShape> scatterShapeVector = {
                                                    QCPScatterStyle::ScatterShape::ssNone,
                                                    QCPScatterStyle::ScatterShape::ssDot,
                                                    QCPScatterStyle::ScatterShape::ssCross,
                                                    QCPScatterStyle::ScatterShape::ssPlus,
                                                    QCPScatterStyle::ScatterShape::ssCircle,
                                                    QCPScatterStyle::ScatterShape::ssDisc,
                                                    QCPScatterStyle::ScatterShape::ssSquare,
                                                    QCPScatterStyle::ScatterShape::ssDiamond,
                                                    QCPScatterStyle::ScatterShape::ssStar,
                                                    QCPScatterStyle::ScatterShape::ssTriangle,
                                                    QCPScatterStyle::ScatterShape::ssTriangleInverted,
                                                    QCPScatterStyle::ScatterShape::ssCrossSquare,
                                                    QCPScatterStyle::ScatterShape::ssPlusSquare,
                                                    QCPScatterStyle::ScatterShape::ssCrossCircle,
                                                    QCPScatterStyle::ScatterShape::ssPlusCircle
                                                    };
        int index = scatterShapeVector.indexOf(scatterShape);
        if (deviation > 0) {
            scatterStyle.setShape(scatterShapeVector[index + 1 < scatterShapeVector.size() ? index + 1 : 0]);
        } else
            scatterStyle.setShape(scatterShapeVector[index - 1 >= 0 ? index - 1 : scatterShapeVector.size() - 1]);
        graph->setScatterStyle(scatterStyle);
//        ui->modelControlWidget->initializeStateFromModel();
        ui->multiModelControlWidget->initializeStateFromModels();
    }
}

void PlotShowEditWidget::switchLineColorOfCurrentGraph(int deviation)
{

    auto selectedGraphs = ui->plotHierarchyWidget->selectedGraphs();
    for (auto graph : selectedGraphs) {
        QPen pen = graph->getPen();
        QColor color = pen.color();
        QVector<QColor> lineColorVector = {
                                            Qt::blue,
                                            Qt::green,
                                            Qt::red,
                                            Qt::magenta,
                                            Qt::black,
                                            Qt::darkGreen,
                                            Qt::darkYellow,
                                            Qt::darkMagenta,
                                            Qt::darkCyan,
                                           // Qt::darkYellow,  //doesn't work,   why i don't know
                                            Qt::darkRed,
                                            Qt::darkGray,
                                            Qt::cyan,
                                            Qt::yellow,
                                            Qt::darkBlue
                                           };
        int index = lineColorVector.indexOf(color);
        if (deviation > 0) {
            graph->setColor(lineColorVector[(index + 1 < lineColorVector.size()) ? (index + 1) : 0]);
        } else {
            graph->setColor(lineColorVector[index - 1 >= 0 ? index - 1 : lineColorVector.size() - 1]);
        }
//        ui->modelControlWidget->initializeStateFromModel();
        ui->multiModelControlWidget->initializeStateFromModels();
    }
}

void PlotShowEditWidget::switchLineInterpolationOfCurrentGraph(int deviation)
{


    auto selectedGraphs = ui->plotHierarchyWidget->selectedGraphs();
    for (auto graph : selectedGraphs) {
        GraphModel::LineStyle graphInterpolation = graph->getLineInterpolation();
        //reduces set of interpolation types
        QVector<GraphModel::LineStyle> lineInterpolationVector = {
                                                        GraphModel::LineStyle::lsLine,
                                                        GraphModel::LineStyle::lsStepLeft
                                                        };
        int index = lineInterpolationVector.indexOf(graphInterpolation);
        if (index != -1) {
            if (deviation > 0) {
                graph->setLineInterpolation(lineInterpolationVector[(index + 1 < lineInterpolationVector.size()) ? (index + 1) : 0]);
            } else {
                graph->setLineInterpolation(lineInterpolationVector[index - 1 >= 0 ? index - 1 : lineInterpolationVector.size() - 1]);
            }
        } else {
            graph->setLineInterpolation(GraphModel::LineStyle::lsLine);
        }
//        treatGraphEditRequest(currentGraph);
//        ui->modelControlWidget->initializeStateFromModel();
        ui->multiModelControlWidget->initializeStateFromModels();
    }
}

bool PlotShowEditWidget::toogleFullScreen()
{
    if (mCurrentPlot) {
//        return mCurrentPlot->toogleFullScreen();
        return ui->tileManagerWithToolbar->toogleFullScreen();
    } else {
        return false;
    }

//    return ui->plotWithToolsWidget->toogleFullScreen();
}

void PlotShowEditWidget::setPreferences(const PlotStyle &pref)
{
    if (mPreferences != pref) {
        mPreferences = pref;
        for (auto layout : mLayoutModelVector) {
            layout->setPlotStyle(mPreferences);
        }
    }
//    if (mCurrentLayoutModel) {
//        return mCurrentLayoutModel->setPreferences(pref);
//    }
}

void PlotShowEditWidget::emitInfoString(double x, double y)
{

    if (mCurrentLayoutModel) {
        emit infoStringEmitted(mCurrentLayoutModel->getInfoString(x, y));
    }
}

void PlotShowEditWidget::savePng(QString fileName)
{
    if (mCurrentPlot) {
        if (mPlotWidgetVector.size() == 1 && !global::gIsFooterVisible) {
            mCurrentPlot->savePng(fileName);
        } else {
            saveRastered(fileName, "PNG");
        }
    }
    return;
//    ui->plotWithToolsWidget->mPlotWidget->savePng(fileName);
//    return saveRastered(fileName, width, height, scale, "PNG", quality);


}

void PlotShowEditWidget::saveBmp(QString fileName)
{
    if (mCurrentPlot) {
        if (mPlotWidgetVector.size() == 1 && !global::gIsFooterVisible) {
            mCurrentPlot->saveBmp(fileName);
        } else {
            saveRastered(fileName, "BMP");
        }
    }
    return;

//    ui->plotWithToolsWidget->mPlotWidget->saveBmp(fileName);
}

void PlotShowEditWidget::saveJpg(QString fileName)
{
//    if (mCurrentPlot) {
//        mCurrentPlot->saveJpg(fileName);
//    }
//    return;

    if (mCurrentPlot) {
        if (mPlotWidgetVector.size() == 1 && !global::gIsFooterVisible) {
            mCurrentPlot->saveJpg(fileName);
        } else {
            saveRastered(fileName, "JPG");
        }
    }
    return;
//    ui->plotWithToolsWidget->mPlotWidget->saveJpg(fileName);
}

void PlotShowEditWidget::activateGraph(int index)
{
    ui->plotHierarchyWidget->activateGraph(index);
}

void PlotShowEditWidget::activateAxis(int index)
{
    ui->plotHierarchyWidget->activateAxis(index);
}

void PlotShowEditWidget::secondaryProcessing()
{
    using namespace global;
    if (!mCurrentLayoutModel)
        return;

    mCurrentLayoutModel->prepareDataForSecondaryProcessing();

    std::string rawFunctionString;
    std::string functionString;
    QString name;
    while (1) {
        SecondaryProcessingDialog dial(mCurrentLayoutModel, this);
        dial.setFunctionString(QString::fromStdString(rawFunctionString));
        //setting x ranges
        std::tuple<double, double> totalRAnge = mCurrentLayoutModel->getXMaXTotalRange();
        dial.setTotalRange(std::get<0>(totalRAnge),  std::get<1>(totalRAnge));
        std::tuple<double, double> visibleRange = mCurrentLayoutModel->getXRange();
        dial.setVisibleRange(std::get<0>(visibleRange),  std::get<1>(visibleRange));

        if (mCurrentLayoutModel->getAxisModelVector().size() == 0) {
            dial.enableOnlyManualRangeSetting(true);
        }

        dial.exec();
        if (dial.result() == QDialog::Rejected) {
            break;
        }
        rawFunctionString = dial.functionString().toStdString();
        if (gIsAutoNanTo0ConversionEnabled)
            functionString = "nanto0(" + rawFunctionString + ")";
        else
            functionString = rawFunctionString;
        name = dial.nameString();
//        expression::grammar<DataContainer, std::string::iterator> gram;
        auto beg = functionString.begin();
        DataContainer result;
        if (mCurrentLayoutModel->getAxisModelVector().size() != 0) {
            DataContainer::setConstraints(std::get<0>(dial.getXRange()), std::get<1>(dial.getXRange()));
        } else {
            DataContainer::setConstraints(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        }
        try {

            if (parse_func_string_and_count(beg, functionString.end(), result) && beg == functionString.end()) {
//            if (parse(beg, functionString.end(), gram, result) && beg == functionString.end()) {
                //ensuring desired x range
//                auto desiredXRange = dial.getXRange();
//                DataContainer xRangeDataContainer(QVector<double>({std::get<0>(desiredXRange), std::get<1>(desiredXRange)}),
//                                                  QVector<double>({1., 1.}));
//                result *=    xRangeDataContainer;

                GraphModel* model = addNewGraph(result.mXData, result.mYData, name);
                if (result.mType == DataContainer::GraphType::Curve)  {
                    model->setGraphType(GraphModel::GraphType::Curve);
                }
                gPrevSecondProcesFunctionVector.push_front(QString::fromStdString(rawFunctionString));
                if (gPrevSecondProcesFunctionVector.size() > gPrevSecondProcesFunctionVectorMaxSize)
                    gPrevSecondProcesFunctionVector.remove(gPrevSecondProcesFunctionVectorMaxSize,1);
                break;
            }
        } catch (...) {
            ;
        }
        DataContainer::setDefaultConstraints();



        //DEBUG
        QString rightPart = QString::fromStdString(std::string(functionString.begin(), beg));
        QString wrongPart = QString::fromStdString(std::string(beg, functionString.end()));
        //DEBUG
        QString warningMessage("Expression is not correct");
        if (rightPart != "") {
            warningMessage += "<br><font color=\"green\">" + rightPart + "</font>";
            warningMessage += "<font color=\"red\">" + wrongPart + "</font>";
        }
        QMessageBox::warning(this, "Error", warningMessage);
    }

    gCurrGraphVector.clear();
}

void PlotShowEditWidget::setAutoUnits()
{
    if (mCurrentLayoutModel) {
        return mCurrentLayoutModel->setAutoDimensions();
    }
    return;
//    mLayoutModel.setAutoDimensions();
}

void PlotShowEditWidget::stepBackInHistory()
{
    if (mCurrentLayoutModel) {
        return mCurrentLayoutModel->stepBackInHistory();
    }
    return;
//    mLayoutModel.stepBackInHistory();
}

void PlotShowEditWidget::stepForwardInHistory()
{
    if (mCurrentLayoutModel) {
        return mCurrentLayoutModel->stepForwardInHistory();
    }
    return;
//    mLayoutModel.stepForwardInHistory();

}

QVector<QString> PlotShowEditWidget::getGraphNames() const
{
    if (!mCurrentLayoutModel)
        return QVector<QString>();

    QVector<GraphModel*>  graphVector = mCurrentLayoutModel->getGraphModelVector();
    QVector<QString> ret;
    foreach (auto graph, graphVector) {
        ret.push_back(graph->getName());
    }
    return ret;
}

QVector<std::pair<QString, QString>> PlotShowEditWidget::currentPlotGraphNamesAndArgs() const
{
    if (!mCurrentLayoutModel)
        return QVector<std::pair<QString, QString>>();

    QVector<GraphModel*>  graphVector = mCurrentLayoutModel->getGraphModelVector();
    QVector<std::pair<QString, QString>> ret;
    foreach (GraphModel* graph, graphVector) {
        if (graph->graphType() == GraphModel::GraphType::Curve)
            ret.push_back(global::getPrameterAndArgNameFromComplexName(graph->getName()));
        else
            ret.push_back(std::make_pair(global::getPrameterNameFromComplexName(graph->getName()), QString()));
    }
    return ret;
}

void PlotShowEditWidget::updateAppearance()
{
    for (auto plot : mPlotWidgetVector) {
        plot->updateAppearance();
    }
}

void PlotShowEditWidget::installToolbarActions()
{
    ui->tileManagerWithToolbar->installToolbarActions();
}

void PlotShowEditWidget::treatExternalGraphMovingToExistingAxisRequest(GraphModel *graph, AxisModel *axis)
{
    PlotWidget *plotWidget = qobject_cast<PlotWidget*>(sender());
    int plotWidgetIndex = mPlotWidgetVector.indexOf(plotWidget);
    if (plotWidget && axis && graph && plotWidgetIndex != -1) {
        for (PlotLayoutModel* layout : mLayoutModelVector) {
            if (layout->containsGraph(graph) && layout->retrieveGraph(graph)) {
                mLayoutModelVector[plotWidgetIndex]->addExternalGraphToAxis(graph, axis);
            }
        }
    }
}

void PlotShowEditWidget::treatExternalGraphMovingToNewAxisRequest(GraphModel *graph, int k)
{

    PlotWidget *plotWidget = qobject_cast<PlotWidget*>(sender());
    int plotWidgetIndex = mPlotWidgetVector.indexOf(plotWidget);
    if (plotWidget &&  graph && plotWidgetIndex != -1) {
        for (PlotLayoutModel* layout : mLayoutModelVector) {
            if (layout->containsGraph(graph) && layout->retrieveGraph(graph)) {
                mLayoutModelVector[plotWidgetIndex]->moveExternalGraphToNewAxis(graph, k);
            }
        }
    }

}

void PlotShowEditWidget::setControlWidgetsVisible(bool visible)
{
    if (visible) {
        ui->axisControlWidget->show();
        ui->eventControlWidget->show();
        ui->anchorControlWidget->show();
        ui->multiModelControlWidget->show();
        ui->arrowedTextControlwidget->show();
        ui->horizontalLevelControlWidget->show();
    } else {
        ui->axisControlWidget->hide();
        ui->eventControlWidget->hide();
        ui->anchorControlWidget->hide();
        ui->multiModelControlWidget->hide();
        ui->arrowedTextControlwidget->hide();
        ui->horizontalLevelControlWidget->hide();
    }
}

void PlotShowEditWidget::makeCurrentPair(PlotLayoutModel *layoutModel, PlotWidget *plotWidget)
{
    if (layoutModel && plotWidget) {
        if (mCurrentLayoutModel)
            disconnectLayout(mCurrentLayoutModel);
        if (mCurrentPlot)
            disconnectPlot(mCurrentPlot);

        mCurrentLayoutModel = layoutModel;
        mCurrentPlot = plotWidget;


        ui->plotHierarchyWidget->enablePlotDeleting(mPlotWidgetVector.size() > 1);
        ui->plotHierarchyWidget->setPlotLayoutModel(mCurrentLayoutModel);
        ui->plotControlWidget->setLayoutModel(mCurrentLayoutModel);

        ui->multiModelControlWidget->setLayoutModel(layoutModel);

        connectLayout(mCurrentLayoutModel);
        connectPlot(mCurrentPlot);
    }
}

void PlotShowEditWidget::connectLayout(PlotLayoutModel *layoutModel)
{
    if (layoutModel) {
//        connect(ui->plotControlWidget, SIGNAL(settingXRangeRequest(double, double)),
//                layoutModel, SLOT(setXRangeWrapper(double,double)));
        connect(layoutModel, SIGNAL(layoutModelChanged()),
                ui->plotControlWidget, SLOT(initializeState()));

//        connect(ui->plotControlWidget, SIGNAL(legendChangingVisibilityRequest(bool)),
//                layoutModel, SLOT(setLegendVisibility(bool)));
//        connect(ui->plotControlWidget, SIGNAL(dimensionChangingVisibilityRequest(bool)),
//                layoutModel, SLOT(setDimensionVisibility(bool)));

        connect(ui->eventControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
                layoutModel, SLOT(addArrowedTextByTime(double)));
        connect(ui->anchorControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
                layoutModel, SLOT(addArrowedTextByTime(double)));
    }
}

void PlotShowEditWidget::disconnectLayout(PlotLayoutModel *layoutModel)
{
    if (layoutModel) {
//        disconnect(ui->plotControlWidget, SIGNAL(settingXRangeRequest(double, double)),
//                layoutModel, SLOT(setXRangeWrapper(double,double)));
        disconnect(layoutModel, SIGNAL(layoutModelChanged()),
                ui->plotControlWidget, SLOT(initializeState()));

//        disconnect(ui->plotControlWidget, SIGNAL(legendChangingVisibilityRequest(bool)),
//                layoutModel, SLOT(setLegendVisibility(bool)));
//        disconnect(ui->plotControlWidget, SIGNAL(dimensionChangingVisibilityRequest(bool)),
//                layoutModel, SLOT(setDimensionVisibility(bool)));

        disconnect(ui->eventControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
                layoutModel, SLOT(addArrowedTextByTime(double)));
        disconnect(ui->anchorControlWidget, SIGNAL(arrowedTextCreationRequest(double)),
                layoutModel, SLOT(addArrowedTextByTime(double)));
    }
}

void PlotShowEditWidget::connectPlot(PlotWidget *plot)
{
    if (plot) {
        connect(plot, SIGNAL(currentCoordinates(double, double)),
                this, SLOT(emitInfoString(double,double)), Qt::UniqueConnection);
        connect(plot, SIGNAL(externalGraphMovingToExistingAxisRequest(GraphModel*,AxisModel*)),
                this, SLOT(treatExternalGraphMovingToExistingAxisRequest(GraphModel*, AxisModel*)), Qt::UniqueConnection);
        connect(plot, SIGNAL(externalGraphMovingToNewAxisRequest(GraphModel*,int)),
                this, SLOT(treatExternalGraphMovingToNewAxisRequest(GraphModel*,int)), Qt::UniqueConnection);
        connect(plot, SIGNAL(fastSecondaryProcessingRequest(double,double,int)),
                this, SLOT(treatFastSecondaryProcessingRequest(double,double,int)), Qt::UniqueConnection);

    }
}

void PlotShowEditWidget::disconnectPlot(PlotWidget *plot)
{
    if (plot) {
        disconnect(plot, SIGNAL(currentCoordinates(double, double)),
                this, SLOT(emitInfoString(double,double)));
        disconnect(plot, SIGNAL(externalGraphMovingToExistingAxisRequest(GraphModel*,AxisModel*)),
                this, SLOT(treatExternalGraphMovingToExistingAxisRequest(GraphModel*, AxisModel*)));
        disconnect(plot, SIGNAL(externalGraphMovingToNewAxisRequest(GraphModel*,int)),
                this, SLOT(treatExternalGraphMovingToNewAxisRequest(GraphModel*,int)));
        disconnect(plot, SIGNAL(fastSecondaryProcessingRequest(double,double,int)),
                this, SLOT(treatFastSecondaryProcessingRequest(double,double,int)));
    }
}

bool PlotShowEditWidget::saveRastered(const QString &fileName, const char *format, int quality)
{
    QPixmap buffer = toPixmap();
    if (!buffer.isNull())
      return buffer.save(fileName, format, quality);
    else
        return false;
}

void PlotShowEditWidget::addNewLayout(PlotLayoutModel *newLayout, Qt::Orientation orientation)
{
    PlotWidget *newPlot = new PlotWidget;
    mLayoutModelVector.push_back(newLayout);
    mPlotWidgetVector.push_back(newPlot);

    connect(newPlot, SIGNAL(plotWidgetMovingIsEnabled(bool)),
            this, SLOT(treatPlotWidgetMovingEnabledChanging(bool)));

    newPlot->setLayoutModel(newLayout);

//    if (mCurrentPlot) {
//        newPlot->setGeometry(mCurrentPlot->geometry());

//    }
    ui->tileManagerWithToolbar->mTileManager->addWidget(newPlot, orientation);
    makeCurrentPair(newLayout, newPlot);
    newLayout->setPlotStyle(mPreferences);
}

void PlotShowEditWidget::treatPlotWidgetMovingEnabledChanging(bool enabled)
{
    ui->tileManagerWithToolbar->mTileManager->enableMoving(enabled);

}



