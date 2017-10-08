#include <QtGui>
#include <QDir>
#include <QSettings>
#include <QSplashScreen>

#include "basic/bassic.h"
#include "core/jagmainwindow.h"
#include "datasource/datasourcesender.h"
#include "global/global_definitions.h"
#include "commonWidgets/texteditdialog.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (lockUniqueResource()) {

        //organization name must be before settings loading
        QCoreApplication::setOrganizationName("jag");
        QCoreApplication::setApplicationName("jag");
        global::setDefaultPreferences();
        PlotStyle::initializeDefaultPlotStyles();

        {
            QSettings settings;
            QVariant varSetting;
            if ((varSetting = settings.value("/Settings/gRussianLanguage")).isValid()) {
                global::gRussianLanguage = varSetting.toBool();
            }
        }

        QTranslator appTranslator;
        if (global::gRussianLanguage) {
            appTranslator.load(":/JaG_ru");
            app.installTranslator(&appTranslator);
        }


        QFont applFont = QApplication::font();
        applFont.setPointSize(9);
        QApplication::setFont(applFont);

        global::gApplicationStartTime = QDateTime::currentDateTime().toString("ddd d MMMM yyyy (hh_mm_ss)");

        QPixmap pixmap(":/wait");
        QSplashScreen *splashScreen = new QSplashScreen(pixmap,  Qt::WindowStaysOnTopHint);
        splashScreen->show();
        qApp->processEvents();
        JagMainWindow jagMainWindow;
        global::gJagMainWindowPointer = &jagMainWindow;
        QVector<DataSource*> dataSourceVector;
        if (argc > 1) {
            for (int i = 1; i < argc; ++i) {
                jagMainWindow.addDataSource(QString::fromLocal8Bit(argv[i]));
                qApp->processEvents();
            }
        }
        delete splashScreen;
        jagMainWindow.show();
        if (global::gIsBootTipShowingEnabled)
            jagMainWindow.showHelpInformation();
        auto ret = app.exec();
        unlockUniqueResource();
        return ret;
    } else {
        // if no arguments
        if (argc <= 1) {
            return EXIT_SUCCESS;
        }

        QVector<QString> fileNameVector;
        for (int i = 1; i < argc; ++i) {
            if (QDir::isAbsolutePath(QDir::fromNativeSeparators(QString::fromLocal8Bit(argv[i]))))
                fileNameVector.push_back(QString::fromLocal8Bit(argv[i]));
            else {
                fileNameVector.push_back(
                            QDir::toNativeSeparators(QDir().absoluteFilePath(QString::fromLocal8Bit(argv[i])))
                                        );
            }
        }
        DataSourceSender sender(fileNameVector);
        return app.exec();
    }
}


