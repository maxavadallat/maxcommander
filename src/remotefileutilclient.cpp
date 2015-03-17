#include <QDebug>

#include <mcwinterface.h>

#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
RemoteFileUtilClient::RemoteFileUtilClient(RemoteFileUtilClientObserver* aObserver, QObject* aParent)
    : QObject(aParent)
    , cID(0)
    , client(NULL)
    , observer(aObserver)
    , reconnectAsRoot(false)
    , needReconnect(false)
{
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void RemoteFileUtilClient::init()
{
    // Create Client
    client = new QLocalSocket();

    // Connect Signals
    connect(client, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(client, SIGNAL(aboutToClose()), this, SLOT(socketAboutToClose()));
    connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));
    connect(client, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(socketError(QLocalSocket::LocalSocketError)));
    connect(client, SIGNAL(readChannelFinished()), this, SLOT(socketReadChannelFinished()));
    connect(client, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(client, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(socketStateChanged(QLocalSocket::LocalSocketState)));

    // ...
}

//==============================================================================
// Connect To File Server
//==============================================================================
void RemoteFileUtilClient::connectToFileServer(const bool& asRoot, const QString& aRootPass)
{
    // Check Client
    if (!client) {
        qWarning() << "RemoteFileUtilClient::connectToFileServer - NO CLIENT SOCKET!!";

        return;
    }

    // Check If Server Runnind
    if (!checkFileServerRunning()) {
        qDebug() << "RemoteFileUtilClient::connectToFileServer - File Server Not Running! Starting...";

        // Launch REmote File Server
        startFileServer(asRoot, aRootPass);

    }

    qDebug() << "RemoteFileUtilClient::connectToFileServer";

    // Connect To Server
    client->connectToServer(asRoot ? DEFAULT_SERVER_LISTEN_ROOT_PATH : DEFAULT_SERVER_LISTEN_PATH);
}

//==============================================================================
// Get Dir List
//==============================================================================
void RemoteFileUtilClient::getDirList(const QString& aDirPath, const int& aOptions, const int& aSortFlags)
{

}

//==============================================================================
// Create Directory
//==============================================================================
void RemoteFileUtilClient::createDir(const QString& aDirPath)
{

}

//==============================================================================
// Delete File/Directory
//==============================================================================
void RemoteFileUtilClient::deleteFile(const QString& aFilePath)
{

}

//==============================================================================
// Scan Directory Size
//==============================================================================
void RemoteFileUtilClient::scanDirSize(const QString& aDirPath)
{

}

//==============================================================================
// Scan Directory Tree
//==============================================================================
void RemoteFileUtilClient::scanDirTree(const QString& aDirPath)
{

}

//==============================================================================
// Copy File
//==============================================================================
void RemoteFileUtilClient::copyFile(const QString& aSource, const QString& aTarget)
{

}

//==============================================================================
// Rename/Move File
//==============================================================================
void RemoteFileUtilClient::moveFile(const QString& aSource, const QString& aTarget)
{

}

//==============================================================================
// Search File
//==============================================================================
void RemoteFileUtilClient::searchFile(const QString& aName, const QString& aContent, const int& aOptions)
{

}

//==============================================================================
// Abort Current Operation
//==============================================================================
void RemoteFileUtilClient::abort()
{
    // Check Client
    if (client) {
        qDebug() << "RemoteFileUtilClient::abort - cID: " << cID;

        // Abort
        client->abort();
    }
}

//==============================================================================
// Close
//==============================================================================
void RemoteFileUtilClient::close()
{
    // Abort
    abort();

    qDebug() << "RemoteFileUtilClient::close - cID: " << cID;

    // Check Client
    if (client) {
        // Disconnect From Server
        client->disconnectFromServer();
        // Close Client
        client->close();
    }
}

//==============================================================================
// Execute Shell Command
//==============================================================================
void RemoteFileUtilClient::executeShellCommand(const QString& aCommand, const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::executeShellCommand - cID: " << cID << " - cmd: " << aCommand << " - asRoot: " << asRoot;

    // Exec Shell Command
    execShellCommand(aCommand, asRoot, aRootPass);
}

//==============================================================================
// Launch Server Test
//==============================================================================
void RemoteFileUtilClient::launchServerTest()
{
    qDebug() << "RemoteFileUtilClient::launchServerTest - cID: " << cID;

    // Connect To File Server
    connectToFileServer();
}

//==============================================================================
// Send Test
//==============================================================================
void RemoteFileUtilClient::sendTestResponse()
{
    qDebug() << "RemoteFileUtilClient::sendTestResponse - cID: " << cID;

}

//==============================================================================
// Shut Down
//==============================================================================
void RemoteFileUtilClient::shutDown()
{
    // Close
    close();

    qDebug() << "RemoteFileUtilClient::shutDown - cID: " << cID;
}

//==============================================================================
// Check If File Server Running
//==============================================================================
bool RemoteFileUtilClient::checkFileServerRunning()
{
    return checkRemoteFileServerRunning();
}

//==============================================================================
// Start File Server
//==============================================================================
void RemoteFileUtilClient::startFileServer(const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::startFileServer - cID: " << cID << " - asRoot: " << asRoot;

    // Launch Remote File Sever
    if (launchRemoteFileServer(asRoot, aRootPass) != 0) {
        qDebug() << "RemoteFileUtilClient::startFileServer - cID: " << cID << " - ERROR LAUNCHING FILE SERVER!!";
    }
}

//==============================================================================
// Write Data
//==============================================================================
void RemoteFileUtilClient::writeData(const QByteArray& aData)
{
    // Check Client
    if (!client) {
        qWarning() << "RemoteFileUtilClient::writeData - cID: " << cID << " - NO CLIENT!!";
        return;
    }

    // Check Data
    if (!aData.isNull() && !aData.isEmpty()) {
        qWarning() << "RemoteFileUtilClient::writeData - cID: " << cID << " - length: " << aData.length();
        // Write Data
        client->write(aData);
    }
}

//==============================================================================
// Socket Connected Slot
//==============================================================================
void RemoteFileUtilClient::socketConnected()
{
    qDebug() << "RemoteFileUtilClient::socketConnected - cID: " << cID;

    // ...
}

//==============================================================================
// Socket Disconnected Slot
//==============================================================================
void RemoteFileUtilClient::socketDisconnected()
{
    qDebug() << "RemoteFileUtilClient::socketDisconnected - cID: " << cID;

    // ...
}

//==============================================================================
// Socket Error Slot
//==============================================================================
void RemoteFileUtilClient::socketError(QLocalSocket::LocalSocketError socketError)
{
    qDebug() << "RemoteFileUtilClient::socketError - cID: " << cID << " - socketError: " << socketError << " - error: " << client->errorString();

    // ...
}

//==============================================================================
// Socket State Changed Slot
//==============================================================================
void RemoteFileUtilClient::socketStateChanged(QLocalSocket::LocalSocketState socketState)
{
    qDebug() << "RemoteFileUtilClient::socketStateChanged - cID: " << cID << " - socketState: " << socketState;

    // ...
}

//==============================================================================
// Socket About To Close Slot
//==============================================================================
void RemoteFileUtilClient::socketAboutToClose()
{
    qDebug() << "RemoteFileUtilClient::socketAboutToClose - cID: " << cID;

    // ...
}

//==============================================================================
// Socket Bytes Written Slot
//==============================================================================
void RemoteFileUtilClient::socketBytesWritten(qint64 bytes)
{
    qDebug() << "RemoteFileUtilClient::socketBytesWritten - cID: " << cID << " - bytes: " << bytes;

    // ...
}

//==============================================================================
// Socket Read Channel Finished Slot
//==============================================================================
void RemoteFileUtilClient::socketReadChannelFinished()
{
    qDebug() << "RemoteFileUtilClient::socketReadChannelFinished - cID: " << cID;

}

//==============================================================================
// Socket Ready Read Slot
//==============================================================================
void RemoteFileUtilClient::socketReadyRead()
{
    qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - bytesAvailable: " << client->bytesAvailable();

    // Read Data
    lastBuffer = client->readAll();

    //qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - lastBuffer: " << lastBuffer;

    // Check ID
    if (cID == 0) {
        // Init Result
        bool ok = false;
        // Set ID
        cID = QString(lastBuffer).toULongLong(&ok);

        qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - Client ID is SET!!";
    }

    // ...

}

//==============================================================================
// Destructor
//==============================================================================
RemoteFileUtilClient::~RemoteFileUtilClient()
{
    // Shut Down
    shutDown();

    // Check Client
    if (client) {
        // Delete Socket
        delete client;
        client = NULL;
    }

    qDebug() << "RemoteFileUtilClient::~RemoteFileUtilClient - cID: " << cID;
}

