#ifndef REMOTEFILEUTILCLIENT_H
#define REMOTEFILEUTILCLIENT_H

#include <QLocalSocket>
#include <QTcpSocket>
#include <QByteArray>
#include <QObject>
#include <QMutex>



//==============================================================================
// Client Status Type
//==============================================================================
enum ClientStatusType
{
    ECSTCreated     = 0,
    ECSTIdle,
    ECSTBusy,
    ECSTSuspended,
    ECSTAborting,
    ECSTAborted,
    ECSTWaiting,
    ECSTError
};




//==============================================================================
// Remote File Util Client
//==============================================================================
class RemoteFileUtilClient : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit RemoteFileUtilClient(QObject* aParent = NULL);

    // Connect To File Server
    void connectToFileServer(const bool& asRoot = false, const QString& aRootPass = "");

    // Is Client Connected
    bool isConnected();

    // Get Status
    ClientStatusType getStatus();

    // Get Last Operation
    QString lastOperation();

    // Get Dir List
    void getDirList(const QString& aDirPath, const int& aFilters, const int& aSortFlags);

    // Create Directory
    void createDir(const QString& aDirPath);
    // Delete File/Directory
    void deleteFile(const QString& aFilePath);

    // Scan Directory Size
    void scanDirSize(const QString& aDirPath);
    // Scan Directory Tree
    void scanDirTree(const QString& aDirPath);

    // Copy File
    void copyFile(const QString& aSource, const QString& aTarget);
    // Rename/Move File
    void moveFile(const QString& aSource, const QString& aTarget);

    // Set File Attributes
    void setFileAttributes(const QString& aFilePath, const int& aAttrib);
    // Set File Owner
    void setFileOwner(const QString& aFilePath, const QString& aOwner);
    // Set File Permissions
    void setFilePermissions(const QString& aFilePath, const int& aPermissions);
    // Set File Date Time
    void setFileDateTime(const QString& aFilePath, const QDateTime& aDateTime);

    // Search File
    void searchFile(const QString& aName, const QString& aDirPath, const QString& aContent, const int& aOptions);

    // Get Drive Info

    // Abort Current Operation
    void abort();
    // Suspend Current Operation
    void suspend();
    // Resume Current Operation
    void resume();
    // Close
    void close();

    // Execute Shell Command
    void executeShellCommand(const QString& aCommand, const bool& asRoot = false, const QString& aRootPass = "");

    // Send Confirm/Response
    void sendUserResponse(const int& aResponse, const QString& aNewPath = "");

    // Launch Server Test
    void launchServerTest(const bool& asRoot = false, const QString& aRootPass = "");
    // Start Test Operation
    void startTestOperation();
    // Stop/Abort Test Operation
    void stopTestOperation();
    // Disconnect Test
    void disconnectTest();

    // Status To String
    static QString statusToString(const int& aStatus);

    // Destructor
    virtual ~RemoteFileUtilClient();

signals:

    // Client Connection Changed Signal
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Signal
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Started Signal
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Progress Signal
    void fileOpProgress(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aCurrFilePath,
                        const quint64& aCurrProgress,
                        const quint64& aCurrTotal,
                        const quint64& aOverallProgress,
                        const quint64& aOverallTotal,
                        const int& aSpeed);

    // åFile Operation Suspended
    void fileOpSuspended(const unsigned int& aID,
                         const QString& aOp);

    // File Operation Resumed
    void fileOpResumed(const unsigned int& aID,
                       const QString& aOp);

    // File Operation Finished Signal
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Signal
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Signal
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Need Confirmation Signal
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const int& aCode,
                           const QString& aPath,
                           const QString& aSource,
                           const QString& aTarget);

    // Dir Size Scan Progress Signal
    void dirSizeScanProgress(const unsigned int& aID,
                             const QString& aPath,
                             const quint64& aNumDirs,
                             const quint64& aNumFiles,
                             const quint64& aScannedSize);

    // Dir List Item Found Signal
    void dirListItemFound(const unsigned int& aID,
                          const QString& aPath,
                          const QString& aFileName);

    // File Operation Queue Item Found Signal
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget);

    // File Search Result Item Found Signal
    void fileSearchResultItemFound(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aFilePath);

protected slots:

    // Init
    void init();

    // Shut Down
    void shutDown();

    // Set Status
    void setStatus(const ClientStatusType& aStatus);

    // Check If File Server Running
    bool checkFileServerRunning();

    // Start File Server
    void startFileServer(const bool& asRoot = false, const QString& aRootPass = "");

    // Parse Last Buffer
    void parseLastBuffer();
    // Parse Last data Map
    void parseLastDataMap();

    // Handle Test
    void handleTest();

    // Handle Preogress
    void handleProgress();
    // Handle Confirm
    void handleConfirm();
    // Handle Finished
    void handleFinished();
    // Handle Aborted
    void handleAbort();
    // Handle Error
    void handleError();
    // Handle Dir List Item
    void handleDirListItem();
    // Handle Queue Item
    void handleQueueItem();
    // Handle Dir Size Update
    void handleDirSizeUpdate();

    // Write Data
    void writeData(const QByteArray& aData);
    // Write Data
    void wirteData(const QVariantMap& aData);

    // Socket Host Found
    void socketHostFound();

    // Socket Connected Slot
    void socketConnected();
    // Socket Disconnected Slot
    void socketDisconnected();
    // Socket Error Slot
    void socketError(QAbstractSocket::SocketError socketError);
    // Socket State Changed Slot
    void socketStateChanged(QAbstractSocket::SocketState socketState);

    // Socket About To Close Slot
    void socketAboutToClose();
    // Socket Bytes Written Slot
    void socketBytesWritten(qint64 bytes);
    // Socket Read Channel Finished Slot
    void socketReadChannelFinished();
    // Socket Ready Read Slot
    void socketReadyRead();

    // Send Acknowledge
    void sendAcknowledge();

private:

    // Client ID
    unsigned int                    cID;

    // Status
    ClientStatusType                status;

    // Client Socket
    QTcpSocket*                     client;

    // Last Buffer
    QByteArray                      lastBuffer;
    // Frame Pattern
    QByteArray                      framePattern;

    // Last Data Map
    QVariantMap                     lastDataMap;

    // Reconnect As Root
    bool                            reconnectAsRoot;

    // Need Reconnect After Disconnect
    bool                            needReconnect;

    // General Mutex
    QMutex                          mutex;
};

#endif // REMOTEFILEUTILCLIENT_H
