#include "imagechoicewidget.h"
#include "ui_imagechoicewidget.h"
#include <QPushButton>
#include <QIcon>
#include <QFileDialog>
#include "global/global_definitions.h"


QString ImageChoiceWidget::sDirectoryForLoading;

ImageChoiceWidget::ImageChoiceWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageChoiceWidget)
{

    ui->setupUi(this);
//    initializeIconsVector();
//    initilizeIcons();

    connect(ui->loadPushButton, SIGNAL(clicked(bool)),
            this, SLOT(loadIcon()));
}

ImageChoiceWidget::~ImageChoiceWidget()
{
    delete ui;
}

void ImageChoiceWidget::initilizeIcons()
{
    int checkedIconIndex = -1;
    for (auto button : mButtonsVector) {
        if (button->isChecked()) {
            checkedIconIndex = mButtonsVector.indexOf(button);
        }
        button->setParent(NULL);
        button->deleteLater();
        disconnect(button, SIGNAL(clicked(bool)),
                this, SLOT(treatButtonClicking()));
    }
    mButtonsVector.clear();

    QPushButton *newB = NULL;
    const int colNum = std::ceil(std::sqrt(iconVector().size()));
    for (int i = 0; i < iconVector().size(); ++i) {
        newB = new QPushButton(iconVector()[i], QString());
        newB->setFlat(true);
        newB->setCheckable(true);
        newB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        newB->setFixedSize(100, 100);
        newB->setIconSize(QSize(95, 95));
        connect(newB, SIGNAL(clicked(bool)),
                this, SLOT(treatButtonClicking()));
        mButtonsVector.push_back(newB);
        ui->picturesGridLayout->addWidget(newB, i / colNum, i % colNum);
    }
    if (checkedIconIndex >= 0 && checkedIconIndex < mButtonsVector.size()) {
        mButtonsVector[checkedIconIndex]->setChecked(true);
    }
}

QIcon ImageChoiceWidget::currentIcon() const
{
    for (auto button : mButtonsVector) {
        if (button->isChecked()) {
            return iconVector()[mButtonsVector.indexOf(button)];
        }
    }
    return QIcon();
}

QPixmap ImageChoiceWidget::currentPixmap(int size) const
{
    for (auto button : mButtonsVector) {
        if (button->isChecked()) {
            return iconVector()[mButtonsVector.indexOf(button)].pixmap(size, size);
        }
    }
    return QPixmap();
}

QPixmap ImageChoiceWidget::currentPixmap() const
{
    for (auto button : mButtonsVector) {
        if (button->isChecked()) {
            return pixmapVector()[mButtonsVector.indexOf(button)];
        }
    }
    return QPixmap();

}

void ImageChoiceWidget::showEvent(QShowEvent *event)
{
    if (mButtonsVector.size() == 0) {
        initializeIconsVector();
        initilizeIcons();
    }
    QDialog::showEvent(event);

}

//void ImageChoiceWidget::addIcon(const QIcon &icon)
//{
//    iconVector().push_back(icon);
//    initilizeIcons();
//}

void ImageChoiceWidget::loadIcon()
{
    if (sDirectoryForLoading.isEmpty()) {
        sDirectoryForLoading = global::gDefaultDirectory;
    }

    //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//#ifdef __linux
    QFileDialog::Option option =  QFileDialog::DontUseNativeDialog;
//#else
//    QFileDialog::Option option = 0x0;
//#endif
    QString fileName = QFileDialog::getOpenFileName (this, tr("Choose image"), sDirectoryForLoading, tr("Images (*.png *.xpm *.jpg)"), NULL, option);

    if (!fileName.isEmpty()) {
        sDirectoryForLoading = QFileInfo(fileName).absoluteDir().absolutePath();
        addPixmap(QPixmap(fileName));
        initilizeIcons();
        setAllButtonsChecked(false);
        mButtonsVector[mButtonsVector.size()-1]->setChecked(true);
    }
}

void ImageChoiceWidget::treatButtonClicking()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        setAllButtonsChecked(false);
        button->setChecked(true);
    }
}



void ImageChoiceWidget::setAllButtonsChecked(bool checked)
{
    for (auto button : mButtonsVector) {
        button->setChecked(checked);
    }
}



