
#include <QApplication>
#include <QDebug>

#include <mcwinterface.h>

#include "mainwindow.h"
#include "utility.h"
#include "constants.h"

//#include "testdialog.h"


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
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


