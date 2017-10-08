#include "fastentoolbutton.h"
#include "global/global_definitions.h"

FastenToolButton::FastenToolButton(QWidget *parent)
    :QToolButton(parent)
{
    setIcon(QIcon(":/pin"));
    setCheckable(true);
    connect(this, SIGNAL(toggled(bool)),
            this, SLOT(treatToggling(bool)));

//    QString prefix;
//    int i = 1;
//    switch (i)
//    {
//        case 1:  prefix = ":/styles/gray/gray_";
//                 break;
//    }

    if (isChecked())
        setIcon(global::getStyledIcon("pin"));
    else
        setIcon(global::getStyledIcon("unpin"));
}

FastenToolButton::~FastenToolButton()
{

}

void FastenToolButton::treatToggling(bool toggled)
{
    setToolTip(toggled ? "Unpin" : "Pin");

    if (isChecked())
        setIcon(global::getStyledIcon("pin"));
    else
        setIcon(global::getStyledIcon("unpin"));
}
