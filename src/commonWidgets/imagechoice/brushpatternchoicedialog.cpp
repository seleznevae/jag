#include "brushpatternchoicedialog.h"

QVector<QIcon> BrushPatternChoiceDialog::sIconsVector;
QVector<QPixmap> BrushPatternChoiceDialog::sPixmapVector;

BrushPatternChoiceDialog::BrushPatternChoiceDialog(QWidget* parent)
    :ImageChoiceWidget(parent)
{

}


void BrushPatternChoiceDialog::initializeIconsVector()
{
    if (sPixmapVector.size() == 0) {
        sPixmapVector.push_back(QPixmap(":/textureGravel"));
    }
}

QVector<QIcon> &BrushPatternChoiceDialog::iconVector()
{
    sIconsVector.clear();
    for (const auto & pixmap : sPixmapVector) {
        sIconsVector.push_back(pixmap);
    }
    return sIconsVector;
}

const QVector<QIcon> &BrushPatternChoiceDialog::iconVector() const
{
    sIconsVector.clear();
    for (const auto & pixmap : sPixmapVector) {
        sIconsVector.push_back(pixmap);
    }
    return sIconsVector;
}

QVector<QPixmap> &BrushPatternChoiceDialog::pixmapVector()
{
    return sPixmapVector;
}

const QVector<QPixmap> &BrushPatternChoiceDialog::pixmapVector() const
{
    return sPixmapVector;
}

void BrushPatternChoiceDialog::addPixmap(const QPixmap &pixmap)
{
    sPixmapVector.push_back(pixmap);
}



//QVector<QIcon> &ScatterIconChoiceDialog::iconVector()
//{
//    return sIconsVector;
//}

//const QVector<QIcon> &ScatterIconChoiceDialog::iconVector() const
//{
//    return sIconsVector;
//}

//QVector<QPixmap> &ScatterIconChoiceDialog::pixmapVector()
//{
//    sPixmapVector.clear();
//    for (const auto & icon : sIconsVector) {
//        sPixmapVector.push_back(icon.pixmap(400, 400));
//    }
//    return sPixmapVector;
//}

//const QVector<QPixmap> &ScatterIconChoiceDialog::pixmapVector() const
//{
//    sPixmapVector.clear();
//    for (const auto & icon : sIconsVector) {
//        sPixmapVector.push_back(icon.pixmap(400, 400));
//    }
//    return sPixmapVector;
//}
