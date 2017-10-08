#ifndef BRUSHPATTERNCHOICEDIALOG_H
#define BRUSHPATTERNCHOICEDIALOG_H

#include <QWidget>
#include "commonWidgets/imagechoice/imagechoicewidget.h"
#include <QPixmap>

class BrushPatternChoiceDialog: public ImageChoiceWidget
{
public:
    BrushPatternChoiceDialog(QWidget* parent = 0);


private:
    static QVector<QIcon> sIconsVector;
    static QVector<QPixmap> sPixmapVector;


    virtual void initializeIconsVector();
    virtual QVector<QIcon>& iconVector();
    virtual const QVector<QIcon>& iconVector() const;
    virtual QVector<QPixmap>& pixmapVector() ;
    virtual const QVector<QPixmap>& pixmapVector() const;
    virtual void addPixmap(const QPixmap& pixmap);
};

#endif // BRUSHPATTERNCHOICEDIALOG_H



