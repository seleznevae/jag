#ifndef IPUSHBUTTON_H
#define IPUSHBUTTON_H

#include <QPushButton>
#include <QToolButton>
#include "controlWidgets/plothierarchywidget.h"
#include "global/global_definitions.h"

class PlotHierarchyWidget;


/*!
 \brief Button that is used in PlotHierarchyWidget for killButtons to remember current activated items in PlotHierarchyWidget when
 mouse enter this button.


*/
class IPushButton : public QToolButton//QPushButton
{
    Q_OBJECT
public:
//    explicit IPushButton(QWidget *parent = 0);


    template <typename... Args>
    /**
     * @brief
     *
     * @param da
     * @param t
     * @param args - any valid arguments that can be used for std::vector<float> construction.
     */
    explicit IPushButton(Args&&... args)
        :QToolButton/*QPushButton*/(std::forward<Args>(args)...), mHierarchyWidget(NULL)
    { setIcon(global::getStyledIcon("close")); }

    void setHierarchyWidget(PlotHierarchyWidget *hierarchyWidget);
signals:

protected:

//    void mousePressEvent(QMouseEvent * event);
    void enterEvent ( QEvent * event );

public slots:


private:
    PlotHierarchyWidget *mHierarchyWidget;

};

#endif // IPUSHBUTTON_H
