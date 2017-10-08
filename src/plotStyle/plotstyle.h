#ifndef PLOTSTYLE_H
#define PLOTSTYLE_H

#include <QFont>
#include <QString>
#include "../core/qcustomplot.h"
#include "../core/graphmodel.h"
#include "utility/utility.h"
#include <QMargins>

class QSettings;

namespace style {


///////////////////////////////////////////////////////////////////////////
/// \brief The Grid struct
///
struct Grid {
    QPen mPen;
    bool mIsVisible = true;

    bool operator ==(const Grid & arg) const {  return mPen == arg.mPen && mIsVisible == arg.mIsVisible; }
    bool operator !=(const Grid & arg) const {  return !(*this==arg); }
};

void saveInSettings(QSettings& settingsObj, const QString &name, const Grid &grid);
bool loadFromSettings(QSettings& settingsObj, const QString &name, Grid &grid);

//struct MainGrid: public Grid {
//    int mSubGridCount = 1;

//    bool operator ==(const MainGrid & arg) const {  return mSubGridCount == arg.mSubGridCount && static_cast<Grid>(*this) == static_cast<Grid>(arg); }
//    bool operator !=(const MainGrid & arg) const {  return !(*this==arg); }
//};

//////////////////////////////////////////////////////////////////////////////
/// \brief The Title struct
///
//struct Title {
//    QFont mFont;
//    QColor mColor;

//    bool operator ==(const Title & arg) const {  return mFont == arg.mFont && mColor == arg.mColor; }
//    bool operator !=(const Title & arg) const {  return !(*this==arg); }
//};
//void saveInSettings(QSettings& settingsObj, const QString &name, const Title &title);
//bool loadFromSettings(QSettings& settingsObj, const QString &name, Title &title);

///////////////////////////////////////////////////////////////////////////////
/// \brief The AxisRect struct
///
struct AxisRect {
    QPen mPen;

//    enum Position {posLeft   = 0x0001,
//                   posTop    = 0x0002,
//                   posRight  = 0x0004,
//                   posBottom = 0x0008
//    };
//    Q_DECLARE_FLAGS(Positions, Position)
    utility::Positions mPositions;
    int mGap;

    bool operator ==(const AxisRect & arg) const {  return mPen == arg.mPen && mPositions == arg.mPositions && mGap == arg.mGap; }
    bool operator !=(const AxisRect & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const AxisRect &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, AxisRect &);

////////////////////////////////////////////////////////////////////////////
/// \brief The Tick struct
///
struct Tick {
//    bool mIsVisible;
    QPen mPen;
    int mLengthIn;
    int mLengthOut;

    utility::Positions mPositions;
    bool mTicksOnInternalAxisRectEnabled;

    bool operator ==(const Tick & arg) const {  return /*mIsVisible == arg.mIsVisible &&*/mPen == arg.mPen && mPositions == arg.mPositions && mLengthIn == arg.mLengthIn && mLengthOut == arg.mLengthOut && mTicksOnInternalAxisRectEnabled == arg.mTicksOnInternalAxisRectEnabled; }
    bool operator !=(const Tick & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const Tick &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, Tick &);

///////////////////////////////////////////////////////////////////////////
/// \brief The MainTick struct
///
struct MainTick: public Tick {
    int mSubTickCount;

    bool operator ==(const MainTick & arg) const {  return mSubTickCount == arg.mSubTickCount && static_cast<Tick>(*this) == static_cast<Tick>(arg); }
    bool operator !=(const MainTick & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const MainTick &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, MainTick &);

///////////////////////////////////////////////////////////////////////////////
/// \brief The Label struct
///
struct Label {
    QColor mColor;
    QFont mFont;

    bool operator ==(const Label & arg) const {  return mColor == arg.mColor && mFont == arg.mFont; }
    bool operator !=(const Label & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const Label &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, Label &);

////////////////////////////////////////////////////////////////////////////
/// \brief The ColorPref struct
///
struct ColorPref {
    QColor mBackgroundColor = Qt::white;
    QBrush mAxesBackgroundBrush = Qt::NoBrush;
//    QColor mAxesBackgroundColor = Qt::white;
    QColor mGraficalPrimitivesColor = Qt::black;
    QColor mFontColor = Qt::black;
    QList<QColor> mGraphColorList;
    QList<QColor> mBrushColorList;

    bool operator ==(const ColorPref & arg) const {
        return mBackgroundColor == arg.mBackgroundColor
                && mAxesBackgroundBrush == arg.mAxesBackgroundBrush
                && mGraficalPrimitivesColor == arg.mGraficalPrimitivesColor
                && mFontColor == arg.mFontColor
                && mGraphColorList == arg.mGraphColorList
                && mBrushColorList == arg.mBrushColorList;
    }
    bool operator !=(const ColorPref & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const ColorPref &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, ColorPref &);


////////////////////////////////////////////////////////////////////////////
/// \brief The LegendPref struct
///

enum class LegendLocation {Left,
                         Right,
                         TopSeparate,
                         TopCombined,
                         TopCombinedInRow,
                         BottomSeparate,
                         BottomCombined,
                         BottomCombinedInRow,
                         FloatingLeft,
                         FloatingRight,
                         FloatingTop,
                         FloatingBottom,
                         FloatingTopLeft,
                         FloatingTopRight,
                         FloatingBottomLeft,
                         FloatingBottomRight,
                        };
bool isLegendFloating(style::LegendLocation location);


struct LegendPref {
    bool mIsLegendVisible = true;
    QFont mLegendFont;
    bool mIsLegendFrameVisible = true;
    LegendLocation mLocation = LegendLocation::FloatingTopRight;
    int mLegendFrameCornerRadius = 10;
    int mIsLegendFrameShadowEnabled = true;

    bool operator ==(const LegendPref & arg) const {  return mIsLegendVisible == arg.mIsLegendVisible && mLegendFont == arg.mLegendFont && mIsLegendFrameVisible == arg.mIsLegendFrameVisible && mLocation == arg.mLocation && mLegendFrameCornerRadius == arg.mLegendFrameCornerRadius && mIsLegendFrameShadowEnabled == arg.mIsLegendFrameShadowEnabled; }
    bool operator !=(const LegendPref & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const LegendPref &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, LegendPref &);

struct AxisPref {
    bool mIsAxisDrawingAtCenterEnabled;
    QCPLineEnding::EndingStyle mAxisLineEnding;

    bool operator ==(const AxisPref & arg) const {  return mIsAxisDrawingAtCenterEnabled == arg.mIsAxisDrawingAtCenterEnabled && mAxisLineEnding == arg.mAxisLineEnding; }
    bool operator !=(const AxisPref & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const AxisPref &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, AxisPref &);


struct LevelPref {
    Label mLabel;
    QPen mPen;

    bool operator ==(const LevelPref & arg) const {  return mLabel == arg.mLabel && mPen == arg.mPen; }
    bool operator !=(const LevelPref & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const LevelPref &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, LevelPref &);





struct ArrowedTextPref {
    QColor mBackgroundColor;
    QCPLineEnding::EndingStyle mArrowEndingStyle;
    QPen mPen;
    Label mLabelPrefs;

    bool operator ==(const ArrowedTextPref & arg) const {  return mBackgroundColor == arg.mBackgroundColor && mArrowEndingStyle == arg.mArrowEndingStyle && mPen == arg.mPen && mLabelPrefs == arg.mLabelPrefs; }
    bool operator !=(const ArrowedTextPref & arg) const {  return !(*this==arg); }
};
void saveInSettings(QSettings& settingsObj, const QString &name, const ArrowedTextPref &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, ArrowedTextPref &);





} //namespace style






class PlotStyle
{
public:
    PlotStyle();

    static QVector<PlotStyle> sSystemPlotStyles;
    static QVector<PlotStyle> sUserPlotStyles;

    static void initializeDefaultPlotStyles();
    static const PlotStyle& defaultPlotStyle();
    static void loadUserStylesFromSettings(QSettings& settingsObj);
    static void saveUserStylesInSettings(QSettings& settingsObj);
    static void setUserDefinedPlotStyles(QVector<PlotStyle> newStyles);
    static void setDefaultPlotStyle(const QString& mStyleName);

    bool operator==(const PlotStyle & arg) const {
        if (mStyleName == arg.mStyleName
                && mXTickLabelProperties == arg.mXTickLabelProperties
                && mYTickLabelProperties == arg.mYTickLabelProperties
                && mEventProperties == arg.mEventProperties
                && mHorLevelProperties == arg.mHorLevelProperties
                && mArrowedTextProperties == arg.mArrowedTextProperties
                && mXTickRotation == arg.mXTickRotation
                && mEventLabelRotation == arg.mEventLabelRotation
                && mMargins == arg.mMargins

                && mXGridProperties == arg.mXGridProperties
                && mYGridProperties == arg.mYGridProperties
                && mXSubGridProperties == arg.mXSubGridProperties
                && mYSubGridProperties == arg.mYSubGridProperties
                && mTitleProperties == arg.mTitleProperties
                && mAxisRectProperties == arg.mAxisRectProperties

                && mSubTickProperties == arg.mSubTickProperties
                && mTickProperties == arg.mTickProperties
                && mXLabelProperties == arg.mXLabelProperties
                && mYLabelProperties == arg.mYLabelProperties
                && mColorPref == arg.mColorPref
                && mLegendProperties == arg.mLegendProperties
                && mXAxisPref == arg.mXAxisPref
                && mYAxisPref == arg.mYAxisPref

                )
            return true;
        else
            return false;
    }

    bool operator!=(const PlotStyle & arg) const {
        return !(*this==arg);
    }

    QString mStyleName;
    style::Label mXTickLabelProperties;
    style::Label mYTickLabelProperties;
    style::LevelPref mEventProperties;
    style::LevelPref mHorLevelProperties;
    style::ArrowedTextPref mArrowedTextProperties;
    double mXTickRotation;
    double mEventLabelRotation;
    QMargins mMargins;

    style::Grid mXGridProperties;
    style::Grid mYGridProperties;
    style::Grid mXSubGridProperties;
    style::Grid mYSubGridProperties;
    style::Label mTitleProperties;
    style::AxisRect mAxisRectProperties;

    style::Tick mSubTickProperties;
    style::MainTick mTickProperties;
    style::Label mXLabelProperties;
    style::Label mYLabelProperties;
    style::ColorPref mColorPref;
    style::LegendPref mLegendProperties;
    style::AxisPref mXAxisPref;
    style::AxisPref mYAxisPref;

//X macros
#define PLOT_STYLE_PARMETERS_LIST_MACRO \
    X(mStyleName)\
    X(mXTickLabelProperties)\
    X(mYTickLabelProperties)\
    X(mEventProperties)\
    X(mHorLevelProperties)\
    X(mArrowedTextProperties)\
    X(mXTickRotation)\
    X(mEventLabelRotation)\
    X(mMargins)\
    X(mXGridProperties)\
    X(mYGridProperties)\
    X(mXSubGridProperties)\
    X(mYSubGridProperties)\
    X(mTitleProperties)\
    X(mAxisRectProperties)\
    X(mSubTickProperties)\
    X(mTickProperties)\
    X(mXLabelProperties)\
    X(mYLabelProperties)\
    X(mColorPref)\
    X(mLegendProperties)\
    X(mXAxisPref)\
    X(mYAxisPref)

private:
    static QString sDefaultPlotStyleName;

};

void saveInSettings(QSettings& settingsObj, const QString &name, const PlotStyle &);
bool loadFromSettings(QSettings& settingsObj, const QString &name, PlotStyle &);

#endif // PLOTSTYLE_H
