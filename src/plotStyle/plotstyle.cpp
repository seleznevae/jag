#include "plotstyle.h"
#include <QSettings>
#include "../utility/settings.h"




PlotStyle::PlotStyle()
    :/*mUnitsVisibility(false), mXLabelString("time, s"), mLineWidth(1),
      mLineInterpolation(GraphModel::LineStyle::lsLine), mTimeAxisType(QCPAxis::ltNumber),
      mScatterSize(8), mScatterDecimation(1),*/ mXTickRotation(0), mEventLabelRotation(0)
{
    mXAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    mXAxisPref.mAxisLineEnding = QCPLineEnding::esNone;
    mYAxisPref.mIsAxisDrawingAtCenterEnabled = false;
    mYAxisPref.mAxisLineEnding = QCPLineEnding::esNone;

    mEventProperties.mPen.setWidthF(2.4);
    mEventProperties.mPen.setColor(Qt::darkGray);
    mEventProperties.mPen.setStyle(Qt::DashDotLine);

    mHorLevelProperties.mPen =  mEventProperties.mPen;

    mAxisRectProperties.mPositions = utility::posLeft | utility::posTop | utility::posRight | utility::posBottom;
    mAxisRectProperties.mPen.setColor(Qt::black);
    mAxisRectProperties.mPen.setWidthF(0.9);
    mAxisRectProperties.mGap = 0;

    mXGridProperties.mPen.setColor(Qt::black);
    mXGridProperties.mPen.setWidthF(0.5);
    mXGridProperties.mPen.setStyle(Qt::SolidLine);
    mYGridProperties.mPen.setColor(Qt::black);
    mYGridProperties.mPen.setWidthF(0.5);
    mYGridProperties.mPen.setStyle(Qt::SolidLine);

    mXSubGridProperties.mPen.setColor(Qt::gray);
    mXSubGridProperties.mPen.setWidthF(0.7);
    mXSubGridProperties.mPen.setStyle(Qt::DashDotDotLine);
    mYSubGridProperties.mPen.setColor(Qt::gray);
    mYSubGridProperties.mPen.setWidthF(0.7);
    mYSubGridProperties.mPen.setStyle(Qt::DashDotDotLine);

    mSubTickProperties.mPen.setColor(Qt::black);
    mSubTickProperties.mPen.setWidth(0.75);
    mSubTickProperties.mLengthIn = 3;
    mSubTickProperties.mLengthOut = 0;
    mSubTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;
    mSubTickProperties.mTicksOnInternalAxisRectEnabled = true;


    mTickProperties.mPen.setColor(Qt::black);
    mTickProperties.mPen.setWidth(1.5);
    mTickProperties.mLengthIn = 6;
    mTickProperties.mLengthOut = 0;
    mTickProperties.mSubTickCount = 1;
    mTickProperties.mPositions = utility::Position::posLeft | utility::Position::posBottom | utility::Position::posTop | utility::Position::posRight;
    mTickProperties.mTicksOnInternalAxisRectEnabled = true;

    mArrowedTextProperties.mArrowEndingStyle = QCPLineEnding::esSpikeArrow;
    QPen pen(Qt::black);
    pen.setWidthF(1.5);
    mArrowedTextProperties.mPen = pen;
    QFont font;
    font.setPointSizeF(14);
    mArrowedTextProperties.mLabelPrefs.mFont = font;
    mArrowedTextProperties.mLabelPrefs.mColor = Qt::black;
    mArrowedTextProperties.mBackgroundColor = Qt::white;

    mColorPref.mGraphColorList  << Qt::blue
                         << Qt::darkGreen
                         << Qt::red
                         << Qt::magenta
                         << Qt::black
                         << Qt::green
                         << Qt::darkYellow
                         << Qt::darkMagenta
                         << Qt::darkCyan
                         << Qt::darkGray;

    mColorPref.mBrushColorList  << QColor(134, 193, 255, 175)
                         << QColor(125, 214, 143, 175)
                         << QColor(255, 153, 135, 175)
                         << QColor(255, 190, 255, 175)
                         << QColor(152, 152, 152, 175)
                         << QColor(213, 255, 216, 175)
                         << QColor(230, 218, 151, 175)
                         << QColor(232, 160, 205, 175)
                         << QColor(177, 244, 250, 175)
                         << QColor(209, 209, 209, 175);


    //DEBUG

}

// ---------        Grid       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::Grid &grid)
{
    ::saveInSettings(settingsObj, name + "/mPen"      , grid.mPen);
    ::saveInSettings(settingsObj, name + "/mIsVisible", grid.mIsVisible);
}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::Grid &grid)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mPen"      , grid.mPen)
              && ::loadFromSettings(settingsObj, name + "/mIsVisible", grid.mIsVisible);
    return ret;
}


// ---------        AxisRect       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::AxisRect &rect)
{
    ::saveInSettings(settingsObj, name + "/mPen"       , rect.mPen);
    ::saveInSettings(settingsObj, name + "/mPositions" , rect.mPositions);
    ::saveInSettings(settingsObj, name + "/mGap"       , rect.mGap);
}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::AxisRect &rect)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mPen"      , rect.mPen)
              && ::loadFromSettings(settingsObj, name + "/mPositions", rect.mPositions)
              && ::loadFromSettings(settingsObj, name + "/mGap"      , rect.mGap);
    return ret;
}

// ---------        Tick       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::Tick &tick)
{
    ::saveInSettings(settingsObj, name + "/mPen"      , tick.mPen);
    ::saveInSettings(settingsObj, name + "/mLengthIn" , tick.mLengthIn);
    ::saveInSettings(settingsObj, name + "/mLengthOut", tick.mLengthOut);
    ::saveInSettings(settingsObj, name + "/mPositions", tick.mPositions);
    ::saveInSettings(settingsObj, name + "/mTicksOnInternalAxisRectEnabled", tick.mTicksOnInternalAxisRectEnabled);
}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::Tick &tick)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mPen"      , tick.mPen)
              && ::loadFromSettings(settingsObj, name + "/mLengthIn" , tick.mLengthIn)
              && ::loadFromSettings(settingsObj, name + "/mLengthOut", tick.mLengthOut)
              && ::loadFromSettings(settingsObj, name + "/mPositions", tick.mPositions)
              && ::loadFromSettings(settingsObj, name + "/mTicksOnInternalAxisRectEnabled", tick.mTicksOnInternalAxisRectEnabled);
     return ret;
}

// ---------        MainTick       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::MainTick &maintick)
{
    ::saveInSettings(settingsObj, name + "/mSubTickCount" , maintick.mSubTickCount);
    style::saveInSettings(settingsObj, name + "/Tick"          , static_cast<const style::Tick&>(maintick));

}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::MainTick &maintick)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mSubTickCount", maintick.mPen)
              && style::loadFromSettings(settingsObj, name + "/Tick"         , static_cast<style::Tick&>(maintick));
    return ret;
}

// ---------        Label       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::Label &label)
{
    ::saveInSettings(settingsObj, name + "/mColor", label.mColor);
    ::saveInSettings(settingsObj, name + "/mFont" , label.mFont);
}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::Label &label)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mColor", label.mColor)
              && ::loadFromSettings(settingsObj, name + "/mFont" , label.mFont);
    return ret;
}

// ---------        ColorPref       ----------------------
void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::ColorPref &colorPref)
{
    ::saveInSettings(settingsObj, name + "/mBackgroundColor"        , colorPref.mBackgroundColor);
    ::saveInSettings(settingsObj, name + "/mAxesBackgroundBrush"    , colorPref.mAxesBackgroundBrush);
    ::saveInSettings(settingsObj, name + "/mGraficalPrimitivesColor", colorPref.mGraficalPrimitivesColor);
    ::saveInSettings(settingsObj, name + "/mFontColor"              , colorPref.mFontColor);
    ::saveInSettings(settingsObj, name + "/mGraphColorList"         , colorPref.mGraphColorList);
    ::saveInSettings(settingsObj, name + "/mBrushColorList"         , colorPref.mBrushColorList);

}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::ColorPref &colorPref)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mBackgroundColor", colorPref.mBackgroundColor)
              && ::loadFromSettings(settingsObj, name + "/mAxesBackgroundBrush" , colorPref.mAxesBackgroundBrush)
              && ::loadFromSettings(settingsObj, name + "/mGraficalPrimitivesColor" , colorPref.mGraficalPrimitivesColor)
              && ::loadFromSettings(settingsObj, name + "/mFontColor"      , colorPref.mFontColor)
              && ::loadFromSettings(settingsObj, name + "/mGraphColorList" , colorPref.mGraphColorList)
              && ::loadFromSettings(settingsObj, name + "/mBrushColorList" , colorPref.mBrushColorList);
    return ret;
}


// ---------        LegendPref       ----------------------
bool style::isLegendFloating(style::LegendLocation location)
{
    return location == style::LegendLocation::FloatingLeft
            ||  location == style::LegendLocation::FloatingRight
            ||  location == style::LegendLocation::FloatingTop
            ||  location == style::LegendLocation::FloatingBottom
            ||  location == style::LegendLocation::FloatingTopLeft
            ||  location == style::LegendLocation::FloatingTopRight
            ||  location == style::LegendLocation::FloatingBottomLeft
            ||  location == style::LegendLocation::FloatingBottomRight
            ;
}

void style::saveInSettings(QSettings &settingsObj, const QString &name, const style::LegendPref &axisPref)
{
    ::saveInSettings(settingsObj, name + "/mIsLegendVisible"            , axisPref.mIsLegendVisible);
    ::saveInSettings(settingsObj, name + "/mLegendFont"                 , axisPref.mLegendFont);
    ::saveInSettings(settingsObj, name + "/mIsLegendFrameVisible"       , axisPref.mIsLegendFrameVisible);
    ::saveInSettings(settingsObj, name + "/mLocation"                   , axisPref.mLocation);
    ::saveInSettings(settingsObj, name + "/mLegendFrameCornerRadius"    , axisPref.mLegendFrameCornerRadius);
    ::saveInSettings(settingsObj, name + "/mIsLegendFrameShadowEnabled" , axisPref.mIsLegendFrameShadowEnabled);
}

bool style::loadFromSettings(QSettings &settingsObj, const QString &name, style::LegendPref &axisPref)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mIsLegendVisible"            , axisPref.mIsLegendVisible)
              && ::loadFromSettings(settingsObj, name + "/mLegendFont"                 , axisPref.mLegendFont)
              && ::loadFromSettings(settingsObj, name + "/mIsLegendFrameVisible"       , axisPref.mIsLegendFrameVisible)
              && ::loadFromSettings(settingsObj, name + "/mLocation"                   , axisPref.mLocation)
              && ::loadFromSettings(settingsObj, name + "/mLegendFrameCornerRadius"    , axisPref.mLegendFrameCornerRadius)
              && ::loadFromSettings(settingsObj, name + "/mIsLegendFrameShadowEnabled" , axisPref.mIsLegendFrameShadowEnabled);
    return ret;
}


// ---------        AxisPref       ----------------------
void style::saveInSettings(QSettings& settingsObj, const QString &name, const style::AxisPref &axisPref)
{
    ::saveInSettings(settingsObj, name + "/mIsAxisDrawingAtCenterEnabled"   , axisPref.mIsAxisDrawingAtCenterEnabled);
    ::saveInSettings(settingsObj, name + "/mAxisLineEnding"                 , axisPref.mAxisLineEnding);
}

bool style::loadFromSettings(QSettings& settingsObj, const QString &name, style::AxisPref &axisPref)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mIsAxisDrawingAtCenterEnabled" , axisPref.mIsAxisDrawingAtCenterEnabled)
              && ::loadFromSettings(settingsObj, name + "/mAxisLineEnding"               , axisPref.mAxisLineEnding);
    return ret;
}

// ---------        LevelPref       ----------------------
void style::saveInSettings(QSettings& settingsObj, const QString &name, const style::LevelPref &levelPref)
{
    style::saveInSettings(settingsObj, name + "/mLabel"   , levelPref.mLabel);
    ::saveInSettings(settingsObj, name + "/mPen"    , levelPref.mPen);
}

bool style::loadFromSettings(QSettings& settingsObj, const QString &name, style::LevelPref &levelPref)
{
    bool ret =   style::loadFromSettings(settingsObj, name + "/mLabel" , levelPref.mLabel)
              && ::loadFromSettings(settingsObj, name + "/mPen"  , levelPref.mPen);
    return ret;
}


// ---------        ArrowedTextPref       ----------------------
void style::saveInSettings(QSettings& settingsObj, const QString &name, const style::ArrowedTextPref &arrTextPref)
{
    ::saveInSettings(settingsObj, name + "/mBackgroundColor"   , arrTextPref.mBackgroundColor);
    ::saveInSettings(settingsObj, name + "/mArrowEndingStyle"  , arrTextPref.mArrowEndingStyle);
    style::saveInSettings(settingsObj, name + "/mLabelPrefs"        , arrTextPref.mLabelPrefs);
}

bool style::loadFromSettings(QSettings& settingsObj, const QString &name, style::ArrowedTextPref &arrTextPref)
{
    bool ret =   ::loadFromSettings(settingsObj, name + "/mBackgroundColor" , arrTextPref.mBackgroundColor)
              && ::loadFromSettings(settingsObj, name + "/mArrowEndingStyle", arrTextPref.mArrowEndingStyle)
              && style::loadFromSettings(settingsObj, name + "/mLabelPrefs"      , arrTextPref.mLabelPrefs);
    return ret;
}




// ---------        PlotStyle       ----------------------
void saveInSettings(QSettings &settingsObj, const QString &name, const PlotStyle &plotStyle)
{
    using namespace style;

    #define X(x_macro_arg) saveInSettings(settingsObj, name + "/" + #x_macro_arg  , plotStyle. x_macro_arg );
    PLOT_STYLE_PARMETERS_LIST_MACRO
    #undef X

//    ::saveInSettings(settingsObj, name + "/mLegendVisibility"        , plotStyle.mLegendVisibility);
//    ::saveInSettings(settingsObj, name + "/mUnitsVisibility", plotStyle.mUnitsVisibility);
//    ::saveInSettings(settingsObj, name + "/mLegendFont"              , plotStyle.mLegendFont);

}

bool loadFromSettings(QSettings &settingsObj, const QString &name, PlotStyle &plotStyle)
{
    using namespace style;

#define X(x_macro_arg) loadFromSettings(settingsObj, name + "/" + #x_macro_arg  , plotStyle. x_macro_arg )  &&
    bool ret =
            PLOT_STYLE_PARMETERS_LIST_MACRO
            true;
    #undef X
    return ret;

//    bool ret =   ::loadFromSettings(settingsObj, name + "/mLegendVisibility", plotStyle.mLegendVisibility)
//              && ::loadFromSettings(settingsObj, name + "/mUnitsVisibility" , plotStyle.mUnitsVisibility)
//              && ::loadFromSettings(settingsObj, name + "/mLegendFont" , plotStyle.mLegendFont);
}





