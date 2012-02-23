
// INCLUDES

#include <QtGui/QApplication>

#include "constants.h"
#include "mainwindow.h"

#include "custompanel.h"
#include "customfilelist.h"
#include "customheader.h"
#include "headeritem.h"
#include "headerresizearea.h"
#include "settingswindow.h"
#include "createdirdialog.h"
#include "fileutils.h"




// IMPLEMENTATION

//==============================================================================
// Message Output Handler
//==============================================================================
void messageHandler(QtMsgType aType, const char* aMsg)
{
    Q_UNUSED(aType);
    // Init Output File
    QFile outFile(QDir::homePath() + QString("/log.txt"));
    // Open Output File
    if (outFile.open(QIODevice::Append | QIODevice::Text)) {
        // Init Text Stream
        QTextStream out(&outFile);
        // Write To Text Stream
        //out << QString("%1: %2\n").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(aMsg);
        out << QString("%1\n").arg(aMsg);
        // Close File
        outFile.close();
    }
}


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    // Install Message Handler
    //qInstallMsgHandler(messageHandler);

    // Init Application
    QApplication app(argc, argv);

    //qDebug() << "main - argc: " << argc;

    // Init Parameter List
    QStringList paramList;

    // Get Parameters
    for (int i=0; i<argc; i++) {
        //qDebug() << "main - argv[" << i << "]: " << argv[i];
        // Append Parameter To Parameter List
        paramList << QString(argv[i]);
    }

    // Get Parameter List Count
    int plCount = paramList.count();

    // Init Server Mode
    bool serverMode = false;
    // Init Operation
    int operation = -1;

    // Go Thru Parameters
    for (int n=0; n<plCount; n++) {
        //qDebug() << "main - param[" << n << "]: " << paramList[n];

        // Check If Srver Mode Parameter is Set
        if (paramList[n].indexOf(QString(PARAM_SERVER_MODE)) >= 0) {
            qDebug() << "main - SERVER MODE";
            // Set Server Mode
            serverMode = true;
        }

        // Check If Operation Parameter Is Set
        if (paramList[n].indexOf(QString(PARAM_OPERATION)) >= 0) {
            // Get Operation Parameter
            QString oParam = paramList[n].right(paramList[n].length() - QString(PARAM_OPERATION).length());

            qDebug() << "main - OPERATION: " << oParam;

            if (oParam == QString(PARAM_OPERATION_READDIR)) {
                // Set Operation
                operation = OPERATION_ID_READDIR;
            } else if (oParam == QString(PARAM_OPERATION_SIZE)) {
                // Set Operation
                operation = OPERATION_ID_SIZE;
            } else if (oParam == QString(PARAM_OPERATION_TREE)) {
                // Set Operation
                operation = OPERATION_ID_TREE;
            } else if (oParam == QString(PARAM_OPERATION_COPY)) {
                // Set Operation
                operation = OPERATION_ID_COPY;
            } else if (oParam == QString(PARAM_OPERATION_MOVE)) {
                // Set Operation
                operation = OPERATION_ID_MOVE;
            } else if (oParam == QString(PARAM_OPERATION_DELETE)) {
                // Set Operation
                operation = OPERATION_ID_DELETE;
            } else {
                qDebug() << "main - UNSUPPORTED OPERATION";
            }
        }
    }

    // Check Arguments Count
    if (argc > 1 && serverMode && operation >= 0) {
        // Set Application Name
        app.setApplicationName(DEFAULT_SERVER_NAME);
        // Set Applicaiton Version
        app.setApplicationVersion(APP_VERSION);

        // ...

        qDebug() << "Starting MaxCommander SERVER...";

        // ...

        return 0;
    } else {
        // Set Application Name
        app.setApplicationName(DEFAULT_APPLICATION_NAME);
        // Set Applicaiton Version
        app.setApplicationVersion(APP_VERSION);
        // Set Window Icon
        app.setWindowIcon(QIcon(":mainIcon"));


        // Init Main Window
        MainWindow mw;
        // Show Main Window
        mw.show();


/*
        HeaderItem hi;

        hi.show();
*/
/*
        CustomHeader ch;

        ch.show();
*/

/*
        QImage fileIcon = FileUtils::getFileIconImage(QFileInfo(QString("/Users/Max")), 32, 32);

        qDebug() << "Icon Dev Type: " << fileIcon.devType();

        if (fileIcon.isNull()) {
            qDebug() << "/Users/Max - NULL ICON";
        }
*/
/*
        CustomPanel cp;

        cp.show();

        cp.setPanelName(QString(DEFAULT_FILELIST_PANEL_NAME1));

        cp.loadSettings();
*/
/*
        DirSizeScanner dsScanner;

        dsScanner.scanDirSize(QString("/Users/Max/Temp"));
*/
/*
        SettingsWindow sw;

        sw.show();
*/
/*
        CustomFilelist cfl;

        cfl.show();
*/
/*
        QFileInfo fileInfo(QString("/Users/Max/Dev/MaxCommander/MaxCommander.app"));
        //QFileInfo fileInfo(QString("/Users/Max/Dev/MaxCommander"));

        qDebug() << "File Info - File Name: " << fileInfo.absoluteFilePath();
        qDebug() << "File Info - Bundle: " << fileInfo.isBundle();
        qDebug() << "File Info - Dir: " << fileInfo.isDir();
        qDebug() << "File Info - Executeable: " << fileInfo.isExecutable();

        return 0;
*/
/*
        CreateDirDialog createDirDialog;

        int result = createDirDialog.exec();

        qDebug() << "Create Dir Dialog - result: " << result;
*/

        qDebug() << "Starting MaxCommander...";

        // Exec Application
        int result = app.exec();

        //dsScanner.stop();

        return result;
    }
}
