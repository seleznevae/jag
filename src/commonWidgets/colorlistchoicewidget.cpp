#include "colorlistchoicewidget.h"
#include "commonWidgets/colorchoisewidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>

ColorListChoiceWidget::ColorListChoiceWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    setLayout(vLayout);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    vLayout->addWidget(scrollArea);
    QWidget *scrollableWidget = new QWidget;
    scrollArea->setWidget(scrollableWidget);

    mGridLayout = new QGridLayout;
    mGridLayout->setSpacing(0);
    mGridLayout->setMargin(0);
//    mGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scrollableWidget->setLayout(mGridLayout);


    mAddColorButton = new QPushButton(QIcon(":/add"), "add", this);
    vLayout->addWidget(mAddColorButton);
    connect(mAddColorButton, SIGNAL(clicked(bool)),
            this, SLOT(addNewRandomColor()));

    scrollableWidget->show();
}

ColorListChoiceWidget::~ColorListChoiceWidget()
{

}

QList<QColor> ColorListChoiceWidget::colorList() const
{
    QList<QColor> ret;
    for (auto colorWidget : mColorChoiceWidgetsVector) {
        ret << colorWidget->color();
    }
    return ret;
}

void ColorListChoiceWidget::setColorList(const QList<QColor> &listOfColors)
{
    if (colorList() != listOfColors) {
        clearState();
        for (const auto &color : listOfColors) {
            addColorWidgetWithButton(color);
        }
    }

}

void ColorListChoiceWidget::addNewRandomColor()
{
    QColor newColor(rand() % 255, rand() % 255, rand() % 255);
    addColorWidgetWithButton(newColor);
}

void ColorListChoiceWidget::treatColorDeletingRequest()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int index = mKillButtonsVector.indexOf(button);
        if (index != -1) {
//            qDebug() << "sizeof mColorList " << mColorList.size();
//            qDebug() << "sizeof mKillButtonsVector " << mKillButtonsVector.size();


            mGridLayout->removeWidget(mColorChoiceWidgetsVector[index]);
            mColorChoiceWidgetsVector[index]->setParent(nullptr);
            mColorChoiceWidgetsVector[index]->deleteLater();
            mGridLayout->removeWidget(mKillButtonsVector[index]);
            mKillButtonsVector[index]->setParent(nullptr);
            mKillButtonsVector[index]->deleteLater();

            mKillButtonsVector.remove(index);
            mColorChoiceWidgetsVector.remove(index);
        }
    }
}


void ColorListChoiceWidget::addColorWidgetWithButton(const QColor &color)
{
    int newRowNumber = mKillButtonsVector.size();
    ColorChoiseWidget* colorChoiceWidget = new ColorChoiseWidget();
    colorChoiceWidget->setColor(color);
    mColorChoiceWidgetsVector.push_back(colorChoiceWidget);
    QPushButton* newButton = new QPushButton(QIcon(":/remove"), "");
    mKillButtonsVector.push_back(newButton);
    mGridLayout->addWidget(colorChoiceWidget, newRowNumber, 0);
    mGridLayout->addWidget(newButton, newRowNumber, 1);
    connect(newButton, SIGNAL(clicked(bool)),
            this, SLOT(treatColorDeletingRequest()));

}

void ColorListChoiceWidget::clearState()
{
    for (auto colorWidget : mColorChoiceWidgetsVector) {
        mGridLayout->removeWidget(colorWidget);
        colorWidget->setParent(nullptr);
        colorWidget->deleteLater();
    }
    for (auto killButton : mKillButtonsVector) {
        mGridLayout->removeWidget(killButton);
        killButton->setParent(nullptr);
        killButton->deleteLater();
    }
    mKillButtonsVector.clear();
    mColorChoiceWidgetsVector.clear();
}

