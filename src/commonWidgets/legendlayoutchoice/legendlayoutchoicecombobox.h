#ifndef LEGENDLAYOUTCHOICECOMBOBOX_H
#define LEGENDLAYOUTCHOICECOMBOBOX_H


#include <QComboBox>
#include "core/plotlayoutmodel.h"

class PopupWidget;
class LegendLayoutChoiceWidget;


class LegendLayoutChoiceComboBox : public QComboBox
{
    Q_OBJECT
public:
    LegendLayoutChoiceComboBox(QWidget*parent = 0);

    void setLegendLocation(style::LegendLocation);

    style::LegendLocation legendLocation() const;

protected:
    void mousePressEvent(QMouseEvent*event);

protected slots:
    void treatLegendLocationChanging();

signals:
    void currentLegendLocationChanged(style::LegendLocation);

private:
    PopupWidget *mPopUpWidget;
    LegendLayoutChoiceWidget *mLegendLayoutChoiceWidget;

    QVector<std::tuple<style::LegendLocation, QString>> mLegendLocationMap;
    void setLabel(style::LegendLocation);
};

#endif // LEGENDLAYOUTCHOICECOMBOBOX_H


