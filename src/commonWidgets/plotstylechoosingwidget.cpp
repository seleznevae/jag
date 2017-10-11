#include "plotstylechoosingwidget.h"
#include "ui_plotstylechoosingwidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include "core/plotlayoutmodel.h"

PlotStyleChoosingWidget::PlotStyleChoosingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotStyleChoosingWidget)
{
    ui->setupUi(this);

    mDefaultStyles = PlotStyle::sSystemPlotStyles;
    mUserDefinedStyles = PlotStyle::sUserPlotStyles;
    mDefaultStyleName = PlotStyle::defaultPlotStyle().mStyleName;

//    auto createListWidgetItem = [] (const PlotStyle& style) {
//        if (style.mStyleName == mCurrentStyleName)
//            return new QListWidgetItem(QIcon(":/apply"), style.mStyleName);
//        else
//            return new QListWidgetItem(style.mStyleName);
//    };

    for (auto & style : mDefaultStyles)
        mDefaultStylesItems.push_back(new QListWidgetItem(style.mStyleName));
    for (auto & style : mUserDefinedStyles)
        mUserDefinedStylesItems.push_back(new QListWidgetItem(style.mStyleName));


    for (auto & item : mDefaultStylesItems)
        ui->styleListWidget->addItem(item);
    for (auto & item : mUserDefinedStylesItems)
        ui->styleListWidget->addItem(item);
    ui->styleListWidget->setCurrentItem(mDefaultStylesItems.front());
    ui->plotStyleEditingWidget->setPlotStyle(PlotStyle::defaultPlotStyle());
    markDefaultPlotStyle();



    connectAll();

    connect(ui->addButton, SIGNAL(clicked(bool)),
            this, SLOT(addNewCustomPlotStyle()));
    connect(ui->removeButton, SIGNAL(clicked(bool)),
            this, SLOT(removeUserPlotStyle()));
    connect(ui->makeCurrentButton, SIGNAL(clicked(bool)),
            this, SLOT(changeDefaultPlotStyle()));

    connect(ui->styleListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(treatCurrentItemChanging()));

    setCurrentStyle(PlotStyle::defaultPlotStyle());
//    treatCurrentItemChanging();
//    ui->plotWidget->updateEverything();


}

PlotStyleChoosingWidget::~PlotStyleChoosingWidget()
{
    delete ui;
}


PlotStyle PlotStyleChoosingWidget::plotStyle()
{
    return ui->plotStyleEditingWidget->plotStyle();
}

void PlotStyleChoosingWidget::setCurrentStyle(const PlotStyle &style)
{
    int index = mDefaultStyles.indexOf(style);
    if (index != -1) {
        ui->styleListWidget->setCurrentItem(mDefaultStylesItems.at(index));
        treatCurrentItemChanging();
    }

    index = mUserDefinedStyles.indexOf(style);
    if (index != -1) {
        ui->styleListWidget->setCurrentItem(mUserDefinedStylesItems.at(index));
        treatCurrentItemChanging();
    }

}

void PlotStyleChoosingWidget::markDefaultPlotStyle()
{
    for (int i = 0; i < mDefaultStylesItems.size(); ++i) {
        if (mDefaultStyles[i].mStyleName == mDefaultStyleName)
            mDefaultStylesItems[i]->setIcon(QIcon(":/apply"));
        else
            mDefaultStylesItems[i]->setIcon(QIcon());
    }
    for (int i = 0; i < mUserDefinedStylesItems.size(); ++i) {
        if (mUserDefinedStyles[i].mStyleName == mDefaultStyleName)
            mUserDefinedStylesItems[i]->setIcon(QIcon(":/apply"));
        else
            mUserDefinedStylesItems[i]->setIcon(QIcon());
    }
}

void PlotStyleChoosingWidget::applyChanges()
{
    PlotStyle::setUserDefinedPlotStyles(mUserDefinedStyles);
    PlotStyle::setDefaultPlotStyle(mDefaultStyleName);
}

void PlotStyleChoosingWidget::treatStyleModification()
{
    if (mLayoutModel.plotStyle() != ui->plotStyleEditingWidget->plotStyle())
    {
//        mLayoutModel.setPlotStyle(ui->plotStyleEditingWidget->plotStyle());
        createPlotWithStyle(ui->plotStyleEditingWidget->plotStyle());

        mArrowedText->setArrowedTextPrefs(ui->plotStyleEditingWidget->plotStyle().mArrowedTextProperties);
//        for (auto & style : mDefaultStyles)
//            if (style.mStyleName == mDefaultStyleName)
//                style = ui->plotStyleEditingWidget->plotStyle();
//        for (auto & style : mUserDefinedStyles)
//            if (style.mStyleName == mDefaultStyleName)
//                style = ui->plotStyleEditingWidget->plotStyle();

        QListWidgetItem *curItem = ui->styleListWidget->currentItem();
        for (int i = 0; i < mDefaultStylesItems.size(); ++i) {
            if (mDefaultStylesItems[i] == curItem)
                mDefaultStyles[i] = ui->plotStyleEditingWidget->plotStyle();
        }
        for (int i = 0; i < mUserDefinedStylesItems.size(); ++i) {
            if (mUserDefinedStylesItems[i] == curItem)
                mUserDefinedStyles[i] = ui->plotStyleEditingWidget->plotStyle();
        }
    }
}

void PlotStyleChoosingWidget::addNewCustomPlotStyle()
{
    bool ok = true;
    QStringList prototypeNames;
    for (const auto & style : mDefaultStyles)
        prototypeNames << style.mStyleName;
    for (const auto & style : mUserDefinedStyles)
        prototypeNames << style.mStyleName;

    QString prototypeName = QInputDialog::getItem(this, tr("New style prototype"),
                                                 tr("Choose new style prototype"), prototypeNames,
                                                  0 /*current*/, false /*editable*/
                                                 &ok);
    if (!ok || prototypeName.isEmpty())
        return;

    QString newStyleName;
    while (true) {
        newStyleName = QInputDialog::getText(this, tr("New style name"),
                                                     tr("Enter new style name"), QLineEdit::Normal,
                                                     "NewStyle", &ok);
        if (!ok || newStyleName.isEmpty())
            return;
        if (prototypeNames.indexOf(newStyleName) == -1)
            break;
         QMessageBox::information(this, "Incorrect data",
                                  "Style with name " + newStyleName + " already exists."
                                  );
    }
    int index = prototypeNames.indexOf(prototypeName);
    PlotStyle newStyle = (index < mDefaultStyles.size())
                          ? mDefaultStyles.at(index)
                          : mUserDefinedStyles.at(index - mDefaultStyles.size());
    newStyle.mStyleName = newStyleName;

    mUserDefinedStyles.push_back(newStyle);
    mUserDefinedStylesItems.push_back(new QListWidgetItem(newStyle.mStyleName));
    ui->styleListWidget->addItem(mUserDefinedStylesItems.back());

}

void PlotStyleChoosingWidget::removeUserPlotStyle()
{
    QListWidgetItem *curItem = ui->styleListWidget->currentItem();
    if (curItem == nullptr )
        return;
    int index = mUserDefinedStylesItems.indexOf(curItem);
    if (index == -1)
        return;
    QMessageBox::StandardButton result = QMessageBox::question(this, ""/*title*/,
           "Do you really want to delete " + mUserDefinedStyles.at(index).mStyleName
            + " style?", QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No)
           );
    if (result == QMessageBox::Yes) {
        QString removedStyleName = mUserDefinedStyles.at(index).mStyleName;
        mUserDefinedStyles.remove(index);
//        ui->styleListWidget->removeItemWidget(mUserDefinedStylesItems.at(index));
        QListWidgetItem* itemToDelete = ui->styleListWidget->takeItem(index + mDefaultStylesItems.size());
        delete itemToDelete;

        mUserDefinedStylesItems.remove(index);
        setCurrentStyle(mDefaultStyles.front());
        if (mDefaultStyleName == removedStyleName) {
            mDefaultStyleName = mDefaultStyles.front().mStyleName;
            changeDefaultPlotStyle();
        }
    }

}

void PlotStyleChoosingWidget::treatCurrentItemChanging()
{
    disconnectAll();
    QListWidgetItem *curItem = ui->styleListWidget->currentItem();
    if (curItem == nullptr)
        return;
    if (mDefaultStylesItems.indexOf(curItem) == -1) {
        ui->removeButton->setEnabled(true);
//        mLayoutModel.setPlotStyle(mUserDefinedStyles.at(mUserDefinedStylesItems.indexOf(curItem)));
        createPlotWithStyle(mUserDefinedStyles.at(mUserDefinedStylesItems.indexOf(curItem)));
        ui->plotStyleEditingWidget->setPlotStyle(mUserDefinedStyles.at(mUserDefinedStylesItems.indexOf(curItem)));
        ui->plotStyleEditingWidget->setVisible(true);
    } else {
        ui->removeButton->setEnabled(false);
//        mLayoutModel.setPlotStyle(mDefaultStyles.at(mDefaultStylesItems.indexOf(curItem)));
        createPlotWithStyle(mDefaultStyles.at(mDefaultStylesItems.indexOf(curItem)));
        ui->plotStyleEditingWidget->setPlotStyle(mDefaultStyles.at(mDefaultStylesItems.indexOf(curItem)));
        ui->plotStyleEditingWidget->setVisible(false);
    }
    ui->plotWidget->updateEverything();
    connectAll();
}

void PlotStyleChoosingWidget::changeDefaultPlotStyle()
{
    QListWidgetItem *curItem = ui->styleListWidget->currentItem();
    if (curItem == nullptr)
        return;
    if (mDefaultStylesItems.indexOf(curItem) != -1) {
        mDefaultStyleName = mDefaultStyles[mDefaultStylesItems.indexOf(curItem)].mStyleName;
        markDefaultPlotStyle();
    } else if (mUserDefinedStylesItems.indexOf(curItem) != -1) {
        mDefaultStyleName = mUserDefinedStyles[mUserDefinedStylesItems.indexOf(curItem)].mStyleName;
        markDefaultPlotStyle();
    }

}

void PlotStyleChoosingWidget::connectAll()
{
    connect(ui->plotStyleEditingWidget, SIGNAL(styleChanged()),
            this, SLOT(treatStyleModification()));
    connect(&mLayoutModel, SIGNAL(layoutAppearanceChanged()),
            ui->plotWidget, SLOT(updateEverything()));
}

void PlotStyleChoosingWidget::disconnectAll()
{
    disconnect(ui->plotStyleEditingWidget, SIGNAL(styleChanged()),
            this, SLOT(treatStyleModification()));
    disconnect(&mLayoutModel, SIGNAL(layoutAppearanceChanged()),
               ui->plotWidget, SLOT(updateEverything()));
}

void PlotStyleChoosingWidget::createPlotWithStyle(const PlotStyle& plotStyle)
{
    disconnectAll();
    while (mLayoutModel.getAxisModelVector().size() != 0) {
        mLayoutModel.removeAxisModel(mLayoutModel.getAxisModelVector()[0]);
    }

    mLayoutModel.addPlot({-12., 2., 12}, {-1., 3, -1.}, "graph1");
//    mLayoutModel.addPlot({1., 2., 3}, {5., 3, 4.}, "graph2");
    QVector<double> epicycloidX;
    QVector<double> epicycloidY;
    double a = 3;
    double b = 4;
    double c = 5;
    for (int i = 0; i < 10000; ++i) {
        double t = i / 100.;
//        epicycloidX.push_back((a+b)*cos(t) - b*cos((a/b + 1)*t));
//        epicycloidY.push_back((a+b)*sin(t) - b*sin((a/b + 1)*t));

        //epitrochoid
        epicycloidX.push_back((a+b)*cos(t) - c*cos((a/b + 1)*t));
        epicycloidY.push_back((a+b)*sin(t) - c*sin((a/b + 1)*t));
    }
    auto graph = mLayoutModel.addPlot(epicycloidX, epicycloidY, "graph2");

    graph->setGraphType(GraphModel::GraphType::Curve);
    mLayoutModel.setTitle("Title");

    QVector<AxisModel*> axes = mLayoutModel.getAxisModelVector();
    mArrowedText = new ArrowedText;
    mArrowedText->setText("Point");
    mArrowedText->setTextCenter (QPointF(3, 2));
    mArrowedText->setArrowFinish(QPointF(-4, 1.5));
    mArrowedText->setArrowedTextPrefs(PlotStyle::defaultPlotStyle().mArrowedTextProperties);
    axes[0]->addArrowedText(mArrowedText);
    axes[0]->setBackgroundBrush(plotStyle.mColorPref.mAxesBackgroundBrush);
    axes[1]->setBackgroundBrush(plotStyle.mColorPref.mAxesBackgroundBrush);

    ui->plotWidget->setLayoutModel(&mLayoutModel);
    mLayoutModel.setPlotStyle(plotStyle);
    ui->plotWidget->updateEverything();

    connectAll();
}
