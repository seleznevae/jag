#include "positionchoicewidget.h"
#include "ui_positionchoicewidget.h"
#include <QPainter>



class PositionPainter: public QWidget {
public:
    explicit PositionPainter(QWidget* parent = nullptr)
        : QWidget(parent),mPositions(0), mPositionType(PositionType::posAxisRect)
        {   }
    virtual ~PositionPainter() {    }
    void setPosition(const utility::Positions &positions);
    void setPositionType(PositionType type);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    utility::Positions mPositions;
    PositionType mPositionType;
};


void PositionPainter::setPositionType(PositionType type)
{
    mPositionType = type;
    update();
}


void PositionPainter::setPosition(const utility::Positions &positions)
{
    mPositions = positions;
    update();
}


template <typename T>
T min(T arg1, T arg2)
{
    return arg1 > arg2 ? arg2 : arg1;
}

template <typename T, typename ...Ts>
T min(T arg, Ts... args)
{
    return min(arg, min(args...));
}



void PositionPainter::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    int wid = width();
    int hei = height();
    QPainter painter(this);
    painter.setBrush(QWidget::palette().color(QWidget::backgroundRole()));
    painter.setPen(Qt::NoPen);
    painter.drawRect(QRectF(0, 0, wid, hei));
    QPen pen(QWidget::palette().color(QPalette::Foreground));
    pen.setWidth(2);
    painter.setPen(pen);

    switch (mPositionType) {
        case PositionType::posAxisRect:
            if (mPositions.testFlag(utility::posLeft))
                painter.drawLine(wid/10, hei/10, wid/10, 9*hei/10);
            if (mPositions.testFlag(utility::posTop))
                painter.drawLine(wid/10, hei/10, 9*wid/10, hei/10);
            if (mPositions.testFlag(utility::posRight))
                painter.drawLine(9*wid/10, hei/10, 9*wid/10, 9*hei/10);
            if (mPositions.testFlag(utility::posBottom))
                painter.drawLine(wid/10, 9*hei/10, 9*wid/10, 9*hei/10);
            break;
        case PositionType::posTick:
        case PositionType::posSubTick:
            const int min_len = 10;
            const int len = min(min_len, wid/10, hei/10);
            if (mPositions.testFlag(utility::posLeft))
                painter.drawLine(wid/10, 5*hei/10, wid/10 + len, 5*hei/10);
            if (mPositions.testFlag(utility::posTop))
                painter.drawLine(5*wid/10, hei/10, 5*wid/10, hei/10 + len);
            if (mPositions.testFlag(utility::posRight))
                painter.drawLine(9*wid/10, 5*hei/10, 9*wid/10 - len, 5*hei/10);
            if (mPositions.testFlag(utility::posBottom))
                painter.drawLine(5*wid/10, 9*hei/10, 5*wid/10, 9*hei/10 - len);
    }
}




PositionChoiceWidget::PositionChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PositionChoiceWidget), mPositionPainter(nullptr)
{
    ui->setupUi(this);

//    ui->gridLayout->setSpacing(0);
//    ui->gridLayout->setMargin(0);

    for (QLayout *lay : QVector<QLayout*>{layout(), ui->gridLayout, ui->horizontalLayout_3, ui->horizontalLayout_4,
                                           ui->horizontalLayout_5
                                        }) {
        lay->setSpacing(0);
        lay->setMargin(0);
    }

    connect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(positionChangedByUser()));
    connect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(positionChangedByUser()));
    connect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(positionChangedByUser()));
    connect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(positionChangedByUser()));

    connect(ui->topCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updatePositionPainter()));
    connect(ui->bottomCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updatePositionPainter()));
    connect(ui->leftCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updatePositionPainter()));
    connect(ui->rightCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updatePositionPainter()));

    mPositionPainter = new PositionPainter(this);
    ui->gridLayout->addWidget(mPositionPainter, 1, 2);
}

PositionChoiceWidget::~PositionChoiceWidget()
{
    delete ui;
}

void PositionChoiceWidget::setPosition(utility::Positions position)
{
    ui->topCheckBox   ->setChecked(position.testFlag(utility::posTop   ));
    ui->bottomCheckBox->setChecked(position.testFlag(utility::posBottom));
    ui->leftCheckBox  ->setChecked(position.testFlag(utility::posLeft  ));
    ui->rightCheckBox ->setChecked(position.testFlag(utility::posRight ));
    mPositionPainter->setPosition(position);
}

utility::Positions PositionChoiceWidget::position() const
{
    utility::Positions ret = utility::Positions(0);
    if (ui->leftCheckBox->isChecked())
        ret |= utility::posLeft;
    if (ui->topCheckBox->isChecked())
        ret |= utility::posTop;
    if (ui->rightCheckBox->isChecked())
        ret |= utility::posRight;
    if (ui->bottomCheckBox->isChecked())
        ret |= utility::posBottom;
    return ret;
}

void PositionChoiceWidget::setPositionType(PositionType type)
{
    mPositionPainter->setPositionType(type);
}

void PositionChoiceWidget::updatePositionPainter()
{
    mPositionPainter->setPosition(position());
}







//AxisRectPainter::AxisRectPainter(QWidget *parent) : QWidget(parent)
//{

//}

//void AxisRectPainter::setRectPosition(const utility::Positions &positions)
//{
//    mPositions = positions;
//    update();
//}

//void AxisRectPainter::paintEvent(QPaintEvent *event)
//{
//    QWidget::paintEvent(event);
//    int wid = width();
//    int hei = height();
//    QPainter painter(this);
//    painter.setBrush(Qt::white);
//    painter.setPen(Qt::NoPen);
//    painter.drawRect(QRectF(0, 0, wid, hei));
//    painter.setPen(Qt::black);
//    if (mPositions.testFlag(utility::posLeft))
//        painter.drawLine(wid/10, hei/10, wid/10, 9*hei/10);
//    if (mPositions.testFlag(utility::posTop))
//        painter.drawLine(wid/10, hei/10, 9*wid/10, hei/10);
//    if (mPositions.testFlag(utility::posRight))
//        painter.drawLine(9*wid/10, hei/10, 9*wid/10, 9*hei/10);
//    if (mPositions.testFlag(utility::posBottom))
//        painter.drawLine(wid/10, 9*hei/10, 9*wid/10, 9*hei/10);
//}

