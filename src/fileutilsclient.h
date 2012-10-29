#ifndef FILEUTILCLIENT_H
#define FILEUTILCLIENT_H

// INCLUDES

#include <QLocalSocket>
#include <QString>

#include "constants.h"


// FORWARD DECLARATIONS

class AdminPassQueryProvider;
class FileOperationEntry;


// DECLARATIONS

//==============================================================================
//! @class FileUtilsClient File Utils Client Class
//!        This class provides client for Remote File Utils Server
//==============================================================================
class FileUtilsClient : public QObject
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    explicit FileUtilsClient(QObject* aParent = NULL);

    //! @brief Get Server Running State - Trying To Connect
    //! @param none
    //! @return true If Server Is Running, false Otherwise
    bool isServerRunning();

    //! @brief Connect To Server
    //! @param aQueryProvider Admin Pass Query Provider
    //! @param aTimeout Timeout in Millisecs
    bool connectToServer(AdminPassQueryProvider* aQueryProvider = NULL, const int& aTimeout = DEFAULT_SOCKET_TIMEOUT);

    //! @brief Connect To Server
    //! @param aAdminPass Admin Pass
    //! @param aTimeout Timeout in Millisecs
    bool connectToServer(const QString& aAdminPass, const int& aTimeout = DEFAULT_SOCKET_TIMEOUT);

    //! @brief Disconnect From Server
    //! @param aTimeout Timeout in Millisecs
    bool disconnectFromServer(const int& aTimeout = DEFAULT_SOCKET_TIMEOUT);

    //! @brief Send Message
    //! @param aMessage Message
    void sendMessage(const QString& aMessage);

    //! @brief Send File Operation Entry
    //! @param aEntry File Operation Entry
    void sendOperation(const FileOperationEntry& aEntry);

    //! @brief Is Dir Readable
    //! @param aDirPath Dir Path
    //! @return true if Dir Is Readable, false Otherwise
    bool isReadable(const QString& aDirPath);

    //! @brief Read Dir - Async
    //! @param aDirPath Dir Path
    //! @param aSortType Sort Type
    //! @param aReverse Reverse Order
    //! @param aHidden Show Hidden
    //! @param aNameFilters Name Filters
    void readDir(const QString& aDirPath, const int& aSortType, const bool& aReverse, const bool& aHidden, const QStringList& aNameFilters);

    //! @brief Scan Dir For Directory Size - Async
    //! @param aDirPath Dir Path
    void scanDir(const QString& aDirPath);

    //! @brief Create A Dir - Async
    //! @param aDirPath Dir Path
    void createDir(const QString& aDirPath);

    //! @brief Remove/Delete Single File/Empty Directory
    //! @param aFilePath File Path
    void remove(const QString& aFilePath);

    //! @brief Copy Single File
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    void copyFile(const QString& aSource, const QString& aTarget);

    //! @brief Move Single File
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    void moveFile(const QString& aSource, const QString& aTarget);

    //! @brief Rename Single File
    //! @param aSource Source File Path
    //! @param aTarget Target File Path
    void renameFile(const QString& aSource, const QString& aTarget);

    //! @brief Search
    //! @param aDirPath Search Start Dir Path
    //! @param aNamePattern File Name Pattern
    //! @param aContentPattern Content Pattern
    //! @param aSearchOptions Search Options
    void search(const QString& aDirPath, const QString& aNamePattern, const QString& aContentPattern, const int& aSearchOptions);

    //! @brief Abort Current Operation
    //! @param none
    void abort();

    //! @brief Destructor
    //! @param none
    virtual ~FileUtilsClient();

signals:

    //! @brief Info Signal
    //! @param aInfoString Info String
    void info(const QString& aInfoString);

    //! @brief Error Signal
    //! @param aErrorCode Error Code
    //! @param aErrorString Error String
    void error(const int& aErrorCode, const QString& aErrorString);

    //! @brief Progress Signal
    //! @param aOperation Operation Code
    //! @param aCurrent Current Item Progress
    //! @param aTotal Total/Overall Progress
    void progress(const int& aOperation, const quint64& aCurrent, const quint64& aTotal);

    //! @brief Entry Found Signal
    //! @param aFilePath Entry File Path
    void entryFound(const QString& aFilePath);

    //! @brief Response Signal
    //! @param aResponse Response
    void response(const QString& aResponse);

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

    //! @brief Ready Read Slot - Called When There is Available Data To Read From Socket
    //! @param none
    void readyRead();

protected:

    //! @brief Create Socket
    //! @param none
    void createSocket();

    //! @brief Delete Socket
    //! @param none
    void deleteSocket();

    //! @brief Launch Server
    //! @param aAdminPass Admin Password
    bool launchServer(const QString& aAdminPass);

    //! @brief Parse Line
    //! @param aLine New Line Recieved
    void parseLine(const QString& aLine);

protected: // Data

    //! Socket
    QLocalSocket*   socket;

    //! Server Name
    QString         serverName;

    //! Server Found
    //bool            serverFound;

    //! Socket Connected
    bool            socketConnected;
};

#endif // FILEUTILCLIENT_H
