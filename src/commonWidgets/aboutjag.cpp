#include "aboutjag.h"
#include "ui_aboutjag.h"

#include "basic/global_definitions.h"


AboutJag::AboutJag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutJag)
{
    ui->setupUi(this);
    ui->jagInfoLabel->setText(QString("<h1>JaG - Just a Grapher</h1><h2>version ")+
                              QString::number(JAG_MAJOR_NUMBER) + "." +
                              QString::number(JAG_MINOR_NUMBER) + "." +
                              QString::number(JAG_RELEASE_NUMBER) +
                              "</h2><br> " +
                              "Based on Qt " + QT_VERSION_STR +
                              " (GCC " +  __VERSION__ + ")<br>"
                              " Compiled on  " +  __DATE__ + "<br>"
                              "Copyright(c) 2016 - 2017<br>"
                              "Seleznev Anton<br>"
                              "seleznevae@protonmail.com"
                              );




    ui->librariesTextBrowser->setHtml(

                "<h2> JaG uses third-party libraries and materials which are licensed as follows:</h2>"

                "<h4>QCustomPlot v. 1.3.2</h4>"
                "Copyright (c) 2011-2015 Emanuel Eichhammer<br>"
                "<a href=\"http://www.qcustomplot.com/\">http://www.qcustomplot.com/</a> <br>"
                "License: GNU GPL v.3<br>"

                "<h4>FFTReal v. 2.11</h4>"
                "Copyright (c) Laurent de Soras <ldesoras@club-internet.fr><br>"
                "License: WTFPL v.2<br>"

                "<h4>QSint v. 0.2.2</h4>"
                "Copyright (c) Sintegrial Technologies 2011-2014<br>"
                "<a href=\"http://www.sintegrial.com\">http://www.sintegrial.com</a> <br>"
                "License: GNU LGPL v.3<br>"

                "<h4>QDarkStylesheet</h4>"
                "Copyright (c) Colin Duquesnoy 2013-2014 <a href=\"colin.duquesnoy@gmail.com\">colin.duquesnoy@gmail.com</a> <br>"
                "<a href=\"https://github.com/ColinDuquesnoy/QDarkStyleSheet\">https://github.com/ColinDuquesnoy/QDarkStyleSheet</a> <br>"
                "License: MIT<br>"

                "<h4>Minimalistic CSV Streams 1.7.7</h4>"
                "Copyright (c) Wong Shao Voon 2014 <a href=\"shaovoon@yahoo.com\">shaovoon@yahoo.com</a> <br>"
                "License: MIT<br>"

                "<h4>Geometric Tools Engine 3.5 (Akima interpolation module)</h4>"
                "Copyright (c) 1998-2016, David Eberly, Geometric Tools, Redmond WA 98052 <br>"
                "License: The Boost Software License, Version 1.0.<br>"

                "<h4>QFullScreenAdapter</h4>"
                "Copyright (c) 2011, Richard Steffen, <a href=\"rsteffen@messbild.de\">rsteffen@messbild.de</a> <br>"
                "License: GNU LGPL v.2.1.<br>"

                "<h4>Icons</h4>"
                "Oxygen Team <a href=\"https://github.com/pasnox/oxygen-icons-png\">https://github.com/pasnox/oxygen-icons-png</a> <br>"
                "License: GNU Lesser General Public License<br>"

                "<h4>Icons</h4>"
                "Everaldo / Yellowicon <a href=\"http://www.everaldo.com/crystal/?action=license\">http://www.everaldo.com/crystal/?action=license</a> <br>"
                "License: GNU Lesser General Public License<br>"

                "<h4>Icons</h4>"
                "Blue Bits icons, by Stan Khodjaev and Blueberry Basic Icons <a href=\"http://www.icojam.com/blog/?p=253\">http://www.icojam.com/blog/?p=253</a> <br>"
                "License: Public Domain<br>"

                );



}

AboutJag::~AboutJag()
{
    delete ui;
}


