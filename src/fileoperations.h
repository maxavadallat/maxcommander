
#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

// INCLUDES

#include "constants.h"


// FORWARD DECLARATIONS


// DECLARATIONS

//==============================================================================
//! @enum FileOperationState File Operation State Enum
//==============================================================================
enum FileOperationState
{
    FOSIdle         = 0x0000,
    FOSRunning,
    FOSPaused,
    FOSDone,
    FOSSkipped,
    FOSFailed
};


//==============================================================================
//! @class FileOperationEntry File Operation Class For File Operation Queue
//==============================================================================
class FileOperationEntry : public QObject
{
public:

    //! @brief Constructor
    //! @param aOperation Composit Operation Token
    FileOperationEntry(const QString& aOperation);

    //! @brief Constructor
    //! @param aOperation Operation Name
    //! @param aSource Operation Source Name
    //! @param aTarget Operation Target Name
    FileOperationEntry(const QString& aOperation, const QString& aSource, const QString& aTarget);

    //! @brief Constructor
    //! @param aOperation Operation Index
    //! @param aSource Operation Source Name
    //! @param aTarget Operation Target Name
    FileOperationEntry(const int& aOperation, const QString& aSource, const QString& aTarget);

    //! @brief Get Operation Index
    //! @param none
    //! @return Operation Index
    int getOperationIndex();

    //! @brief Get Operation State
    //! @param none
    //! @return Operation State
    FileOperationState getState();

    //! @brief Get Operation Name
    //! @param none
    //! @return Operation Name
    QString getOperationName();

    //! @brief Get Operation Source
    //! @param none
    //! @return Operation Source
    QString getSource();

    //! @brief Get Operation Target
    //! @param none
    //! @return Operation Target
    QString getTarget();

    //! @brief Get Operation Token
    //! @param none
    //! @return Operation Token
    QString getOperation() const;

    //! @brief Destructor
    //! @param none
    virtual ~FileOperationEntry();

protected: // Data
    friend class FileOperationQueue;
    friend class FileOperationQueueHandler;

    //! Operation Index
    int                 opIndex;
    //! Operation Name
    QString             opName;
    //! Operation Source
    QString             source;
    //! Operation Target
    QString             target;

    //! File Operation State
    FileOperationState  opState;
/*
    //! Running
    bool                running;
    //! Done
    bool                done;
    //! Failed
    bool                failed;
    //! Processed
    bool                processed;
*/
};


#endif // FILEOPERATIONS_H
