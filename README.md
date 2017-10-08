# JaG
JaG - Just a Grapher

Jag is a simple grapher which was created for telemetry data analyzing and simple plot creation. JaG is based on QCustomPlot v.1.3.2 (http://www.qcustomplot.com/ ). Some features were not implemented on purpose to keep JaG as simple as possible. As a result **JaG can't plot surfaces, graphs in polar coordinates, financial plots, bar plots etc**.
At the moment JaG supports CSV format and data in table format (see examples in "tlmExamples" directory). If you want JaG to support specific binary formats you can connect with author (seleznevae@protonmail.com)  or implement them yourself.

# Features

1. Arbitrary layout. You can easily change number of graphs and axes, move graphs from one axes box to another, change axes boxes location.
2. Comprehensive dialog to choose parameters from data sources (particularly useful if you have dozens or hundreds of parameters in each data source).
3. Built-in secondary processing which allows you easily perform basic mathematical computations on your data (library of available functions is not very huge at the moment (about 40 functions) but in future it will be significantly expanded). Of course basic mathematical operators (+,-,*,/,^) are available.
4. All usual means to change graph properties, convert plots to images, preferences to customize JaG appearance and behavior, shortcuts for all essential actions.

# Building
To build JaG you will need:

1. Boost 1.58
2. QSint 0.2.2

# Platforms

JaG is based on Boost, Qt and libraries based on Qt. Hence in theory JaG is a cross-platform application. In "bin" directory executable files for Linux x64 and Windows7 x32 are available.

# Languages
At the moment JaG GUI supports English and Russian languages.

# Screenshots

![Basic JaG features](https://github.com/seleznevae/JaG/blob/dev/plotExamples/basic_JaG.png "Basic JaG features")

![Arrowed text in JaG](https://github.com/seleznevae/JaG/blob/dev/plotExamples/Arrowed%20Text%20in%20JaG%20-%20Nice%20'N'%20Easy.png "Arrowed text in JaG")

![JaG_Custom](https://github.com/seleznevae/JaG/blob/dev/plotExamples/JaG_custom.png "JaG custom")





# License
GNU GENERAL PUBLIC LICENSE v3



