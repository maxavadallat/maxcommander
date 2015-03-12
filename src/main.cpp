
#include <QApplication>
#include <QDebug>

#include "mainwindow.h"
#include "constants.h"


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    // Init Application
    QApplication app(argc, argv);

    // Set Application Name
    app.setApplicationName(DEFAULT_APPLICATION_NAME);
    // Set Organization Name
    app.setOrganizationName(DEFAULT_ORGANIZATION_NAME);
    // Set Organization Domain
    app.setOrganizationDomain(DEFAULT_ORGANIZATION_DOMAIN);

    // Init Main Window
    MainWindow* mainWindow = MainWindow::getInstance();

    // Show Main Window
    mainWindow->showWindow();

    // ...

    // Execute App
    int result = app.exec();

    // Release Main Window Instance
    mainWindow->release();

    return result;
}
