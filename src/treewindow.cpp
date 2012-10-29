
// INCLUDES

#include <QDebug>
#include <QFile>

#include "treewindow.h"
#include "ui_treewindow.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
TreeWindow::TreeWindow(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TreeWindow)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Set Tree Root
//==============================================================================
void TreeWindow::setRoot(const QString& aRoot)
{
    // Check New Root
    if (QFile::exists(aRoot) && root != aRoot) {
        qDebug() << "TreeWindow::setRoot - aRoot: " << aRoot;

        // Set Root Path
        root = aRoot;
        // Set Window Title
        setWindowTitle(root);

        // ...
    }
}

//==============================================================================
// Destructor
//==============================================================================
TreeWindow::~TreeWindow()
{
    // Delete UI
    delete ui;
}

