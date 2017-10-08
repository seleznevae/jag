#ifndef COLORGRID_H
#define COLORGRID_H

#include <QWidget>
typedef QList<QColor> ColorList;

class ColorGrid : public QWidget
{
    Q_OBJECT
public:

    ColorGrid(QWidget *parent = 0);

    virtual ~ColorGrid();

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;


    inline int cellSize() const { return m_cellSize; }
    void setCellSize(int size);
    inline int widthInCells() const { return m_widthInCells; }

    void setWidthInCells(int width);
    int heightInCells() const;
    inline bool autoSize() const { return m_autoSize; }
    void setAutoSize(bool autosize);
    inline const QColor &lastHighlighted() const { return m_hlColor; }
    inline const QColor &lastPicked() const { return m_selColor; }
    inline bool pickByDrag() const { return m_pickDrag; }
    inline void setPickByDrag(bool set) { m_pickDrag = set; }
    inline ColorList* scheme() const { return m_colors; }
    void setScheme(ColorList *scheme);

signals:
    /** Emitted when user moves mouse cursor over the grid or changes selection
        with the cursor keys.

      \a color is the color value under the cursor.
      */
    void highlighted(const QColor &color);
    /** Emitted when user picks a color from the grid.

      \a color is the picked color value.
      */
    void picked(const QColor &color);
    void accepted();
    void rejected();

protected:
    virtual void paintEvent ( QPaintEvent * event );
//    virtual bool event ( QEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void leaveEvent ( QEvent * event );
//    virtual void keyPressEvent ( QKeyEvent * event );

    void redraw();

    int index() const;

    int m_cellSize;
    int m_widthInCells;
    bool m_autoSize;
    int m_row, m_col, m_idx;
    QPixmap m_pix;
    bool m_pickDrag;
//    ClickMode m_clickMode;
    QPoint m_pos;

    QColor m_hlColor, m_selColor;

    ColorList *m_colors;
};

#endif // COLORGRID_H
