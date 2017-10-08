/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** FullScreenAdapter is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#include "QFullScreenAdapter.h"
#include <QDebug>

QFullScreenAdapter::QFullScreenAdapter(QWidget* self) : tmpFullScreenParent_(NULL)
{
    self_ = self;
}

bool QFullScreenAdapter::toogleFullScreen()
{
    // its a top level widget
    if (!self_->parent())
    {
        if (self_->isFullScreen())
            self_->showNormal();
        else
            self_->showFullScreen();
        return true;
    }

    if (!tmpFullScreenParent_)
    {        
        QWidget* wparent = qobject_cast<QWidget*>(self_->parent());
        if (!wparent)
        {
            //cout << __FUNCTION__ << " Parent is not a widget, dont know how to handle it" << endl;
            return false;
        }

        if (!wparent->layout())
        {
            //cout << "Parent has no Layout: " << wparent->objectName() << " " << wparent->isWidgetType () << endl;
            // QSplitter
            if (qobject_cast<QSplitter*>(wparent))
            {
                saveFullScreenParentLayoutIndex_ = qobject_cast<QSplitter*>(wparent)->indexOf(self_);
                //cout << "QSplitter: " << saveFullScreenParentLayoutIndex_ << endl;
            }
            else
            {
                //cout << __FUNCTION__ << " Parent(Layoutless) not supported yet" << endl;
                return false;
            }
        }
        else if (qobject_cast<QBoxLayout*>(wparent->layout()))
        {
            //cout << "QBoxLayout" << endl;
            saveFullScreenParentLayoutIndex_ = wparent->layout()->indexOf(self_);
        }
        else if (qobject_cast<QGridLayout*>(wparent->layout()))
        {
            //cout << "QGridLayout" << endl;
			QGridLayout* lay = qobject_cast<QGridLayout*>(wparent->layout());
			for (int i=0; i<lay->count (); i++)
			{
				saveFullScreenGridRow_ = i/lay->columnCount();
				saveFullScreenGridCol_ = i % lay->columnCount();
				if (lay->itemAtPosition (saveFullScreenGridRow_, saveFullScreenGridCol_)->widget()==self_) break;
			}
			
        }
        else if (qobject_cast<QStackedLayout*>(wparent->layout()))
        {
            //cout << "QStackLayout ... not implemented yet" << endl;
			return false;
        }
        else
        {
            //cout << "Unknown Layout  ... not implemented yet" << endl;
            return false;
        }

        saveEmbeddedParent_ = wparent;

        // create a new Widget and insert the fullscreen widget into
        tmpFullScreenParent_ = new DummyWidget();
        tmpFullScreenParent_->setContentsMargins(0,0,0,0);
        tmpFullScreenParent_->setLayout(new QVBoxLayout());
        tmpFullScreenParent_->layout()->addWidget(self_);
        tmpFullScreenParent_->layout()->setContentsMargins(1,1,1,1);
        tmpFullScreenParent_->showFullScreen();
        tmpFullScreenParent_->releaseKeyboard();//SAE

    }
    else // restore
    {
        //cout << "Restore FullScreen ... embedding ..." << endl;
        self_->setParent(0);

        // QSplitter
        if (qobject_cast<QSplitter*>(saveEmbeddedParent_))
        {
            //cout << "QSplitter" << endl;
            qobject_cast<QSplitter*>(saveEmbeddedParent_)->insertWidget(saveFullScreenParentLayoutIndex_, self_);
        }
        // QBoxLayout
        else if (qobject_cast<QBoxLayout*>(saveEmbeddedParent_->layout()))
        {
            //cout << "QBoxLayout" << endl;
            qobject_cast<QBoxLayout*>(saveEmbeddedParent_->layout())->insertWidget(saveFullScreenParentLayoutIndex_, self_);
        }
        // QGridLayout
        else if (qobject_cast<QGridLayout*>(saveEmbeddedParent_->layout()))
        {
            //cout << "QGridLayout" << endl;
			QGridLayout* gl = qobject_cast<QGridLayout*>(saveEmbeddedParent_->layout());			
			gl->addWidget(self_, saveFullScreenGridRow_, saveFullScreenGridCol_);
        }
        else if (qobject_cast<QStackedLayout*>(saveEmbeddedParent_->layout()))
        {
            //cout << "QStackedLayout" << endl;
        }
        else
        {
            //cout << "Layout or parent type unknown" << endl;
        }
        tmpFullScreenParent_->hide();
        tmpFullScreenParent_->deleteLater();
        tmpFullScreenParent_ = NULL;
		saveFullScreenParentLayoutIndex_ = 0;
    }

    return true;
}

bool QFullScreenAdapter::saveScreenShot()
{
    QPixmap pix = QPixmap::grabWindow(QApplication::desktop()->winId());
    qApp->beep();

    QString fpath = QDir::currentPath() + "/screenShot.png";
    QFileDialog fileDialog(NULL, QObject::tr("Save Image File to ..."));
    fileDialog.setDirectory(fpath);

    QStringList filters;
    filters << (QObject::tr("Image files") + " (*.png *.jpg *.tif *.bmp)")
        << "PNG (*.png)"
        << "JPG (*.jpg)"
        << "TIFF (*.tif)"
        << "BMP (*.bmp)";

    fileDialog.setNameFilters(filters);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if (fileDialog.exec())
    {
        QString fpath = fileDialog.selectedFiles().first();
        //cout << fpath.toStdString() << endl;

        if (fpath.toLower().endsWith(".png") || fpath.toLower().endsWith(".jpg") || fpath.toLower().endsWith(".tif") || fpath.toLower().endsWith(".bmp"))
        {
            // nothing
        }
        else
        {
            QString selFilter;
#ifdef USE_QT5
            selFilter = fileDialog.selectedNameFilter();
#else
//            selFilter = fileDialog.selectedFilter();
            selFilter = fileDialog.selectedNameFilter();
#endif
            if (selFilter == "JPG (*.jpg)")
                fpath.append(".jpg");
            else if (selFilter == "TIFF (*.tif)")
                fpath.append(".tif");
            else if (selFilter == "BMP (*.bmp)")
                fpath.append(".bmp");
            else fpath.append(".png");
        }

        pix.save(fpath);
        return true;
    }
    return false;
}
