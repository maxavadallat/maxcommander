
// INCLUDES

#include <QDebug>
#include "headerpopup.h"
#include "ui_headerpopup.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
HeaderPopup::HeaderPopup(QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::HeaderPopup)
{
    // Set Window Modality
    setWindowModality(Qt::ApplicationModal);
    // Set Window Flags
    setWindowFlags(Qt::Popup);

    // Setup UI
    ui->setupUi(this);

    // Set Visibility
    setVisible(false);
}

//==============================================================================
// Popup
//==============================================================================
void HeaderPopup::popup(const QPoint& aPosition)
{
    qDebug() << "HeaderPopup::popup";

    // Set Position
    move(aPosition);

    // Set Visible
    //setVisible(true);

    // Show Settings Window
    show();

    // Execute Event Loop
    //eventLoop.exec(QEventLoop::DialogExec);
}

//==============================================================================
// Set Item Selected
//==============================================================================
void HeaderPopup::setItemSelected(const int& aIndex, const bool& aSelected)
{
    // Check UI
    if (ui) {
        // Switch Index
        switch (aIndex) {
            case 0: {
/*
                // Check File Name Check Box
                if (ui->fileNameCheckBox) {
                    // Set Checked
                    ui->fileNameCheckBox->setChecked(aSelected);
                }
*/
            } break;

            case 1: {
                // Check Extension Check Box
                if (ui->extCheckBox) {
                    // Set Checked
                    ui->extCheckBox->setChecked(aSelected);
                }
            } break;

            case 2: {
                // Check Size Check Box
                if (ui->sizeCheckBox) {
                    // Set Checked
                    ui->sizeCheckBox->setChecked(aSelected);
                }
            } break;

            case 3: {
                // Check Date Check Box
                if (ui->dateCheckBox) {
                    // Set Checked
                    ui->dateCheckBox->setChecked(aSelected);
                }
            } break;

            case 4: {
                // Check Owner Check Box
                if (ui->ownerCheckBox) {
                    // Set Checked
                    ui->ownerCheckBox->setChecked(aSelected);
                }
            } break;

            case 5: {
                // Check Permissions Check Box
                if (ui->permsCheckBox) {
                    // Set Checked
                    ui->permsCheckBox->setChecked(aSelected);
                }
            } break;
        }
    }
}

//==============================================================================
// Get Item Selected
//==============================================================================
bool HeaderPopup::getItemSelected(const int& aIndex)
{
    // Check UI
    if (ui) {
        // Switch Index
        switch (aIndex) {
            case 0: {
                // Check File Name Check Box
                if (ui->fileNameCheckBox) {
                    // Return Checked
                    return ui->fileNameCheckBox->isChecked();
                }
            } break;

            case 1: {
                // Check Extension Check Box
                if (ui->extCheckBox) {
                    // Return Checked
                    return ui->extCheckBox->isChecked();
                }
            } break;

            case 2: {
                // Check Size Check Box
                if (ui->sizeCheckBox) {
                    // Return Checked
                    return ui->sizeCheckBox->isChecked();
                }
            } break;

            case 3: {
                // Check Date Check Box
                if (ui->dateCheckBox) {
                    // Return Checked
                    return ui->dateCheckBox->isChecked();
                }
            } break;

            case 4: {
                // Check Owner Check Box
                if (ui->ownerCheckBox) {
                    // Return Checked
                    return ui->ownerCheckBox->isChecked();
                }
            } break;

            case 5: {
                // Check Permissions Check Box
                if (ui->permsCheckBox) {
                    // Return Checked
                    return ui->permsCheckBox->isChecked();
                }
            } break;
        }
    }

    return false;
}

//==============================================================================
// Close Button Clicked Slot
//==============================================================================
void HeaderPopup::on_closeButton_clicked()
{
    // Close
    close();

    // Exit Event Loop
    eventLoop.exit(0);

    // Emit Close Button Clicked
    emit closeButtonClicked();
}

//==============================================================================
// Key Press Event
//==============================================================================
void HeaderPopup::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (!keyPressed) {
            // Set Key Pressed
            keyPressed = true;

            // ...

            //qDebug() << "HeaderPopup::keyPressEvent - key: " << aEvent->key();
        }
    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void HeaderPopup::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Key Pressed
        if (keyPressed) {
            // Set Key Pressed
            keyPressed = false;
            // Check Key
            if (aEvent->key() == Qt::Key_Escape) {
                // Close
                on_closeButton_clicked();
            } else {
                //qDebug() << "HeaderPopup::keyReleaseEvent - key: " << aEvent->key();

                // ...
            }
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
HeaderPopup::~HeaderPopup()
{
    // Delete UI
    delete ui;
}

