#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QList>
#include <QAbstractListModel>


namespace Ui {
class TestDialog;
}


class RemoteFileUtilClient;
class ClientListModel;

//==============================================================================
// TestDialogClass
//==============================================================================
class TestDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    // Constructor
    explicit TestDialog(QWidget* aParent = NULL);

    // Get Current Index
    int getCurrentIndex();
    // Set Current Index
    void setCurrentIndex(const int& aIndex);

    // Destructor
    virtual ~TestDialog();

public slots:

    // Delete Client
    void deleteClient(const int& aIndex);

signals:

    // Current Index Changed Signal
    void currentIndexChanged(const int& aCurrentIndex);

protected slots:

    // Init
    void init();
    // Clear
    void clear();
    // Shut Down
    void shutDown();
    // Update Current Index
    void updateCurrentIndex();
    // Configure Buttons
    void configureButtons();

protected slots: // Client List Model

    // Clinet Updated Slot
    void clientUpdated();

protected slots: // Buttons

    // On Create Client Button Clicked Slot
    void on_createClientButton_clicked();
    // On Connect Client Button Clicked Slot
    void on_connectButton_clicked();
    // On Start Client Button Clicked Slot
    void on_startButton_clicked();
    // On Send Yes To Client Button Clicked Slot
    void on_yesButton_clicked();
    // On Send No To Client Button Clicked Slot
    void on_noButton_clicked();
    // On Stop Client Button Clicked Slot
    void on_stopButton_clicked();
    // On Pause Client Button Clicked Slot
    void on_pauseButton_clicked();
    // On Resume Client Button Clicked Slot
    void on_resumeButton_clicked();
    // On Disconnect Client Button Clicked Slot
    void on_disconnectButton_clicked();
    // On Clear Clients Button Clicked Slot
    void on_clearButton_clicked();

private slots:

private:
    // UI
    Ui::TestDialog*     ui;
    // Client List Model
    ClientListModel*    clientListModel;
    // Current Index
    int                 currentIndex;
};















//==============================================================================
// Test Client Class
//==============================================================================
class TestClient : public QObject
{
    Q_OBJECT

public:
    // Constructor
    explicit TestClient(QObject* aParent = NULL);

    // Connect
    void connectClient();
    // Start
    void start();
    // Abort
    void abort();
    // Suspend
    void suspend();
    // Resume
    void resume();
    // Send Response
    void sendResponse(const int& aResponse);
    // Disconnect
    void disconnectClient();

    // Get ID
    unsigned int getID();
    // Is Connected
    bool isConnected();
    // Is Busy
    bool isBusy();
    // Is Suspended
    bool isSuspended();

    // Destructor
    virtual ~TestClient();

signals:

    // Connection Changed Signal
    void connectionChanged(TestClient* aClient, const bool& aConnected);
    // Status Changed Signal
    void statusChanged(TestClient* aClient, const int& aStatus);

protected slots: // For RemoteFileUtilClient

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Started Slot
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Progress Slot
    void fileOpProgress(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aCurrFilePath,
                        const quint64& aCurrProgress,
                        const quint64& aCurrTotal);

    // File Operation Skipped Slot
    void fileOpSkipped(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Need Confirmation Slot
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const int& aCode,
                           const QString& aPath,
                           const QString& aSource,
                           const QString& aTarget);

    // File Operation Queue Item Found Slot
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget);

protected:
    friend class ClientListModel;

    // File Util
    RemoteFileUtilClient*   fileUtil;
};












//==============================================================================
// Test Client List Model Class
//==============================================================================
class ClientListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    // Constructor
    explicit ClientListModel(QObject* aParent = NULL);

    // Create New Client
    TestClient* createNewClient();

    // Get Client ID
    unsigned int clientID(const int& aIndex);

    // Is Client Connect
    bool clientConnected(const int& aIndex);

    // Connect CLient
    void connectClient(const int& aIndex);
    // Disconnect Client
    void disconnectClient(const int& aIndex);

    // Start Client
    void startClient(const int& aIndex);
    // Stop Client
    void stopClient(const int& aIndex);

    // Send User Response
    void sendUserReponse(const int& aIndex, const int& aResponse);

    // Pause Client
    void pauseClient(const int& aIndex);
    // Resume Client
    void resumeClient(const int& aIndex);

    // Get Client
    TestClient* getClient(const int& aIndex);

    // Remove Client
    void removeClient(const int& aIndex);

    // Clear
    void clear();

    // Destructor
    virtual ~ClientListModel();

signals:

    // Client Updated Signal
    void clientUpdated();

public: // From QAbstractListModel

    // Get Role Names
    virtual QHash<int, QByteArray> roleNames() const;
    // Get Row Count
    virtual int rowCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Column Count
    virtual int columnCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Data
    virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const;
    // Set Data
    virtual bool setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole = Qt::EditRole);

protected slots:

    // Client Connection Changed Slot
    void clientConnectionChanged(TestClient* aClient, const bool& aConnected);
    // Status Changed SLot
    void clientStatusChanged(TestClient* aClient, const int& aStatus);

protected:

    // Roles
    enum Roles {
        ClientID            = Qt::UserRole + 1,
        ClientConnected,
        ClientBusy,
        ClientListModelRolesCount
    };

    // Item List
    QList<TestClient*>    clientList;
};





#endif // TESTDIALOG_H
