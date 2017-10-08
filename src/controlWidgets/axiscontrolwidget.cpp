#include "axiscontrolwidget.h"
#include "ui_axiscontrolwidget.h"
#include <QDoubleValidator>
#include <boost/any.hpp>
#include "commonWidgets/imagechoice/brushpatternchoicedialog.h"

AxisControlWidget::AxisControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::AxisControlWidget)
{
    ui->setupUi(this);

    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);




//    connect(ui->yLabelLineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatYLabelChanging()));
//    connect(ui->yLowMarginLineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatSetYLowMarginRequest()));
//    connect(ui->yHighMarginLineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatSetYHighMarginRequest()));
//    connect(ui->scaleTypeComboBox, SIGNAL(valueActivated(boost::any)),
//            this, SLOT(treatScalingTypeChanging()));
//    connect(ui->stretchFactorDoubleSpinBox, SIGNAL(valueChanged(double)),
//            this, SLOT(treatStretchFactorChanging()));
//    connect(ui->backgroundColorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
//            this, SLOT(treatBackgroundColorChanging()));
//    connect(ui->backgroundStyleComboBox, SIGNAL(valueActivated(boost::any)),
//            this, SLOT(treatBackgroundStyleChanging()));
//    connect(ui->backgroundTextureChoiceToolButton, SIGNAL(clicked(bool)),
//            this, SLOT(showBackgroundTextureChoiceDialog()));
//    connect(ui->legendTitleLineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatLegendTitleChanging()));

    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
    ui->stretchFactorDoubleSpinBox->setKeyboardTracking(false);

    ui->scaleTypeComboBox->addItem("Linear"     ,  AxisModel::ScaleType::Linear     );
    ui->scaleTypeComboBox->addItem("Logarithmic",  AxisModel::ScaleType::Logarithmic);


    ui->backgroundStyleComboBox->addItem("NoBrush", Qt::NoBrush);
    ui->backgroundStyleComboBox->addItem("Solid", Qt::SolidPattern);
    ui->backgroundStyleComboBox->addItem("Dense1", Qt::Dense1Pattern);
    ui->backgroundStyleComboBox->addItem("Dense2", Qt::Dense2Pattern);
    ui->backgroundStyleComboBox->addItem("Dense3", Qt::Dense3Pattern);
    ui->backgroundStyleComboBox->addItem("Dense4", Qt::Dense4Pattern);
    ui->backgroundStyleComboBox->addItem("Dense5", Qt::Dense5Pattern);
    ui->backgroundStyleComboBox->addItem("Dense6", Qt::Dense6Pattern);
    ui->backgroundStyleComboBox->addItem("Dense7", Qt::Dense7Pattern);
    ui->backgroundStyleComboBox->addItem("Hor", Qt::HorPattern);
    ui->backgroundStyleComboBox->addItem("Ver", Qt::VerPattern);
    ui->backgroundStyleComboBox->addItem("Cross", Qt::CrossPattern);
    ui->backgroundStyleComboBox->addItem("BDiag", Qt::BDiagPattern);
    ui->backgroundStyleComboBox->addItem("FDiag", Qt::FDiagPattern);
    ui->backgroundStyleComboBox->addItem("DiagCross", Qt::DiagCrossPattern);
    ui->backgroundStyleComboBox->addItem("TexturePattern", Qt::TexturePattern);

     QDoubleValidator *doubleValidator = new QDoubleValidator(this);
     ui->yHighMarginLineEdit->setValidator(doubleValidator);
     ui->yLowMarginLineEdit->setValidator(doubleValidator);
}

AxisControlWidget::~AxisControlWidget()
{
    delete ui;
}

void AxisControlWidget::setAxisModels(QVector<AxisModel*> modelVector)
{
    if (mAxisModelVector != modelVector) {
        disconnectWidgets();
        mAxisModelVector = modelVector;

        for (const AxisModel* model : modelVector) {
            connect(model, SIGNAL(deletingStateIsOccuring()),
                    this, SLOT(treatAxisModelRemoval()));
        }

//        connect(mAxisModel, SIGNAL(destroyed(QObject*)),
//                this, SLOT(treatAxisModelRemoval()));
        initializeState();
        connectWidgets();
    }
}

void AxisControlWidget::setExtendedMode(bool)
{

}

void AxisControlWidget::treatMoveUpRequest()
{
    for (AxisModel* model : mAxisModelVector) {
        model->treatMovementRequest(model, AxisModel::AxisMovement::Up);
    }
//    emit movementRequest(mAxisModel,AxisModel::AxisMovement::Up);
}

void AxisControlWidget::treatMoveDownRequest()
{
    for (AxisModel* model : mAxisModelVector) {
        model->treatMovementRequest(model,AxisModel::AxisMovement::Down);
    }
//    emit movementRequest(mAxisModel,AxisModel::AxisMovement::Down);
}

//void AxisControlWidget::treatSetYMarginRequest()
//{
////    ui->yLowMarginLineEdit->clearFocus();
////    ui->yHighMarginLineEdit->clearFocus();
////    if (mAxisModelVector != NULL) {
////        mAxisModelVector->setYRange(
////                              ui->yLowMarginLineEdit->text().toDouble(),
////                              ui->yHighMarginLineEdit->text().toDouble()
////                             );
////    }


//    ui->yLowMarginLineEdit->clearFocus();
//    ui->yHighMarginLineEdit->clearFocus();
//    if (mAxisModelVector.size() > 0) {
//        for (AxisModel* model : mAxisModelVector) {
//            mAxisModelVector->treatMovementRequest(mAxisModelVector,AxisModel::AxisMovement::Down);
//            model->setYRange(
//                             ui->yLowMarginLineEdit->text().toDouble(),
//                             ui->yHighMarginLineEdit->text().toDouble()
//                             );
//        }
//    }


////    try {
////    emit settingYRangeRequest(
////                              ui->yLowMarginLineEdit->text().toDouble(),
////                              ui->yHighMarginLineEdit->text().toDouble()
////                              );
////    } catch (...) {

////    }

//}

void AxisControlWidget::treatSetYHighMarginRequest()
{
    ui->yHighMarginLineEdit->clearFocus();
    if (mAxisModelVector.size() > 0) {
        for (AxisModel* model : mAxisModelVector) {
            model->setYRange(
                             std::get<0>(model->getYRange()),
                             ui->yHighMarginLineEdit->text().toDouble()
                             );
        }
    }
}

void AxisControlWidget::treatSetYLowMarginRequest()
{
    ui->yLowMarginLineEdit->clearFocus();
    if (mAxisModelVector.size() > 0) {
        for (AxisModel* model : mAxisModelVector) {
            model->setYRange(
                             ui->yLowMarginLineEdit->text().toDouble(),
                             std::get<1>(model->getYRange())
                             );
        }
    }
}

void AxisControlWidget::treatScalingTypeChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setScaleType(ui->scaleTypeComboBox->currentValue<AxisModel::ScaleType>());
//    }
    ui->scaleTypeComboBox->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setScaleType(ui->scaleTypeComboBox->currentValue<AxisModel::ScaleType>());
    }
}

void AxisControlWidget::initializeState()
{
    if (mAxisModelVector.size() > 0) {
        bool equal = true;

        equal = true;
        double yRangeMin = std::get<0>(mAxisModelVector[0]->getYRange());
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (yRangeMin != std::get<0>(mAxisModelVector[i]->getYRange())) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->yLowMarginLineEdit->setText(QString::number(yRangeMin));
        } else {
            ui->yLowMarginLineEdit->setText("");
        }

        equal = true;
        double yRangeMax = std::get<1>(mAxisModelVector[0]->getYRange());
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (yRangeMax != std::get<1>(mAxisModelVector[i]->getYRange())) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->yHighMarginLineEdit->setText(QString::number(yRangeMax));
        } else {
            ui->yHighMarginLineEdit->setText("");
        }


        double stretch = mAxisModelVector[0]->stretchFactor();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (stretch != mAxisModelVector[i]->stretchFactor()) {
                stretch = -1.;
                break;
            }
        }
        if (stretch >= 0) {
            ui->stretchFactorDoubleSpinBox->setValue(stretch);
        } else {
            ui->stretchFactorDoubleSpinBox->setValue(0.);
            ui->stretchFactorDoubleSpinBox->setSpecialValueText(" ");
        }


        equal = true;
        auto scaleType = mAxisModelVector[0]->scaleType();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (scaleType != mAxisModelVector[i]->scaleType()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->scaleTypeComboBox->setCurrentValue(scaleType);
        } else {
            ui->scaleTypeComboBox->setCurrentValue(-1);
        }

        equal = true;
        auto backgroundStyle = mAxisModelVector[0]->backgroundBrush().style();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (backgroundStyle != mAxisModelVector[i]->backgroundBrush().style()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->backgroundStyleComboBox->setCurrentValue(backgroundStyle);
        } else {
            ui->backgroundStyleComboBox->setCurrentValue(-1);
        }

        equal = true;
        auto backgroundColor = mAxisModelVector[0]->backgroundBrush().color();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (backgroundColor != mAxisModelVector[i]->backgroundBrush().color()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->backgroundColorChoiceWidget->setColor(backgroundColor);
        } else {
            ui->backgroundColorChoiceWidget->setColor(ui->backgroundColorChoiceWidget->palette().color(QWidget::backgroundRole()));
        }

        equal = true;
        auto yAxisLabel = mAxisModelVector[0]->yAxisLabel();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (yAxisLabel != mAxisModelVector[i]->yAxisLabel()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->yLabelLineEdit->setText(yAxisLabel);
        } else {
            ui->yLabelLineEdit->setText("");
        }

        equal = true;
        auto legendTitle = mAxisModelVector[0]->legendTitle();
        for (int i = 1; i < mAxisModelVector.size(); ++i) {
            if (legendTitle != mAxisModelVector[i]->legendTitle()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->legendTitleLineEdit->setText(legendTitle);
        } else {
            ui->legendTitleLineEdit->setText("");
        }

    }
}

void AxisControlWidget::treatDeletingRequest()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->treatDeletingRequest();
//    }
    for (AxisModel* model : mAxisModelVector) {
        model->treatDeletingRequest();
    }
}

void AxisControlWidget::treatStretchFactorChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setStretchFactor(ui->stretchFactorDoubleSpinBox->value());
//    }
    ui->stretchFactorDoubleSpinBox->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setStretchFactor(ui->stretchFactorDoubleSpinBox->value());
    }
}

void AxisControlWidget::treatAxisModelRemoval()
{
    if (mAxisModelVector.size() > 0) {
        AxisModel* model = qobject_cast<AxisModel*>(sender());
        if (model && mAxisModelVector.indexOf(model) != -1) {
            hide();
            for (const AxisModel* axisModel : mAxisModelVector) {
                if (axisModel != model) {
                    disconnect(axisModel, 0, this, 0);
                }
            }
            mAxisModelVector.clear();
            emit widgetIsHiding();
        }
    }

}

void AxisControlWidget::treatYLabelChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setYAxisLabel(ui->yLabelLineEdit->text());
//        ui->yLabelLineEdit->clearFocus();
//    }

    ui->yLabelLineEdit->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setYAxisLabel(ui->yLabelLineEdit->text());
    }
}

void AxisControlWidget::treatLegendTitleChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setLegendTitle(ui->legendTitleLineEdit->text());
//        ui->legendTitleLineEdit->clearFocus();
//    }

    ui->legendTitleLineEdit->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setLegendTitle(ui->legendTitleLineEdit->text());
    }
}

void AxisControlWidget::treatBackgroundStyleChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setBackgroundStyle(ui->backgroundStyleComboBox->currentValue<Qt::BrushStyle>());
//        ui->backgroundStyleComboBox->clearFocus();
//    }

    ui->backgroundStyleComboBox->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setBackgroundStyle(ui->backgroundStyleComboBox->currentValue<Qt::BrushStyle>());
    }
}

void AxisControlWidget::treatBackgroundColorChanging()
{
//    if (mAxisModelVector != NULL) {
//        mAxisModelVector->setBackgroundColor(ui->backgroundColorChoiceWidget->color());
//        ui->backgroundColorChoiceWidget->clearFocus();
//    }

    ui->backgroundColorChoiceWidget->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        model->setBackgroundColor(ui->backgroundColorChoiceWidget->color());
    }
}

void AxisControlWidget::showBackgroundTextureChoiceDialog()
{
//    if (mAxisModelVector != NULL) {
//        BrushPatternChoiceDialog choiceWidget;
//        if (choiceWidget.exec() == QDialog::Accepted) {
//            mAxisModelVector->setBackgroundTexture(choiceWidget.currentPixmap());
//            ui->backgroundStyleComboBox->setCurrentValue(Qt::TexturePattern);
//        }
//        ui->backgroundTextureChoiceToolButton->clearFocus();
//    }

    ui->backgroundTextureChoiceToolButton->clearFocus();
    for (AxisModel* model : mAxisModelVector) {
        BrushPatternChoiceDialog choiceWidget;
        if (choiceWidget.exec() == QDialog::Accepted) {
            model->setBackgroundTexture(choiceWidget.currentPixmap());
            ui->backgroundStyleComboBox->setCurrentValue(Qt::TexturePattern);
        }
    }
}

void AxisControlWidget::disconnectWidgets()
{
    disconnect(ui->yLabelLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatYLabelChanging()));
    disconnect(ui->yLowMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatSetYLowMarginRequest()));
    disconnect(ui->yHighMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatSetYHighMarginRequest()));
    disconnect(ui->scaleTypeComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatScalingTypeChanging()));
    disconnect(ui->stretchFactorDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatStretchFactorChanging()));
    disconnect(ui->backgroundColorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBackgroundColorChanging()));
    disconnect(ui->backgroundStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBackgroundStyleChanging()));
    disconnect(ui->backgroundTextureChoiceToolButton, SIGNAL(clicked(bool)),
            this, SLOT(showBackgroundTextureChoiceDialog()));
    disconnect(ui->legendTitleLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatLegendTitleChanging()));
}

void AxisControlWidget::connectWidgets()
{
    connect(ui->yLabelLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatYLabelChanging()));
    connect(ui->yLowMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatSetYLowMarginRequest()));
    connect(ui->yHighMarginLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatSetYHighMarginRequest()));
    connect(ui->scaleTypeComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatScalingTypeChanging()));
    connect(ui->stretchFactorDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatStretchFactorChanging()));
    connect(ui->backgroundColorChoiceWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBackgroundColorChanging()));
    connect(ui->backgroundStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBackgroundStyleChanging()));
    connect(ui->backgroundTextureChoiceToolButton, SIGNAL(clicked(bool)),
            this, SLOT(showBackgroundTextureChoiceDialog()));
    connect(ui->legendTitleLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatLegendTitleChanging()));
}
