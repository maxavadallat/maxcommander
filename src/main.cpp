
// INCLUDES

#include <QtGui/QApplication>
#include <QProcess>

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
#include "fileutilsserver.h"
#include "fileutilsclient.h"

#include "copyprogressdialog.h"
#include "deleteprogressdialog.h"
#include "mainqueuedialog.h"


// IMPLEMENTATION

//==============================================================================
// Message Output Handler
//==============================================================================
void messageHandler(QtMsgType aType, const char* aMsg)
{
    Q_UNUSED(aType);
    // Init Output File
    QFile outFile(QDir::homePath() + QString("/mclog.txt"));
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
    //int operation = -1;

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
/*
            if (oParam == QString(PARAM_OPERATION_READDIR)) {
                // Set Operation
                operation = OPERATION_ID_READDIR;
            } else if (oParam == QString(PARAM_OPERATION_SIZE)) {
                // Set Operation
                operation = OPERATION_ID_SIZE;
            } else if (oParam == QString(PARAM_OPERATION_TREE)) {
                // Set Operation
                operation = OPERATION_ID_TREE;
            } else if (oParam == QString(PARAM_OPERATION_MAKEDIR)) {
                // Set Operation
                operation = OPERATION_ID_MAKEDIR;
            } else if (oParam == QString(PARAM_OPERATION_COPY)) {
                // Set Operation
                operation = OPERATION_ID_COPY;
            } else if (oParam == QString(PARAM_OPERATION_MOVE)) {
                // Set Operation
                operation = OPERATION_ID_MOVE;
            } else if (oParam == QString(PARAM_OPERATION_RENAME)) {
                // Set Operation
                operation = OPERATION_ID_RENAME;
            } else if (oParam == QString(PARAM_OPERATION_DELETE)) {
                // Set Operation
                operation = OPERATION_ID_DELETE;
            } else {
                qDebug() << "main - UNSUPPORTED OPERATION";
            }
*/
        }
    }

    // Check Arguments Count
    if (argc > 1 && serverMode /*&& operation >= 0*/) {
        // Set Application Name
        app.setApplicationName(DEFAULT_SERVER_NAME);
        // Set Applicaiton Version
        app.setApplicationVersion(APP_VERSION);

#if defined(Q_OS_MAC)
/*      Temporary Disabled
        // Get Process Number
        ProcessSerialNumber psn = { 0, kCurrentProcess };
        // Transform Process Type
        OSStatus status = TransformProcessType(&psn, kProcessTransformToUIElementApplication);

        qDebug() << "Changing Process psn: " << psn.highLongOfPSN << " " << psn.lowLongOfPSN << "  - status: " << status;
*/
#endif // Q_OS_MAC

        // ...

        qDebug() << "Starting MaxCommander SERVER...";

        // Init Local File Util Server
        FileUtilsServer localServer;

        // Start To Listen
        if (localServer.startListen(DEFAULT_SERVER_NAME)) {

            qDebug() << "MaxCommander SERVER: " << DEFAULT_SERVER_NAME << "listening, starting main loop...";

            // Exec Application In Server Mode
            int result = app.exec();

            qDebug() << "MaxCommander SERVER: " << DEFAULT_SERVER_NAME << "exited from main loop...";

            // Close Local Server
            localServer.close();

            return result;
        } else {
            qDebug() << "MaxCommander SERVER is NOT Listening - errorString: " << localServer.errorString();

            //qDebug() << localServer.fullServerName();
        }

        return -1;

    } else {
        //qDebug() << "\nDebugging MaxCommander...\n";

/*
        // Init Command Line
        QString commandLine = QString(DEFAULT_WORKER_PROCESS_EXEC_COMMAND).arg("Duzzasztott").arg(QApplication::applicationFilePath());

        QProcess p;

        QString passInput = QString("%1/pass.sh").arg(QDir::tempPath());

        qDebug() << "passInput: " << passInput;

        system((QString("echo \"%1\" > %2").arg(commandLine).arg(passInput)).toAscii().data());

        //p.setStandardInputFile(passInput);

        bool res = p.startDetached(QString("/bin/sh %1").arg(passInput));

        //int res = system(commandLine.toAscii().data());

        qDebug() << commandLine << " - started: " << res;
*/
/*
        FileUtilClient fuClient;

        fuClient.connectToServer();

        if (fuClient.waitForConnected()) {

            fuClient.sendMessage(QString("Testing Testing Testing... Hello QLocalSocket World! = )"));

            //sleep(2);

            fuClient.sendMessage(QString("Testing Testing Testing... Next Message = )"));

            //sleep(2);

            fuClient.sendMessage(QString("Testing Testing Testing... Third Message = )"));

            //sleep(2);

            fuClient.sendMessage(QString("Testing Testing Testing... One More Message = )"));
        }

        //sleep(10);

        //fuClient.disconnectFromServer();

        //fuClient.waitForDisconnected();

        QEventLoop el;

        el.exec();

        fuClient.disconnectFromServer();

        //fuClient.waitForDisconnected();

        return 0;
*/
/*
        QFileInfo fileInfo(QString("/.DocumentRevisions-V100/PerUID"));

        qDebug() << "/.DocumentRevisions-V100/PerUID - isReadable: " <<  fileInfo.isReadable();
        qDebug() << "/.DocumentRevisions-V100/PerUID - size: " <<  fileInfo.size();
        qDebug() << "/.DocumentRevisions-V100/PerUID - permissions: " <<  fileInfo.permissions();
        qDebug() << "/.DocumentRevisions-V100/PerUID - isDir: " <<  fileInfo.isDir();
*/

        //qDebug() << "\nDebugging MaxCommander...\n";

        //return 0;

        qDebug() << "System Locale: " << QLocale::system().name();

/*
        //QString fileName1 = QString("/Users/Max/Temp3/Temp4/Top.Gear.S19E04.PROPER.HDTV.X264-FoVBiA.mp4");
        //QString fileName2 = QString("/Users/Max/Temp2/Temp4/Top.Gear.S19E04.PROPER.HDTV.X264-FoVBiA.mp4");
        QString fileName1 = QString("/Users/Max/Temp2/Temp4");
        QString fileName2 = QString("/Users/Max/Temp3/Temp4");

        bool renResult = false;

        if (QFile::exists(fileName1)) {

            renResult = QFile::rename(fileName1, fileName2);

            qDebug() << "rename: " << fileName1 << " -> " << fileName2 << QString(renResult ? "SUCCESS" : "FAILED");

        } else if (QFile::exists(fileName2)) {

            renResult = QFile::rename(fileName2, fileName1);

            qDebug() << "rename: " << fileName2 << " -> " << fileName1 << QString(renResult ? "SUCCESS" : "FAILED");
        }

        return 0;
*/
/*
        QDir volumes(QString(DEFAULT_DRIVES_PATH));

        QStringList veList = volumes.entryList(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);

        int veCount = veList.count();

        qDebug() << "Volumes";

        for (int i=0; i<veCount; ++i) {
            qDebug() << " +-- " << veList[i];
        }

        return 0;
*/
/*
        //QString examplePath = QString("/Volumes/WIN8ENTRPRS");
        QString examplePath = QString("/Users/Max/Temp3/Top.Gear.S19E04.PROPER.HDTV.X264-FoVBiA.mp4");

        qDebug() << examplePath << " - volume: " << FileUtils::extractDriveName(examplePath);

        return 0;
*/
/*
        QString fileName1 = QString("/Users/Max/Temp2/Temp4");
        QString fileName2 = QString("/Users/Max/Temp3/Temp4");


        if (QFile::rename(fileName1, fileName2)) {

            qDebug() << "rename: " << fileName1 << " -> " << fileName2 << " - SUCCESS";

        } else {

            qDebug() << "rename: " << fileName1 << " -> " << fileName2 << " - FAILED";

        }

        return 0;
*/


        // Set Application Name
        app.setApplicationName(DEFAULT_APPLICATION_NAME);
        // Set Applicaiton Version
        app.setApplicationVersion(APP_VERSION);
        // Set Window Icon
        app.setWindowIcon(QIcon(":mainIcon"));


        // Init Main Window
        MainWindow* mw = MainWindow::getInstance();
        // Show Main Window
        mw->show();


        qDebug() << "Starting MaxCommander APPLICATION...";

        // Exec Application
        int result = app.exec();

        // Check Main Window Instance
        if (mw) {
            // Release
            mw->release();
        }

        return result;
    }
}
