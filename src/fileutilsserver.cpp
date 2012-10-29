
// INCLUDES

#include <QDebug>
#include <QDir>

#if defined(Q_OS_UNIX)

#include <sys/stat.h>
#include <errno.h>

#else // Q_OS_UNIX

#endif // Q_OS_UNIX

#include "fileutilsserver.h"
#include "constants.h"


// IMPLEMENTATION





//==============================================================================
// Constructor
//==============================================================================
FileUtilsServerTask::FileUtilsServerTask(FileUtilsSocketHandler* aSocketHandler)
    : QObject(NULL)
    , socketHandler(aSocketHandler)
{
    // Set Auto Deletion
    setAutoDelete(false);
}

//==============================================================================
// Destructor
//==============================================================================
void FileUtilsServerTask::run()
{
    qDebug() << "FileUtilServerTask::run";

    // ...

    qDebug() << "FileUtilServerTask::run...done";

    // Emit Finished Signal
    emit finished();
}

//==============================================================================
// Destructor
//==============================================================================
FileUtilsServerTask::~FileUtilsServerTask()
{
}













//==============================================================================
// Constructor
//==============================================================================
FileUtilsSocketHandler::FileUtilsSocketHandler(QLocalSocket* aSocket, FileUtilsServer* aServer, QObject* aParent)
    : QObject(aParent)
    , socket(aSocket)
    , server(aServer)
{
    qDebug() << "Creating FileUtilSocketHandler...";

    // Connect Signals
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
    connect(socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
    connect(socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));

    // ...

    qDebug() << "Creating FileUtilSocketHandler...done";
}

//==============================================================================
// Connected Slot
//==============================================================================
void FileUtilsSocketHandler::connected()
{
    qDebug() << "FileUtilSocketHandler::connected";

    // ...
}

//==============================================================================
// Disconnected Slot
//==============================================================================
void FileUtilsSocketHandler::disconnected()
{
    qDebug() << "FileUtilSocketHandler::disconnected";

    // Check Server
    if (server) {
/*
        // Connect Signal
        connect(this, SIGNAL(removeFromServer(FileUtilSocketHandler*)), server, SLOT(removeSocketHandler(FileUtilSocketHandler*)));
        // Emit Remove From Server Signal
        emit removeFromServer(this);
        // Disconnect Signal
        disconnect(this, SIGNAL(removeFromServer(FileUtilSocketHandler*)), server, SLOT(removeSocketHandler(FileUtilSocketHandler*)));
*/
        // Remove Socket Handler
        server->removeSocketHandler(this);
    }
}

//==============================================================================
// Error Slot
//==============================================================================
void FileUtilsSocketHandler::error(QLocalSocket::LocalSocketError aSocketError)
{
    // Switch Socket Error
    switch (aSocketError) {
        case QLocalSocket::PeerClosedError:
            qDebug() << "FileUtilSocketHandler::error - DISCONNECTING";

            // ...
        break;

        default:
            qDebug() << "FileUtilSocketHandler::error - aSocketError: " << aSocketError;

            // ...
        break;
    }
}

//==============================================================================
// State Changed Slot
//==============================================================================
void FileUtilsSocketHandler::stateChanged(QLocalSocket::LocalSocketState aSocketState)
{
    qDebug() << "FileUtilSocketHandler::stateChanged - aSocketState: " << aSocketState;

    // ...
}

//==============================================================================
// About To Close Slot
//==============================================================================
void FileUtilsSocketHandler::aboutToClose()
{
    qDebug() << "FileUtilSocketHandler::aboutToClose";

    // ...
}

//==============================================================================
// Bytes Written Slot
//==============================================================================
void FileUtilsSocketHandler::bytesWritten(qint64 aBytes)
{
    qDebug() << "FileUtilSocketHandler::bytesWritten - aBytes: " << aBytes;

    // ...
}

//==============================================================================
// Read Channel Finished Slot
//==============================================================================
void FileUtilsSocketHandler::readChannelFinished()
{
    qDebug() << "FileUtilSocketHandler::readChannelFinished";

    // ...
}

//==============================================================================
// Ready Read Slot
//==============================================================================
void FileUtilsSocketHandler::readyRead()
{
    // Check Socket
    if (socket) {
        // While Bytes Available Loop
        while (socket->bytesAvailable() > 0) {
            // Read All Data To Buffer
            QByteArray buf = socket->readLine();
            // Check Buf
            if (buf.endsWith('\n')) {
                // Chop New Line Char From The End
                buf.chop(1);
            }

            qDebug() << "FileUtilSocketHandler::readyRead - readLine => buf: " << buf;

            // Parse Line
            parseLine(buf);
        }
    }
}

//==============================================================================
// Parse Line
//==============================================================================
void FileUtilsSocketHandler::parseLine(const QString& aLine)
{
    qDebug() << "FileUtilSocketHandler::parseLine - aLine: " << aLine;

    // Get Line Elements Split Line
    QStringList elements = aLine.split(FILE_OPERATIONS_TOKEN_SEPARATOR);

    // Handle Command

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
FileUtilsSocketHandler::~FileUtilsSocketHandler()
{
    // Check Socket
    if (socket) {
        // Abort
        socket->abort();
        // Delete Later
        socket->deleteLater();
        // Delete Socket
        //delete socket;
        //socket = NULL;
    }

    qDebug() << "Deleting FileUtilSocketHandler...done";
}



























//==============================================================================
// Constructor
//==============================================================================
FileUtilsServer::FileUtilsServer(QObject* aParent)
    : QLocalServer(aParent)
{
    // Connect Signals
    connect(this, SIGNAL(newConnection()), this, SLOT(newClientConnection()));

    qDebug() << "Creating FileUtilServer...done";
}

//==============================================================================
// Start To Listen
//==============================================================================
bool FileUtilsServer::startListen(const QString& aServerName)
{
    // Init Full Server Path
    QString fullServerPath = QString("%1/%2").arg(DEFAULT_SERVER_PIPE_PATH).arg(aServerName);
    // Start To listen
    bool result = listen(fullServerPath);

    // Check Result
    if (result) {
        // Get Full Server Name
        QString fsName = fullServerName();

        qDebug() << "FileUtilServer::startListen - fullServerName: " << fsName;

#if defined(Q_OS_UNIX) // Mac And Linux

        // Change File Permissions
        if (chmod(fsName.toAscii().data(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH) != 0) {
            qDebug() << "FileUtilServer::startListen - ERROR CHANGING PIPE PERMISSIONS: " << errno;
        }

#else // Q_OS_UNIX // Wingyoz! = )

        // ...

#endif // Q_OS_UNIX

    }

    return result;
}

//==============================================================================
// New Client Connection Slot
//==============================================================================
void FileUtilsServer::newClientConnection()
{
    // Check If Has Pending Connections
    if (hasPendingConnections()) {
        // Get Next Pending Connection
        QLocalSocket* newSocket = nextPendingConnection();

        // Check New Socket
        if (newSocket) {
            qDebug() << "FileUtilServer::newClientConnection - newSocket: " << newSocket;

            // Create New Socket Handler
            FileUtilsSocketHandler* newSocketHandler = new FileUtilsSocketHandler(newSocket, this);

            // Adding Socket To Socket List
            sockets << newSocketHandler;
        }
    }
}

//==============================================================================
// Remove Socket handler
//==============================================================================
void FileUtilsServer::removeSocketHandler(FileUtilsSocketHandler* aSocketHandler)
{
    // Check Socket Handler
    if (aSocketHandler) {
        qDebug() << "FileUtilServer::removeSocketHandler - aSocketHandler: " << aSocketHandler;

        // Get Number Of Connected Sockets
        int sCount = sockets.count();
        // Go Thru Sockets
        for (int i=sCount-1; i>=0; i--) {
            // Get Socket Handler
            FileUtilsSocketHandler* socketHandler = sockets[i];
            // Check Socket Handler
            if (socketHandler == aSocketHandler) {
                // Remove From Socket handlers List
                sockets.removeAt(i);

                // Delete Socket Handler
                delete aSocketHandler;
                aSocketHandler = NULL;
            }
        }
    }
}

//==============================================================================
// Delete Sockets
//==============================================================================
void FileUtilsServer::deleteSockets()
{
    qDebug() << "FileUtilServer::deleteSockets";

    // Get Number Of Connected Sockets
    int sCount = sockets.count();

    // Go Thru Sockets
    for (int i=sCount-1; i>=0; i--) {
        // Get Socket Handler
        FileUtilsSocketHandler* socketHandler = sockets[i];
        // Check Socket Handler
        if (socketHandler) {
            // Delete Socket Handler
            delete socketHandler;
            socketHandler = NULL;
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
FileUtilsServer::~FileUtilsServer()
{
    // ...

    // Delete Sockets
    deleteSockets();

    qDebug() << "Deleting FileUtilServer...done";
}


// End Of File
