#include "legendlayoutchoicewidget.h"
#include "ui_legendlayoutchoicewidget.h"

LegendLayoutChoiceWidget::LegendLayoutChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegendLayoutChoiceWidget), mCurrentButton(nullptr)
{
    ui->setupUi(this);

    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::Left, ui->leftPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::Right, ui->rightPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::TopSeparate, ui->topSeparatedPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::TopCombined, ui->topCombinedPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::TopCombinedInRow, ui->topCombinedInRowPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::BottomSeparate, ui->bottomSeparatedPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::BottomCombined, ui->bottomCombinedPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::BottomCombinedInRow, ui->bottomCombinedInRowPushButton);


    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingLeft, ui->floatingLeftPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingRight, ui->floatingRightPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingTop, ui->floatingTopPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingBottom, ui->floatingBottomPushButton);

    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingTopLeft, ui->floatingTopLeftPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingTopRight, ui->floatingTopRightPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingBottomLeft, ui->floatingBottomLeftPushButton);
    mLegendLocationMap << std::tuple<style::LegendLocation, QPushButton*>(
                style::LegendLocation::FloatingBottomRight, ui->floatingBottomRightPushButton);

    for (auto &tup : mLegendLocationMap) {
        std::get<1>(tup)->setCheckable(true);
    }


    for (auto &tup : mLegendLocationMap) {
        connect(std::get<1>(tup), SIGNAL(clicked()),
                this, SLOT(treatButtonClicked()));

    }
    setCurrentButton(ui->rightPushButton);

}

LegendLayoutChoiceWidget::~LegendLayoutChoiceWidget()
{
    delete ui;
}

void LegendLayoutChoiceWidget::setCurrentLegendLocation(style::LegendLocation newCurrentLocation)
{
    for (auto &tup : mLegendLocationMap) {
        if (std::get<0>(tup) == newCurrentLocation) {
//            setEverythingChecked(false);
//            std::get<1>(tup)->setChecked(true);

            setCurrentButton(std::get<1>(tup));
            return;
        }
    }
}

void LegendLayoutChoiceWidget::treatButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        if (button == mCurrentButton) {
            return;
        }
        setCurrentButton(button);
        emit currentLegendLocationChanged(currentLocation());



//        for (auto &tup : mLegendLocationMap) {
//            if (std::get<1>(tup)->isChecked()) {
//                if (std::get<1>(tup) == button)
//                    return;
//                std::get<1>(tup)->setChecked(false);
//                button->setChecked(true);
//                for (auto &tup : mLegendLocationMap) {
//                    if (std::get<1>(tup)->isChecked()) {
//                        emit currentLegendLocationChanged(std::get<0>(tup));
//                        return;
//                    }
//                }
//            }
//        }
    }
}

style::LegendLocation LegendLayoutChoiceWidget::currentLocation() const
{
    for (auto &tup : mLegendLocationMap) {
        if (std::get<1>(tup)->isChecked()) {
            return std::get<0>(tup);
        }
    }
    return style::LegendLocation::Right;
}

void LegendLayoutChoiceWidget::setEverythingChecked(bool checked)
{
    for (auto &tup : mLegendLocationMap) {
        std::get<1>(tup)->setChecked(checked);
    }
}

void LegendLayoutChoiceWidget::setCurrentButton(QPushButton *newButton)
{
    setEverythingChecked(false);
    mCurrentButton = newButton;
    mCurrentButton->setChecked(true);

}
