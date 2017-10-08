#include "colorbutton.h"
#include "colorgrid.h"
#include "popupwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>


ColorButton::ColorButton(QWidget *parent)
    : QToolButton(parent),
    m_modeLeft(PM_COLORGRID),
    m_modeRight(PM_NONE),
    m_cellSize(12),
    m_colors(0)
{
    setColor(Qt::white);
}

ColorButton::~ColorButton()
{
}

void ColorButton::setColor(const QColor& color)
{
    m_color = color;

    int w = qMax(size().width(), size().height());
    QPixmap pm(w,w);
    drawColorItem(pm, color);
    setIcon(QIcon(pm));

    setText(color.name());
}

void ColorButton::drawColorItem(QPixmap &pm, const QColor& color)
{
    QPainter p(&pm);
    p.setBrush(color);
    p.setPen(palette().color(QPalette::Shadow));
    p.drawRect(pm.rect().adjusted(0,0,-1,-1));
}

void ColorButton::setPickModeLeft(const PickMode& mode)
{
    m_modeLeft = mode;
}

void ColorButton::setPickModeRight(const PickMode& mode)
{
    m_modeRight = mode;
}

void ColorButton::resizeEvent ( QResizeEvent * /*event */)
{
    setColor(m_color);
}

void ColorButton::mousePressEvent ( QMouseEvent * event )
{
    QToolButton::mousePressEvent(event);

    event->accept();
    setDown(false);

    int mod;

    switch (event->button()) {
        case Qt::LeftButton:
            mod = m_modeLeft;
            break;
        case Qt::RightButton:
            mod = m_modeRight;
            break;
        default:
            return;
    }

    switch (mod)
    {
        case PM_COLORDIALOG:
        {
            QColor c = QColorDialog::getColor(m_color, this);
            if (c.isValid()) {
                setColor(c);
                emit colorChanged(c);
            }
        }
        break;

        case PM_COLORGRID:
        {
            ColorGrid *grid = new ColorGrid();
            grid->setPickByDrag(false);
            grid->setAutoSize(true);
            grid->setScheme(m_colors);
            grid->setCellSize(m_cellSize);
            connect(grid, SIGNAL(picked(const QColor &)), this, SLOT(setColor(const QColor&)));
            connect(grid, SIGNAL(picked(const QColor &)), this, SIGNAL(colorChanged(const QColor&)));

            PopupWidget *popup = new PopupWidget();
            popup->setWidget(grid);
            popup->show(mapToGlobal(rect().bottomLeft()));

            connect(grid, SIGNAL(accepted()), popup, SLOT(close()));
        }
        break;

        default:;
    }
}

void ColorButton::setScheme(ColorList *scheme)
{
    m_colors = scheme;
}

void ColorButton::setCellSize(int size)
{
    m_cellSize = size;
}


