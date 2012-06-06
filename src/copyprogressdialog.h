#ifndef COPYPROGRESSDIALOG_H
#define COPYPROGRESSDIALOG_H

// INCLUDES

#include <QDialog>

#include "fileutils.h"


// FORWARD DECLARATIONS

namespace Ui {
class CopyProgressDialog;
}


// DECLARATIONS

//==============================================================================
//! @class CopyProgressDialog Copy Progress Dialog Class
//==============================================================================
class CopyProgressDialog : public QDialog, public FileOperationQueueHandler
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit CopyProgressDialog(QWidget* aParent = NULL);

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
    void reset();

    //! @brief Set Progress
    //! @param aProgress Progress Value
    void setCurrentProgress(const quint64& aProgress);

    //! @brief Add To Progress
    //! @param aValue Additional Progress Value
    void addToCurrentProgress(const quint64& aValue);

    //! @brief Get Progress
    //! @param none
    //! @brief Progress Value
    quint64 getCurrentProgress();

    //! @brief Set Max Value
    //! @param aMaxValue Max Value
    void setCurrentMaxValue(const quint64& aMaxValue);

    //! @brief Add To Max Value
    //! @param aValue Additional Value To Max Value
    void addToCurrentMaxValue(const quint64& aValue);

    //! @brief Get Max Value
    //! @param none
    //! @brief Max Value
    quint64 getCurrentMaxValue();


    //! @brief Set Total Progress
    //! @param aProgress Progress Value
    void setTotalProgress(const quint64& aProgress);

    //! @brief Add To Total Progress
    //! @param aValue Additional Progress Value
    void addToTotalProgress(const quint64& aValue);

    //! @brief Get Total Progress
    //! @param none
    //! @brief Progress Value
    quint64 getTotalProgress();

    //! @brief Set Total Max Value
    //! @param aMaxValue Max Value
    void setTotalMaxValue(const quint64& aMaxValue);

    //! @brief Add To Total Max Value
    //! @param aValue Additional Value To Max Value
    void addToTotalMaxValue(const quint64& aValue);

    //! @brief Get Total Max Value
    //! @param none
    //! @brief Max Value
    quint64 getTotalMaxValue();

    //! @brief Destructor
    //! @param none
    virtual ~CopyProgressDialog();

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

protected:

    //! UI
    Ui::CopyProgressDialog* ui;

    //! Details Shown
    bool                    detailsShown;

    //! Curent Progress Real Value
    quint64                 currProgress;
    //! Current Max Value Real Value
    quint64                 currMax;
    //! Total Progress Real Value
    quint64                 totalProgress;
    //! Total Max Real Value
    quint64                 totalMax;

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

#endif // COPYPROGRESSDIALOG_H
