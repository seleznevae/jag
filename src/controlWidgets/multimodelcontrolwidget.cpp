#include "multimodelcontrolwidget.h"
#include "ui_multimodelcontrolwidget.h"
#include "global/global_definitions.h"
#include <QColorDialog>
#include <QDoubleValidator>
#include "commonWidgets/imagechoice/scattericonchoicedialog.h"
#include "commonWidgets/imagechoice/brushpatternchoicedialog.h"
#include "dataconverter/graphmodelfiltercontainer.h"
#include "controlWidgets/fastentoolbutton.h"


QVector<bool> MultiModelControlWidget::sTabVisibilityVector = {true, true, true, true, true};
QVector<QString> MultiModelControlWidget::sTabNamesVector = {"Properties", "Markers", "Brush", "Transformation", "Filters"};

MultiModelControlWidget::MultiModelControlWidget(QWidget *parent) :
    ControlWidget(parent),
    ui(new Ui::MultiModelControlWidget), mLayoutModel(NULL)
{
    ui->setupUi(this);


    mFastenToolButtonVector << ui->fastenToolButton_0;
    mFastenToolButtonVector << ui->fastenToolButton_1;
    mFastenToolButtonVector << ui->fastenToolButton_2;
    mFastenToolButtonVector << ui->fastenToolButton_3;
    mFastenToolButtonVector << ui->fastenToolButton_4;
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        mFastenToolButtonVector[i]->setChecked(sTabVisibilityVector[i]);
        connect(mFastenToolButtonVector[i], SIGNAL(toggled(bool)),
                this, SLOT(treatFastenToolButtonPressed()));
    }
    mTabsVector << ui->tab_0;
    mTabsVector << ui->tab_1;
    mTabsVector << ui->tab_2;
    mTabsVector << ui->tab_3;
    mTabsVector << ui->tab_4;




    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setMargin(0);
    ui->gridLayout_2->setSpacing(0);
    ui->gridLayout_2->setMargin(0);
    ui->gridLayout_3->setSpacing(0);
    ui->gridLayout_3->setMargin(0);
    ui->gridLayout_2->setSpacing(0);
    ui->gridLayout_2->setMargin(0);
    ui->gridLayout_3->setSpacing(0);
    ui->gridLayout_3->setMargin(0);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout_3->setSpacing(0);
    ui->verticalLayout_3->setMargin(0);
    ui->verticalLayout_4->setSpacing(0);
    ui->verticalLayout_4->setMargin(0);
    ui->verticalLayout_5->setSpacing(0);
    ui->verticalLayout_5->setMargin(0);
    ui->verticalLayout_6->setSpacing(0);
    ui->verticalLayout_6->setMargin(0);
    ui->tab_2->layout()->setSpacing(0);
    ui->tab_2->layout()->setMargin(0);
    ui->tab_1->layout()->setSpacing(0);
    ui->tab_1->layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setMargin(0);

    ui->lineWidthDoubleSpinBox->setSpecialValueText(" ");


//     ui->actionNext_scope->setIcon(global::getStyledIcon("redo"));


    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssNone"),             QCPScatterStyle::ScatterShape::ssNone);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssDot"),              QCPScatterStyle::ScatterShape::ssDot);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssCross"),            QCPScatterStyle::ScatterShape::ssCross);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssPlus"),             QCPScatterStyle::ScatterShape::ssPlus);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssCircle"),           QCPScatterStyle::ScatterShape::ssCircle);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssDisc"),             QCPScatterStyle::ScatterShape::ssDisc);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssSquare"),           QCPScatterStyle::ScatterShape::ssSquare);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssDiamond"),          QCPScatterStyle::ScatterShape::ssDiamond);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssStar"),             QCPScatterStyle::ScatterShape::ssStar);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssTriangle"),         QCPScatterStyle::ScatterShape::ssTriangle);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssTriangleInverted"), QCPScatterStyle::ScatterShape::ssTriangleInverted);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssCrossSquare"),      QCPScatterStyle::ScatterShape::ssCrossSquare);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssPlusSquare"),       QCPScatterStyle::ScatterShape::ssPlusSquare);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssCrossCircle"),      QCPScatterStyle::ScatterShape::ssCrossCircle);
    ui->scatterShapeComboBox->addItem(global::getStyledIcon("ssPlusCircle"),       QCPScatterStyle::ScatterShape::ssPlusCircle);
    ui->scatterShapeComboBox->addItem(QString("Custom pixmap"),                    QCPScatterStyle::ScatterShape::ssPixmap);

    ui->lineStyleComboBox->addItem(global::getStyledIcon("ls_solid"), Qt::SolidLine);
    ui->lineStyleComboBox->addItem(global::getStyledIcon("ls_dash"), Qt::DashLine);
    ui->lineStyleComboBox->addItem(global::getStyledIcon("ls_dot"), Qt::DotLine);
    ui->lineStyleComboBox->addItem(global::getStyledIcon("ls_dash_dot"), Qt::DashDotLine);
    ui->lineStyleComboBox->addItem(global::getStyledIcon("ls_dash_dot_dot"), Qt::DashDotDotLine);

    ui->lineInterpolationComboBox->addItem("Line",          GraphModel::LineStyle::lsLine);
    ui->lineInterpolationComboBox->addItem("Smooth",        GraphModel::LineStyle::lsSmooth);
    ui->lineInterpolationComboBox->addItem("Step Left",     GraphModel::LineStyle::lsStepLeft);
    ui->lineInterpolationComboBox->addItem("Step Right",    GraphModel::LineStyle::lsStepRight);
    ui->lineInterpolationComboBox->addItem("Step Center",   GraphModel::LineStyle::lsStepCenter);
    ui->lineInterpolationComboBox->addItem("Impulse",       GraphModel::LineStyle::lsImpulse);
    ui->lineInterpolationComboBox->addItem("None",          GraphModel::LineStyle::lsNone);

    ui->brushStyleComboBox->addItem("NoBrush", Qt::NoBrush);
    ui->brushStyleComboBox->addItem("Solid", Qt::SolidPattern);
    ui->brushStyleComboBox->addItem("Dense1", Qt::Dense1Pattern);
    ui->brushStyleComboBox->addItem("Dense2", Qt::Dense2Pattern);
    ui->brushStyleComboBox->addItem("Dense3", Qt::Dense3Pattern);
    ui->brushStyleComboBox->addItem("Dense4", Qt::Dense4Pattern);
    ui->brushStyleComboBox->addItem("Dense5", Qt::Dense5Pattern);
    ui->brushStyleComboBox->addItem("Dense6", Qt::Dense6Pattern);
    ui->brushStyleComboBox->addItem("Dense7", Qt::Dense7Pattern);
    ui->brushStyleComboBox->addItem("Hor", Qt::HorPattern);
    ui->brushStyleComboBox->addItem("Ver", Qt::VerPattern);
    ui->brushStyleComboBox->addItem("Cross", Qt::CrossPattern);
    ui->brushStyleComboBox->addItem("BDiag", Qt::BDiagPattern);
    ui->brushStyleComboBox->addItem("FDiag", Qt::FDiagPattern);
    ui->brushStyleComboBox->addItem("DiagCross", Qt::DiagCrossPattern);
    ui->brushStyleComboBox->addItem("TexturePattern", Qt::TexturePattern);

    ui->stringRepresentationComboBox->addItem("Decimal", GraphModel::StringRepresentation::Decimal);
    ui->stringRepresentationComboBox->addItem("Hexadecimal", GraphModel::StringRepresentation::Hexadecimal);
    ui->stringRepresentationComboBox->addItem("Binary", GraphModel::StringRepresentation::Binary);

    ui->brushBasisComboBox->addItem("Low Basis", QCPGraph::BasisFillType::LowBasis);
    ui->brushBasisComboBox->addItem("Zero Basis", QCPGraph::BasisFillType::ZeroBasis);
    ui->brushBasisComboBox->addItem("High Basis", QCPGraph::BasisFillType::HighBasis);
    ui->brushBasisComboBox->addItem("Total Fill", QCPGraph::BasisFillType::TotalFill);


    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->x0LineEdit->setValidator(doubleValidator);
    ui->kXLineEdit->setValidator(doubleValidator);
    ui->dXLineEdit->setValidator(doubleValidator);
    ui->y0LineEdit->setValidator(doubleValidator);
    ui->kYLineEdit->setValidator(doubleValidator);
    ui->dYLineEdit->setValidator(doubleValidator);



    //NOTE: To avoid signal valueChanged emitting on each keyboard button pressing. ValueChanged signal will be emitted
    //only after Return is pressed or if widget loses focus.
    ui->lineWidthDoubleSpinBox->setKeyboardTracking(false);
    ui->scatterSizeDoubleSpinBox->setKeyboardTracking(false);
    ui->scatterDecimationSpinBox->setKeyboardTracking(false);

    connect(ui->markerIconChoiceToolButton, SIGNAL(clicked(bool)),
            this, SLOT(showMarkerIconChoiceDialog()));
    connect(ui->brushTextureChoiceToolButton, SIGNAL(clicked(bool)),
            this, SLOT(showBrushTextureChoiceDialog()));

    connect(ui->copyPropertiesPushButton, SIGNAL(clicked()),
            this, SLOT(copyGraphProperties()));
    connect(ui->pastePropertiesPushButton, SIGNAL(clicked()),
            this, SLOT(pasteGraphProperties()));




}

MultiModelControlWidget::~MultiModelControlWidget()
{
    delete ui;
}

void MultiModelControlWidget::setModelVector(QVector<GraphModel *> newModelVector)
{
    if (newModelVector != mGraphVector) {
        disconnectFromModels();
        mGraphVector = newModelVector;
        initializeStateFromModels();
        connectToModels();
    }
}

void MultiModelControlWidget::setLayoutModel(PlotLayoutModel *model)
{
    if (mLayoutModel != model) {
        mLayoutModel = model;
    }
}

void MultiModelControlWidget::setExtendedMode(bool isModeExtended)
{
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        mFastenToolButtonVector[i]->setVisible(isModeExtended);
    }

    int limit = ui->tabWidget->count();
    for (int i = 0; i < limit; ++i)
        ui->tabWidget->removeTab(0);
    for (int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        ui->tabWidget->insertTab(i, mTabsVector[i], sTabNamesVector[i]);

    }

    if (isModeExtended == false) {
        for (int i = sTabVisibilityVector.size() - 1; i >= 0; --i) {
            if (sTabVisibilityVector[i] == false)
                ui->tabWidget->removeTab(i);
        }
    }
}

void MultiModelControlWidget::initializeStateFromModels()
{
    if (mGraphVector.size() > 0) {
        bool equal = true;



        bool isVisible = mGraphVector[0]->isVisible();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (isVisible != mGraphVector[i]->isVisible()) {
                isVisible = false;
                break;
            }
        }
        ui->visibilityCheckBox->setChecked(isVisible);

        bool isLegendable = mGraphVector[0]->isLegendable();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (isLegendable != mGraphVector[i]->isLegendable()) {
                isLegendable = false;
                break;
            }
        }
        ui->legendabilityCheckBox->setChecked(isLegendable);

        equal = true;
        auto name = mGraphVector[0]->getName();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (name != mGraphVector[i]->getName()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->nameLineEdit->setText(name);
        } else {
            ui->nameLineEdit->setText("");
        }

        equal = true;
        auto dimension = mGraphVector[0]->unit();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (dimension != mGraphVector[i]->unit()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->unitLineEdit->setText(dimension);
        } else {
            ui->unitLineEdit->setText("");
        }


        QColor lineColor = mGraphVector[0]->getPen().color();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (lineColor != mGraphVector[i]->getPen().color()) {
                lineColor = ui->lineColorWidget->palette().color(QWidget::backgroundRole());
                break;
            }
        }
        ui->lineColorWidget->setColor(lineColor);


        double width = mGraphVector[0]->getPen().widthF();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (width != mGraphVector[i]->getPen().widthF()) {
                width = -1.;
                break;
            }
        }
        if (width >= 0) {
            ui->lineWidthDoubleSpinBox->setValue(width);
        } else {
            ui->lineWidthDoubleSpinBox->setValue(0.);
            ui->lineWidthDoubleSpinBox->setSpecialValueText(" ");
        }


        equal = true;
        Qt::PenStyle penStyle = mGraphVector[0]->getPen().style();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (penStyle != mGraphVector[i]->getPen().style()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->lineStyleComboBox->setCurrentValue(penStyle);
        } else {
            ui->lineStyleComboBox->setCurrentIndex(-1);
        }

        equal = true;
        GraphModel::LineStyle interpolation = mGraphVector[0]->getLineInterpolation();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (interpolation != mGraphVector[i]->getLineInterpolation()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->lineInterpolationComboBox->setCurrentValue(interpolation);
        } else {
            ui->lineInterpolationComboBox->setCurrentIndex(-1);
        }


        equal = true;
        GraphModel::StringRepresentation representation = mGraphVector[0]->stringRepresentation();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (representation != mGraphVector[i]->stringRepresentation()) {
                equal = false;
                break;
            }
        }
        if (equal) {
           ui->stringRepresentationComboBox->setCurrentValue(representation);
        } else {
            ui->stringRepresentationComboBox->setCurrentIndex(-1);
        }

        equal = true;
        QCPScatterStyle::ScatterShape scatterShape = mGraphVector[0]->getScatterStyle().shape();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (scatterShape != mGraphVector[i]->getScatterStyle().shape()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->scatterShapeComboBox->setCurrentValue(scatterShape);
        } else {
            ui->scatterShapeComboBox->setCurrentIndex(-1);
        }

        double scatterSize = mGraphVector[0]->getScatterStyle().size();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (scatterSize != mGraphVector[i]->getScatterStyle().size()) {
                scatterSize = -1.;
                break;
            }
        }
        if (scatterSize >= 0) {
            ui->scatterSizeDoubleSpinBox->setValue(scatterSize);
        } else {
            ui->scatterSizeDoubleSpinBox->setValue(0.);
            ui->scatterSizeDoubleSpinBox->setSpecialValueText(" ");
        }

        double scatterDecimation = mGraphVector[0]->getScatterStyle().decimation();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (scatterDecimation != mGraphVector[i]->getScatterStyle().decimation()) {
                scatterDecimation = -1.;
                break;
            }
        }
        if (scatterDecimation >= 0) {
            ui->scatterDecimationSpinBox->setValue(scatterDecimation);
        } else {
            ui->scatterDecimationSpinBox->setValue(0.);
            ui->scatterDecimationSpinBox->setSpecialValueText(" ");
        }

        bool areDataLabelsVisible = mGraphVector[0]->getScatterStyle().dataLabelsEnabled();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (areDataLabelsVisible != mGraphVector[i]->getScatterStyle().dataLabelsEnabled()) {
                areDataLabelsVisible = false;
                break;
            }
        }
        ui->dataLabelsCheckBox->setChecked(areDataLabelsVisible);
        ui->dataLabelsColorWidget->setEnabled(ui->dataLabelsCheckBox->isChecked());

        QColor dataLabelsFontColor = mGraphVector[0]->getScatterStyle().dataLabelsFontColor();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (dataLabelsFontColor != mGraphVector[i]->getScatterStyle().dataLabelsFontColor()) {
                dataLabelsFontColor = ui->dataLabelsColorWidget->palette().color(QWidget::backgroundRole());
                break;
            }
        }
        ui->dataLabelsColorWidget->setColor(dataLabelsFontColor);



        QColor markerBrushColor = mGraphVector[0]->getScatterStyle().brush().color();
        if (mGraphVector[0]->getScatterStyle().brush() == Qt::NoBrush)
            markerBrushColor = ui->scatterBrushColorWidget->palette().color(QWidget::backgroundRole());
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (markerBrushColor != mGraphVector[i]->getScatterStyle().brush().color()
                    || mGraphVector[i]->getScatterStyle().brush() == Qt::NoBrush) {
                markerBrushColor = ui->scatterBrushColorWidget->palette().color(QWidget::backgroundRole());
                break;
            }
        }
        ui->scatterBrushColorWidget->setColor(markerBrushColor);

        QColor markerPenColor = mGraphVector[0]->getScatterStyle().pen().color();
        if (!mGraphVector[0]->getScatterStyle().isPenDefined())
            markerPenColor = ui->scatterPenColorWidget->palette().color(QWidget::backgroundRole());
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (markerPenColor != mGraphVector[i]->getScatterStyle().pen().color()
                    || mGraphVector[i]->getScatterStyle().isPenDefined()) {
                markerPenColor = ui->scatterPenColorWidget->palette().color(QWidget::backgroundRole());
                break;
            }
        }
        ui->scatterPenColorWidget->setColor(markerPenColor);


        double scatterPenWidth = mGraphVector[0]->getScatterStyle().pen().widthF();
        if (!mGraphVector[0]->getScatterStyle().isPenDefined())
            scatterPenWidth = -1;
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (scatterPenWidth != mGraphVector[i]->getScatterStyle().pen().widthF()
                    || !mGraphVector[i]->getScatterStyle().isPenDefined()) {
                scatterPenWidth = -1.;
                break;
            }
        }
        if (scatterPenWidth >= 0) {
            ui->scatterPenWidthDoubleSpinBox->setValue(scatterPenWidth);
        } else {
            ui->scatterPenWidthDoubleSpinBox->setValue(0.);
            ui->scatterPenWidthDoubleSpinBox->setSpecialValueText(" ");
        }


        QPen scatterPen = mGraphVector[0]->getScatterStyle().pen();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (scatterPen != mGraphVector[i]->getScatterStyle().pen()) {
                scatterPen = QPen(Qt::NoPen);
                break;
            }
        }
        ui->lineColorWidget->setColor(lineColor);





        equal = true;
        Qt::BrushStyle brushStyle = mGraphVector[0]->brush().style();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (brushStyle != mGraphVector[i]->brush().style()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->brushStyleComboBox->setCurrentValue(brushStyle);
        } else {
            ui->brushStyleComboBox->setCurrentIndex(-1);
        }


//            ui->brushBasisComboBox->addItem("High Basis", QCPGraph::BasisFillType::HighBasis);

        equal = true;
        QCPGraph::BasisFillType brushBasis = mGraphVector[0]->basisFillType();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (brushBasis != mGraphVector[i]->basisFillType()) {
                equal = false;
                break;
            }
        }
        if (equal) {
            ui->brushBasisComboBox->setCurrentValue(brushBasis);
        } else {
            ui->brushBasisComboBox->setCurrentIndex(-1);
        }

        QColor brushColor = mGraphVector[0]->brush().color();
        for (int i = 1; i < mGraphVector.size(); ++i) {
            if (brushColor != mGraphVector[i]->brush().color()) {
                brushColor = ui->brushColorWidget->palette().color(QWidget::backgroundRole());
                break;
            }
        }
        ui->brushColorWidget->setColor(brushColor);


        bool equalX0 = true;
        bool equalKX = true;
        bool equalDX = true;
        bool equalY0 = true;
        bool equalKY = true;
        bool equalDY = true;

        std::shared_ptr<AffineConverter> affineConverter = mGraphVector[0]->getAffineConverter();
        double x0 = 0.;
        double dX = 0.;
        double kX = 1.;
        double y0 = 0.;
        double dY = 0.;
        double kY = 1.;
        if (affineConverter != NULL) {
            x0 = affineConverter->x0();
            dX = affineConverter->dX();
            kX = affineConverter->kX();

            y0 = affineConverter->y0();
            dY = affineConverter->dY();
            kY = affineConverter->kY();
        }
        for (int i = 1; i < mGraphVector.size(); ++i) {
            affineConverter = mGraphVector[i]->getAffineConverter();
            double cur_x0 = 0.;
            double cur_dX = 0.;
            double cur_kX = 1.;
            double cur_y0 = 0.;
            double cur_dY = 0.;
            double cur_kY = 1.;
            if (affineConverter != NULL) {
                cur_x0 = affineConverter->x0();
                cur_dX = affineConverter->dX();
                cur_kX = affineConverter->kX();
                cur_y0 = affineConverter->y0();
                cur_dY = affineConverter->dY();
                cur_kY = affineConverter->kY();
            }
            equalX0 = (x0 == cur_x0) ? true : false;
            equalDX = (dX == cur_dX) ? true : false;
            equalKX = (kX == cur_kX) ? true : false;
            equalY0 = (y0 == cur_y0) ? true : false;
            equalDY = (dY == cur_dY) ? true : false;
            equalKY = (kY == cur_kY) ? true : false;
        }
        ui->x0LineEdit->setText(equalX0 ? QString::number(x0) : "");
        ui->dXLineEdit->setText(equalDX ? QString::number(dX) : "");
        ui->kXLineEdit->setText(equalKX ? QString::number(kX) : "");
        ui->y0LineEdit->setText(equalY0 ? QString::number(y0) : "");
        ui->dYLineEdit->setText(equalDY ? QString::number(dY) : "");
        ui->kYLineEdit->setText(equalKY ? QString::number(kY) : "");
    }

    if (mGraphVector.size() == 1 && mGraphVector[0]) {
        ui->copyPropertiesPushButton->show();
        ui->pastePropertiesPushButton->show();
        ui->tabWidget->setTabEnabled(4, true);

        auto modelWrapper = std::make_shared<GraphModelFilterContainer>();
        modelWrapper->setGraphModel(mGraphVector[0]);
        ui->filterContainer->setModel(modelWrapper);
    } else {
        ui->copyPropertiesPushButton->hide();
        ui->pastePropertiesPushButton->hide();
        ui->tabWidget->setTabEnabled(4, false);
    }

    bool areAllGraphsCurves = true;
    for (auto model : mGraphVector) {
        if (model->graphType() == GraphModel::GraphType::Function) {
            areAllGraphsCurves = false;
            break;
        }
    }
    if (areAllGraphsCurves) {
//        ui->lineInterpolationComboBox->setEnabled(false);
        ui->brushBasisComboBox->setEnabled(false);
    } else {
//        ui->lineInterpolationComboBox->setEnabled(true);
        ui->brushBasisComboBox->setEnabled(true);
    }
}

void MultiModelControlWidget::treatVisibilityChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setVisibility(ui->visibilityCheckBox->isChecked());
            }
        }
    }
    ui->visibilityCheckBox->clearFocus();
}

void MultiModelControlWidget::treatLegendabilityChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setLegendability(ui->legendabilityCheckBox->isChecked());
            }
        }
    }
    ui->legendabilityCheckBox->clearFocus();
}

void MultiModelControlWidget::treatNameChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setName(ui->nameLineEdit->text());
            }
        }
    }
    ui->nameLineEdit->clearFocus();
}

void MultiModelControlWidget::treatDimensionChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setUnit(ui->unitLineEdit->text());
            }
        }
    }
    ui->unitLineEdit->clearFocus();
}

void MultiModelControlWidget::treatLineWidthChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setWidth(ui->lineWidthDoubleSpinBox->value());
            }
        }
    }
    ui->lineWidthDoubleSpinBox->clearFocus();
}

void MultiModelControlWidget::treatLineColorChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setColor(ui->lineColorWidget->color());
            }
        }
    }
    ui->lineColorWidget->clearFocus();
}

//void MultiModelControlWidget::showLineColorDialog()
//{
//    QColor newColor = QColorDialog::getColor(ui->lineColorColorButton->color(), this, "Choose line color", QColorDialog::ShowAlphaChannel);
//    if (newColor.isValid()) {
//        if (gColorSceme.indexOf(newColor) == -1) {
//            gColorSceme << newColor;
//            ui->lineColorColorButton->setScheme(&gColorSceme);
//        }
//        ui->lineColorColorButton->setColor(newColor);
//        treatLineColorChanging();
//    }
//    ui->lineColorDialogToolButton->clearFocus();
//}

void MultiModelControlWidget::treatLineStyleChanging()
{
//    if (index >= 0 && index < lineStyleMap.size()) {
//        if (mGraphVector.size() != 0) {
//            for (const auto & graph : mGraphVector) {
//                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setLineStyle(std::get<0>(lineStyleMap[index]));
//                }
//            }
//        }
//    }
    if (ui->lineStyleComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto lineStyle = ui->lineStyleComboBox->currentValue<Qt::PenStyle>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setLineStyle(lineStyle);
                }
            }
        }
    }

    ui->lineStyleComboBox->clearFocus();
}

void MultiModelControlWidget::treatInterpolationChanging()
{
//    if (index >= 0 && index < lineInterpolationMap.size()) {
//        if (mGraphVector.size() != 0) {
//            for (const auto & graph : mGraphVector) {
//                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setLineInterpolation(std::get<0>(lineInterpolationMap[index]));
//                }
//            }
//        }
//    }

    if (ui->lineInterpolationComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto lineInterpolation = ui->lineInterpolationComboBox->currentValue<GraphModel::LineStyle>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setLineInterpolation(lineInterpolation);
                }
            }
        }
    }

    ui->lineStyleComboBox->clearFocus();
    ui->lineInterpolationComboBox->clearFocus();
}

void MultiModelControlWidget::showMarkerIconChoiceDialog()
{
    ScatterIconChoiceDialog choiceWidget;
    if (choiceWidget.exec() == QDialog::Accepted) {
        if (mGraphVector.size() != 0) {
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setScatterPixmap(choiceWidget.currentPixmap(graph->getScatterStyle().size()));
                    graph->setScatterIcon(choiceWidget.currentIcon());
                }
            }
            ui->scatterShapeComboBox->setCurrentValue(QCPScatterStyle::ScatterShape::ssPixmap);
        }
    }
    ui->markerIconChoiceToolButton->clearFocus();
}

void MultiModelControlWidget::showBrushTextureChoiceDialog()
{
    BrushPatternChoiceDialog choiceWidget;
    if (choiceWidget.exec() == QDialog::Accepted) {
        if (mGraphVector.size() != 0) {
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setScatterPixmap(choiceWidget.currentPixmap(graph->getScatterStyle().size()));
                    graph->setBrushTexture(choiceWidget.currentPixmap());
                }
            }
            ui->brushStyleComboBox->setCurrentValue(Qt::TexturePattern);
        }
    }
    ui->brushTextureChoiceToolButton->clearFocus();
}

void MultiModelControlWidget::treatScatterShapeChanging()
{
//    if (index >= 0 && index < scatterShapeMap.size()) {
//        if (mGraphVector.size() != 0) {
//            for (const auto & graph : mGraphVector) {
//                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setScatterShape(std::get<0>(scatterShapeMap[index]));
//                }
//            }
//        }
//    }

    if (ui->scatterShapeComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto scatterShape = ui->scatterShapeComboBox->currentValue<QCPScatterStyle::ScatterShape>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setScatterShape(scatterShape);
                }
            }
        }
    }

    ui->scatterShapeComboBox->clearFocus();
    initializeStateFromModels();
}

void MultiModelControlWidget::treatScatterSizeChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setScatterSize(ui->scatterSizeDoubleSpinBox->value());
            }
        }
    }
    ui->scatterSizeDoubleSpinBox->clearFocus();
}

void MultiModelControlWidget::treatScatterDecimationChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setScatterDecimation(ui->scatterDecimationSpinBox->value());
            }
        }
    }
    ui->scatterDecimationSpinBox->clearFocus();
}

void MultiModelControlWidget::treatScatterBrushColorChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setScatterStyleBrushColor(ui->scatterBrushColorWidget->color());
            }
        }
    }
    ui->scatterBrushColorWidget->clearFocus();
}

void MultiModelControlWidget::treatScatterPenColorChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setScatterStylePenColor(ui->scatterPenColorWidget->color());
            }
        }
    }
    ui->scatterPenColorWidget->clearFocus();
}

void MultiModelControlWidget::treatScatterPenWidthChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setScatterStylePenWidth(ui->scatterPenWidthDoubleSpinBox->value());
            }
        }
    }
    ui->scatterPenWidthDoubleSpinBox->clearFocus();
}

void MultiModelControlWidget::treatBrushStyleChanging()
{
//    if (index >= 0 && index < brushStyleMap.size()) {
//        if (mGraphVector.size() != 0) {
//            for (const auto & graph : mGraphVector) {
//                if (mLayoutModel->containsGraph(graph)) {
//                    graph->setBrushStyle(std::get<0>(brushStyleMap[index]));
//                }
//            }
//        }
//    }

    if (ui->brushStyleComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto brushStyle = ui->brushStyleComboBox->currentValue<Qt::BrushStyle>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setBrushStyle(brushStyle);
                }
            }
        }
    }
    ui->brushStyleComboBox->clearFocus();
}

void MultiModelControlWidget::treatBrushBasisChanging()
{
    if (ui->brushBasisComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto brushBasis = ui->brushBasisComboBox->currentValue<QCPGraph::BasisFillType>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setFillBasis(brushBasis);
                }
            }
        }
    }
    ui->brushBasisComboBox->clearFocus();
}

void MultiModelControlWidget::treatBrushColorChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setBrushColor(ui->brushColorWidget->color());
            }
        }
    }
    ui->brushColorWidget->clearFocus();
}

void MultiModelControlWidget::treatStringRepresentationChanging()
{
//    if (mGraphVector.size() != 0) {
//        for (const auto & graph : mGraphVector) {
//            if (mLayoutModel->containsGraph(graph)) {
////                graph->setBrushColor(ui->brushColorColorButton->color());
//                graph->setStringRepresentation(std::get<0>(stringRepresentaionMap[ui->stringRepresentationComboBox->currentIndex()]));
//            }
//        }
//    }

    if (ui->stringRepresentationComboBox->currentIndex() >= 0) {
        if (mGraphVector.size() != 0) {
            auto stringRepresentation = ui->stringRepresentationComboBox->currentValue<GraphModel::StringRepresentation>();
            for (const auto & graph : mGraphVector) {
                if (mLayoutModel->containsGraph(graph)) {
                    graph->setStringRepresentation(stringRepresentation);
                }
            }
        }
    }
    ui->stringRepresentationComboBox->clearFocus();
}

void MultiModelControlWidget::treatDataLabelsVisibilityChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setDataLabelsVisibility(ui->dataLabelsCheckBox->isChecked());
            }
        }
    }
    ui->visibilityCheckBox->clearFocus();
    ui->dataLabelsColorWidget->setEnabled(ui->visibilityCheckBox->isChecked());
}

void MultiModelControlWidget::treatDataLabelsColorChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                graph->setDataLabelsFontColor(ui->dataLabelsColorWidget->color());
            }
        }
    }
    ui->dataLabelsColorWidget->clearFocus();
}

void MultiModelControlWidget::treatX0Changing()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setX0(ui->x0LineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->x0LineEdit->clearFocus();
}

void MultiModelControlWidget::treatKXChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setKX(ui->kXLineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->kXLineEdit->clearFocus();
}

void MultiModelControlWidget::treatDXChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setDX(ui->dXLineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->dXLineEdit->clearFocus();
}

void MultiModelControlWidget::treatY0Changing()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setY0(ui->y0LineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->y0LineEdit->clearFocus();
}

void MultiModelControlWidget::treatKYChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setKY(ui->kYLineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->kYLineEdit->clearFocus();
}

void MultiModelControlWidget::treatDYChanging()
{
    if (mGraphVector.size() != 0) {
        for (const auto & graph : mGraphVector) {
            if (mLayoutModel->containsGraph(graph)) {
                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
                if (affineConverter == NULL) {
                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 0., 1., 0.);
                }

                //it is essential to create a new AffineConverter and not to apply changes to the old one
                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
                newAffineConverter->setDY(ui->dYLineEdit->text().toDouble());
                graph->setAffineConverter(newAffineConverter);
            }
        }
    }
    ui->dYLineEdit->clearFocus();
}



//void MultiModelControlWidget::treatXA0Changing()
//{

//    if (mGraphVector.size() != 0) {
//        for (const auto & graph : mGraphVector) {
//            if (mLayoutModel->containsGraph(graph)) {
//                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
//                if (affineConverter == NULL) {
//                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 1.);
//                }

//                //it is essential to create a new AffineConverter and not to apply changes to the old one
//                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
//                newAffineConverter->setA0X(ui->xA0LineEdit->text().toDouble());
//                graph->setAffineConverter(newAffineConverter);
//            }
//        }
//    }
//    ui->xA0LineEdit->clearFocus();
//}

//void MultiModelControlWidget::treatXA1Changing()
//{
//    if (mGraphVector.size() != 0) {
//        for (const auto & graph : mGraphVector) {
//            if (mLayoutModel->containsGraph(graph)) {
//                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
//                if (affineConverter == NULL) {
//                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 1.);
//                }

//                //it is essential to create a new AffineConverter and not to apply changes to the old one
//                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
//                newAffineConverter->setA1X(ui->xA1LineEdit->text().toDouble());
//                graph->setAffineConverter(newAffineConverter);
//            }
//        }
//    }
//    ui->xA1LineEdit->clearFocus();
//}

//void MultiModelControlWidget::treatYB0Changing()
//{
//    if (mGraphVector.size() != 0) {
//        for (const auto & graph : mGraphVector) {
//            if (mLayoutModel->containsGraph(graph)) {
//                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
//                if (affineConverter == NULL) {
//                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 1.);
//                }

//                //it is essential to create a new AffineConverter and not to apply changes to the old one
//                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
//                newAffineConverter->setB0Y(ui->yB0LineEdit->text().toDouble());
//                graph->setAffineConverter(newAffineConverter);
//            }
//        }
//    }
//    ui->yB0LineEdit->clearFocus();
//}

//void MultiModelControlWidget::treatYB1Changing()
//{
//    if (mGraphVector.size() != 0) {
//        for (const auto & graph : mGraphVector) {
//            if (mLayoutModel->containsGraph(graph)) {
//                std::shared_ptr<AffineConverter> affineConverter = graph->getAffineConverter();
//                if (affineConverter == NULL) {
//                    affineConverter = std::make_shared<AffineConverter>(0., 1., 0., 1.);
//                }

//                //it is essential to create a new AffineConverter and not to apply changes to the old one
//                auto newAffineConverter = std::make_shared<AffineConverter>(*affineConverter);
//                newAffineConverter->setB1Y(ui->yB1LineEdit->text().toDouble());
//                graph->setAffineConverter(newAffineConverter);
//            }
//        }
//    }
//    ui->yB1LineEdit->clearFocus();
//}

void MultiModelControlWidget::copyGraphProperties()
{
//    std::shared_ptr<GraphModel> gGlobalGraphModel = NULL;
    if (mGraphVector.size() == 1 && mGraphVector[0]) {
        global::gGlobalGraphModel = std::make_shared<GraphModel>();
        global::gGlobalGraphModel->copyPropertiesFromGraph(mGraphVector[0]);
    }
}

void MultiModelControlWidget::pasteGraphProperties()
{
    if (mGraphVector.size() == 1 && mGraphVector[0]) {
        mGraphVector[0]->copyPropertiesFromGraph(global::gGlobalGraphModel.get());
        initializeStateFromModels();
    }
}

void MultiModelControlWidget::treatFastenToolButtonPressed()
{
    FastenToolButton *button = qobject_cast<FastenToolButton*>(sender());
    if (button == nullptr)
        return;
    for(int i = 0; i < mFastenToolButtonVector.size(); ++i) {
        if (button == mFastenToolButtonVector[i]) {
            sTabVisibilityVector[i] = mFastenToolButtonVector[i]->isChecked();
        }
    }

}

void MultiModelControlWidget::treatModelDeleting()
{
    mGraphVector.clear();
    hide();
    emit widgetIsHiding();
}

void MultiModelControlWidget::disconnectFromModels()
{
    disconnect(ui->visibilityCheckBox, SIGNAL(clicked()),
            this, SLOT(treatVisibilityChanging()));
    disconnect(ui->legendabilityCheckBox, SIGNAL(clicked()),
            this, SLOT(treatLegendabilityChanging()));

    disconnect(ui->nameLineEdit, SIGNAL(editingFinished ()),
            this, SLOT(treatNameChanging()));
    disconnect(ui->unitLineEdit, SIGNAL(editingFinished ()),
            this, SLOT(treatDimensionChanging()));
    disconnect(ui->lineWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatLineWidthChanging()));
    disconnect(ui->lineColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatLineColorChanging()));
    disconnect(ui->lineStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatLineStyleChanging()));
    disconnect(ui->lineInterpolationComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatInterpolationChanging()));
    disconnect(ui->stringRepresentationComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatStringRepresentationChanging()));

    disconnect(ui->scatterShapeComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatScatterShapeChanging()));
    disconnect(ui->scatterSizeDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterSizeChanging()));
    disconnect(ui->scatterDecimationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatScatterDecimationChanging()));
    disconnect(ui->scatterBrushColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatScatterBrushColorChanging()));
    disconnect(ui->scatterPenColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatScatterPenColorChanging()));
    disconnect(ui->scatterPenWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterPenWidthChanging()));
    disconnect(ui->dataLabelsCheckBox, SIGNAL(clicked()),
            this, SLOT(treatDataLabelsVisibilityChanging()));
    disconnect(ui->dataLabelsColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatDataLabelsColorChanging()));

    disconnect(ui->brushStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushStyleChanging()));
    disconnect(ui->brushBasisComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushBasisChanging()));
    disconnect(ui->brushColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBrushColorChanging()));


//    disconnect(ui->xA0LineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatXA0Changing()));
//    disconnect(ui->xA1LineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatXA1Changing()));
//    disconnect(ui->yB0LineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatYB0Changing()));
//    disconnect(ui->yB1LineEdit, SIGNAL(editingFinished()),
//            this, SLOT(treatYB1Changing()));

    disconnect(ui->x0LineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatX0Changing()));
    disconnect(ui->kXLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatKXChanging()));
    disconnect(ui->dXLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatDXChanging()));
    disconnect(ui->y0LineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatY0Changing()));
    disconnect(ui->kYLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatKYChanging()));
    disconnect(ui->dYLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatDYChanging()));

    for (auto graph : mGraphVector) {
        disconnect(graph, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatModelDeleting()));
    }
}

void MultiModelControlWidget::connectToModels()
{
    connect(ui->visibilityCheckBox, SIGNAL(clicked()),
            this, SLOT(treatVisibilityChanging()));
    connect(ui->legendabilityCheckBox, SIGNAL(clicked()),
            this, SLOT(treatLegendabilityChanging()));

    connect(ui->nameLineEdit, SIGNAL(editingFinished ()),
            this, SLOT(treatNameChanging()));
    connect(ui->unitLineEdit, SIGNAL(editingFinished ()),
            this, SLOT(treatDimensionChanging()));
    connect(ui->lineWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatLineWidthChanging()));
    connect(ui->lineColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatLineColorChanging()));
    connect(ui->lineStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatLineStyleChanging()));
    connect(ui->lineInterpolationComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatInterpolationChanging()));
    connect(ui->stringRepresentationComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatStringRepresentationChanging()));

    connect(ui->scatterShapeComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatScatterShapeChanging()));
    connect(ui->scatterSizeDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterSizeChanging()));
    connect(ui->scatterDecimationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatScatterDecimationChanging()));
    connect(ui->scatterBrushColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatScatterBrushColorChanging()));
    connect(ui->scatterPenColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatScatterPenColorChanging()));
    connect(ui->scatterPenWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterPenWidthChanging()));
    connect(ui->dataLabelsCheckBox, SIGNAL(clicked()),
            this, SLOT(treatDataLabelsVisibilityChanging()));
    connect(ui->dataLabelsColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatDataLabelsColorChanging()));

    connect(ui->brushStyleComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushStyleChanging()));
    connect(ui->brushBasisComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatBrushBasisChanging()));
    connect(ui->brushColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatBrushColorChanging()));

    connect(ui->x0LineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatX0Changing()));
    connect(ui->kXLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatKXChanging()));
    connect(ui->dXLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatDXChanging()));
    connect(ui->y0LineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatY0Changing()));
    connect(ui->kYLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatKYChanging()));
    connect(ui->dYLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatDYChanging()));

    for (auto graph : mGraphVector) {
        connect(graph, SIGNAL(destroyed(QObject*)),
                this, SLOT(treatModelDeleting()));
    }

}
