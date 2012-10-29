#ifndef FILEUTILSERVER_H
#define FILEUTILSERVER_H

// INCLUDES

#include <QLocalServer>
#include <QList>
#include <QThreadPool>
#include <QRunnable>
#include <QWaitCondition>


#include "fileutils.h"


// FORWARD DECLARATIONS

class FileUtilsServer;
class FileUtilsSocketHandler;


// DECLARATIONS








//==============================================================================
//! @class FileUtilsServerTask File Util Server Task/Operation Class
//==============================================================================
class FileUtilsServerTask : public QObject, public QRunnable
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aSocketHandler Parent Socket Handler
    FileUtilsServerTask(FileUtilsSocketHandler* aSocketHandler);

    //! @brief Destructor
    //! @param none
    virtual ~FileUtilsServerTask();

signals:

    //! @brief Finished Signal
    //! @param none
    void finished();

protected: // From QRunnable

    //! @brief Destructor
    //! @param none
    virtual void run();

protected: // Data

    //! Thread Mutex
    QMutex                  mutex;

    //! Error Wait Condition
    QWaitCondition          waitForError;

    //! Confirmation Wait Condition
    QWaitCondition          waitForConfirm;

    //! Socket Handler
    FileUtilsSocketHandler* socketHandler;
};











//==============================================================================
//! @class FileUtilsSocketHandler File Utils Socket Handler Class
//==============================================================================
class FileUtilsSocketHandler : public QObject
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aSocket Socket
    //! @param aServer Server
    //! @param aParent Parent
    FileUtilsSocketHandler(QLocalSocket* aSocket, FileUtilsServer* aServer, QObject* aParent = NULL);

    //! @brief Destructor
    //! @param none
    virtual ~FileUtilsSocketHandler();

signals:

    //! @brief Remove From Server Signal
    //! @param aSelf Self - Socket Handler
    void removeFromServer(FileUtilsSocketHandler* aSelf);

public slots:

protected slots:

    //! @brief Connected Slot
    //! @param none
    void connected();

    //! @brief Disconnected Slot
    //! @param none
    void disconnected();

    //! @brief Error Slot
    //! @param aSocketError Socket Error
    void error(QLocalSocket::LocalSocketError aSocketError);

    //! @brief State Changed Slot
    //! @param aSocketState Socket State
    void stateChanged(QLocalSocket::LocalSocketState aSocketState);

    //! @brief About To Close Slot
    //! @param none
    void aboutToClose();

    //! @brief Bytes Written Slot
    //! @param none
    void bytesWritten(qint64 aBytes);

    //! @brief Read Channel Finished Slot
    //! @param none
    void readChannelFinished();

    //! @brief Ready Read Slot
    //! @param none
    void readyRead();

protected:

    //! @brief Parse Line
    //! @param aLine New Line To Parse
    void parseLine(const QString& aLine);

protected: // Data

    //! Local Socket
    QLocalSocket*   socket;
    //! Server
    FileUtilsServer* server;
    //! Thread Pool
    QThreadPool*    threadPool;
};










//==============================================================================
//! @class FileUtilsServer File Utils Server Class
//==============================================================================
class FileUtilsServer : public QLocalServer
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit FileUtilsServer(QObject* aParent = NULL);

    //! @brief Start To Listen
    //! @param aServerName Server Name
    //! @return true if Successfully Start To Listen
    bool startListen(const QString& aServerName);

    //! @brief Destructor
    //! @param none
    virtual ~FileUtilsServer();

public slots:

    //! @brief Remove Socket Handler Slot
    //! @param aSocketHandler Socket handler
    void removeSocketHandler(FileUtilsSocketHandler* aSocketHandler);

protected slots:

    //! @brief New Client Connection Slot
    //! @param none
    void newClientConnection();

protected:

    //! @brief Delete Sockets
    //! @param none
    void deleteSockets();

protected: // Data

    //! Socket Handlers List
    QList<FileUtilsSocketHandler*> sockets;
};

#endif // FILEUTILSERVER_H
