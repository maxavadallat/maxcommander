#ifndef MAINQUEUEDIALOG_H
#define MAINQUEUEDIALOG_H

// INCLUDES

#include <QDialog>
#include "fileutils.h"


// FORWARD DECLARATIONS

namespace Ui {
class MainQueueDialog;
}


// DECLARATIONS

//==============================================================================
//! @class MainQueueDialog Main Queue Dialog Class
//==============================================================================
class MainQueueDialog : public QDialog, public FileOperationQueueHandler
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit MainQueueDialog(QWidget* aParent = NULL);

    //! @brief Add File Operation Entry
    //! @param aEntry File Operation Entry
    virtual void addOperationEntry(FileOperationEntry* aEntry);

    //! @brief Remove Operation
    //! @param aIndex Operation Index
    virtual void removeOperationEntry(const int& aIndex);

    //! @brief Get Operations Count
    //! @param none
    //! @return Operations Count
    virtual int opEntryCount();

    //! @brief Get Operation
    //! @param aIndex Operation Index
    //! @return Operation
    virtual FileOperationEntry* getOperationEntry(const int& aIndex);

    //! @brief Process Queue
    //! @param none
    virtual void processQueue();

    //! @brief Show
    //! @param none
    virtual void show();

    //! @brief Hide
    //! @param none
    virtual void hide();

    //! @brief Set Modal
    //! @param aModal Modal Setting
    virtual void setModal(const bool& aModal);

    //! @brief Reset All Count & Progress
    //! @param none
    void resetProgress();

    //! @brief Set Progress
    //! @param aProgress Progress Value
    void setProgress(const int& aProgress);

    //! @brief Add To Progress
    //! @param aValue Additional Progress Value
    void addToProgress(const int& aValue);

    //! @brief Get Progress
    //! @param none
    //! @brief Progress Value
    int getProgress();

    //! @brief Set Max Value
    //! @param aMaxValue Max Value
    void setMaxValue(const int& aMaxValue);

    //! @brief Add To Max Value
    //! @param aValue Additional Value To Max Value
    void addToMaxValue(const int& aValue);

    //! @brief Get Max Value
    //! @param none
    //! @brief Max Value
    int getMaxValue();

    //! @brief Destructor
    //! @param none
    virtual ~MainQueueDialog();

protected:

    //! @brief Abort
    //! @param none
    void abort();

    //! @brief Pause
    //! @param none
    void pause();

    //! @brief Resume
    //! @param none
    void resume();

    //! @brief Clear
    //! @param none
    void clear();

protected slots:

    //! @brief Pause Button Pressed Slot
    //! @param none
    void pauseButtonPressed();

    //! @brief Abort Button Pressed Slot
    //! @param none
    void abortButtonPressed();

    //! @brief Clear Button Pressed Slot
    //! @param none
    void clearButtonPressed();

    //! @brief Operation Queue Processing Started Slot
    //! @param none
    void opQueueStarted();

    //! @brief Operation Queue Processing Stopped Slot
    //! @param none
    void opQueueStopped();

    //! @brief Operation Queue Processing Finished Slot
    //! @param none
    void opQueueFinished();

    //! @brief Operation Added Slot
    //! @param aIndex Operation Index
    //! @param aCount New Operations Count
    void operationAdded(const int& aIndex, const int& aCount);

    //! @brief Operation Removed Slot
    //! @param aIndex Operation Index
    //! @param aCount Remaining Operations Count
    void operationRemoved(const int& aIndex, const int& aCount);

    //! @brief Operation Started Slot
    //! @param aIndex Operation Index
    void operationStarted(const int& aIndex);

    //! @brief Operation Stopped Slot
    //! @param aIndex Operation Index
    void operationStopped(const int& aIndex);

    //! @brief Operation Completed Slot
    //! @param aIndex Operation Index
    void operationCompleted(const int& aIndex);

protected: // Data

    //! UI
    Ui::MainQueueDialog*    ui;

    //! Pause Button
    QPushButton*            pauseButton;
    //! Abort Button
    QPushButton*            abortButton;
    //! Clear Button
    QPushButton*            clearButton;

    //! Paused
    bool                    paused;

    //! Operation Queue
    FileOperationQueue*     opQueue;
};

#endif // MAINQUEUEDIALOG_H
