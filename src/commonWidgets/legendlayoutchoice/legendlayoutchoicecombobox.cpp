#include "legendlayoutchoicecombobox.h"
#include "commonWidgets/legendlayoutchoice/legendlayoutchoicewidget.h"
#include "popupwidget.h"

LegendLayoutChoiceComboBox::LegendLayoutChoiceComboBox(QWidget*parent)
    :QComboBox(parent), mPopUpWidget(nullptr), mLegendLayoutChoiceWidget(nullptr)
{
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::Left, "Left");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::Right, "Right");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::TopSeparate, "Top separated");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::TopCombined, "Top combined");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::TopCombinedInRow, "Top combined in a row");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::BottomSeparate, "Bottom separated");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::BottomCombined, "Bottom combined");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::BottomCombinedInRow, "Bottom combined in a row");

    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingLeft, "Inside left");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingRight, "Inside right");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingTop, "Inside top");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingBottom, "Inside bottom");

    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingTopLeft, "Inside top left");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingTopRight, "Inside top right");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingBottomLeft, "Inside bottom left");
    mLegendLocationMap << std::tuple<style::LegendLocation, QString>(
                style::LegendLocation::FloatingBottomRight, "Inside bottom right");

    mPopUpWidget = new PopupWidget(this);
    mLegendLayoutChoiceWidget = new LegendLayoutChoiceWidget;
    mPopUpWidget->setWidget(mLegendLayoutChoiceWidget);

    connect(mLegendLayoutChoiceWidget, SIGNAL(currentLegendLocationChanged(style::LegendLocation)),
            this, SLOT(treatLegendLocationChanging()));

}

void LegendLayoutChoiceComboBox::setLegendLocation(style::LegendLocation location)
{
    mLegendLayoutChoiceWidget->setCurrentLegendLocation(location);
    setLabel(location);
}

style::LegendLocation LegendLayoutChoiceComboBox::legendLocation() const
{
    return mLegendLayoutChoiceWidget->currentLocation();
}

void LegendLayoutChoiceComboBox::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mPopUpWidget->show(QPoint(QCursor::pos().x() - mPopUpWidget->width()/2, mapToGlobal(QPoint(0, height())).y()));
}

void LegendLayoutChoiceComboBox::treatLegendLocationChanging()
{
    mPopUpWidget->hide();
    auto location = legendLocation();
    setLabel(location);
    emit currentLegendLocationChanged(location);
}

void LegendLayoutChoiceComboBox::setLabel(style::LegendLocation location)
{
    for (const auto & tup : mLegendLocationMap) {
        if (std::get<0>(tup) == location) {
            clear();
            addItem(std::get<1>(tup));
        }
    }
}


