TARGET = jag
QT += network
QT += widgets
QT += printsupport

QMAKE_CXXFLAGS += -std=gnu++11 -Wall -Wextra -Werror

POLICY = GPLv3
DEFINES += $$POLICY

CONFIG += c++14

LIBS += -lm -ldl -lboost_system -lboost_filesystem

#CONFIG(release,debug|release){
#    LIBS += -lQSCore
#    LIBS += -lQSCharts
#}
#CONFIG(debug,debug|release){
#    LIBS +=  -lQSCored
#    LIBS +=  -lQSChartsd
#    #QMAKE_LFLAGS +=  -fsanitize=address
#   # QMAKE_CXXFLAGS += -fsanitize=address
#}

include(./commonWidgets/widgettilemanager/widgettilemanager.pri)
include(./commonWidgets/qsint/qsint.pri)

TRANSLATIONS += JaG_ru.ts

SOURCES += \
    main.cpp \
    commonWidgets/graphmodelinfowidget.cpp \
    core/qcustomplot.cpp \
    core/plotwidget.cpp \
    datasource/datasource.cpp \
    datasource/dummydatasource.cpp \
    basic/basic.cpp \
    builder/csv_builder.cpp \
    builder/infobuilder.cpp \
    builder/nullbuilder.cpp \
    data/data_attribute.cpp \
    filter/decoratorfilter.cpp \
    filter/nameequalityfilter.cpp \
    filter/nameregexfilter.cpp \
    filter/nullfilter.cpp \
    filter/parameternamefilter.cpp \
    filter/timefilter.cpp \
    basic/bassic.cpp \
    builder/proxybuilder.cpp \
    core/jagmainwindow.cpp \
    basic/selectionframe.cpp \
    dataconverter/dataconverter.cpp \
    dataconverter/identityconverter.cpp \
    dataconverter/affineconverter.cpp \
    dataconverter/borderfilter.cpp \
    dataconverter/windowfilter.cpp \
    dataconverter/filtercontrollerwidget.cpp \
    dataconverter/marginfiltercontrollerwidget.cpp \
    dataconverter/windowfiltercontrollerwidget.cpp \
    dataconverter/filterchoisedialog.cpp \
    datasource/csvdatasource.cpp \
    director/csv_director.cpp \
    datasource/datasourcebasic.cpp \
    global/global_definitions.cpp \
    datasource/dummylineedit.cpp \
    secondaryprocessing/secondaryprocessing.cpp \
    secondaryprocessing/datacontainer.cpp \
    secondaryprocessing/secondaryprocessingdialog.cpp \
    secondaryprocessing/fft/my_fft_api.cpp \
    dimensiondialog/defaultdimensionwidget.cpp \
    dataconverter/medianwindowfilter.cpp \
    dataconverter/medianfiltercontrollerwidget.cpp \
    datasource/datasourcesender.cpp \
    basic/messagelabel.cpp \
    fullScreen/QFullScreenAdapter.cpp \
    fullScreen/dummywidget.cpp \
    event/timeevent.cpp \
    datasource/datasourceadapter.cpp \
    datasource/abstractdatasourceadapter.cpp \
    graph/graphiconengine.cpp \
    datasource/simpledatasource.cpp \
    secondaryprocessing/functiondescription.cpp \
    commonWidgets/aboutjag.cpp \
    commonWidgets/comboboxwithtips.cpp \
    secondaryprocessing/special_functions/us_stand_atm1976.cpp \
    datasource/datasourcecontrolwidget.cpp \
    datasource/datasourcecontroldialog.cpp \
    secondaryprocessing/special_functions/svrg_coorder.cpp \
    secondaryprocessing/special_functions/AtmGost4401_81.cpp \
    anchor/anchor.cpp \
    anchor/anchorcontrolwidget.cpp \
    anchor/dockanchorviewwidget.cpp \
    anchor/anchortablewidget.cpp \
    commonWidgets/ipushbutton.cpp \
    datasource/graphstyle.cpp \
    dataconverter/abstractfiltercontainer.cpp \
    dataconverter/graphmodelfiltercontainer.cpp \
    dataconverter/datasourcefiltercontainer.cpp \
    controlwrappers/axiscontrolwidgetwrapper.cpp \
    controlwrappers/anchorcontrolwidgetwrapper.cpp \
    controlwrappers/plotcontrolwidgetwrapper.cpp \
    controlwrappers/multimodelcontrolwidgetwrapper.cpp \
    commonWidgets/bitchoisedialog.cpp \
    commonWidgets/noncriticalerrorsdialogwithinfo.cpp \
    director/director.cpp \
    tablePrinter/tableprinter.cpp \
    core/graphcontainer.cpp \
    dataconverter/clusterfilter.cpp \
    dataconverter/clusterfiltercontrollerwidget.cpp \
    director/tabledirector.cpp \
    datasource/tabledatasource.cpp \
    controlWidgets/axiscontrolwidget.cpp \
    controlWidgets/multimodelcontrolwidget.cpp \
    commonWidgets/plotshoweditwidget.cpp \
    controlWidgets/plotcontrolwidget.cpp \
    core/axismodel.cpp \
    core/axisstate.cpp \
    controlWidgets/filtercontainerwidget.cpp \
    commonWidgets/tipdialog.cpp \
    core/plotlayoutmodel.cpp \
    controlWidgets/plothierarchywidget.cpp \
    core/graphmodel.cpp \
    commonWidgets/mytabwidget.cpp \
    core/arrowedtext.cpp \
    controlWidgets/arrowedtextcontrolwidget.cpp \
    controlwrappers/arrowedtextcontrolwidgetwrapper.cpp \
    core/plotwidgetstate/plotwidgetstate.cpp \
    core/plotwidgetstate/plotwidgetmeasuringstate.cpp \
    core/plotwidgetstate/plotwidgetmotionstate.cpp \
    core/plotwidgetstate/plotwidgetxzoomstate.cpp \
    core/plotwidgetstate/plotwidgetyzoomstate.cpp \
    core/plotwidgetstate/plotwidgetxyzoomstate.cpp \
    core/plotwidgetstate/plotwidgetnormalstate.cpp \
    core/plotwidgetstate/plotwidgetarrowdrawingstate.cpp \
    event/eventsource.cpp \
    event/eventsourceadapter.cpp \
    controlWidgets/eventcontrolwidget.cpp \
    controlwrappers/eventcontrolwidgetwrapper.cpp \
    commonWidgets/colorchoisewidget.cpp \
    commonWidgets/anycombobox.cpp \
    datasource/datachoicedialog.cpp \
    datasource/datachoicewidget.cpp \
    datasource/datachoicewidgetwrapper.cpp \
    controlwrappers/controlwidgetwrapper.cpp \
    commonWidgets/imagechoice/imagechoicewidget.cpp \
    commonWidgets/imagechoice/scattericonchoicedialog.cpp \
    commonWidgets/imagechoice/brushpatternchoicedialog.cpp \
    controlwrappers/widgetwrapperwithheader.cpp \
    datasource/textdatasourcedialog.cpp \
    commonWidgets/colorlistchoicewidget.cpp \
    core/plotwidgetstate/plotwidgetfastsecondaryprocessingstate.cpp \
    core/plotwidgetstate/plotwidgetleveldrawingstate.cpp \
    core/horizontallevel.cpp \
    controlWidgets/horizontallevelcontrolwidget.cpp \
    controlwrappers/horizontallevelcontrolwidgetwrapper.cpp \
    commonWidgets/plotwidgetwithtoolbar.cpp \
    commonWidgets/tilemanagerwithtoolbar.cpp \
    commonWidgets/legendlayoutchoice/legendlayoutchoicewidget.cpp \
    commonWidgets/legendlayoutchoice/legendlayoutchoicecombobox.cpp \
    commonWidgets/treewidgetwithbuttonreleaseinfo.cpp \
    core/colormap/colormapevaluator.cpp \
    core/colormap/dummycolormapevaluator.cpp \
    latex2html/latex2html.cpp \
    basic/htmldrawing.cpp \
    basic/utility.cpp \
    commonWidgets/colorslistchoicewidget.cpp \
    commonWidgets/texteditdialog.cpp \
    commonWidgets/lineeditwithunicodedialog.cpp \
    commonWidgets/fastsecondaryprocessingdialog.cpp \
    commonWidgets/fastsecprocrequestvectoreditor.cpp \
    utility/settings.cpp \
    commonWidgets/penchoicewidget.cpp \
    controlwrappers/extendedcontrolwrapperdialog.cpp \
    controlWidgets/controlwidget.cpp \
    controlWidgets/fastentoolbutton.cpp \
    commonWidgets/editingWidgets/grideditingwidget.cpp \
    commonWidgets/editingWidgets/titleeditingwidget.cpp \
    commonWidgets/editingWidgets/axisrecteditingwidget.cpp \
    commonWidgets/editingWidgets/axisrectpainter.cpp \
    commonWidgets/editingWidgets/tickeditingwidget.cpp \
    commonWidgets/positionchoicewidget.cpp \
    commonWidgets/editingWidgets/colorseteditingwidget.cpp \
    dataconverter/akima.cpp \
    plotStyle/plotstyle.cpp \
    plotStyle/plotstyleeditingwidget.cpp \
    commonWidgets/editingWidgets/marginseditingwidget.cpp \
    commonWidgets/fontwidet.cpp \
    commonWidgets/editingWidgets/legendeditingwidget.cpp \
    commonWidgets/editingWidgets/labelprefseditingwidget.cpp \
    commonWidgets/editingWidgets/axisprefeditingwidget.cpp \
    commonWidgets/plotstylechoosingwidget.cpp \
    plotStyle/defaultplotsstyles.cpp \
    commonWidgets/plotpreferencesdialog.cpp \
    core/plotpreferences.cpp \
    commonWidgets/editingWidgets/levelprefseditingwidget.cpp \
    commonWidgets/editingWidgets/arrowedtextprefseditingwidget.cpp \
    datasource/plugindatasource.cpp \
    plugins/pluginlibrary.cpp \
    commonWidgets/pluginlistvectoreditor.cpp




HEADERS += \
    commonWidgets/graphmodelinfowidget.h \
    core/qcustomplot.h \
    core/plotwidget.h \
    datasource/datasource.h \
    datasource/dummydatasource.h \
    basic/basic.h \
    basic/global_definitions.h \
    basic/minicsv.h \
    builder/builder.h \
    builder/csv_builder.h \
    builder/infobuilder.h \
    builder/nullbuilder.h \
    data/data_attribute.h \
    data/data_sample.h \
    data/data_sample_types.h \
    director/director.h \
    filter/decoratorfilter.h \
    filter/filter.h \
    filter/nameequalityfilter.h \
    filter/nameregexfilter.h \
    filter/nullfilter.h \
    filter/parameternamefilter.h \
    filter/timefilter.h \
    basic/bassic.h \
    builder/proxybuilder.h \
    core/jagmainwindow.h \
    basic/selectionframe.h \
    dataconverter/dataconverter.h \
    dataconverter/identityconverter.h \
    dataconverter/affineconverter.h \
    dataconverter/borderfilter.h \
    dataconverter/windowfilter.h \
    dataconverter/filtercontrollerwidget.h \
    dataconverter/marginfiltercontrollerwidget.h \
    dataconverter/windowfiltercontrollerwidget.h \
    dataconverter/filterchoisedialog.h \
    datasource/csvdatasource.h \
    director/csv_director.h \
    datasource/datasourcebasic.h \
    global/global_definitions.h \
    datasource/dummylineedit.h \
    secondaryprocessing/datacontainer.h \
    secondaryprocessing/secondaryprocessingdialog.h \
    secondaryprocessing/fft/my_fft_api.h \
    dimensiondialog/defaultdimensionwidget.h \
    dataconverter/medianwindowfilter.h \
    dataconverter/medianfiltercontrollerwidget.h \
    datasource/datasourcesender.h \
    basic/messagelabel.h \
    fullScreen/QFullScreenAdapter.h \
    fullScreen/dummywidget.h \
    datasource/datasourceadapter.h \
    datasource/abstractdatasourceadapter.h \
    graph/graphiconengine.h \
    datasource/simpledatasource.h \
    secondaryprocessing/functiondescription.h \
    commonWidgets/aboutjag.h \
    commonWidgets/comboboxwithtips.h \
    secondaryprocessing/special_functions/us_stand_atm1976.h \
    datasource/datasourcecontrolwidget.h \
    datasource/datasourcecontroldialog.h \
    secondaryprocessing/special_functions/svrg_coorder.h \
    secondaryprocessing/special_functions/AtmGost4401_81.h \
    anchor/anchor.h \
    anchor/anchorcontrolwidget.h \
    anchor/dockanchorviewwidget.h \
    anchor/anchortablewidget.h \
    commonWidgets/ipushbutton.h \
    datasource/graphstyle.h \
    dataconverter/abstractfiltercontainer.h \
    dataconverter/graphmodelfiltercontainer.h \
    dataconverter/datasourcefiltercontainer.h \
    controlwrappers/axiscontrolwidgetwrapper.h \
    controlwrappers/anchorcontrolwidgetwrapper.h \
    controlwrappers/plotcontrolwidgetwrapper.h \
    controlwrappers/multimodelcontrolwidgetwrapper.h \
    commonWidgets/bitchoisedialog.h \
    commonWidgets/noncriticalerrorsdialogwithinfo.h \
    tablePrinter/tableprinter.h \
    core/graphcontainer.h \
    dataconverter/clusterfilter.h \
    dataconverter/clusterfiltercontrollerwidget.h \
    director/tabledirector.h \
    datasource/tabledatasource.h \
    controlWidgets/axiscontrolwidget.h \
    controlWidgets/multimodelcontrolwidget.h \
    commonWidgets/plotshoweditwidget.h \
    controlWidgets/plotcontrolwidget.h \
    core/axismodel.h \
    core/axisstate.h \
    controlWidgets/filtercontainerwidget.h \
    commonWidgets/preferencesdialog.h \
    commonWidgets/tipdialog.h \
    core/plotlayoutmodel.h \
    controlWidgets/plothierarchywidget.h \
    core/graphmodel.h \
    commonWidgets/mytabwidget.h \
    core/arrowedtext.h \
    controlWidgets/arrowedtextcontrolwidget.h \
    controlwrappers/arrowedtextcontrolwidgetwrapper.h \
    core/plotwidgetstate/plotwidgetstate.h \
    core/plotwidgetstate/plotwidgetmeasuringstate.h \
    core/plotwidgetstate/plotwidgetmotionstate.h \
    core/plotwidgetstate/plotwidgetxzoomstate.h \
    core/plotwidgetstate/plotwidgetyzoomstate.h \
    core/plotwidgetstate/plotwidgetxyzoomstate.h \
    core/plotwidgetstate/plotwidgetnormalstate.h \
    core/plotwidgetstate/plotwidgetarrowdrawingstate.h \
    event/timeevent.h \
    event/eventsource.h \
    event/eventsourceadapter.h \
    controlWidgets/eventcontrolwidget.h \
    controlwrappers/eventcontrolwidgetwrapper.h \
    commonWidgets/colorchoisewidget.h \
    commonWidgets/anycombobox.h \
    datasource/datachoicedialog.h \
    datasource/datachoicewidget.h \
    datasource/datachoicewidgetwrapper.h \
    controlwrappers/controlwidgetwrapper.h \
    commonWidgets/imagechoice/imagechoicewidget.h \
    commonWidgets/imagechoice/scattericonchoicedialog.h \
    commonWidgets/imagechoice/brushpatternchoicedialog.h \
    controlwrappers/widgetwrapperwithheader.h \
    datasource/textdatasourcedialog.h \
    commonWidgets/colorlistchoicewidget.h \
    secondaryprocessing/fft/ffft/FFTReal.h \
    secondaryprocessing/fft/ffft/def.h \
    secondaryprocessing/fft/ffft/DynArray.h \
    secondaryprocessing/fft/ffft/DynArray.hpp \
    secondaryprocessing/fft/ffft/FFTReal.hpp \
    secondaryprocessing/fft/ffft/OscSinCos.h \
    secondaryprocessing/fft/ffft/OscSinCos.hpp \
    core/plotwidgetstate/plotwidgetfastsecondaryprocessingstate.h \
    core/plotwidgetstate/plotwidgetleveldrawingstate.h \
    core/horizontallevel.h \
    controlWidgets/horizontallevelcontrolwidget.h \
    controlwrappers/horizontallevelcontrolwidgetwrapper.h \
    commonWidgets/plotwidgetwithtoolbar.h \
    commonWidgets/tilemanagerwithtoolbar.h \
    commonWidgets/legendlayoutchoice/legendlayoutchoicewidget.h \
    commonWidgets/legendlayoutchoice/legendlayoutchoicecombobox.h \
    commonWidgets/treewidgetwithbuttonreleaseinfo.h \
    secondaryprocessing/secondaryprocessing.hpp \
    secondaryprocessing/secondaryprocessing.h \
    core/colormap/colormapevaluator.h \
    core/colormap/dummycolormapevaluator.h \
    latex2html/latex2html.h \
    basic/htmldrawing.h \
    basic/utility.h \
    commonWidgets/colorslistchoicewidget.h \
    commonWidgets/texteditdialog.h \
    commonWidgets/lineeditwithunicodedialog.h \
    commonWidgets/fastsecondaryprocessingdialog.h \
    commonWidgets/fastsecprocrequestvectoreditor.h \
    utility/settings.h \
    commonWidgets/penchoicewidget.h \
    controlwrappers/extendedcontrolwrapperdialog.h \
    controlWidgets/controlwidget.h \
    controlWidgets/fastentoolbutton.h \
    commonWidgets/editingWidgets/grideditingwidget.h \
    commonWidgets/editingWidgets/titleeditingwidget.h \
    commonWidgets/editingWidgets/axisrecteditingwidget.h \
    commonWidgets/editingWidgets/axisrectpainter.h \
    commonWidgets/editingWidgets/tickeditingwidget.h \
    utility/utility.h \
    commonWidgets/positionchoicewidget.h \
    commonWidgets/editingWidgets/colorseteditingwidget.h \
    utility/akima/GteIntpAkimaNonuniform1.h \
    utility/akima/GteIntpAkima1.h \
    dataconverter/akima.h \
    utility/enum.h \
    plotStyle/plotstyle.h \
    plotStyle/plotstyleeditingwidget.h \
    commonWidgets/editingWidgets/marginseditingwidget.h \
    commonWidgets/fontwidet.h \
    commonWidgets/editingWidgets/legendeditingwidget.h \
    commonWidgets/editingWidgets/labelprefseditingwidget.h \
    commonWidgets/editingWidgets/axisprefeditingwidget.h \
    commonWidgets/plotstylechoosingwidget.h \
    core/plotpreferences.h \
    commonWidgets/editingWidgets/levelprefseditingwidget.h \
    commonWidgets/editingWidgets/arrowedtextprefseditingwidget.h \
    plugins/plugins_api.h \
    datasource/plugindatasource.h \
    plugins/pluginlibrary.h \
    commonWidgets/pluginlistvectoreditor.h


FORMS += \
    core/jagmainwindow.ui \
    dataconverter/marginfiltercontrollerwidget.ui \
    dataconverter/windowfiltercontrollerwidget.ui \
    dataconverter/filterchoisedialog.ui \
    secondaryprocessing/secondaryprocessingdialog.ui \
    dimensiondialog/defaultdimensionwidget.ui \
    dataconverter/medianfiltercontrollerwidget.ui \
    commonWidgets/aboutjag.ui \
    datasource/datasourcecontrolwidget.ui \
    datasource/datasourcecontroldialog.ui \
    anchor/anchorcontrolwidget.ui \
    commonWidgets/bitchoisedialog.ui \
    commonWidgets/noncriticalerrorsdialogwithinfo.ui \
    dataconverter/clusterfiltercontrollerwidget.ui \
    controlWidgets/axiscontrolwidget.ui \
    controlWidgets/multimodelcontrolwidget.ui \
    commonWidgets/plotshoweditwidget.ui \
    controlWidgets/plotcontrolwidget.ui \
    controlWidgets/filtercontainerwidget.ui \
    commonWidgets/preferencesdialog.ui \
    commonWidgets/tipdialog.ui \
    controlWidgets/arrowedtextcontrolwidget.ui \
    controlWidgets/eventcontrolwidget.ui \
    commonWidgets/colorchoisewidget.ui \
    datasource/datachoicewidgetwrapper.ui \
    datasource/datachoicedialog.ui \
    datasource/datachoicewidgetdecorator.ui \
    commonWidgets/imagechoice/imagechoicewidget.ui \
    controlwrappers/widgetwrapperwithheader.ui \
    datasource/textdatasourcedialog.ui \
    controlWidgets/horizontallevelcontrolwidget.ui \
    commonWidgets/legendlayoutchoice/legendlayoutchoicewidget.ui \
    commonWidgets/texteditdialog.ui \
    commonWidgets/lineeditwithunicodedialog.ui \
    commonWidgets/penchoicewidget.ui \
    controlwrappers/extendedcontrolwrapperdialog.ui \
    commonWidgets/editingWidgets/grideditingwidget.ui \
    commonWidgets/editingWidgets/titleeditingwidget.ui \
    commonWidgets/editingWidgets/axisrecteditingwidget.ui \
    commonWidgets/editingWidgets/tickeditingwidget.ui \
    commonWidgets/positionchoicewidget.ui \
    commonWidgets/editingWidgets/colorseteditingwidget.ui \
    plotStyle/plotstyleeditingwidget.ui \
    commonWidgets/editingWidgets/marginseditingwidget.ui \
    commonWidgets/fontwidet.ui \
    commonWidgets/editingWidgets/legendeditingwidget.ui \
    commonWidgets/editingWidgets/labelprefseditingwidget.ui \
    commonWidgets/editingWidgets/axisprefeditingwidget.ui \
    commonWidgets/plotstylechoosingwidget.ui \
    commonWidgets/editingWidgets/levelprefseditingwidget.ui \
    commonWidgets/editingWidgets/arrowedtextprefseditingwidget.ui

RESOURCES     = JaG.qrc \
    resources/qdarkstyle/style.qrc \
    resources/darkorangestyle/darkorangestyle.qrc


win32:RC_FILE += JaG.rc




#INCLUDEPATH += qsint/include
#INCLUDEPATH += qsint/src/Charts
#INCLUDEPATH += qsint/src/Core
INCLUDEPATH += builder \
            director \
            datasource
INCLUDEPATH += secondaryprocessing/fft

!contains(POLICY, GPLv3) {
    include(./bin_data_formats/bin_data_formats.pri)
    INCLUDEPATH += bin_data_formats
}

DISTFILES += \
    ../version_history.html










