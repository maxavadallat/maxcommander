
// INCLUDES

#include <QDebug>
#include <QApplication>

#include "helpdialog.h"
#include "ui_helpdialog.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
HelpDialog::HelpDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::HelpDialog)
{
    qDebug() << "Creating HelpDialog...";

    // Setup UI
    ui->setupUi(this);

#if defined(Q_OS_MAC)

    qDebug() << "Creating HelpDialog - applicationPath: " << QApplication::applicationDirPath();

    // Load Help HTML
    ui->helpWebView->setUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + QString("/../../Help/maxcommander.html")));

#elif defined(Q_OS_WIN)

#elif defined(Q_OS_UNIX)

#endif

    qDebug() << "Creating HelpDialog...done";
}

//==============================================================================
// Destructor
//==============================================================================
HelpDialog::~HelpDialog()
{
    delete ui;
}
