/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** FullScreenAdapter is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/


#ifndef QFullScreenAdapter_HH
#define QFullScreenAdapter_HH

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QMdiSubWindow>
#include <QStackedLayout>
#include <QSplitter>
#include <QFileDialog>

#include <iostream>
#include "fullScreen/dummywidget.h"
using namespace std;

class QFullScreenAdapter
{

public:
    /// Constructor
    QFullScreenAdapter(QWidget* self);

    /// toogle fullscreen
    bool toogleFullScreen();

	/// make a screenshot
	bool saveScreenShot();
	
    /// the widget itself
    QWidget* self_;

private:

    /// temporary parent while full screen, to store the widget embedded when fullscreen
//    QWidget* tmpFullScreenParent_;
    DummyWidget* tmpFullScreenParent_;

    /// to store the widget embedded when fullscreen
    QWidget* saveEmbeddedParent_;

    /// the index in the boxlayout
    int saveFullScreenParentLayoutIndex_;

	int saveFullScreenGridRow_;
	int saveFullScreenGridCol_;
};

#endif // QFullScreenAdapter_HH
