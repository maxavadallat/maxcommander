
// INCLUDES

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QDebug>

#include "fileutils.h"
#include "fileutilsclient.h"
#include "fileoperations.h"


// IMPLEMENTATION

//==============================================================================
// Constructor
//==============================================================================
FileUtilsClient::FileUtilsClient(QObject* aParent)
    : QObject(aParent)
    , socket(NULL)
    , serverName(DEFAULT_SERVER_NAME)
//    , serverFound(false)
{
    // ...

    qDebug() << "Creating FileUtilClient...done";
}

//==============================================================================
// Create Socket
//==============================================================================
void FileUtilsClient::createSocket()
{
    // Check Socket
    if (!socket) {
        qDebug() << "FileUtilClient::createSocket";

        // Create Socket
        socket = new QLocalSocket();

        // Connect Signals
        connect(socket, SIGNAL(connected()), this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
        connect(socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        connect(socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
        connect(socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
    }
}

//==============================================================================
// Get Server Running State - Trying To Connect
//==============================================================================
bool FileUtilsClient::isServerRunning()
{
    // Init Result
    bool result = false;

    // Try To Connect
    if (connectToServer()) {
        // Set Result
        result = true;
        // Disconnect
        disconnectFromServer();
    }

    return result;
}

//==============================================================================
// Connect To Server
//==============================================================================
bool FileUtilsClient::connectToServer(AdminPassQueryProvider* aQueryProvider, const int& aTimeout)
{
    // Check Socket Connected
    if (socketConnected) {
        return true;
    }

    // Create Socket
    createSocket();

    // Check Socket
    if (socket && !serverName.isEmpty()) {
        // Init Full Server Path
        QString fullServerPath = QString("%1/%2").arg(DEFAULT_SERVER_PIPE_PATH).arg(serverName);

        qDebug() << "FileUtilClient::connectToServer - fullServerPath: " << fullServerPath;

        // Init Repeat
        bool repeat = false;

        do {
            // Connect To Server
            socket->connectToServer(fullServerPath);

            // Check For Timeout And Wait For Connected
            if ((aTimeout > 0) && socket->waitForConnected(aTimeout)) {
                return true;
            }

            qDebug() << "FileUtilClient::connectToServer - UNABLE TO CONNECT";

            // Check Password Query Provider
            if (aQueryProvider) {
                // Get Admin Password - Must Be Done GUI Thread Context
                QString pass = aQueryProvider->launchAdminPassQuery();

                // Try To Launch Server
                repeat = launchServer(pass);

            } else {
                qDebug() << "FileUtilClient::connectToServer - NO ADMIN PASS QUERY PROVIDER";
            }

        } while (repeat);
    }

    return false;
}

//==============================================================================
// Connect To Server
//==============================================================================
bool FileUtilsClient::connectToServer(const QString& aAdminPass, const int& aTimeout)
{
    // Check Socket Connected
    if (socketConnected) {
        return true;
    }

    // Create Socket
    createSocket();

    // Check Socket
    if (socket && !serverName.isEmpty()) {
        // Init Full Server Path
        QString fullServerPath = QString("%1/%2").arg(DEFAULT_SERVER_PIPE_PATH).arg(serverName);

        qDebug() << "FileUtilClient::connectToServer - fullServerPath: " << fullServerPath;

        // Init Repeat
        bool repeat = false;

        do {
            // Connect To Server
            socket->connectToServer(fullServerPath);

            // Check For Timeout And Wait For Connected
            if ((aTimeout > 0) && socket->waitForConnected(aTimeout)) {
                return true;
            }

            qDebug() << "FileUtilClient::connectToServer - UNABLE TO CONNECT";

            // Try To Launch Server
            repeat = launchServer(aAdminPass);

        } while (repeat);
    }

    return false;
}

//==============================================================================
// Disconnect From Server
//==============================================================================
bool FileUtilsClient::disconnectFromServer(const int& aTimeout)
{
    // Check Socket
    if (socket) {
        qDebug() << "FileUtilClient::disconnectFromServer";
        // Abort
        socket->abort();
        // Disconnect From Server
        socket->disconnectFromServer();
        // Check Timeout
        if (aTimeout > 0) {
            // Wait For Disconnected
            return socket->waitForDisconnected(aTimeout);
        }
    }

    return false;
}

//==============================================================================
// Send Message
//==============================================================================
void FileUtilsClient::sendMessage(const QString& aMessage)
{
    // Check Socket
    if (socket) {
        qDebug() << "FileUtilClient::sendMessage - aMessage: " << aMessage;

        // Write Data
        if (socket->write((aMessage + QString("\n")).toLocal8Bit()) < 0) {
            qDebug() << "FileUtilClient::sendMessage - ERROR: " << socket->errorString();
        } else {
            // Wait For Bytes Written
            socket->waitForBytesWritten();
        }
        // Flush
        socket->flush();
    }
}

//==============================================================================
// Send File Operation Entry
//==============================================================================
void FileUtilsClient::sendOperation(const FileOperationEntry& aEntry)
{
    // Get Message
    QString message = aEntry.getOperation();
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Is Dir Readable
//==============================================================================
bool FileUtilsClient::isReadable(const QString& aDirPath)
{
    // Init File Info
    QFileInfo fileInfo(aDirPath);

    return fileInfo.isReadable();
}

//==============================================================================
// Read Dir - Async
//==============================================================================
void FileUtilsClient::readDir(const QString& aDirPath, const int& aSortType, const bool& aReverse, const bool& aHidden, const QStringList& aNameFilters)
{
    // Create Message To Send
    QString message = QString("%1;%2;%3;%4;%5;%6").arg(PARAM_OPERATION_READDIR).arg(aDirPath).arg(aSortType).arg(aReverse).arg(aHidden).arg(aNameFilters.join("$"));
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Scan Dir For Directory Size - Async
//==============================================================================
void FileUtilsClient::scanDir(const QString& aDirPath)
{
    // Create Message To Send
    QString message = QString("%1;%2").arg(PARAM_OPERATION_SIZE).arg(aDirPath);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Create A Dir - Async
//==============================================================================
void FileUtilsClient::createDir(const QString& aDirPath)
{
    // Create Message To Send
    QString message = QString("%1;%2").arg(PARAM_OPERATION_MAKEDIR).arg(aDirPath);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Remove/Delete Single File/Empty Directory
//==============================================================================
void FileUtilsClient::remove(const QString& aFilePath)
{
    // Create Message To Send
    QString message = QString("%1;%2").arg(PARAM_OPERATION_DELETE).arg(aFilePath);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Copy Single File
//==============================================================================
void FileUtilsClient::copyFile(const QString& aSource, const QString& aTarget)
{
    // Create Message To Send
    QString message = QString("%1;%2;%3").arg(PARAM_OPERATION_COPY).arg(aSource).arg(aTarget);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Move Single File
//==============================================================================
void FileUtilsClient::moveFile(const QString& aSource, const QString& aTarget)
{
    // Create Message To Send
    QString message = QString("%1;%2;%3").arg(PARAM_OPERATION_MOVE).arg(aSource).arg(aTarget);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Rename Single File
//==============================================================================
void FileUtilsClient::renameFile(const QString& aSource, const QString& aTarget)
{
    // Create Message To Send
    QString message = QString("%1;%2;%3").arg(PARAM_OPERATION_RENAME).arg(aSource).arg(aTarget);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Search
//==============================================================================
void FileUtilsClient::search(const QString& aDirPath, const QString& aNamePattern, const QString& aContentPattern, const int& aSearchOptions)
{
    // Create Message To Send
    QString message = QString("%1;%2;%3;%4;%5").arg(PARAM_OPERATION_SEARCH).arg(aDirPath).arg(aNamePattern).arg(aContentPattern).arg(aSearchOptions);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Abort Current Operation
//==============================================================================
void FileUtilsClient::abort()
{
    // Create Message To Send
    QString message = QString("%1").arg(PARAM_OPERATION_ABORT);
    // Send Message
    sendMessage(message);
}

//==============================================================================
// Connected Slot
//==============================================================================
void FileUtilsClient::connected()
{
    qDebug() << "FileUtilClient::connected";

    // Set Socket Connected
    socketConnected = true;
}

//==============================================================================
// Disconnected Slot
//==============================================================================
void FileUtilsClient::disconnected()
{
    qDebug() << "FileUtilClient::disconnected";

    // Reset Socket Connected
    socketConnected = false;
}

//==============================================================================
// Error Slot
//==============================================================================
void FileUtilsClient::error(QLocalSocket::LocalSocketError aSocketError)
{
    qDebug() << "FileUtilClient::error - aSocketError: " << aSocketError;

    // Switch Socket Error
    switch (aSocketError) {
        case QLocalSocket::ServerNotFoundError:
            // Reset Socket Connected
            socketConnected = false;
        break;

        default:
            // ...
        break;
    }
}

//==============================================================================
// State Changed Slot
//==============================================================================
void FileUtilsClient::stateChanged(QLocalSocket::LocalSocketState aSocketState)
{
    qDebug() << "FileUtilClient::stateChanged - aSocketState: " << aSocketState;

    // Switch State
    switch (aSocketState) {
        case QLocalSocket::ClosingState:
            // Reset Socket Connected
            socketConnected = false;
        break;

        default:
        break;
    }
}

//==============================================================================
// About To Close Slot
//==============================================================================
void FileUtilsClient::aboutToClose()
{
    qDebug() << "FileUtilClient::aboutToClose";

}

//==============================================================================
// Bytes Written Slot
//==============================================================================
void FileUtilsClient::bytesWritten(qint64 aBytes)
{
    qDebug() << "FileUtilClient::bytesWritten - aBytes: " << aBytes;

}

//==============================================================================
// Read Channel Finished Slot
//==============================================================================
void FileUtilsClient::readChannelFinished()
{
    qDebug() << "FileUtilClient::readChannelFinished";

}

//==============================================================================
// Ready Read Slot
//==============================================================================
void FileUtilsClient::readyRead()
{
    // Check Socket
    if (socket) {
/*
        // Check If Data Available
        if (socket->bytesAvailable() > 0) {
            // Read All Available Data
            QByteArray buf = socket->readAll();

            qDebug() << "FileUtilClient::readyRead - buf: " << buf;

            // ...
        }
*/
        // While Bytes Available Loop
        while (socket->bytesAvailable() > 0) {
            // Read All Data To Buffer
            QByteArray buf = socket->readLine();
            // Check Buf
            if (buf.endsWith('\n')) {
                // Chop New Line Char From The End
                buf.chop(1);
            }

            //qDebug() << "FileUtilClient::readyRead - readLine => buf: " << buf;

            // Parse Line
            parseLine(buf);
        }
    }
}

//==============================================================================
// Delete Socket
//==============================================================================
void FileUtilsClient::deleteSocket()
{
    // Check Socket
    if (socket) {
        qDebug() << "FileUtilClient::deleteSocket";

        // Abort Socket
        socket->abort();

        // Disconnect From Server
        socket->disconnectFromServer();
        // Wait For Disconnected
        //socket->waitForDisconnected();

        // Delete Socket
        delete socket;
        socket = NULL;
    }
}

//==============================================================================
// Launch Server
//==============================================================================
bool FileUtilsClient::launchServer(const QString& aAdminPass)
{
#if defined (Q_OS_UNIX) // Mac & Linux
    // Check Admin Pass
    if (!aAdminPass.isEmpty()) {
        // Start Server
        qDebug() << "FileUtilClient::launchServer - STARTING Server: " << DEFAULT_FULL_SERVER_PATH;

        // Init Command Line
        QString commandLine = QString(DEFAULT_WORKER_PROCESS_EXEC_COMMAND).arg(aAdminPass).arg(QApplication::applicationFilePath());

        // Init Pass Input Shell Script Path
        QString passInput = QString(DEFAULT_WORKER_PROCESS_SHELL_SCRIPT_PATH);

        // Create Shell Script
        if (system((QString(DEFAULT_WORKER_PROCESS_SHELL_SCRIPT_MAKER).arg(commandLine).arg(passInput)).toAscii().data()) != 0) {
            qDebug() << "FileUtilClient::launchServer - ERROR Creating Worker Process Shell Script: " << passInput;

            return false;
        }

        // Start Remote Process Detached Using a Shell Script
        if (!QProcess::startDetached(QString(DEFAULT_WORKER_PROCESS_SHELL_SCRIPT_EXEC_COMMAND).arg(passInput))) {
            qDebug() << "FileUtilClient::launchServer - ERROR executing: " << passInput;

            return false;
        }

        // Remove Shell Script
        if (system((QString(DEFAULT_WORKER_PROCESS_SHELL_SCRIPT_REMOVE_COMMAND).arg(passInput)).toAscii().data()) != 0) {
            qDebug() << "FileUtilClient::launchServer - ERROR Deleting Worker Process Shell Script: " << passInput;
        }
    } else {
        qDebug() << "FileUtilClient::launchServer - EMPTY ADMIN PASS";
    }

    return true;

#else // Q_OS_UNIX

    return false;

#endif // Q_OS_UNIX
}

//==============================================================================
// Parse Line
//==============================================================================
void FileUtilsClient::parseLine(const QString& aLine)
{
    qDebug() << "FileUtilsClient::parseLine - aLine: " << aLine;

    // Handle Server Response/Notification/Message


}

//==============================================================================
// Destructor
//==============================================================================
FileUtilsClient::~FileUtilsClient()
{
    // Delete Socket
    deleteSocket();

    // ...

    qDebug() << "Deleting FileUtilClient...done";
}


// End Of File
