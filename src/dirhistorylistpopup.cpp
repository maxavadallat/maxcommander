#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>

#include "dirhistorylistpopup.h"
#include "dirhistorylistmodel.h"
#include "ui_dirhistorylistpopup.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
DirHistoryListPopup::DirHistoryListPopup(DirHistoryListModel* aModel, QWidget* aParent)
    : QWidget(aParent)
    , ui(new Ui::DirHistoryListPopup)
    , historyModel(aModel)
    , currentIndex(-1)
{
    // Setup UI
    ui->setupUi(this);
    // Set Focus Policy
    ui->dirlistWidget->setFocusPolicy(Qt::StrongFocus);

    // Set Window Flag
    //setWindowFlags(Qt::Popup);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // Set Attribute for Transparency
    setAttribute(Qt::WA_TranslucentBackground);

    // Set Resize Mode
    ui->dirlistWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Clear Color for Transparency
    ui->dirlistWidget->setClearColor(QColor(0, 0, 0, 0));

    // Set Context Properties
    QQmlContext* ctx = ui->dirlistWidget->rootContext();
    // Set Context Properties - Controller
    ctx->setContextProperty(DEFAULT_DIR_HISTORY_LIST_CONTROLLER, this);
    // Set Context Properties - Model
    ctx->setContextProperty(DEFAULT_DIR_HISTORY_LIST_MODEL, historyModel);

    // Set Source
    ui->dirlistWidget->setSource(QUrl("qrc:/qml/DirHistory.qml"));
}

//==============================================================================
// Launch Popup
//==============================================================================
void DirHistoryListPopup::launchPopup(const QPoint& aPos, const int& aWidth)
{
    // Get Item Count
    int hmCount = historyModel ? historyModel->rowCount() + 1 : 0;
    // Calculate Popup Height
    int popupHeight = historyModel ? (hmCount * DEFAULT_FILE_LIST_DIR_HISTORY_ITEM_HEIGHT + DEFAULT_FILE_LIST_DIR_HISTORY_RADIUS * 3) : DEFAULT_FILE_LIST_DIR_HISTORY_EMPTY_HEIGHT;
    // Set Geometry
    setGeometry(aPos.x(), aPos.y(), aWidth, popupHeight);

    // Show
    show();
    // Raise
    raise();

    // Set Focus
    ui->dirlistWidget->setFocus();
}

//==============================================================================
// Get Current Index
//==============================================================================
int DirHistoryListPopup::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Set Current Index
//==============================================================================
void DirHistoryListPopup::setCurrentIndex(const int& aCurrentIndex)
{
    // Check Current Index
    if (currentIndex != aCurrentIndex) {
        // Set Current Index
        currentIndex = aCurrentIndex;
        // Emit Signal
        emit currentIndexChanged(currentIndex);
    }
}

//==============================================================================
// Dir History List Item Selected
//==============================================================================
void DirHistoryListPopup::dirHistoryListItemSelected(const int& aIndex)
{
    qDebug() << "DirHistoryListPopup::dirHistoryListItemSelected - aIndex: " << aIndex;

    // Check Index
    if (historyModel) {
        // Emit
        emit dirHistoryItemSelected(historyModel->getItem(aIndex));
    }

    // Close
    close();
}

//==============================================================================
// Dir History Clear Clicked
//==============================================================================
void DirHistoryListPopup::dirHistoryClearClicked()
{
    //qDebug() << "DirHistoryListPopup::dirHistoryClearClicked";

    // Check Index
    if (historyModel) {
        // Clear
        historyModel->clear();
    }

    // Emit Resize Needed
    //emit resizeNeeded();

    // Close
    close();

    // ...
}

//==============================================================================
// Focus In Event
//==============================================================================
void DirHistoryListPopup::focusInEvent(QFocusEvent* aEvent)
{
    Q_UNUSED(aEvent);

    // Check Focus
    if (!ui->dirlistWidget->hasFocus()) {
        // Set Focus
        ui->dirlistWidget->setFocus();
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void DirHistoryListPopup::focusOutEvent(QFocusEvent* aEvent)
{
    Q_UNUSED(aEvent);

    // Close
    // close();

    // ...
}

//==============================================================================
// Key Release Event
//==============================================================================
void DirHistoryListPopup::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Close
                close();
            break;

            case Qt::Key_Enter:
            case Qt::Key_Return:
                // Directory  History List Item Selected
                dirHistoryListItemSelected(currentIndex);
            break;

            default:
            break;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
DirHistoryListPopup::~DirHistoryListPopup()
{
    // Delete Quick Widget
    delete ui->dirlistWidget;

    // Delete UI
    delete ui;

    // ...
}
