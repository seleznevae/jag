#include "plotwidgetnormalstate.h"
#include "global/global_definitions.h"

PlotWidgetNormalState::PlotWidgetNormalState(PlotWidget *parent):
        PlotWidgetState(parent),
        mDraggedGraphIndex(-1), mDraggedArrowedTextIndex(-1),  mDraggedArrowEndIndex(-1),
        mGraphDragState(State::Off), mArrowedTextDragState(State::Off), mArrowEndDragState(State::Off),
        mCurrentPos(-1, -1)
{
}

void PlotWidgetNormalState::treatMousePressEvent(QMouseEvent *event)
{
    mDraggedGraphIndex = -1;

    if (mPlotWidget->axisRectCount() != 0) {
        if (event->buttons() & Qt::LeftButton) {

            mPlotWidget->setSelectionTolerance(global::gGraphCaptureDistance);
            for (int i = 0; i < mPlotWidget->mTextLabelsVector.size(); ++i) {
                if (mPlotWidget->mTextLabelsVector[i]->selectTest(event->pos(), false) < global::gGraphCaptureDistance
                        && mPlotWidget->mTextLabelsVector[i]->selectTest(event->pos(), false) >= 0) {
    //                qDebug() << "distance to" << i << " is" << mGraphVector[i]->selectTest(event->pos(), true);
                    mTextStartDragPos = event->pos();
                    mDraggedArrowedTextIndex = i;
                    break;
                }
            }
            mArrowedTextDragState = State::Off;

            for (int i = 0; i < mPlotWidget->mArrowsVector.size(); ++i) {
                int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
                QPointF arrowEnd(mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atBottom)->coordToPixel(mPlotWidget->mArrowsVector[i]->end->key()),
                                 mPlotWidget->axisRect(axisRectIndex)->axis(QCPAxis::atLeft)->coordToPixel(mPlotWidget->mArrowsVector[i]->end->value()));
                if ((arrowEnd - event->pos()).manhattanLength() < 5) {
    //                qDebug() << "distance to" << i << " is" << (arrowEnd - event->posF()).manhattanLength();
                    mArrowEndStartDragPos = event->pos();
                    mDraggedArrowEndIndex = i;
                    break;
                }
            }
            mArrowEndDragState = State::Off;

            if (mDraggedArrowedTextIndex < 0 && mDraggedArrowEndIndex < 0) {
                for (int i = 0; i < mPlotWidget->mGraphVector.size(); ++i) {
                    if (mPlotWidget->mGraphVector[i]->selectTest(event->pos(), true) < global::gGraphCaptureDistance
                            && mPlotWidget->mGraphVector[i]->selectTest(event->pos(), true) >= 0) {
                        mStartDragPos = event->pos();
                        mDraggedGraphIndex = i;
        //                qDebug() << "distance to" << i << " is" << mGraphVector[i]->selectTest(event->pos(), true);
                        break;
                    }
                }
                mGraphDragState = State::Off;
            }
        }
    }
}

void PlotWidgetNormalState::treatMouseReleaseEvent(QMouseEvent *event)
{
    auto boundPositionInRect = [](const QCPAxisRect* rect, const QPointF &point) {
        auto ret = point;
        if (point.x() > rect->axis(QCPAxis::atBottom)->range().upper) {
            ret.setX(rect->axis(QCPAxis::atBottom)->range().upper);
        } else if (point.x() < rect->axis(QCPAxis::atBottom)->range().lower) {
            ret.setX(rect->axis(QCPAxis::atBottom)->range().lower);
        }
        if (point.y() > rect->axis(QCPAxis::atLeft)->range().upper) {
            ret.setY(rect->axis(QCPAxis::atLeft)->range().upper);
        } else if (point.y() < rect->axis(QCPAxis::atLeft)->range().lower) {
            ret.setY(rect->axis(QCPAxis::atLeft)->range().lower);
        }
        return ret;
    };

    if (mPlotWidget->axisRectCount() != 0) {
//        if (mGraphDragState == State::On) {
//            mGraphDragState = State::Off;
//            qDebug() << "mGraphDragState = State::Off";
////            mPlotWidget->emitPlotWidgetMovingIsEnabled(true);
//            emit mPlotWidget->plotWidgetMovingIsEnabled(true);
//    //        qDebug() << "drop";//startDrag();

//            QApplication::setOverrideCursor(Qt::ArrowCursor);

//            //finding dragged graph
//            GraphModel *draggedGraph = NULL;
//            int graphIndex = 0;
//            for (auto axis : mPlotWidget->mLayoutModel->getVisibleAxisModelVector()) {
//                if (draggedGraph)
//                    break;
//                for (auto graph : axis->getGraphModelVector()) {
//                    if (graphIndex == mDraggedGraphIndex) {
//                        draggedGraph = const_cast<GraphModel*>(graph);
//                        break;
//                    }
//                    graphIndex++;
//                }
//            }

//            //trying to drop into existing axis
//            bool dropWasImplemented = false;
//            int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
//            if (axisRectIndex != -1) {
//                QVector<AxisModel *> axisModelVector = mPlotWidget->mLayoutModel->getVisibleAxisModelVector();
//                AxisModel *destinationAxis = axisModelVector[axisRectIndex];
//                if (draggedGraph) {
//                    const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->moveGraphToAxis(draggedGraph, destinationAxis);
//                    dropWasImplemented = true;
//                }
//            }

//            //drop to a new axis if it was not dropped to an existing one
//            if (dropWasImplemented == false) {
//                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->moveGraphToNewAxis(draggedGraph, event->y() < mPlotWidget->height()/2 ? 0 : 1);
//            }
//        }


        if (mArrowedTextDragState == State::On) {
    //        qDebug() << "drop";//startDrag();

            QApplication::setOverrideCursor(Qt::ArrowCursor);

            //finding dragged text
            ArrowedText *draggedArrowedText = NULL;
            int axisIndex = -1;
            int textIndex = 0;
            for (auto axis : mPlotWidget->mLayoutModel->getVisibleAxisModelVector()) {
                axisIndex++;
                if (draggedArrowedText) {
                    axisIndex--;
                    break;
                }
                for (auto arrText : axis->arrowedTextVector()) {
                    if (textIndex == mDraggedArrowedTextIndex) {
                        draggedArrowedText = arrText;
                        break;
                    }
                    textIndex++;
                }
            }

            if (draggedArrowedText) {
                // to take into account the fact that user draggs not the center of the text
                auto extraDeviation = draggedArrowedText->textCenter() - QPointF(
                                                                                mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(mTextStartDragPos.x()),
                                                                                mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(mTextStartDragPos.y())
                                                                                );
                draggedArrowedText->setTextCenter(boundPositionInRect(mPlotWidget->axisRect(axisIndex), QPointF(
                                                                      mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(event->x()),
                                                                      mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(event->y())
                                                                         ) + extraDeviation));
            }
            mArrowedTextDragState = State::Off;
            mDraggedArrowedTextIndex = -1;
        }

        if (mArrowEndDragState == State::On) {
    //        qDebug() << "drop";//startDrag();

            QApplication::setOverrideCursor(Qt::ArrowCursor);

            //finding dragged text
            ArrowedText *draggedArrowedText = NULL;
            int axisIndex = -1;
            int textIndex = 0;
            for (auto axis : mPlotWidget->mLayoutModel->getVisibleAxisModelVector()) {
                axisIndex++;
                if (draggedArrowedText) {
                    axisIndex--;
                    break;
                }
                for (auto arrText : axis->arrowedTextVector()) {
                    if (textIndex == mDraggedArrowEndIndex) {
                        draggedArrowedText = arrText;
                        break;
                    }
                    textIndex++;
                }
            }

            if (draggedArrowedText) {
                draggedArrowedText->setArrowFinish(QPointF(
                                                      mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atBottom)->pixelToCoord(event->x()),
                                                      mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft)->pixelToCoord(event->y())
                                                           ));
            }
            mArrowEndDragState = State::Off;
            mDraggedArrowEndIndex = -1;
        }
    }

}

void PlotWidgetNormalState::treatMouseMoveEvent(QMouseEvent *event)
{

    if (mPlotWidget->axisRectCount() != 0) {
//        if (mGraphDragState == State::On) {
//            QBrush brushOrigin;
//            for (int i = 0; i < mPlotWidget->axisRectCount(); ++ i)
//                mPlotWidget->axisRect(i)->setBackground(brushOrigin);
//            int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
//            if (axisRectIndex != -1) {
//                QBrush brush;
//                brush.setStyle(Qt::SolidPattern);
//                brush.setColor(QColor(125, 233, 247, 55));
//                mPlotWidget->axisRect(axisRectIndex)->setBackground(brush);
//            }
//            mPlotWidget->replot();
//        }

        if (event->buttons() & Qt::LeftButton && mGraphDragState == State::Off
                && mDraggedGraphIndex >= 0) {
            int distance = (mStartDragPos - event->pos()).manhattanLength();
            if (distance > QApplication::startDragDistance()) {
    //            qDebug() << "start drag";//startDrag();
                mGraphDragState = State::On;
//                mPlotWidget->emitPlotWidgetMovingIsEnabled(false);
                emit mPlotWidget->plotWidgetMovingIsEnabled(false);
//                QApplication::setOverrideCursor(Qt::DragMoveCursor);

                //DEBUG
                qDebug() << "-----------treatLeaveEvent";
                GraphModel *draggedGraph = nullptr;
                int graphIndex = 0;
                for (auto axis : mPlotWidget->mLayoutModel->getVisibleAxisModelVector()) {
                    if (draggedGraph)
                        break;
                    for (auto graph : axis->getGraphModelVector()) {
                        if (graphIndex == mDraggedGraphIndex) {
                            draggedGraph = const_cast<GraphModel*>(graph);
                            break;
                        }
                        graphIndex++;
                    }
                }
                if (draggedGraph == nullptr)
                    return;
                QDrag *drag = new QDrag(mPlotWidget);
                GraphMimeData *mimeData = new GraphMimeData;
                mimeData->mDraggedGraph = draggedGraph;
                drag->setMimeData(mimeData);
                qDebug() << "********************drag->exec()";
                drag->exec();

                //DEBUG
            }
        }

        if ( event->buttons() & Qt::LeftButton && mArrowedTextDragState == State::Off
                && mDraggedArrowedTextIndex >= 0) {
            int distance = (mTextStartDragPos - event->pos()).manhattanLength();
            if (distance > QApplication::startDragDistance()) {
                mArrowedTextDragState = State::On;
                QApplication::setOverrideCursor(Qt::DragMoveCursor);
            }
        }

        if (event->buttons() & Qt::LeftButton && mArrowEndDragState == State::Off
                && mDraggedArrowEndIndex >= 0) {
            int distance = (mArrowEndStartDragPos - event->pos()).manhattanLength();
            if (distance > QApplication::startDragDistance()) {
                mArrowEndDragState = State::On;
                QApplication::setOverrideCursor(Qt::DragMoveCursor);
            }
        }


        //TODO
        if (mArrowedTextDragState == State::On) {
            mCurrentPos = event->pos();
            mPlotWidget->update();
        } else {
            mCurrentPos = QPoint(-1, -1);
        }

    }

}

void PlotWidgetNormalState::treatPaintEvent(QPaintEvent *)
{
    if (mPlotWidget->axisRectCount() != 0) {
        if (mArrowedTextDragState == State::On) {
            QPainter painter(mPlotWidget);

            //finding dragged text
            ArrowedText *draggedArrowedText = NULL;
            int axisIndex = -1;
            int textIndex = 0;
            for (auto axis : mPlotWidget->mLayoutModel->getVisibleAxisModelVector()) {
                axisIndex++;
                if (draggedArrowedText) {
                    axisIndex--;
                    break;
                }
                for (auto arrText : axis->arrowedTextVector()) {
                    if (textIndex == mDraggedArrowedTextIndex) {
                        draggedArrowedText = arrText;
                        break;
                    }
                    textIndex++;
                }
            }

            if (draggedArrowedText) {
                auto deviationFromCenter = QPoint(mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atBottom)->coordToPixel(draggedArrowedText->textCenter().x()),
                                              mPlotWidget->axisRect(axisIndex)->axis(QCPAxis::atLeft  )->coordToPixel(draggedArrowedText->textCenter().y()))
                                     - mTextStartDragPos;
                QPoint diag = (mPlotWidget->mTextLabelsVector[mDraggedArrowedTextIndex]->topLeft->pixelPoint()
                               - mPlotWidget->mTextLabelsVector[mDraggedArrowedTextIndex]->bottomRight->pixelPoint()).toPoint();
                painter.drawRect(QRect(mCurrentPos + deviationFromCenter - diag/2,
                                       mCurrentPos + deviationFromCenter + diag/2));
            }

        }
    }
}

void PlotWidgetNormalState::treatLeaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    mGraphDragState = State::Off;
}

void PlotWidgetNormalState::treatDropEvent(QDropEvent *event)
{
    auto data = qobject_cast<const GraphMimeData*>(event->mimeData());
    if (data && data->mDraggedGraph) {
        mGraphDragState = State::Off;
        emit mPlotWidget->plotWidgetMovingIsEnabled(true);
//        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QApplication::restoreOverrideCursor();
        event->acceptProposedAction();
        auto draggedGraph = data->mDraggedGraph;
        //trying to drop into existing axis
        bool dropWasImplemented = false;
        int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
        bool isDroppedGraphInternal = mPlotWidget->mLayoutModel->containsGraph(draggedGraph);
        if (axisRectIndex != -1) {
            QVector<AxisModel *> axisModelVector = mPlotWidget->mLayoutModel->getVisibleAxisModelVector();
            AxisModel *destinationAxis = axisModelVector[axisRectIndex];
            if (isDroppedGraphInternal) {
                if (destinationAxis->contains(draggedGraph)) {
                    //to turn off highlighting of the current axis rect
                    highlightAxisRectUnderPosition(QPoint(-1, -1));
                } else {
                    const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->moveGraphToAxis(draggedGraph, destinationAxis);
                }

            } else {
                emit mPlotWidget->externalGraphMovingToExistingAxisRequest(draggedGraph, destinationAxis);
            }
            dropWasImplemented = true;
        }

        //drop to a new axis if it was not dropped to an existing one
        if (dropWasImplemented == false) {
            if (isDroppedGraphInternal) {
                const_cast<PlotLayoutModel*>(mPlotWidget->mLayoutModel)->moveGraphToNewAxis(draggedGraph, event->pos().y() < mPlotWidget->height()/2 ? 0 : 1);

            } else {
                emit mPlotWidget->externalGraphMovingToNewAxisRequest(draggedGraph, event->pos().y() < mPlotWidget->height()/2 ? 0 : 1);
            }
        }
    }

}

void PlotWidgetNormalState::treatDragMoveEvent(QDragMoveEvent *event)
{
    if (mPlotWidget->axisRectCount() != 0) {
        auto data = qobject_cast<const GraphMimeData*>(event->mimeData());
        if (data) {
//            QBrush brushOrigin;
//            for (int i = 0; i < mPlotWidget->axisRectCount(); ++ i)
//                mPlotWidget->axisRect(i)->setBackground(brushOrigin);
//            int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(event->pos());
//            if (axisRectIndex != -1) {
//                QBrush brush;
//                brush.setStyle(Qt::SolidPattern);
//                brush.setColor(QColor(125, 233, 247, 55));
//                mPlotWidget->axisRect(axisRectIndex)->setBackground(brush);
//            }
//            mPlotWidget->replot();
            highlightAxisRectUnderPosition(event->pos());
        }

    }
}

void PlotWidgetNormalState::treatDragLeaveEvent(QDragLeaveEvent *)
{
    mGraphDragState = State::Off;
    if (mPlotWidget->axisRectCount() != 0) {
            QBrush brushOrigin;
            for (int i = 0; i < mPlotWidget->axisRectCount(); ++ i)
                mPlotWidget->axisRect(i)->setBackground(brushOrigin);
            mPlotWidget->replot();
    }
}

void PlotWidgetNormalState::treatDragEnterEvent(QDragEnterEvent *event)
{
    auto data = qobject_cast<const GraphMimeData*>(event->mimeData());
    if (data && data->mDraggedGraph) {
        mGraphDragState = State::On;
    }

}

void PlotWidgetNormalState::highlightAxisRectUnderPosition(QPoint currentPos)
{
    QBrush brushOrigin;
    for (int i = 0; i < mPlotWidget->axisRectCount(); ++ i)
        mPlotWidget->axisRect(i)->setBackground(brushOrigin);
    int axisRectIndex = mPlotWidget->getIndexOfAxisRectByPoint(currentPos);
    if (axisRectIndex != -1) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(125, 233, 247, 55));
        mPlotWidget->axisRect(axisRectIndex)->setBackground(brush);
    }
    mPlotWidget->replot();
}
