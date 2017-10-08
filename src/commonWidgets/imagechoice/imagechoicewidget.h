#ifndef IMAGECHOICEWIDGET_H
#define IMAGECHOICEWIDGET_H

#include <QDialog>
#include <QVector>
#include <QIcon>

class QPushButton;


namespace Ui {
class ImageChoiceWidget;
}

class ImageChoiceWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ImageChoiceWidget(QWidget *parent = 0);
    ~ImageChoiceWidget();

    void initilizeIcons();

    QIcon currentIcon() const;
    QPixmap currentPixmap(int size) const;
    QPixmap currentPixmap() const;


protected:
    virtual void showEvent(QShowEvent * event);

private slots:
    void treatButtonClicking();
//    void addIcon(const QIcon& icon);

    void loadIcon();
private:
    Ui::ImageChoiceWidget *ui;
    QVector<QPushButton*> mButtonsVector;

    void setAllButtonsChecked(bool);

    static QString sDirectoryForLoading;

    virtual void initializeIconsVector() = 0;
    virtual QVector<QIcon>& iconVector() = 0;
    virtual const QVector<QIcon>& iconVector() const = 0;
    virtual QVector<QPixmap>& pixmapVector() = 0;
    virtual const QVector<QPixmap>& pixmapVector() const = 0;
    virtual void addPixmap(const QPixmap& pixamp) = 0;

};

#endif // IMAGECHOICEWIDGET_H
