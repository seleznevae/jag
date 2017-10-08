#include "colorslistchoicewidget.h"
#include "commonWidgets/colorchoisewidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>
#include <QLabel>

ColorsListChoiceWidget::ColorsListChoiceWidget(QWidget *parent)
    : QWidget(parent)
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

    mHeaderLabel0 = new QLabel();
    mHeaderLabel1 = new QLabel();
    mGridLayout->addWidget(mHeaderLabel0, 0, 0);
    mGridLayout->addWidget(mHeaderLabel1, 0, 1);
    mGridLayout->setRowStretch(0, 0.1);
    vLayout->addStretch();
    scrollableWidget->show();
}

ColorsListChoiceWidget::~ColorsListChoiceWidget()
{

}

std::tuple<QList<QColor>, QList<QColor>> ColorsListChoiceWidget::colorLists() const
{
    QList<QColor> ret0;
    QList<QColor> ret1;
    for (auto colorWidget : mColorChoiceWidgetsVector0) {
        ret0 << colorWidget->color();
    }
    for (auto colorWidget : mColorChoiceWidgetsVector1) {
        ret1 << colorWidget->color();
    }
    return std::tuple<QList<QColor>, QList<QColor>>(ret0, ret1);
}

void ColorsListChoiceWidget::setHeader(const QString &header0, const QString &header1)
{
    mHeaderLabel0->setText(header0);
    mHeaderLabel1->setText(header1);
}

void ColorsListChoiceWidget::setColorLists(const QList<QColor>& colorList0, const QList<QColor>& colorList1)
{
    if (colorList0.size() != colorList1.size()) {
        qDebug() << "Incorrect argument sizes in ColorsListChoiceWidget::setColorLists";
        return;
    }
    auto currentColors = colorLists();
    if (std::get<0>(currentColors) != colorList0 || std::get<1>(currentColors) != colorList1) {
        clearState();
        for (int i = 0; i < colorList0.size(); ++i) {
            addColorWidgetWithButton(colorList0[i], colorList1[i]);
        }
        emit colorListsChanged();
    }

}

void ColorsListChoiceWidget::addNewRandomColor()
{
    QColor newColor0(rand() % 255, rand() % 255, rand() % 255);
    QColor newColor1(rand() % 255, rand() % 255, rand() % 255);
    addColorWidgetWithButton(newColor0, newColor1);
    emit colorListsChanged();
}

void ColorsListChoiceWidget::treatColorDeletingRequest()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int index = mKillButtonsVector.indexOf(button);
        if (index != -1) {
//            qDebug() << "sizeof mColorList " << mColorList.size();
//            qDebug() << "sizeof mKillButtonsVector " << mKillButtonsVector.size();

            disconnect(mColorChoiceWidgetsVector0[index], SIGNAL(currentColorChanged(QColor)),
                    this, SIGNAL(colorListsChanged()));
            disconnect(mColorChoiceWidgetsVector1[index], SIGNAL(currentColorChanged(QColor)),
                    this, SIGNAL(colorListsChanged()));

            mGridLayout->removeWidget(mColorChoiceWidgetsVector0[index]);
            mColorChoiceWidgetsVector0[index]->setParent(nullptr);
            mColorChoiceWidgetsVector0[index]->deleteLater();
            mGridLayout->removeWidget(mColorChoiceWidgetsVector1[index]);
            mColorChoiceWidgetsVector1[index]->setParent(nullptr);
            mColorChoiceWidgetsVector1[index]->deleteLater();
            mGridLayout->removeWidget(mKillButtonsVector[index]);
            mKillButtonsVector[index]->setParent(nullptr);
            mKillButtonsVector[index]->deleteLater();

            mKillButtonsVector.remove(index);
            mColorChoiceWidgetsVector0.remove(index);
            mColorChoiceWidgetsVector1.remove(index);
            emit colorListsChanged();
        }
    }
}


void ColorsListChoiceWidget::addColorWidgetWithButton(const QColor& color0, const QColor& color1)
{
    int newRowNumber = mKillButtonsVector.size() + 1;  //first row contains header
    ColorChoiseWidget* colorChoiceWidget0 = new ColorChoiseWidget();
    colorChoiceWidget0->setColor(color0);
    mColorChoiceWidgetsVector0.push_back(colorChoiceWidget0);
    ColorChoiseWidget* colorChoiceWidget1 = new ColorChoiseWidget();
    colorChoiceWidget1->setColor(color1);
    mColorChoiceWidgetsVector1.push_back(colorChoiceWidget1);
    QPushButton* newButton = new QPushButton(QIcon(":/remove"), "");
    mKillButtonsVector.push_back(newButton);
    mGridLayout->addWidget(colorChoiceWidget0, newRowNumber, 0);
    connect(colorChoiceWidget0, SIGNAL(currentColorChanged(QColor)),
            this, SIGNAL(colorListsChanged()));
    mGridLayout->addWidget(colorChoiceWidget1, newRowNumber, 1);
    connect(colorChoiceWidget1, SIGNAL(currentColorChanged(QColor)),
            this, SIGNAL(colorListsChanged()));
    mGridLayout->addWidget(newButton, newRowNumber, 2);
    connect(newButton, SIGNAL(clicked(bool)),
            this, SLOT(treatColorDeletingRequest()));

}

void ColorsListChoiceWidget::clearState()
{
    for (auto colorWidget : mColorChoiceWidgetsVector0) {
        mGridLayout->removeWidget(colorWidget);
        colorWidget->setParent(nullptr);
        colorWidget->deleteLater();
    }
    for (auto colorWidget : mColorChoiceWidgetsVector1) {
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
    mColorChoiceWidgetsVector0.clear();
    mColorChoiceWidgetsVector1.clear();


}

