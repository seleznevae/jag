#include "horizontallevelcontrolwidget.h"
#include "ui_horizontallevelcontrolwidget.h"
#include <core/horizontallevel.h>
#include <QDoubleValidator>

HorizontalLevelControlWidget::HorizontalLevelControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::HorizontalLevelControlWidget), mHorizontalLevel(NULL)
{
    ui->setupUi(this);

    connect(ui->levelLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatLevelChanging()));
    connect(ui->labelLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatLabelChanging()));

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->levelLineEdit->setValidator(doubleValidator);
}

HorizontalLevelControlWidget::~HorizontalLevelControlWidget()
{
    delete ui;
}

void HorizontalLevelControlWidget::setHorizontalLevel(HorizontalLevel *level)
{
    if (mHorizontalLevel != level) {
        mHorizontalLevel = level;
        initializeState();
    }
}

void HorizontalLevelControlWidget::setExtendedMode(bool)
{

}

void HorizontalLevelControlWidget::treatArrowedTextDeleting(QObject *level)
{
    if (mHorizontalLevel != NULL) {
        if (level && level == mHorizontalLevel) {
            mHorizontalLevel = NULL;
            hide();
            emit widgetIsHiding();
        }
    }
}

void HorizontalLevelControlWidget::treatLevelChanging()
{
    if (mHorizontalLevel) {
        mHorizontalLevel->setLevel(ui->levelLineEdit->text().toDouble());
        ui->levelLineEdit->clearFocus();
    }
}

void HorizontalLevelControlWidget::treatLabelChanging()
{
    if (mHorizontalLevel) {
        mHorizontalLevel->setLabel(ui->labelLineEdit->text());
        ui->labelLineEdit->clearFocus();
    }
}

void HorizontalLevelControlWidget::initializeState()
{
    if (mHorizontalLevel) {
        connect(mHorizontalLevel, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatArrowedTextDeleting(QObject*)));

        ui->levelLineEdit->setText(QString::number(mHorizontalLevel->level()));
        ui->labelLineEdit->setText(mHorizontalLevel->label());

    }
}
