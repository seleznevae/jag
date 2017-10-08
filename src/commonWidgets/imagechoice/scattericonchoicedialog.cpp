#include "scattericonchoicedialog.h"


QVector<QIcon> ScatterIconChoiceDialog::sIconsVector;
QVector<QPixmap> ScatterIconChoiceDialog::sPixmapVector;


ScatterIconChoiceDialog::ScatterIconChoiceDialog(QWidget* parent)
    :ImageChoiceWidget(parent)
{


}

void ScatterIconChoiceDialog::initializeIconsVector()
{
    if (sIconsVector.size() == 0) {
        sIconsVector.push_back(QIcon(":/markerMoon"));
        sIconsVector.push_back(QIcon(":/markerSun"));
    }
}

QVector<QIcon> &ScatterIconChoiceDialog::iconVector()
{
    return sIconsVector;
}

const QVector<QIcon> &ScatterIconChoiceDialog::iconVector() const
{
    return sIconsVector;
}

QVector<QPixmap> &ScatterIconChoiceDialog::pixmapVector()
{
    sPixmapVector.clear();
    for (const auto & icon : sIconsVector) {
        sPixmapVector.push_back(icon.pixmap(400, 400));
    }
    return sPixmapVector;
}

const QVector<QPixmap> &ScatterIconChoiceDialog::pixmapVector() const
{
    sPixmapVector.clear();
    for (const auto & icon : sIconsVector) {
        sPixmapVector.push_back(icon.pixmap(400, 400));
    }
    return sPixmapVector;
}

void ScatterIconChoiceDialog::addPixmap(const QPixmap &pixmap)
{
    sIconsVector.push_back(QIcon(pixmap));
}

