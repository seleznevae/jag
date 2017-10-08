#include "levelprefseditingwidget.h"
#include "ui_levelprefseditingwidget.h"

LevelPrefsEditingWidget::LevelPrefsEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelPrefsEditingWidget)
{
    ui->setupUi(this);
    ui->penEditingWidget->setHeader("");
}

LevelPrefsEditingWidget::~LevelPrefsEditingWidget()
{
    delete ui;
}

void LevelPrefsEditingWidget::setLevelPrefs(const style::LevelPref &pref)
{
    disconnectAll();
    ui->labelEditingWidget->setLabelPrefs(pref.mLabel);
    ui->penEditingWidget->setPen(pref.mPen);
    connectAll();
}

style::LevelPref LevelPrefsEditingWidget::levelPrefs() const
{
    style::LevelPref result;
    result.mLabel = ui->labelEditingWidget->labelPrefs();
    result.mPen = ui->penEditingWidget->pen();
    return result;
}

void LevelPrefsEditingWidget::connectAll()
{
    connect(ui->labelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SLOT(treatPropertiesChanging()));
    connect(ui->penEditingWidget, SIGNAL(penChanged(QPen)),
            this, SLOT(treatPropertiesChanging()));
}

void LevelPrefsEditingWidget::disconnectAll()
{

    disconnect(ui->labelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SLOT(treatPropertiesChanging()));
    disconnect(ui->penEditingWidget, SIGNAL(penChanged(QPen)),
               this, SLOT(treatPropertiesChanging()));
}

void LevelPrefsEditingWidget::treatPropertiesChanging()
{
    emit levelPrefsChanged(levelPrefs());
}
