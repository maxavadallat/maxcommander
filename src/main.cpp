
#include <QApplication>
#include <QDir>
#include <QDebug>

#include <mcwinterface.h>

#include "mainwindow.h"
#include "utility.h"
#include "constants.h"

//#include "testdialog.h"


//==============================================================================
// My Message Handler
//==============================================================================
void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    Q_UNUSED(context);

    // Init Output String
    QString txt;

    // Switch Type
    switch (type) {
        default:
        case QtDebugMsg:    txt = QString("Debug: %1").arg(msg);    break;
        case QtWarningMsg:  txt = QString("Warning: %1").arg(msg);  break;
        case QtCriticalMsg: txt = QString("Critical: %1").arg(msg); break;
        case QtFatalMsg:    txt = QString("Fatal: %1").arg(msg);    break;
    }

    // Init Output File
    QFile outFile(QDir::homePath() + "/log.txt");
    // Open Output File
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        // Init Text Stream
        QTextStream ts(&outFile);
        ts << txt << "\n";
        // Close Output File
        outFile.close();
    }
}

//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
#ifdef FILE_LOG
    qInstallMessageHandler(myMessageHandler);
#endif

    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << "Starting Max Commander...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    // Init Application
    QApplication app(argc, argv);

    // Set Application Name
    app.setApplicationName(DEFAULT_APPLICATION_NAME);
    // Set Organization Name
    app.setOrganizationName(DEFAULT_ORGANIZATION_NAME);
    // Set Organization Domain
    app.setOrganizationDomain(DEFAULT_ORGANIZATION_DOMAIN);

    // Store App Exec Path
    storeAppExecPath(argv[0]);

    // Init Result
    int result = 0;

    // Init Main Window
    MainWindow* mainWindow = MainWindow::getInstance();

    // Show Main Window
    mainWindow->showWindow();

    // Execute App
    result = app.exec();

    // Release Main Window Instance
    mainWindow->release();

//    TestDialog testDialog;
//    testDialog.show();
//    app.exec();

    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << "Exiting Max Commander...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    return result;
}


