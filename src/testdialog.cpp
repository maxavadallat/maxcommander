#include <QQmlContext>
#include <QDebug>

#include <mcwinterface.h>

#include "passworddialog.h"
#include "testdialog.h"
#include "ui_testdialog.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
TestDialog::TestDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::TestDialog)
    , clientListModel(new ClientListModel)
    , currentIndex(-1)
{
    // Setup UI
    ui->setupUi(this);
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void TestDialog::init()
{
    // Set Context Properties
    QQmlContext* ctx = ui->testView->rootContext();
    // Set Context Properties - Main Controller
    ctx->setContextProperty(DEFAULT_MAIN_CONTROLLER_NAME, this);
    // Set Context Properties - File List Model
    ctx->setContextProperty(DEFAULT_CLIENT_LIST_MODEL_NAME, clientListModel);

    // Set Resize Mode
    ui->testView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // Set Source
    ui->testView->setSource(QUrl("qrc:/qml/testMain.qml"));

    // Connect Signals
    connect(clientListModel, SIGNAL(clientUpdated()), this, SLOT(clientUpdated()));

    // ...
}

//==============================================================================
// Clear
//==============================================================================
void TestDialog::clear()
{
    // Check Model
    if (clientListModel) {
        // Clear
        clientListModel->clear();
    }

    // Set Current Index
    setCurrentIndex(-1);

    // Configure Buttons
    configureButtons();
}

//==============================================================================
// Shut Down
//==============================================================================
void TestDialog::shutDown()
{
    // Clear
    clear();
}

//==============================================================================
// Update Current Index
//==============================================================================
void TestDialog::updateCurrentIndex()
{
    // Check Model
    if (clientListModel) {
        // Check Current Index
        if (currentIndex > clientListModel->rowCount()-1) {
            // Set Current Index
            setCurrentIndex(clientListModel->rowCount()-1);
        }
    }
}

//==============================================================================
// Configure Buttons
//==============================================================================
void TestDialog::configureButtons()
{
    // Check Model
    if (clientListModel) {
        // Enable/Disable Buttons
        ui->createClientButton->setEnabled(true);

        // Get Test Client
        TestClient* client = clientListModel->getClient(currentIndex);

        // Enable/Disable Buttons
        ui->connectButton->setEnabled(client && !client->isConnected() && !client->isAdminModeOn());
        ui->connectAsRootButton->setEnabled(client && (!client->isConnected() || (client->isConnected() && !client->isAdminModeOn())));
        ui->disconnectButton->setEnabled(client && client->isConnected());

        ui->startButton->setEnabled(client && client->isConnected() && !client->isBusy());
        ui->yesButton->setEnabled(client && client->isConnected() && client->isBusy());
        ui->noButton->setEnabled(client && client->isConnected() && client->isBusy());
        ui->stopButton->setEnabled(client && client->isConnected() && client->isBusy());

        //ui->pauseButton->setEnabled(client && client->isConnected() && client->isBusy() && !client->isSuspended());
        //ui->resumeButton->setEnabled(client && client->isConnected() && client->isBusy() && client->isSuspended());

        ui->clearButton->setEnabled(clientListModel->rowCount() > 0);

    } else {
        // Enable/Disable Buttons
        ui->createClientButton->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->connectAsRootButton->setEnabled(false);
        ui->startButton->setEnabled(false);
        ui->yesButton->setEnabled(false);
        ui->noButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->resumeButton->setEnabled(false);
        ui->disconnectButton->setEnabled(false);
        ui->clearButton->setEnabled(false);
    }
}

//==============================================================================
// Clinet Updated Slot
//==============================================================================
void TestDialog::clientUpdated()
{
    // Configure Buttons
    configureButtons();
}

//==============================================================================
// Get Current Index
//==============================================================================
int TestDialog::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Set Current Index
//==============================================================================
void TestDialog::setCurrentIndex(const int& aIndex)
{
    // Check Current Index
    if (currentIndex != aIndex) {
        // Set Current Index
        currentIndex = aIndex;

        // ...

        // Configure Buttons
        configureButtons();

        // Emit Current Index Changed Signal
        emit currentIndexChanged(currentIndex);
    }
}

//==============================================================================
// Delete Client
//==============================================================================
void TestDialog::deleteClient(const int& aIndex)
{
    // Check Model
    if (clientListModel) {
        // Remove Client
        clientListModel->removeClient(aIndex);
    }
}

//==============================================================================
// On Create Client Button Clicked Slot
//==============================================================================
void TestDialog::on_createClientButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Create Client
        clientListModel->createNewClient();
        // Set Current Index
        setCurrentIndex(clientListModel->rowCount()-1);
    }
}

//==============================================================================
// On Connect Client Button Clicked Slot
//==============================================================================
void TestDialog::on_connectButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->connectClient(currentIndex);
    }
}

//==============================================================================
// Connect As Root Button Clicked Slot
//==============================================================================
void TestDialog::on_connectAsRootButton_clicked()
{
    // Check Model
    if (clientListModel) {

        // Init Root Pass
        QString rootPass = "";

        // Check If Remote File Server Running
        if (!checkRemoteFileServerRunning(DEFAULT_ROOT)) {
            // Init Password Dialog
            PasswordDialog passwordDialog;
            // Exec Password Dialog
            if (passwordDialog.exec()) {
                // Get Root Pass
                rootPass = passwordDialog.getPass().trimmed();

            } else {
                return;
            }
        }

        // Connect As Admin
        clientListModel->connectClientAsAdmin(currentIndex, rootPass);
    }
}

//==============================================================================
// On Start Client Button Clicked Slot
//==============================================================================
void TestDialog::on_startButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->startClient(currentIndex);
    }
}

//==============================================================================
// On Send Yes To Client Button Clicked Slot
//==============================================================================
void TestDialog::on_yesButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Send Response
        clientListModel->sendUserReponse(currentIndex, DEFAULT_CONFIRM_YES);
    }
}

//==============================================================================
// On Send No To Client Button Clicked Slot
//==============================================================================
void TestDialog::on_noButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Send Response
        clientListModel->sendUserReponse(currentIndex, DEFAULT_CONFIRM_NO);
    }
}

//==============================================================================
// On Stop Client Button Clicked Slot
//==============================================================================
void TestDialog::on_stopButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->stopClient(currentIndex);
    }
}

//==============================================================================
// On Pause Client Button Clicked Slot
//==============================================================================
void TestDialog::on_pauseButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->pauseClient(currentIndex);
    }
}

//==============================================================================
// On Resume Client Button Clicked Slot
//==============================================================================
void TestDialog::on_resumeButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->resumeClient(currentIndex);
    }
}

//==============================================================================
// On Disconnect Client Button Clicked Slot
//==============================================================================
void TestDialog::on_disconnectButton_clicked()
{
    // Check Model
    if (clientListModel) {
        // Connect
        clientListModel->disconnectClient(currentIndex);
    }
}

//==============================================================================
// On Clear Clients Button Clicked Slot
//==============================================================================
void TestDialog::on_clearButton_clicked()
{
    // Clear
    clear();
}

//==============================================================================
// Destructor
//==============================================================================
TestDialog::~TestDialog()
{
    // Shut Down
    shutDown();

    // Delete Quick Widget Explicitly to Avoid Crash
    delete ui->testView;

    // Delete UI
    delete ui;

    // Check Model
    if (clientListModel) {
        // Delete Model
        delete clientListModel;
        clientListModel = NULL;
    }
}
























//==============================================================================
// Constructor
//==============================================================================
TestClient::TestClient(QObject* aParent)
    : QObject(aParent)
    , fileUtil(new RemoteFileUtilClient)
{
    // Connect Signals
    connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
    connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
    connect(fileUtil, SIGNAL(clientAdminModeChanged(uint, bool)), this, SLOT(clientAdminModeChanged(uint,bool)));
    connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64)));
    connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
    connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
}

//==============================================================================
// Connect
//==============================================================================
void TestClient::connectClient()
{
    // Check File Util
    if (fileUtil) {
        // Connect To Server
        fileUtil->connectToFileServer();
    }
}

//==============================================================================
// Connect Client As Admin
//==============================================================================
void TestClient::connectClientAsAdmin(const QString& aRootPass)
{
    // Check File Util
    if (fileUtil) {
        // Reconnect As Root
        fileUtil->reconnectAsRoot(aRootPass);
    }
}

//==============================================================================
// Start
//==============================================================================
void TestClient::start()
{
    // Check File Util
    if (fileUtil) {
        // Start Test
        fileUtil->startTestOperation();
    }
}

//==============================================================================
// Abort
//==============================================================================
void TestClient::abort()
{
    // Check File Util
    if (fileUtil) {
        // Stop Test
        fileUtil->stopTestOperation();
    }
}

//==============================================================================
// Suspend
//==============================================================================
void TestClient::suspend()
{
    // Check File Util
    if (fileUtil) {
        // Suspend
        fileUtil->suspend();
    }
}

//==============================================================================
// Resume
//==============================================================================
void TestClient::resume()
{
    // Check File Util
    if (fileUtil) {
        // Resume
        fileUtil->resume();
    }
}

//==============================================================================
// Send Response
//==============================================================================
void TestClient::sendResponse(const int& aResponse)
{
    // Check File Util
    if (fileUtil) {
        // Send Response
        fileUtil->sendUserResponse(aResponse);
    }
}

//==============================================================================
// Disconnect
//==============================================================================
void TestClient::disconnectClient()
{
    // Check File Util
    if (fileUtil) {
        // Disconnect/Close
        fileUtil->close();
    }
}

//==============================================================================
// Get ID
//==============================================================================
unsigned int TestClient::getID()
{
    return fileUtil ? fileUtil->getID() : 0;
}

//==============================================================================
// Is Connected
//==============================================================================
bool TestClient::isConnected()
{
    return fileUtil ? fileUtil->isConnected() : false;
}

//==============================================================================
// Is Busy
//==============================================================================
bool TestClient::isBusy()
{
    return fileUtil ? (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTWaiting || fileUtil->getStatus() == ECSTSuspended)
                    : false;
}

//==============================================================================
// Is Suspended
//==============================================================================
bool TestClient::isSuspended()
{
    return fileUtil ? fileUtil->getStatus() == ECSTSuspended : false;
}

//==============================================================================
// Is Admin Mode On
//==============================================================================
bool TestClient::isAdminModeOn()
{
    return fileUtil ? fileUtil->isAdminModeOn() : false;
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void TestClient::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "TestClient::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // ...

    // Emit Connection Changed Signal
    emit connectionChanged(this, aConnected);
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void TestClient::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "TestClient::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...

    // Emit Status Changed Signal
    emit statusChanged(this, aStatus);
}

//==============================================================================
// Admin Mode Changed Slot
//==============================================================================
void TestClient::clientAdminModeChanged(const unsigned int& aID, const bool& aAdminMode)
{
    qDebug() << "TestClient::clientAdminModeChanged - aID: " << aID << " - aAdminMode: " << aAdminMode;

    // ...

    // Emit Admin Mode Changed Signal
    emit adminModeChanged(this, aAdminMode);
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void TestClient::fileOpStarted(const unsigned int& aID,
                               const QString& aOp,
                               const QString& aPath,
                               const QString& aSource,
                               const QString& aTarget)
{
    qDebug() << "TestClient::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void TestClient::fileOpProgress(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aCurrFilePath,
                                const quint64& aCurrProgress,
                                const quint64& aCurrTotal)
{
    qDebug() << "TestClient::fileOpProgress - aID: " << aID << " - aOp: " << aOp << " - aCurrFilePath: " << aCurrFilePath << " - aCurrProgress: " << aCurrProgress << " - aCurrTotal: " << aCurrTotal;

    // ...

}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void TestClient::fileOpSkipped(const unsigned int& aID,
                               const QString& aOp,
                               const QString& aPath,
                               const QString& aSource,
                               const QString& aTarget)
{
    qDebug() << "TestClient::fileOpSkipped - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void TestClient::fileOpFinished(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget)
{
    qDebug() << "TestClient::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Check Operation
    if (aOp == DEFAULT_OPERATION_TEST) {

    }

    // ...

    // Emit Finished Signal
    emit finished(this);
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void TestClient::fileOpAborted(const unsigned int& aID,
                               const QString& aOp,
                               const QString& aPath,
                               const QString& aSource,
                               const QString& aTarget)
{
    qDebug() << "TestClient::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void TestClient::fileOpError(const unsigned int& aID,
                             const QString& aOp,
                             const QString& aPath,
                             const QString& aSource,
                             const QString& aTarget,
                             const int& aError)
{
    qDebug() << "TestClient::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aError: " << aError;

    // ...

}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void TestClient::fileOpNeedConfirm(const unsigned int& aID,
                                   const QString& aOp,
                                   const int& aCode,
                                   const QString& aPath,
                                   const QString& aSource,
                                   const QString& aTarget)
{
    qDebug() << "TestClient::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aCode: " << aCode;

    // ...

}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void TestClient::fileOpQueueItemFound(const unsigned int& aID,
                                      const QString& aOp,
                                      const QString& aPath,
                                      const QString& aSource,
                                      const QString& aTarget)
{
    qDebug() << "TestClient::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// Destructor
//==============================================================================
TestClient::~TestClient()
{
    // Abort
    abort();

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }
}
























//==============================================================================
// Constructor
//==============================================================================
ClientListModel::ClientListModel(QObject* aParent)
    : QAbstractListModel(aParent)
{

}

//==============================================================================
// Create New Client
//==============================================================================
TestClient* ClientListModel::createNewClient()
{
    // Create New Test Client
    TestClient* newClient = new TestClient();

    // Connect Signals
    connect(newClient, SIGNAL(connectionChanged(TestClient*,bool)), this, SLOT(clientConnectionChanged(TestClient*,bool)));
    connect(newClient, SIGNAL(statusChanged(TestClient*,int)), this, SLOT(clientStatusChanged(TestClient*,int)));
    connect(newClient, SIGNAL(finished(TestClient*)), this, SLOT(clientFinished(TestClient*)));

    // Begin Insert Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Add To Client
    clientList << newClient;

    // End Insert Rows
    endInsertRows();

    return newClient;
}

//==============================================================================
// Get Client ID
//==============================================================================
unsigned int ClientListModel::clientID(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        return clientList[aIndex]->getID();
    }

    return 0;
}

//==============================================================================
// Is Client Connect
//==============================================================================
bool ClientListModel::clientConnected(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        return clientList[aIndex]->isConnected();
    }

    return false;
}

//==============================================================================
// Is Admin Mode On
//==============================================================================
bool ClientListModel::adminModeOn(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        return clientList[aIndex]->isAdminModeOn();
    }

    return false;
}

//==============================================================================
// Connect CLient
//==============================================================================
void ClientListModel::connectClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Connect
        clientList[aIndex]->connectClient();
    }
}

//==============================================================================
// Connect Client In Admin Mode
//==============================================================================
void ClientListModel::connectClientAsAdmin(const int& aIndex, const QString& aRootPass)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Connect
        clientList[aIndex]->connectClientAsAdmin(aRootPass);
    }
}

//==============================================================================
// Disconnect Client
//==============================================================================
void ClientListModel::disconnectClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Disconnect
        clientList[aIndex]->disconnectClient();
    }
}

//==============================================================================
// Start Client
//==============================================================================
void ClientListModel::startClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Start
        clientList[aIndex]->start();
    }
}

//==============================================================================
// Stop Client
//==============================================================================
void ClientListModel::stopClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Stop
        clientList[aIndex]->abort();
    }
}

//==============================================================================
// Send User Response
//==============================================================================
void ClientListModel::sendUserReponse(const int& aIndex, const int& aResponse)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Send Reponse
        clientList[aIndex]->sendResponse(aResponse);
    }
}

//==============================================================================
// Pause Client
//==============================================================================
void ClientListModel::pauseClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Pause/Suspend
        clientList[aIndex]->suspend();
    }
}

//==============================================================================
// Resume Client
//==============================================================================
void ClientListModel::resumeClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        // Resume
        clientList[aIndex]->resume();
    }
}

//==============================================================================
// Get Client
//==============================================================================
TestClient* ClientListModel::getClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {
        return clientList[aIndex];
    }

    return NULL;
}

//==============================================================================
// Remove Client
//==============================================================================
void ClientListModel::removeClient(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < clientList.count()) {

        // Begin Remove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);

        // Get Client
        TestClient* client = clientList.takeAt(aIndex);
        // Delete Client
        delete client;
        client = NULL;

        // End Remove Rows
        endRemoveRows();
    }
}

//==============================================================================
// Clear
//==============================================================================
void ClientListModel::clear()
{
    // Begin Reset Model
    beginResetModel();

    // Loop While Item List Is Not Empty
    while (clientList.count() > 0) {
        // Get Test Client
        TestClient* client = clientList.takeLast();

        // Delete Client
        delete client;
        client = NULL;
    }

    // End Reset Model
    endResetModel();
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> ClientListModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // Client ID
    roles[ClientID]         = "clientID";
    // Client Connected
    roles[ClientConnected]  = "clientConnected";
    // Client Admin
    roles[ClientAdmin]      = "clientAdmin";
    // Client Busy
    roles[ClientBusy]       = "clientBusy";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int ClientListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return clientList.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int ClientListModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 2;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant ClientListModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        // Get Model Item
        TestClient* item = clientList[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ClientID:          return item->getID();
            case ClientConnected:   return item->isConnected();
            case ClientAdmin:       return item->isAdminModeOn();
            case ClientBusy:        return item->isBusy();

            default:
            break;
        }
    }

    return QString("");
}

//==============================================================================
// Set Data
//==============================================================================
bool ClientListModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    Q_UNUSED(aIndex);
    Q_UNUSED(aValue);
    Q_UNUSED(aRole);

    return false;
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void ClientListModel::clientConnectionChanged(TestClient* aClient, const bool& aConnected)
{
    qDebug() << "ClientListModel::clientConnectionChanged - aClient: " << aClient << " - aConnected: " << aConnected;

    // Get Client List Count
    int tclCount = clientList.count();

    // Go Thru Client List
    for (int i=0; i<tclCount; ++i) {
        // Get Client
        TestClient* client = clientList[i];
        // Check Client
        if (client == aClient) {
            // Get Index
            QModelIndex updateIndex = createIndex(i, 0);
            // Emit Data Changed
            emit dataChanged(updateIndex, updateIndex);

            // Emit Client Updated Signal
            emit clientUpdated();
            return;
        }
    }
}

//==============================================================================
// Status Changed SLot
//==============================================================================
void ClientListModel::clientStatusChanged(TestClient* aClient, const int& aStatus)
{
    qDebug() << "ClientListModel::clientStatusChanged - aClient: " << aClient << " - aStatus: " << aStatus;

    // Get Client List Count
    int tclCount = clientList.count();

    // Go Thru Client List
    for (int i=0; i<tclCount; ++i) {
        // Get Client
        TestClient* client = clientList[i];
        // Check Client
        if (client == aClient) {
            // Get Index
            QModelIndex updateIndex = createIndex(i, 0);
            // Emit Data Changed
            emit dataChanged(updateIndex, updateIndex);

            // Emit Client Updated Signal
            emit clientUpdated();
            return;
        }
    }
}

//==============================================================================
// Admin Mode Changed Slot
//==============================================================================
void ClientListModel::clientAdminModeChanged(TestClient* aClient, const bool& aAdminModeIsOn)
{
    qDebug() << "ClientListModel::clientStatusChanged - aClient: " << aClient << " - aAdminModeIsOn: " << aAdminModeIsOn;

    // Get Client List Count
    int tclCount = clientList.count();

    // Go Thru Client List
    for (int i=0; i<tclCount; ++i) {
        // Get Client
        TestClient* client = clientList[i];
        // Check Client
        if (client == aClient) {
            // Get Index
            QModelIndex updateIndex = createIndex(i, 0);
            // Emit Data Changed
            emit dataChanged(updateIndex, updateIndex);

            // Emit Client Updated Signal
            emit clientUpdated();
            return;
        }
    }
}

//==============================================================================
// Client Operation Finished Slot
//==============================================================================
void ClientListModel::clientFinished(TestClient* aClient)
{
    qDebug() << "ClientListModel::clientFinished - aClient: " << aClient;

    // Emit Client Updated Signal
    emit clientUpdated();
}


//==============================================================================
// Destructor
//==============================================================================
ClientListModel::~ClientListModel()
{
    // Clear
    clear();
}



