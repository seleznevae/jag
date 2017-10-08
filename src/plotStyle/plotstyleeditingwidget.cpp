#include "plotstyleeditingwidget.h"
#include "ui_plotstyleeditingwidget.h"

PlotStyleEditingWidget::PlotStyleEditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotStyleEditingWidget)
{
    ui->setupUi(this);

    ui->xGridEditingWidget->setHead("X grid");
    ui->yGridEditingWidget->setHead("Y grid");
    ui->xSubgridEditingWidget->setHead("X sub grid");
    ui->ySubgridEditingWidget->setHead("Y sub grid");

    ui->subtickEditingWidget->setSubTickCountVisibility(false);
    ui->colorPrefEditingWidget->enableColorListWidget(true);

    connetAll();
}

PlotStyleEditingWidget::~PlotStyleEditingWidget()
{
    delete ui;
}

void PlotStyleEditingWidget::setPlotStyle(const PlotStyle &style)
{
    disconnetAll();

    ui->axisRectEditingWidget->setAxisRect(style.mAxisRectProperties);
    ui->marginsEditingWidget->setMargins(style.mMargins);
    ui->xGridEditingWidget->setGrid(style.mXGridProperties);
    ui->yGridEditingWidget->setGrid(style.mYGridProperties);
    ui->xSubgridEditingWidget->setGrid(style.mXSubGridProperties);
    ui->ySubgridEditingWidget->setGrid(style.mYSubGridProperties);
    ui->tickEditingWidget->setTick(style.mTickProperties);
    ui->subtickEditingWidget->setSubTick(style.mSubTickProperties);
    ui->legendEditingWidget->setLegendProperties(style.mLegendProperties);
    ui->colorPrefEditingWidget->setColorPref(style.mColorPref);

    ui->xLabelEditingWidget->setLabelPrefs(style.mXLabelProperties);
    ui->yLabelEditingWidget->setLabelPrefs(style.mYLabelProperties);
    ui->xTickLabelEditingWidget->setLabelPrefs(style.mXTickLabelProperties);
    ui->yTickLabelEditingWidget->setLabelPrefs(style.mYTickLabelProperties);
    ui->titleEditingWidget->setLabelPrefs(style.mTitleProperties);
    ui->xAxisPrefWidget->setAxisPref(style.mXAxisPref);
    ui->yAxisPrefWidget->setAxisPref(style.mYAxisPref);
    ui->eventEditingWidget->setLevelPrefs(style.mEventProperties);
    ui->horLevelEditingWidget->setLevelPrefs(style.mHorLevelProperties);
    ui->arrowedTextEditingWidget->setArrowedTextPrefs(style.mArrowedTextProperties);
    mPlotStyleName = style.mStyleName;

    connetAll();
}

PlotStyle PlotStyleEditingWidget::plotStyle() const
{
    PlotStyle ret;
    ret.mStyleName = mPlotStyleName;
    ret.mAxisRectProperties = ui->axisRectEditingWidget->axisRect();
    ret.mMargins            = ui->marginsEditingWidget->margins();
    ret.mXGridProperties    = ui->xGridEditingWidget->grid();
    ret.mYGridProperties    = ui->yGridEditingWidget->grid();
    ret.mXSubGridProperties = ui->xSubgridEditingWidget->grid();
    ret.mYSubGridProperties = ui->ySubgridEditingWidget->grid();
    ret.mTickProperties     = ui->tickEditingWidget->tick();
    ret.mSubTickProperties  = ui->subtickEditingWidget->tick();
    ret.mLegendProperties   = ui->legendEditingWidget->legendProperties();
    ret.mColorPref          = ui->colorPrefEditingWidget->colorPref();

    ret.mXLabelProperties     = ui->xLabelEditingWidget->labelPrefs();
    ret.mYLabelProperties     = ui->yLabelEditingWidget->labelPrefs();
    ret.mXTickLabelProperties = ui->xTickLabelEditingWidget->labelPrefs();
    ret.mYTickLabelProperties = ui->yTickLabelEditingWidget->labelPrefs();
    ret.mTitleProperties      = ui->titleEditingWidget->labelPrefs();

    ret.mXAxisPref            = ui->xAxisPrefWidget->axisPref();
    ret.mYAxisPref            = ui->yAxisPrefWidget->axisPref();

    ret.mEventProperties      = ui->eventEditingWidget->levelPrefs();
    ret.mHorLevelProperties   = ui->horLevelEditingWidget->levelPrefs();
    ret.mArrowedTextProperties= ui->arrowedTextEditingWidget->arrowedTextPrefs();
    return ret;
}

void PlotStyleEditingWidget::connetAll()
{
    connect(ui->axisRectEditingWidget, SIGNAL(axisRectPropertiesChanged()),
            this, SIGNAL(styleChanged()));
    connect(ui->marginsEditingWidget, SIGNAL(marginsEdited(QMargins)),
            this, SIGNAL(styleChanged()));
    connect(ui->xGridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    connect(ui->yGridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    connect(ui->xSubgridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    connect(ui->ySubgridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    connect(ui->tickEditingWidget, SIGNAL(tickChanged()),
            this, SIGNAL(styleChanged()));
    connect(ui->subtickEditingWidget, SIGNAL(tickChanged()),
            this, SIGNAL(styleChanged()));
    connect(ui->legendEditingWidget, SIGNAL(legendPropertiesEdited(style::LegendPref)),
            this, SIGNAL(styleChanged()));
    connect(ui->colorPrefEditingWidget, SIGNAL(colorPrefChangedByUser()),
            this, SIGNAL(styleChanged()));

    connect(ui->xLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    connect(ui->yLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    connect(ui->xTickLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    connect(ui->yTickLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    connect(ui->titleEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));

    connect(ui->xAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SIGNAL(styleChanged()));
    connect(ui->yAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SIGNAL(styleChanged()));

    connect(ui->eventEditingWidget, SIGNAL(levelPrefsChanged(style::LevelPref)),
            this, SIGNAL(styleChanged()));
    connect(ui->horLevelEditingWidget, SIGNAL(levelPrefsChanged(style::LevelPref)),
            this, SIGNAL(styleChanged()));
    connect(ui->arrowedTextEditingWidget, SIGNAL(arrowedTextPrefChanged()),
            this, SIGNAL(styleChanged()));

}

void PlotStyleEditingWidget::disconnetAll()
{
    disconnect(ui->axisRectEditingWidget, SIGNAL(axisRectPropertiesChanged()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->marginsEditingWidget, SIGNAL(marginsEdited(QMargins)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->xGridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->yGridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->xSubgridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->ySubgridEditingWidget, SIGNAL(gridChanged(style::Grid)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->tickEditingWidget, SIGNAL(tickChanged()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->subtickEditingWidget, SIGNAL(tickChanged()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->legendEditingWidget, SIGNAL(legendPropertiesEdited(style::LegendPref)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->colorPrefEditingWidget, SIGNAL(colorPrefChangedByUser()),
            this, SIGNAL(styleChanged()));


    disconnect(ui->xLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->yLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->xTickLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->yTickLabelEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->titleEditingWidget, SIGNAL(labelPrefsEdited()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->xAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->yAxisPrefWidget, SIGNAL(axisPrefChanged()),
            this, SIGNAL(styleChanged()));
    disconnect(ui->eventEditingWidget, SIGNAL(levelPrefsChanged(style::LevelPref)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->horLevelEditingWidget, SIGNAL(levelPrefsChanged(style::LevelPref)),
            this, SIGNAL(styleChanged()));
    disconnect(ui->arrowedTextEditingWidget, SIGNAL(arrowedTextPrefChanged()),
            this, SIGNAL(styleChanged()));
}
