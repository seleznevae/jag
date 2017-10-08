#include "datasourcecontrolwidget.h"
#include "ui_datasourcecontrolwidget.h"
#include <QFileDialog>
#include <QDoubleValidator>
#include "dataconverter/datasourcefiltercontainer.h"
#include "global/global_definitions.h"

DataSourceControlWidget::DataSourceControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSourceControlWidget), mDataSource(NULL), mDataSourceObserverTimer(0)
{
    ui->setupUi(this);

    connect(ui->aliasLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatAliasChanging()));
    connect(ui->saveAsPushButton, SIGNAL(clicked()),
            this, SLOT(saveAs()));
    connect(ui->isStyleAvailableCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(setStyleOptionsVisibility(bool)));

//    connect(ui->lineWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
//            this, SLOT(treatLineWidthChanging()));
//    connect(ui->lineStyleComboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(treatLineStyleChanging()));
//    connect(ui->lineInterpolationComboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(treatLineInterpolationChanging()));
//    connect(ui->scatterShapeComboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(treatScatterShapeChanging()));
//    connect(ui->scatterSizeDoubleSpinBox, SIGNAL(valueChanged(double)),
//            this, SLOT(treatScatterSizeChanging()));
//    connect(ui->scatterDecimationSpinBox, SIGNAL(valueChanged(int)),
//            this, SLOT(treatScatterDecimationChanging()));
//    connect(ui->isColorFixedCheckBox, SIGNAL(toggled(bool)),
//            this, SLOT(treatLineColorFixing()));
//    connect(ui->lineColorWidget, SIGNAL(currentColorChanged(QColor)),
//            this, SLOT(treatLineColorChanging()));

    connect(ui->timeDeviationLineEdit, SIGNAL(editingFinished()),
            this, SLOT(treatTimeDeviationChanging()));

    //lineStyleMap initializing
    lineStyleMap.push_back(std::tuple<Qt::PenStyle, QString>(Qt::SolidLine, "Solid"));
    lineStyleMap.push_back(std::tuple<Qt::PenStyle, QString>(Qt::DashLine, "Dash"));
    lineStyleMap.push_back(std::tuple<Qt::PenStyle, QString>(Qt::DotLine, "Dot"));
    lineStyleMap.push_back(std::tuple<Qt::PenStyle, QString>(Qt::DashDotLine, "DashDot"));
    lineStyleMap.push_back(std::tuple<Qt::PenStyle, QString>(Qt::DashDotDotLine, "DashDotDot"));
    for (auto it = lineStyleMap.begin(); it != lineStyleMap.end(); ++it) {
        ui->lineStyleComboBox->addItem(std::get<1>(*it));
    }


    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssNone, global::getStyledIcon("ssNone")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssDot, global::getStyledIcon("ssDot")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssCross, global::getStyledIcon("ssCross")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssPlus, global::getStyledIcon("/ssPlus")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssCircle, global::getStyledIcon("ssCircle")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssDisc, global::getStyledIcon("ssDisc")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssSquare, global::getStyledIcon("ssSquare")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssDiamond, global::getStyledIcon("ssDiamond")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssStar, global::getStyledIcon("ssStar")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssTriangle, global::getStyledIcon("ssTriangle")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssTriangleInverted, global::getStyledIcon("ssTriangleInverted")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssCrossSquare, global::getStyledIcon("ssCrossSquare")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssPlusSquare, global::getStyledIcon("ssPlusSquare")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssCrossCircle, global::getStyledIcon("ssCrossCircle")));
    scatterShapeMap.push_back(std::tuple<QCPScatterStyle::ScatterShape, QIcon>
                                (QCPScatterStyle::ScatterShape::ssPlusCircle, global::getStyledIcon("ssPlusCircle")));
    for (auto it = scatterShapeMap.begin(); it != scatterShapeMap.end(); ++it) {
        ui->scatterShapeComboBox->addItem(std::get<1>(*it), QString());
    }

    //lineInterpolationMap initializing
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsLine, "Line"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsSmooth, "Smooth"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsStepLeft, "Step Left"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsStepRight, "Step Right"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsStepCenter, "Step Center"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsImpulse, "Impulse"));
    lineInterpolationMap.push_back(std::tuple<GraphModel::LineStyle, QString>
                                (GraphModel::LineStyle::lsNone, "None"));
    for (auto it = lineInterpolationMap.begin(); it != lineInterpolationMap.end(); ++it) {
        ui->lineInterpolationComboBox->addItem(std::get<1>(*it));
    }

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->timeDeviationLineEdit->setValidator(doubleValidator);

}

DataSourceControlWidget::~DataSourceControlWidget()
{
    delete ui;
}

void DataSourceControlWidget::setDataSource(DataSource *dataSource)
{
    mDataSource = dataSource;
    ui->nameLabel->setText(mDataSource->getDataSourceName());
    ui->aliasLineEdit->setText(mDataSource->getDataSourceAlias());
    ui->timeDeviationLineEdit->setText(QString::number(mDataSource->timeDeviation()));
    if (mDataSourceObserverTimer != 0) {
        killTimer(mDataSourceObserverTimer);
        mDataSourceObserverTimer = 0;
    }
    if (!dataSource->isDataReady()) {
        mDataSourceObserverTimer = startTimer(1000);
        ui->saveAsPushButton->hide();
    }

    auto graphStyle = mDataSource->graphStyle();
    ui->isStyleAvailableCheckBox->setChecked(graphStyle->isSpecified());
    if (graphStyle->isSpecified()) {
        ui->graphStyleWidget->show();
        initiazeStyleWidgets();
    } else {
        ui->graphStyleWidget->hide();
    }

    auto dataSourceWrapper = std::make_shared<DataSourceFilterContainer>();
    dataSourceWrapper->setDataSource(mDataSource);
    ui->filterContainerWidget->setModel(dataSourceWrapper);


}



void DataSourceControlWidget::treatAliasChanging()
{
    if (mDataSource) {
        mDataSource->setDataSourceAlias(ui->aliasLineEdit->text());
    }
}

void DataSourceControlWidget::saveAs()
{
    if (mDataSource->isDataReady()) {
        //NOTE: on linux QFileDialog static methods sometimes show an empty window in case of using native dialog
//    #ifdef __linux
        QFileDialog::Options option =  QFileDialog::DontUseNativeDialog;
//    #else
//        QFileDialog::Option option = 0x0;
//    #endif
        QString fileName = QFileDialog::getSaveFileName(this, "Enter file name",
                                QString(), "*.tt", NULL, option);
        if (fileName.size() != 0) {
            mDataSource->saveAs(fileName);
        }
    }
}

void DataSourceControlWidget::setStyleOptionsVisibility(bool visibility)
{
    if (visibility) {
        ui->graphStyleWidget->show();
        initiazeStyleWidgets();
    } else {
        ui->graphStyleWidget->hide();
    }
    auto graphStyle = mDataSource->graphStyle();
    graphStyle->setSpecifiedProperty(visibility);
}

void DataSourceControlWidget::initiazeStyleWidgets()
{
    disconnectStyleWidgets();
    auto graphStyle = mDataSource->graphStyle();

    auto pen = graphStyle->pen();
    ui->lineWidthDoubleSpinBox->setValue(pen.widthF());
    for (auto i = 0; i != lineStyleMap.size(); i++)
        if (std::get<0>(lineStyleMap[i]) == pen.style()) {
            ui->lineStyleComboBox->setCurrentIndex(i);
            break;
        }

    auto interpolation = graphStyle->lineInterpolation();
    for (auto i = 0; i != lineInterpolationMap.size(); i++)
        if (std::get<0>(lineInterpolationMap[i]) == interpolation) {
            ui->lineInterpolationComboBox->setCurrentIndex(i);
            break;
        }

    auto scatter = graphStyle->scatterStyle();
    ui->scatterSizeDoubleSpinBox->setValue(scatter.size());
    ui->scatterDecimationSpinBox->setValue(scatter.decimation());
    for (auto i = 0; i != scatterShapeMap.size(); i++)
        if (std::get<0>(scatterShapeMap[i]) == scatter.shape()) {
            ui->scatterShapeComboBox->setCurrentIndex(i);
            break;
        }

    ui->isColorFixedCheckBox->setChecked(graphStyle->isColorFixed());
    ui->lineColorWidget->setColor(graphStyle->lineColor());
    ui->lineColorWidget->setEnabled(ui->isColorFixedCheckBox->isChecked());

    connectStyleWidgets();
}

void DataSourceControlWidget::treatLineWidthChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        auto pen = graphStyle->pen();
        pen.setWidthF(ui->lineWidthDoubleSpinBox->value());
        graphStyle->setPen(pen);
    }
}

void DataSourceControlWidget::treatLineStyleChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        auto pen = graphStyle->pen();
        pen.setStyle(std::get<0>(lineStyleMap[ui->lineStyleComboBox->currentIndex()]));
        graphStyle->setPen(pen);
    }
}

void DataSourceControlWidget::treatLineInterpolationChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        graphStyle->setLineInterpolation(std::get<0>(lineInterpolationMap[ui->lineInterpolationComboBox->currentIndex()]));
    }
}

void DataSourceControlWidget::treatScatterShapeChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        auto scatterStyle = graphStyle->scatterStyle();
        scatterStyle.setShape(std::get<0>(scatterShapeMap[ui->scatterShapeComboBox->currentIndex()]));
        graphStyle->setScatterStyle(scatterStyle);
    }
}

void DataSourceControlWidget::treatScatterSizeChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        auto scatterStyle = graphStyle->scatterStyle();
        scatterStyle.setSize(ui->scatterSizeDoubleSpinBox->value());
        graphStyle->setScatterStyle(scatterStyle);
    }
}

void DataSourceControlWidget::treatScatterDecimationChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        auto scatterStyle = graphStyle->scatterStyle();
        scatterStyle.setDecimation(ui->scatterDecimationSpinBox->value());
        graphStyle->setScatterStyle(scatterStyle);
    }
}

void DataSourceControlWidget::treatTimeDeviationChanging()
{
    if (mDataSource) {
        mDataSource->setTimeDeviation(ui->timeDeviationLineEdit->text().toDouble());
    }
}

void DataSourceControlWidget::treatLineColorFixing()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        bool isFixed = ui->isColorFixedCheckBox->isChecked();
        graphStyle->setColorFixed(isFixed);
        ui->lineColorWidget->setEnabled(isFixed);
    }
}

void DataSourceControlWidget::treatLineColorChanging()
{
    if (mDataSource) {
        auto graphStyle = mDataSource->graphStyle();
        graphStyle->setColor(ui->lineColorWidget->color());
    }
}

void DataSourceControlWidget::timerEvent(QTimerEvent *event)
{

    if (event->timerId() == mDataSourceObserverTimer) {
        if (mDataSource && mDataSource->isDataReady()) {
            killTimer(mDataSourceObserverTimer);
            ui->saveAsPushButton->show();
        }
    }
}

void DataSourceControlWidget::connectStyleWidgets()
{
    connect(ui->lineWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatLineWidthChanging()));
    connect(ui->lineStyleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatLineStyleChanging()));
    connect(ui->lineInterpolationComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatLineInterpolationChanging()));
    connect(ui->scatterShapeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatScatterShapeChanging()));
    connect(ui->scatterSizeDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterSizeChanging()));
    connect(ui->scatterDecimationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatScatterDecimationChanging()));
    connect(ui->isColorFixedCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLineColorFixing()));
    connect(ui->lineColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatLineColorChanging()));
}

void DataSourceControlWidget::disconnectStyleWidgets()
{
    disconnect(ui->lineWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatLineWidthChanging()));
    disconnect(ui->lineStyleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatLineStyleChanging()));
    disconnect(ui->lineInterpolationComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatLineInterpolationChanging()));
    disconnect(ui->scatterShapeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(treatScatterShapeChanging()));
    disconnect(ui->scatterSizeDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(treatScatterSizeChanging()));
    disconnect(ui->scatterDecimationSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatScatterDecimationChanging()));
    disconnect(ui->isColorFixedCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(treatLineColorFixing()));
    disconnect(ui->lineColorWidget, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(treatLineColorChanging()));
}

