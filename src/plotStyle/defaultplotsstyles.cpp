#include "plotstyle.h"
#include "utility/settings.h"
QVector<PlotStyle> PlotStyle::sSystemPlotStyles;
QVector<PlotStyle> PlotStyle::sUserPlotStyles;
QString PlotStyle::sDefaultPlotStyleName = "Dark theme";


void PlotStyle::loadUserStylesFromSettings(QSettings& settingsObj)
{
    sUserPlotStyles.clear();

    ::loadFromSettings(settingsObj, "/Settings/PlotStyle/sDefaultPlotStyleName", sDefaultPlotStyleName);

    QStringList sUserPlotStylesNames;
    ::loadFromSettings(settingsObj, "/Settings/PlotStyle/sUserPlotStylesNames", sUserPlotStylesNames);
    for (const auto &styleName : sUserPlotStylesNames) {
        PlotStyle style;
        if (::loadFromSettings(settingsObj, "/Settings/PlotStyle/sUserPlotStyles/" + styleName, style)) {
            sUserPlotStyles.push_back(style);
        }
    }
}

void PlotStyle::saveUserStylesInSettings(QSettings& settingsObj)
{
    settingsObj.setValue("/Settings/PlotStyle/sDefaultPlotStyleName", sDefaultPlotStyleName);
    QStringList sUserPlotStylesNames;
    for (const auto &style : sUserPlotStyles)
        sUserPlotStylesNames << style.mStyleName;
    ::saveInSettings(settingsObj, "/Settings/PlotStyle/sUserPlotStylesNames", sUserPlotStylesNames);
    for (const auto& style : sUserPlotStyles) {
        ::saveInSettings(settingsObj, "/Settings/PlotStyle/sUserPlotStyles/" + style.mStyleName, style);
    }
}

void initializeDefaultPlotStyles();

const PlotStyle& PlotStyle::defaultPlotStyle()
{
    auto index = std::find_if(sSystemPlotStyles.begin(),
                           sSystemPlotStyles.end(),
                           [] (const PlotStyle& style) {
                                return style.mStyleName == sDefaultPlotStyleName;
                                }
                            );
    if (index != sSystemPlotStyles.end())
        return sSystemPlotStyles[index - sSystemPlotStyles.begin()];

    index = std::find_if(sUserPlotStyles.begin(),
                               sUserPlotStyles.end(),
                               [] (const PlotStyle& style) {
                                    return style.mStyleName == sDefaultPlotStyleName;
                                    }
                                );
    if (index != sUserPlotStyles.end())
        return sUserPlotStyles[index - sUserPlotStyles.begin()];

    sDefaultPlotStyleName = sSystemPlotStyles[0].mStyleName;
    return sSystemPlotStyles[0];
}

void PlotStyle::setUserDefinedPlotStyles(QVector<PlotStyle> newStyles)
{
    sUserPlotStyles = newStyles;
    setDefaultPlotStyle(sDefaultPlotStyleName);
}

void PlotStyle::setDefaultPlotStyle(const QString& styleName)
{
    QStringList styleNames;
    for (const auto &style : sSystemPlotStyles)
        styleNames << style.mStyleName;
    for (const auto &style : sUserPlotStyles)
        styleNames << style.mStyleName;
    if (styleNames.indexOf(styleName) == -1) {
        sDefaultPlotStyleName = sSystemPlotStyles.front().mStyleName;
    } else {
        sDefaultPlotStyleName = styleName;
    }
}

void PlotStyle::initializeDefaultPlotStyles()
{
    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Working horse";

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(Qt::darkGray);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posLeft | utility::posTop | utility::posRight | utility::posBottom;
    plotStyle.mAxisRectProperties.mPen.setColor(Qt::black);
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.9);
    plotStyle.mAxisRectProperties.mGap = 0;

    plotStyle.mXGridProperties.mPen.setColor(Qt::black);
    plotStyle.mXGridProperties.mPen.setWidthF(0.5);
    plotStyle.mXGridProperties.mPen.setStyle(Qt::SolidLine);
    plotStyle.mYGridProperties.mPen.setColor(Qt::black);
    plotStyle.mYGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::SolidLine);

    plotStyle.mXSubGridProperties.mPen.setColor(Qt::gray);
    plotStyle.mXSubGridProperties.mPen.setWidthF(0.7);
    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::DashDotDotLine);
    plotStyle.mYSubGridProperties.mPen.setColor(Qt::gray);
    plotStyle.mYSubGridProperties.mPen.setWidthF(0.7);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::DashDotDotLine);

    plotStyle.mSubTickProperties.mPen.setColor(Qt::black);
    plotStyle.mSubTickProperties.mPen.setWidth(0.75);
    plotStyle.mSubTickProperties.mLengthIn = 3;
    plotStyle.mSubTickProperties.mLengthOut = 0;
    plotStyle.mSubTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;


    plotStyle.mTickProperties.mPen.setColor(Qt::black);
    plotStyle.mTickProperties.mPen.setWidth(1.5);
    plotStyle.mTickProperties.mLengthIn = 6;
    plotStyle.mTickProperties.mLengthOut = 0;
    plotStyle.mTickProperties.mSubTickCount = 1;
    plotStyle.mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;

    plotStyle.mArrowedTextProperties.mArrowEndingStyle = QCPLineEnding::esSpikeArrow;
    QPen pen(Qt::black);
    pen.setWidthF(1.5);
    plotStyle.mArrowedTextProperties.mPen = pen;
    QFont font;
    font.setPointSizeF(14);
    plotStyle.mArrowedTextProperties.mLabelPrefs.mFont = font;
    plotStyle.mArrowedTextProperties.mLabelPrefs.mColor = Qt::black;
    plotStyle.mArrowedTextProperties.mBackgroundColor = Qt::white;

    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  << Qt::blue
                         << Qt::darkGreen
                         << Qt::red
                         << Qt::magenta
                         << Qt::black
                         << Qt::green
                         << Qt::darkYellow
                         << Qt::darkMagenta
                         << Qt::darkCyan
                         << Qt::darkGray;

    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  << QColor(134, 193, 255, 175)
                         << QColor(125, 214, 143, 175)
                         << QColor(255, 153, 135, 175)
                         << QColor(255, 190, 255, 175)
                         << QColor(152, 152, 152, 175)
                         << QColor(213, 255, 216, 175)
                         << QColor(230, 218, 151, 175)
                         << QColor(232, 160, 205, 175)
                         << QColor(177, 244, 250, 175)
                         << QColor(209, 209, 209, 175);
    sSystemPlotStyles.push_back(plotStyle);
    }



    //--------------------------------------------------------------------------
    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Blue print";

    QColor foregroundColor = QColor(210, 210, 210);

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(foregroundColor);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posLeft | utility::posTop | utility::posRight | utility::posBottom;
    plotStyle.mAxisRectProperties.mPen.setColor(foregroundColor);
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.75);
    plotStyle.mAxisRectProperties.mGap = 0;

    plotStyle.mXGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXGridProperties.mPen.setWidthF(0.5);
    plotStyle.mXGridProperties.mPen.setStyle(Qt::SolidLine);
    plotStyle.mYGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::SolidLine);

    plotStyle.mXSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXSubGridProperties.mPen.setWidthF(0.5);
    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::DashDotLine);
    plotStyle.mYSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYSubGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mSubTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mSubTickProperties.mPen.setWidth(0.1);
    plotStyle.mSubTickProperties.mLengthIn = 0;
    plotStyle.mSubTickProperties.mLengthOut = 0;
    plotStyle.mSubTickProperties.mPositions = utility::Position::posNone;


    plotStyle.mTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mTickProperties.mPen.setWidth(1.5);
    plotStyle.mTickProperties.mLengthIn = 2;
    plotStyle.mTickProperties.mLengthOut = 0;
    plotStyle.mTickProperties.mSubTickCount = 3;
    plotStyle.mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;


    plotStyle.mLegendProperties.mIsLegendFrameShadowEnabled = false;

    plotStyle.mTitleProperties.mColor = foregroundColor;
    plotStyle.mXLabelProperties.mColor = foregroundColor;
    plotStyle.mYLabelProperties.mColor = foregroundColor;
    plotStyle.mXTickLabelProperties.mColor = foregroundColor;
    plotStyle.mYTickLabelProperties.mColor = foregroundColor;

    plotStyle.mColorPref.mFontColor = foregroundColor;
    plotStyle.mColorPref.mBackgroundColor = QColor(36, 73, 126);
    plotStyle.mColorPref.mGraficalPrimitivesColor = foregroundColor;


    plotStyle.mArrowedTextProperties.mBackgroundColor = QColor(36, 73, 126);
    plotStyle.mArrowedTextProperties.mPen.setColor(foregroundColor);
    plotStyle.mArrowedTextProperties.mLabelPrefs.mColor = foregroundColor;

    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  <<  QColor(160, 246, 255)
                         << QColor(255, 55, 92)
                         << QColor(255, 239, 2)
                         << QColor(170, 255, 101)
                         << QColor(255, 192, 236)
                         << QColor(85, 170, 127)
                         << QColor(255, 255, 255);

    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  << QColor(134, 193, 255, 175)
                         << QColor(255, 51, 98, 175)
                         << QColor(251, 255, 11, 175)
                         << QColor(170, 255, 101, 175)
                         << QColor(255, 192, 236, 175)
                         << QColor(125, 214, 143, 175)
                         << QColor(255, 255, 255, 175);
    sSystemPlotStyles.push_back(plotStyle);

    }


    //--------------------------------------------------------------------------
    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Plain";

    QColor foregroundColor = Qt::darkGray;

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(foregroundColor);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posTop  | utility::posBottom;
    plotStyle.mAxisRectProperties.mPen.setColor(foregroundColor);
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.6);
    plotStyle.mAxisRectProperties.mGap = 0;


    plotStyle.mXGridProperties.mPen.setStyle(Qt::NoPen);
    plotStyle.mYGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYGridProperties.mPen.setWidthF(0.6);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::SolidLine);


    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::NoPen);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::NoPen);

    plotStyle.mSubTickProperties.mPositions = utility::Position::posNone;

    plotStyle.mTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mTickProperties.mPen.setWidth(1.5);
    plotStyle.mTickProperties.mLengthIn = 0;
    plotStyle.mTickProperties.mLengthOut = 3;
    plotStyle.mTickProperties.mSubTickCount = 0;
    plotStyle.mTickProperties.mPositions =  utility::Position::posBottom;


    plotStyle.mLegendProperties.mIsLegendFrameShadowEnabled = false;
    plotStyle.mLegendProperties.mIsLegendFrameVisible = false;
    plotStyle.mLegendProperties.mLocation = style::LegendLocation::Right;

    plotStyle.mTitleProperties.mColor = foregroundColor;
    plotStyle.mXLabelProperties.mColor = foregroundColor;
    plotStyle.mYLabelProperties.mColor = foregroundColor;
    plotStyle.mXTickLabelProperties.mColor = foregroundColor;
    plotStyle.mYTickLabelProperties.mColor = foregroundColor;

    plotStyle.mColorPref.mFontColor = foregroundColor;
    plotStyle.mColorPref.mBackgroundColor = Qt::white;

    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  <<  QColor(255, 114, 114)
                         << QColor(70, 170, 170)
                         << QColor(154, 159, 255)
                         << QColor(170, 255, 150)
                         << QColor(150, 205, 255)
                         << QColor(90, 90, 98)
                         << QColor(255, 190, 117)
                         << QColor(255, 190, 115)
                         << QColor(255, 115, 155)
                         << QColor(255, 235, 110)


                            ;

    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  <<  QColor(255, 114, 114, 175)
                                     << QColor(70, 170, 170, 175)
                                     << QColor(154, 159, 255, 175)
                                     << QColor(170, 255, 150, 175)
                                     << QColor(150, 205, 255, 175)
                                     << QColor(90, 90, 98, 175)
                                     << QColor(255, 190, 117, 175)
                                     << QColor(255, 190, 115, 175)
                                     << QColor(255, 115, 155, 175)
                                     << QColor(255, 235, 110, 175);
    sSystemPlotStyles.push_back(plotStyle);
    }



    //--------------------------------------------------------------------------
    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Ggplot2";

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(Qt::darkGray);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posNone;
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.9);
    plotStyle.mAxisRectProperties.mGap = 4;

    plotStyle.mXGridProperties.mPen.setColor(Qt::white);
    plotStyle.mXGridProperties.mPen.setWidthF(1);
    plotStyle.mXGridProperties.mPen.setStyle(Qt::SolidLine);
    plotStyle.mYGridProperties.mPen.setColor(Qt::white);
    plotStyle.mYGridProperties.mPen.setWidthF(1);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::SolidLine);

    plotStyle.mXSubGridProperties.mPen.setColor(Qt::white);
    plotStyle.mXSubGridProperties.mPen.setWidthF(0.4);
    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::SolidLine);
    plotStyle.mYSubGridProperties.mPen.setColor(Qt::white);
    plotStyle.mYSubGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::SolidLine);

    plotStyle.mSubTickProperties.mPen.setColor(Qt::darkGray);
    plotStyle.mSubTickProperties.mPen.setWidth(0.75);
    plotStyle.mSubTickProperties.mLengthIn = 3;
    plotStyle.mSubTickProperties.mLengthOut = 0;
    plotStyle.mSubTickProperties.mPositions = utility::Position::posNone;
    plotStyle.mSubTickProperties.mTicksOnInternalAxisRectEnabled = false;

    plotStyle.mTickProperties.mPen.setColor(Qt::darkGray);
    plotStyle.mTickProperties.mPen.setWidth(1.4);
    plotStyle.mTickProperties.mLengthIn = 0;
    plotStyle.mTickProperties.mLengthOut = 5;
    plotStyle.mTickProperties.mSubTickCount = 1;
    plotStyle.mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom;
    plotStyle.mTickProperties.mTicksOnInternalAxisRectEnabled = false;

    plotStyle.mArrowedTextProperties.mArrowEndingStyle = QCPLineEnding::esSpikeArrow;
    QPen pen(Qt::black);
    pen.setWidthF(1.5);
    plotStyle.mArrowedTextProperties.mPen = pen;
    QFont font;
    font.setPointSizeF(14);
    plotStyle.mArrowedTextProperties.mLabelPrefs.mFont = font;
    plotStyle.mArrowedTextProperties.mLabelPrefs.mColor = Qt::black;
    plotStyle.mArrowedTextProperties.mBackgroundColor = Qt::white;

    QBrush brush(Qt::SolidPattern);
    brush.setColor(QColor(225,225,225));
    plotStyle.mColorPref.mAxesBackgroundBrush = brush;
    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  << Qt::blue
                         << Qt::darkGreen
                         << Qt::red
                         << Qt::magenta
                         << Qt::black
                         << Qt::green
                         << Qt::darkYellow
                         << Qt::darkMagenta
                         << Qt::darkCyan
                         << Qt::darkGray;

    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  << QColor(134, 193, 255, 175)
                         << QColor(125, 214, 143, 175)
                         << QColor(255, 153, 135, 175)
                         << QColor(255, 190, 255, 175)
                         << QColor(152, 152, 152, 175)
                         << QColor(213, 255, 216, 175)
                         << QColor(230, 218, 151, 175)
                         << QColor(232, 160, 205, 175)
                         << QColor(177, 244, 250, 175)
                         << QColor(209, 209, 209, 175);
    sSystemPlotStyles.push_back(plotStyle);
    }

    //--------------------------------------------------------------------------
    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Dark theme";

    QColor foregroundColor = QColor(0xBD, 0xC3, 0xC7);
    QColor backgroundolor = QColor(0x23, 0x26, 0x29);

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(foregroundColor);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posLeft | utility::posTop | utility::posRight | utility::posBottom;
    plotStyle.mAxisRectProperties.mPen.setColor(foregroundColor);
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.75);
    plotStyle.mAxisRectProperties.mGap = 3;

    plotStyle.mXGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXGridProperties.mPen.setWidthF(0.5);
    plotStyle.mXGridProperties.mPen.setStyle(Qt::SolidLine);
    plotStyle.mYGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::SolidLine);

    plotStyle.mXSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXSubGridProperties.mPen.setWidthF(0.5);
    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::DashDotLine);
    plotStyle.mYSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYSubGridProperties.mPen.setWidthF(0.5);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mSubTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mSubTickProperties.mPen.setWidth(1);
    plotStyle.mSubTickProperties.mLengthIn = 3;
    plotStyle.mSubTickProperties.mLengthOut = 0;
    plotStyle.mSubTickProperties.mPositions = utility::Position::posNone;


    plotStyle.mTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mTickProperties.mPen.setWidth(1.5);
    plotStyle.mTickProperties.mLengthIn = 3;
    plotStyle.mTickProperties.mLengthOut = 0;
    plotStyle.mTickProperties.mSubTickCount = 1;
    plotStyle.mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;


    plotStyle.mLegendProperties.mIsLegendFrameShadowEnabled = false;

    plotStyle.mTitleProperties.mColor = foregroundColor;
    plotStyle.mXLabelProperties.mColor = foregroundColor;
    plotStyle.mYLabelProperties.mColor = foregroundColor;
    plotStyle.mXTickLabelProperties.mColor = foregroundColor;
    plotStyle.mYTickLabelProperties.mColor = foregroundColor;

    plotStyle.mColorPref.mFontColor = foregroundColor;
    plotStyle.mColorPref.mBackgroundColor = QColor(0x31, 0x36, 0x3b);
    plotStyle.mColorPref.mGraficalPrimitivesColor = foregroundColor;


    plotStyle.mArrowedTextProperties.mBackgroundColor = backgroundolor;
    plotStyle.mArrowedTextProperties.mPen.setColor(foregroundColor);
    plotStyle.mArrowedTextProperties.mLabelPrefs.mColor = foregroundColor;

    plotStyle.mHorLevelProperties.mLabel.mColor = foregroundColor;

    QBrush brush(Qt::SolidPattern);
    brush.setColor(backgroundolor);
    plotStyle.mColorPref.mAxesBackgroundBrush = brush;
    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  <<  QColor(160, 246, 255)
                         << QColor(255, 55, 92)
                         << QColor(255, 239, 2)
                         << QColor(170, 255, 101)
                         << QColor(255, 192, 236)
                         << QColor(85, 170, 127)
                         << QColor(255, 255, 255)
                         << QColor(66, 149, 243);


    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  << QColor(134, 193, 255, 120)
                         << QColor(255, 51, 98, 120)
                         << QColor(251, 255, 11, 120)
                         << QColor(170, 255, 101, 120)
                         << QColor(255, 192, 236, 120)
                         << QColor(125, 214, 143, 120)
                         << QColor(255, 255, 255, 120)
                         << QColor(66, 149, 243, 120);
    sSystemPlotStyles.push_back(plotStyle);

    }


    //--------------------------------------------------------------------------

    {
    PlotStyle plotStyle;
    plotStyle.mStyleName = "Fusion";

    QColor foregroundColor = QColor(0xBD, 0xC3, 0xC7);
    QColor backgroundolor = QColor(0x32, 0x32, 0x32);

    plotStyle.mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    plotStyle.mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    plotStyle.mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    plotStyle.mEventProperties.mPen.setWidthF(2.4);
    plotStyle.mEventProperties.mPen.setColor(foregroundColor);
    plotStyle.mEventProperties.mPen.setStyle(Qt::DashDotLine);

    plotStyle.mHorLevelProperties.mPen =  plotStyle.mEventProperties.mPen;

    plotStyle.mAxisRectProperties.mPositions = utility::posNone; //utility::posLeft | utility::posTop | utility::posRight | utility::posBottom;
    plotStyle.mAxisRectProperties.mPen.setColor(foregroundColor);
    plotStyle.mAxisRectProperties.mPen.setWidthF(0.75);
    plotStyle.mAxisRectProperties.mPen.setStyle(Qt::DashLine);
    plotStyle.mAxisRectProperties.mGap = 3;

    plotStyle.mXGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXGridProperties.mPen.setWidthF(0.6);
    plotStyle.mXGridProperties.mPen.setStyle(Qt::DashLine);
    plotStyle.mYGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYGridProperties.mPen.setWidthF(0.6);
    plotStyle.mYGridProperties.mPen.setStyle(Qt::DashLine);

    plotStyle.mXSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mXSubGridProperties.mPen.setWidthF(0.4);
    plotStyle.mXSubGridProperties.mPen.setStyle(Qt::DotLine);
    plotStyle.mYSubGridProperties.mPen.setColor(foregroundColor);
    plotStyle.mYSubGridProperties.mPen.setWidthF(0.4);
    plotStyle.mYSubGridProperties.mPen.setStyle(Qt::DotLine);

    plotStyle.mSubTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mSubTickProperties.mPen.setWidth(1);
    plotStyle.mSubTickProperties.mLengthIn = 3;
    plotStyle.mSubTickProperties.mLengthOut = 0;
    plotStyle.mSubTickProperties.mPositions = utility::Position::posNone;


    plotStyle.mTickProperties.mPen.setColor(foregroundColor);
    plotStyle.mTickProperties.mPen.setWidth(1.5);
    plotStyle.mTickProperties.mLengthIn = 0;
    plotStyle.mTickProperties.mLengthOut = 5;
    plotStyle.mTickProperties.mSubTickCount = 1;
    plotStyle.mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom;
    plotStyle.mTickProperties.mTicksOnInternalAxisRectEnabled = false;

    plotStyle.mLegendProperties.mIsLegendFrameShadowEnabled = false;

    plotStyle.mTitleProperties.mColor = foregroundColor;
    plotStyle.mXLabelProperties.mColor = foregroundColor;
    plotStyle.mYLabelProperties.mColor = foregroundColor;
    plotStyle.mXTickLabelProperties.mColor = foregroundColor;
    plotStyle.mYTickLabelProperties.mColor = foregroundColor;

    plotStyle.mColorPref.mFontColor = foregroundColor;
    plotStyle.mColorPref.mBackgroundColor = backgroundolor;
    plotStyle.mColorPref.mGraficalPrimitivesColor = foregroundColor;


    plotStyle.mArrowedTextProperties.mBackgroundColor = backgroundolor;
    plotStyle.mArrowedTextProperties.mPen.setColor(foregroundColor);
    plotStyle.mArrowedTextProperties.mLabelPrefs.mColor = foregroundColor;

    plotStyle.mHorLevelProperties.mLabel.mColor = foregroundColor;

    plotStyle.mColorPref.mGraphColorList.clear();
    plotStyle.mColorPref.mGraphColorList  <<  QColor(160, 246, 255)
                         << QColor(255, 55, 92)
                         << QColor(255, 239, 2)
                         << QColor(170, 255, 101)
                         << QColor(255, 192, 236)
                         << QColor(85, 170, 127)
                         << QColor(255, 255, 255)
                         << QColor(66, 149, 243);


    plotStyle.mColorPref.mBrushColorList.clear();
    plotStyle.mColorPref.mBrushColorList  << QColor(134, 193, 255, 120)
                         << QColor(255, 51, 98, 120)
                         << QColor(251, 255, 11, 120)
                         << QColor(170, 255, 101, 120)
                         << QColor(255, 192, 236, 120)
                         << QColor(125, 214, 143, 120)
                         << QColor(255, 255, 255, 120)
                         << QColor(66, 149, 243, 120);
    sSystemPlotStyles.push_back(plotStyle);

    }
}
