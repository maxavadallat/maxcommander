#ifndef DELETEPROGRESSDIALOG_H
#define DELETEPROGRESSDIALOG_H

// INCLUDES

#include <QDialog>

#include "fileutils.h"


// FORWARD DECLARATIONS

namespace Ui {
class DeleteProgressDialog;
}


// DECLARATIONS

//==============================================================================
//! @class DeleteProgressDialog Delete Progress Dialog Class
//==============================================================================
class DeleteProgressDialog : public QDialog, public FileOpQueueViewAPI
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit DeleteProgressDialog(QWidget* aParent = NULL);

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

    //! @brief Operation Added Callback - SIGNALS DON't WORK
    //! @param aIndex Inserted Index
    //! @param aCount Current Count
    virtual void operationEntryAdded(const int& aIndex, const int& aCount);

    //! @brief Operation Entry Removed Callback - SIGNALS DON'T WORK
    //! @param aIndex Removed Index
    //! @param aCount Count
    virtual void operationEntryRemoved(const int& aIndex, const int& aCount);

    //! @brief Operation Entry Updated Callback - SIGNALS DON'T WORK
    //! @param aIndex Updated Item Index
    virtual void operationEntryUpdated(const int& aIndex);

    //! @brief Get File Operations Queue Handler
    //! @param none
    //! @return File Operations Queue Handler
    virtual FileOpQueueHandler* queueHandler();

    //! @brief Reset All Count & Progress
    //! @param none
    void reset();

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
    virtual ~DeleteProgressDialog();

protected slots:

    //! @brief On Show Details Button Clicked
    //! @param none
    void on_showDetailsButton_clicked();

    //! @brief Pause Button Pressed
    //! @param none
    void pauseButtonPressed();

    //! @brief Abort Button Pressed
    //! @param none
    void abortButtonPressed();

    //! @brief Clear Button Pressed
    //! @param none
    void clearButtonPressed();

protected:

    //! @brief Setup UI
    //! @param none
    void updateUI();

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

protected: // Data

    //! UI
    Ui::DeleteProgressDialog*   ui;

    //! Details Shown
    bool                        detailsShown;

    //! Pause Button
    QPushButton*                pauseButton;
    //! Abort Button
    QPushButton*                abortButton;
    //! Clear Button
    QPushButton*                clearButton;

    //! Paused
    bool                        paused;

    //! Operation Queue
    FileOpQueueHandler*         opQueueHandler;
};

#endif // DELETEPROGRESSDIALOG_H
