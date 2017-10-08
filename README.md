# jag

[![Build Status](https://travis-ci.org/seleznevae/jag.svg?branch=master)](https://travis-ci.org/seleznevae/jag)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub tag](https://img.shields.io/github/tag/expressjs/express.svg)]()

jag - just a grapher

Jag is a simple grapher which was created for telemetry data analyzing and simple plot creation. Jag is based on QCustomPlot v.1.3.2 (http://www.qcustomplot.com/ ). Some features were not implemented on purpose to keep jag as simple as possible. As a result **jag can't plot surfaces, graphs in polar coordinates, financial plots, bar plots etc**.
At the moment jag supports CSV format and data in table format (see examples in "tlmExamples" directory). 

See the [project page](https://seleznevae.github.io/jag.org/) for more information.

# Features

1. Arbitrary layout. You can easily change number of graphs and axes, move graphs from one axes box to another, change axes boxes location.
2. Comprehensive dialog to choose parameters from data sources (particularly useful if you have dozens or hundreds of parameters in each data source).
3. Built-in secondary processing which allows you easily perform basic mathematical computations on your data (library of available functions is not very huge at the moment (about 40 functions) but in future it will be significantly expanded). Of course basic mathematical operators (+,-,*,/,^) are available.
4. All usual means to change graph properties, convert plots to images, preferences to customize JaG appearance and behavior, shortcuts for all essential actions.

# Building
To build jag you will need:

1. Boost 1.58
2. QSint 0.2.2

# Platforms

Jag is based on Boost, Qt and libraries based on Qt. Hence in theory jag is a cross-platform application. 

# Languages
At the moment JaG GUI supports English and Russian languages.

# Plugins 

You can extend the number of supported data formats with plugins. See API in [src/plugins/plugins_api.h](https://github.com/seleznevae/jag/blob/master/src/plugins/plugins_api.h).
See a simple dummy example of such plugin in [pluginExample directory](https://github.com/seleznevae/jag/blob/master/pluginExample/).


# Screenshots

![Basic usage](https://github.com/seleznevae/jag/blob/master/plotExamples/gallery_1.png "Basic usage")

![Events with fill and arrows with comments](https://github.com/seleznevae/jag/blob/master/plotExamples/gallery_2.png "Events with fill and arrows with comments")

![Sophisticated layout with different markers on the graphs](https://github.com/seleznevae/jag/blob/master/plotExamples/gallery_3.png "Sophisticated layout with different markers on the graphs")

# Download
You can download the lates version of jag on the [release page](https://github.com/seleznevae/jag/releases).




# License
GNU GENERAL PUBLIC LICENSE v3



