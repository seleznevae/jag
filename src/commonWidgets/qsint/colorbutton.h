#ifndef COLORBUTTON_H
#define COLORBUTTON_H

//#include "colordefs.h"

#include <QToolButton>


typedef QList<QColor> ColorList;

class ColorGrid;

/**
    \brief A tool button to select a color from color dialog or grid.

    \image html ColorButton.png An example of ColorButton

    ColorButton is an extension of QToolButton designed to allow the user
    interactively choose a color from popping up dialog window.

    Depending on PickMode, different kinds of color dialogs will be shown when
    user clicks left or right mouse button over the ColorButton (see setPickModeLeft()
    and setPickModeRight() functions).

    You can modify the outlook of ColorButton by changing appropriate QToolButton
    properties. For instance, show text along with color box (ToolButtonStyle property),
    or show drop-down menu arrow (PopupMode property).

    \image html ColorButtons.png Different types of ColorButton
*/
class ColorButton : public QToolButton
{
    Q_OBJECT

public:
    /// \brief Defines color dialog type.
    enum PickMode {
        /// no dialog
        PM_NONE,
        /// standard system color dialog
        PM_COLORDIALOG,
        /// color grid based dialog
        PM_COLORGRID
    };

    /** Constructor.
      */
    ColorButton(QWidget *parent = 0);
    /** Destructor.
      */
    virtual ~ColorButton();

    /** Returns currently selected color.
      */
    inline const QColor& color() const { return m_color; }

    /** Returns type of color dialog shown on left mouse click (PM_COLORGRID by default).
      \sa setPickModeLeft()
      */
    inline const PickMode& pickModeLeft() const { return m_modeLeft; }
    /** Returns type of color dialog shown on right mouse click (PM_NONE by default).
      \sa pickModeRight()
      */
    inline const PickMode& pickModeRight() const { return m_modeRight; }
    /** Sets type of color dialog shown on left mouse click to \a mode.
      */
    void setPickModeLeft(const PickMode& mode);
    /** Sets type of color dialog shown on right mouse click to \a mode.
      */
    void setPickModeRight(const PickMode& mode);

    /** Returns currently active color scheme (by default, defaultColors() is used).
      \sa setScheme()
      */
    inline ColorList* scheme() const { return m_colors; }
    /** Sets color scheme to \a scheme.
      */
    void setScheme(ColorList *scheme);

    /** Returns size of a color cell in pixels.
      */
    inline int cellSize() const { return m_cellSize; }
    /** Sets size of a color cell in pixels to \a size (must be > 0).
      */
    void setCellSize(int size);

public slots:
    /** Sets current color to \a color.
      */
    void setColor(const QColor& color);

signals:
    /** Emitted when user selects a color from the dialog.

      \a color is the picked color value.
      */
    void colorChanged(const QColor &color);

protected:
    virtual void drawColorItem(QPixmap &pm, const QColor& color);

    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    QColor m_color;
    PickMode m_modeLeft, m_modeRight;

    int m_cellSize;
    ColorList *m_colors;
};

#endif // COLORBUTTON_H
