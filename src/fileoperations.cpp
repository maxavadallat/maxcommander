
// INCLUDES

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "fileoperations.h"
#include "fileutils.h"


// IMPLEMENTAITON


//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(const QString& aOpName, const QString& aSource, const QString& aTarget)
    : QObject(NULL)
    , opIndex(-1)
    , opName(aOpName)
    , source(aSource)
    , target(aTarget)
    , opState(FOSIdle)
/*
    , running(false)
    , done(false)
    , failed(false)
    , processed(false)
*/
{
    // Check Operation Name
    if (opName == QString(PARAM_OPERATION_COPY)) {
        // Set Operation Index
        opIndex = OPERATION_ID_COPY;
    } else if (opName == QString(PARAM_OPERATION_MOVE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MOVE;
    } else if (opName == QString(PARAM_OPERATION_RENAME)) {
        // Set Operation Index
        opIndex = OPERATION_ID_RENAME;
    } else if (opName == QString(PARAM_OPERATION_DELETE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_DELETE;
    } else if (opName == QString(PARAM_OPERATION_MAKEDIR)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MAKEDIR;
    } else {
        // Set Operation Index
        opIndex = OPERATION_ID_NOOP;
    }

    //qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(const QString& aOperation)
    : QObject(NULL)
    , opIndex(-1)
    , opName(QString(""))
    , source(QString(""))
    , target(QString(""))
    , opState(FOSIdle)
/*
    , running(false)
    , done(false)
    , failed(false)
    , processed(false)
*/
{
    //qDebug() << "Creating FileOperationEntry - aOperation: " << aOperation;

    // Split Operation Token
    QStringList opList = aOperation.split(QChar(FILE_OPERATIONS_TOKEN_SEPARATOR));

    // Set Operation Name
    opName = opList[0];

    // Check Operation Name
    if (opName == QString(PARAM_OPERATION_COPY)) {
        // Set Operation Index
        opIndex = OPERATION_ID_COPY;
    } else if (opName == QString(PARAM_OPERATION_MOVE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MOVE;
    } else if (opName == QString(PARAM_OPERATION_RENAME)) {
        // Set Operation Index
        opIndex = OPERATION_ID_RENAME;
    } else if (opName == QString(PARAM_OPERATION_DELETE)) {
        // Set Operation Index
        opIndex = OPERATION_ID_DELETE;
    } else if (opName == QString(PARAM_OPERATION_MAKEDIR)) {
        // Set Operation Index
        opIndex = OPERATION_ID_MAKEDIR;
    } else {
        // Set Operation Index
        opIndex = OPERATION_ID_NOOP;
    }

    // Set Operation Source
    source = opList[1];
    // set Operation Target
    target = opList[2];

    //qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Constructor
//==============================================================================
FileOperationEntry::FileOperationEntry(const int& aOpIndex, const QString& aSource, const QString& aTarget)
    : QObject(NULL)
    , opIndex(aOpIndex)
    , opName(QString(""))
    , source(aSource)
    , target(aTarget)
    , opState(FOSIdle)
/*
    , running(false)
    , done(false)
    , failed(false)
    , processed(false)
*/
{
    // Switch Operation Index
    switch (opIndex) {
        default:
        case OPERATION_ID_NOOP: opName = QString(PARAM_OPERATION_NOOP); break;
        case OPERATION_ID_COPY: opName = QString(PARAM_OPERATION_COPY); break;
        case OPERATION_ID_MOVE: opName = QString(PARAM_OPERATION_MOVE); break;
        case OPERATION_ID_RENAME: opName = QString(PARAM_OPERATION_RENAME); break;
        case OPERATION_ID_DELETE: opName = QString(PARAM_OPERATION_DELETE); break;
        case OPERATION_ID_MAKEDIR: opName = QString(PARAM_OPERATION_MAKEDIR); break;
    }

    //qDebug() << "Creating FileOperationEntry...done";
}

//==============================================================================
// Get Operation Index
//==============================================================================
int FileOperationEntry::getOperationIndex()
{
    return opIndex;
}

//==============================================================================
// Get Operation State
//==============================================================================
FileOperationState FileOperationEntry::getState()
{
    return opState;
}

//==============================================================================
// Get Operation Name
//==============================================================================
QString FileOperationEntry::getOperationName()
{
    return opName;
}

//==============================================================================
// Get Operation Source
//==============================================================================
QString FileOperationEntry::getSource()
{
    return source;
}

//==============================================================================
// Get Operation Target
//==============================================================================
QString FileOperationEntry::getTarget()
{
    return target;
}

//==============================================================================
// Get Operation Token
//==============================================================================
QString FileOperationEntry::getOperation() const
{
    return QString("%1%2%3%4%5").arg(opName).arg(FILE_OPERATIONS_TOKEN_SEPARATOR).arg(source).arg(FILE_OPERATIONS_TOKEN_SEPARATOR).arg(target);
}

//==============================================================================
// Destructor
//==============================================================================
FileOperationEntry::~FileOperationEntry()
{
    // ...

    //qDebug() << "Deleting FileOperationEntry...done";
}






