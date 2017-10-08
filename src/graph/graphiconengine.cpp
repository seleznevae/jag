#include "graphiconengine.h"
#include <QPainter>

GraphIconEngine::GraphIconEngine(const GraphModel*model)
    :mGraphModel(model), mEventModel(NULL), mAnchorModel(NULL), mArrowedTextModel(NULL), mHorizontalLevelModel(NULL)
{

}

GraphIconEngine::GraphIconEngine(const TimeEvent *model)
    :mGraphModel(NULL), mEventModel(model), mAnchorModel(NULL), mArrowedTextModel(NULL), mHorizontalLevelModel(NULL)
{

}

GraphIconEngine::GraphIconEngine(const Anchor *model)
    :mGraphModel(NULL), mEventModel(NULL), mAnchorModel(model), mArrowedTextModel(NULL), mHorizontalLevelModel(NULL)
{

}

GraphIconEngine::GraphIconEngine(const ArrowedText *model)
    :mGraphModel(NULL), mEventModel(NULL), mAnchorModel(NULL), mArrowedTextModel(model), mHorizontalLevelModel(NULL)
{

}

GraphIconEngine::GraphIconEngine(const HorizontalLevel *model)
    :mGraphModel(NULL), mEventModel(NULL), mAnchorModel(NULL), mArrowedTextModel(NULL), mHorizontalLevelModel(model)
{

}



void GraphIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{

    if (mGraphModel) {
        QPen pen = mGraphModel->getPen();
        pen.setWidthF(2);
        painter->setPen(pen);
        QBrush brush = painter->brush();
        brush.setColor(mGraphModel->getPen().color());
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        drawScatterShape(painter,rect.center().x(), rect.center().y());
        painter->drawLine(0.5*(rect.topLeft()+rect.bottomLeft()), 0.5*(rect.bottomRight()+rect.topRight()));
    } else if (mEventModel) {
        QPen pen = painter->pen();
        pen.setWidthF(2.5);
        painter->setPen(pen);
        painter->drawLine(rect.bottomLeft(), 0.5*(rect.bottomLeft() + rect.bottomRight()));
        painter->drawLine(0.5*(rect.bottomLeft() + rect.bottomRight()), 0.5*(rect.topLeft() + rect.topRight()));
        painter->drawLine(0.5*(rect.topLeft() + rect.topRight()), rect.topRight());
    } else if (mAnchorModel) {
        QPen pen = painter->pen();
        pen.setWidthF(2.5);
        pen.setColor(mAnchorModel->borderPen().color());
        painter->setPen(pen);
        painter->setBrush(mAnchorModel->brushColor());
        switch (mAnchorModel->style()) {
            case(QCPItemTracer::tsNone):
                pen.setColor(Qt::lightGray);
                pen.setWidthF(1.5);
                painter->setPen(pen);
                painter->drawLine(rect.topLeft(), rect.bottomRight());
                painter->drawLine(rect.topRight(), rect.bottomLeft());
                break;
            case(QCPItemTracer::tsPlus):
            case(QCPItemTracer::tsCrosshair):
                painter->drawLine((rect.bottomLeft() + rect.topLeft())*0.5, (rect.bottomRight() + rect.topRight())*0.5);
                painter->drawLine((rect.topLeft() + rect.topRight())*0.5, (rect.bottomLeft() + rect.bottomRight())*0.5);
                break;
            case(QCPItemTracer::tsCircle):
                painter->drawEllipse(rect);
                break;
            case(QCPItemTracer::tsSquare):
                painter->drawRect(QRectF(rect.topLeft(), rect.bottomRight()));
                break;
            default:
                break;
        }
    } else if (mArrowedTextModel) {
        QPen pen = painter->pen();
        pen.setWidthF(2.5);
        painter->setPen(pen);
        painter->drawLine(rect.bottomLeft(), rect.topRight());
        painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + 0.25*(rect.bottomRight() - rect.bottomLeft()));
        painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + 0.25*(rect.topLeft() - rect.bottomLeft()));

    } else if (mHorizontalLevelModel) {
        QPen pen = painter->pen();
        pen.setWidthF(2.5);
        painter->setPen(pen);
        painter->drawLine(rect.bottomLeft(), 0.5*(rect.bottomLeft() + rect.topLeft()));
        painter->drawLine(0.5*(rect.bottomLeft() + rect.topLeft()), 0.5*(rect.topRight() + rect.bottomRight()));
        painter->drawLine(0.5*(rect.topRight() + rect.bottomRight()), rect.topRight());

    }


}

QIconEngine *GraphIconEngine::clone() const
{
    GraphIconEngine *result = new GraphIconEngine(*this);
    result->mGraphModel = mGraphModel;
    result->mEventModel = mEventModel;
    result->mAnchorModel = mAnchorModel;
    result->mArrowedTextModel = mArrowedTextModel;
    result->mHorizontalLevelModel = mHorizontalLevelModel;
    return result;
}




void GraphIconEngine::drawScatterShape(QPainter *painter, double x, double y) const
{
  double scatterSize = 10.;
  double w = scatterSize/2.0;
  switch (mGraphModel->getScatterStyle().shape())
  {
    case QCPScatterStyle::ssNone: break;
    case QCPScatterStyle::ssDot:
    {
      painter->drawLine(QPointF(x, y), QPointF(x+0.0001, y));
      break;
    }
    case QCPScatterStyle::ssCross:
    {
      painter->drawLine(QLineF(x-w, y-w, x+w, y+w));
      painter->drawLine(QLineF(x-w, y+w, x+w, y-w));
      break;
    }
    case QCPScatterStyle::ssPlus:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      break;
    }
    case QCPScatterStyle::ssCircle:
    {
      painter->drawEllipse(QPointF(x , y), w, w);
      break;
    }
    case QCPScatterStyle::ssDisc:
    {
      QBrush b = painter->brush();
      painter->setBrush(painter->pen().color());
      painter->drawEllipse(QPointF(x , y), w, w);
      painter->setBrush(b);
      break;
    }
    case QCPScatterStyle::ssSquare:
    {
      painter->drawRect(QRectF(x-w, y-w, scatterSize, scatterSize));
      break;
    }
    case QCPScatterStyle::ssDiamond:
    {
      painter->drawLine(QLineF(x-w,   y,   x, y-w));
      painter->drawLine(QLineF(  x, y-w, x+w,   y));
      painter->drawLine(QLineF(x+w,   y,   x, y+w));
      painter->drawLine(QLineF(  x, y+w, x-w,   y));
      break;
    }
    case QCPScatterStyle::ssStar:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.707, y+w*0.707));
      painter->drawLine(QLineF(x-w*0.707, y+w*0.707, x+w*0.707, y-w*0.707));
      break;
    }
    case QCPScatterStyle::ssTriangle:
    {
       painter->drawLine(QLineF(x-w, y+0.755*w, x+w, y+0.755*w));
       painter->drawLine(QLineF(x+w, y+0.755*w,   x, y-0.977*w));
       painter->drawLine(QLineF(  x, y-0.977*w, x-w, y+0.755*w));
      break;
    }
    case QCPScatterStyle::ssTriangleInverted:
    {
       painter->drawLine(QLineF(x-w, y-0.755*w, x+w, y-0.755*w));
       painter->drawLine(QLineF(x+w, y-0.755*w,   x, y+0.977*w));
       painter->drawLine(QLineF(  x, y+0.977*w, x-w, y-0.755*w));
      break;
    }
    case QCPScatterStyle::ssCrossSquare:
    {
       painter->drawLine(QLineF(x-w, y-w, x+w*0.95, y+w*0.95));
       painter->drawLine(QLineF(x-w, y+w*0.95, x+w*0.95, y-w));
       painter->drawRect(QRectF(x-w, y-w, scatterSize, scatterSize));
      break;
    }
    case QCPScatterStyle::ssPlusSquare:
    {
       painter->drawLine(QLineF(x-w,   y, x+w*0.95,   y));
       painter->drawLine(QLineF(  x, y+w,        x, y-w));
       painter->drawRect(QRectF(x-w, y-w, scatterSize, scatterSize));
      break;
    }
    case QCPScatterStyle::ssCrossCircle:
    {
       painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.670, y+w*0.670));
       painter->drawLine(QLineF(x-w*0.707, y+w*0.670, x+w*0.670, y-w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case QCPScatterStyle::ssPlusCircle:
    {
       painter->drawLine(QLineF(x-w,   y, x+w,   y));
       painter->drawLine(QLineF(  x, y+w,   x, y-w));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case QCPScatterStyle::ssPeace:
    {
       painter->drawLine(QLineF(x, y-w,         x,       y+w));
       painter->drawLine(QLineF(x,   y, x-w*0.707, y+w*0.707));
       painter->drawLine(QLineF(x,   y, x+w*0.707, y+w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
   default:
      break;
  }



}
