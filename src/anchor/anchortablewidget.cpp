#include "anchor/anchortablewidget.h"
#include <QStringList>
#include "core/plotlayoutmodel.h"
#include "core/graphmodel.h"
#include "core/axismodel.h"
#include <QDebug>
#include "graph/graphiconengine.h"
#include "core/qcustomplot.h"
#include <QHeaderView>
#include <QScrollBar>
#include "global/global_definitions.h"
#include "basic/bassic.h"
#include <QToolButton>

AnchorTableWidget::AnchorTableWidget(QWidget *parent) :
    QTableWidget(parent), mPlotWidget(NULL), mIsColumnsWidthFixed(true)
{

    setStyleSheet("QHeaderView::section:horizontal {margin-right: 2; border: 1px solid}");
    setShowGrid(false);
    verticalScrollBar()->setDisabled(true);
    horizontalScrollBar()->setDisabled(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    verticalHeader()->hide();

    setStyleSheet("AnchorTableWidget { color : black; }");
}

void AnchorTableWidget::setPlotWidget(const PlotWidget *plotWidget)
{
    if (mPlotWidget != plotWidget) {
        mPlotWidget = plotWidget;
        if (mPlotWidget) {
            connect(mPlotWidget, SIGNAL(currentCoordinates(double,double)),
                    this, SLOT(initializeState(double)));
        }
        initializeState();
    }
}

bool AnchorTableWidget::isColumnWidthFixed() const
{
    return mIsColumnsWidthFixed;
}

void AnchorTableWidget::initializeState(double currentTime)
{
    clear();
    if (mPlotWidget) {

//        setShowGrid(false);

//        setCellWidget(0, 0, new QToolButton);
        QTableWidgetItem *buttonItem = new QTableWidgetItem("asdf");
//        buttonItem->addW
        setHorizontalHeaderItem(0, buttonItem);

        int columnNumber = 1;
        const PlotLayoutModel *plotLayout = mPlotWidget->plotLayoutModel();
        QVector<Anchor*> anchorVector = plotLayout->anchorVector();

//        setRowCount(plotLayout->getGraphModelVector().size() + 1); //one extra row for time
//        setColumnCount(5);

        QTableWidgetItem *newItem = new QTableWidgetItem("Time");
        setItem(0, 0, newItem);
//        newItem = new QTableWidgetItem(QString::number(currentTime));
        newItem = new QTableWidgetItem(valueToString(currentTime, GraphModel::StringRepresentation::Decimal));
        setItem(0, 1, newItem);

        QVector<QCPItemTracer *> tracerVector = mPlotWidget->tracerVector();

        int rowCounter = 0;
        QVector<AxisModel*> axisVector = plotLayout->getVisibleAxisModelVector();
        int totalGraphIndex = 0;
        int axisNumberToCreateAnchorHeader = std::numeric_limits<int>::max();
        for (int axisIndex = 0; axisIndex < axisVector.size(); ++axisIndex) {
            QVector<const GraphModel*> graphVector = axisVector[axisIndex]->getFunctionModelVector();
            for (int graphIndex = 0; graphIndex < graphVector.size(); ++graphIndex) {
                QTableWidgetItem *newItem = new QTableWidgetItem(graphVector[graphIndex]->getName());
                setItem(totalGraphIndex+1, 0, newItem);
                rowCounter++;


                //icon for item
                GraphIconEngine *eng = new GraphIconEngine(graphVector[graphIndex]);
                QIcon icon(eng);
                newItem->setIcon (icon);

                if (!std::isnan(currentTime)) {
                    //what for this check I don't understand but with visibility property it doesn't work
//                    if (tracerVector.size() == plotLayout->getGraphModelVector().size()) {
                        auto currentValue = graphVector[graphIndex]->getValue(currentTime);
                        QTableWidgetItem *tracerItem = new QTableWidgetItem(valueToString(currentValue, graphVector[graphIndex]->stringRepresentation()));
                        setItem(totalGraphIndex+1, 1, tracerItem);

                        for (int anchorIndex = 0; anchorIndex < anchorVector.size(); ++anchorIndex) {
                            auto currentAnchorValue = graphVector[graphIndex]->getValue(anchorVector[anchorIndex]->time());
                            QTableWidgetItem *tracerItem = new QTableWidgetItem(valueToString(currentAnchorValue, graphVector[graphIndex]->stringRepresentation()));
                            setItem(totalGraphIndex+1, 2 + anchorIndex*2, tracerItem);
                            tracerItem = new QTableWidgetItem(valueToString(currentValue - currentAnchorValue, graphVector[graphIndex]->stringRepresentation()));
                            setItem(totalGraphIndex+1, 2 + anchorIndex*2 +  1, tracerItem);


                            if (axisIndex <= axisNumberToCreateAnchorHeader) {
                                axisNumberToCreateAnchorHeader = axisIndex;
                                tracerItem = new QTableWidgetItem(valueToString(anchorVector[anchorIndex]->time(), GraphModel::StringRepresentation::Decimal));
                                setItem(0, 2 + anchorIndex*2 , tracerItem);
                                tracerItem = new QTableWidgetItem(valueToString(currentTime - anchorVector[anchorIndex]->time(), GraphModel::StringRepresentation::Decimal));
                                setItem(0, 2 + anchorIndex*2 + 1, tracerItem);
                            }
                        }
//                    }
                }
               totalGraphIndex++;
            }


        }
        if (!std::isnan(currentTime))
            columnNumber += 1 + anchorVector.size() * 2;


//        setRowCount(plotLayout->getGraphModelVector().size() + 1); //one extra row for time
        setRowCount(rowCounter + 1); //one extra row for time
        setColumnCount(columnNumber);

        for (int i = 0; i < rowCount(); ++i) {
            for (int j = 0; j < columnCount(); ++j) {
                QTableWidgetItem *	it = item (i, j);
                if (it) {
                    it->setBackgroundColor((i % 2) ? QColor(200, 200, 250): QColor(190, 250, 250));
                }
            }
            setRowHeight(i, 17);
            setRowHeight(i, qApp->fontMetrics().height()+3);
        }

        //setting column 0 width to show all the content
        resizeColumnToContents(0);

        if (!mIsColumnsWidthFixed) {
            resizeColumnsToContents();
        }

        int widthOverall = 0;
        for(int column = 0; column < columnCount(); column++)
            widthOverall += columnWidth(column);
        setMinimumWidth(widthOverall);
        setMaximumWidth(widthOverall);

        int heightOverall = horizontalHeader()->height();
        for(int row = 0; row < rowCount(); row++)
            heightOverall += rowHeight(row);
        setMinimumHeight(heightOverall+4);
        setMaximumHeight(heightOverall+4);

        char greekDelta[] = {'\xCE', '\x94', '\0'};
        QStringList headerList = {"", ""};
        for (int i = 0; i < anchorVector.size(); ++i) {
            headerList << "Anchor" << QString::fromUtf8(greekDelta);
        }
        setHorizontalHeaderLabels(headerList);


    }
}

void AnchorTableWidget::setColumnWidthFixed(bool isFixed)
{
    if (mIsColumnsWidthFixed != isFixed) {
        mIsColumnsWidthFixed = isFixed;
    }
}

void AnchorTableWidget::paintEvent(QPaintEvent *event)
{

    QTableWidget::paintEvent(event);
        QPainter painter(viewport());
//        int myHeight = horizontalHeader()->height();
        int i = 0;
        for (i = 0; i < columnCount(); i++) {
            int startPos = horizontalHeader()->sectionViewportPosition(i);
            QPoint myFrom = QPoint(startPos, 0);
            QPoint myTo = QPoint(startPos, height());
            painter.drawLine(myFrom, myTo);
            startPos += horizontalHeader()->sectionSize(i) - 3;
            myFrom = QPoint(startPos, 0);
            myTo = QPoint(startPos, height());
            painter.drawLine(myFrom, myTo);
        }
        for (i = 0; i < rowCount(); i++) {
            int startPos = verticalHeader()->sectionViewportPosition(i);
            QPoint myFrom = QPoint(0, startPos);
            QPoint myTo = QPoint(width(), startPos);
            painter.drawLine(myFrom, myTo);
            startPos += verticalHeader()->sectionSize(i);
            myFrom = QPoint(0, startPos);
            myTo = QPoint(width(), startPos);
            painter.drawLine(myFrom, myTo);
        }

}
