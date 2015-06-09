#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QVariant>

//==============================================================================
// Settings Settings Controller
//==============================================================================
class SettingsController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool showFunctionKeys READ getShowFunctionKeys WRITE setShowFunctionKeys NOTIFY showFunctionKeysChanged)
    Q_PROPERTY(bool showDirHotKeys READ getShowDirHotKeys WRITE setShowDirHotKeys NOTIFY showDirHotKeysChanged)
    Q_PROPERTY(bool showDriveButtons READ getShowDriveButtons WRITE setShowDriveButtons NOTIFY showDriveButtonsChanged)
    Q_PROPERTY(bool closeWhenFinished READ getCloseWhenFinished WRITE setCloseWhenFinished NOTIFY closeWhenFinishedChanged)

    Q_PROPERTY(bool selectDirectories READ getSelectDirectories WRITE setSelectDirectories NOTIFY selectDirectoriesChanged)

    Q_PROPERTY(bool showHiddenFiles READ getShowHiddenFiles WRITE setShowHiddenFiles NOTIFY showHiddenFilesChanged)
    Q_PROPERTY(bool showDirsFirst READ getShowDirsFirst WRITE setShowDirsFirst NOTIFY showDirsFirstChanged)
    Q_PROPERTY(bool caseSensitiveSort READ getCaseSensitiveSort WRITE setCaseSensitiveSort NOTIFY caseSensitiveSortChanged)

    Q_PROPERTY(bool useDefaultIcons READ getUseDefaultIcons WRITE setUseDefaultIcons NOTIFY useDefaultIconsChanged)
    Q_PROPERTY(bool showFullSizes READ getShowFullSizes WRITE setShowFullSizes NOTIFY showFullSizesChanged)
    Q_PROPERTY(bool copyHiddenFiles READ getCopyHiddenFiles WRITE setCopyHiddenFiles NOTIFY copyHiddenFilesChanged)

    Q_PROPERTY(QString textColor READ getTextColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString textBGColor READ gettextBGColor WRITE setTextBGColor NOTIFY textBGColorChanged)

    Q_PROPERTY(QString currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QString currentBGColor READ getCurrentBGColor WRITE setCurrentBGColor NOTIFY currentBGColorChanged)

    Q_PROPERTY(QString selectedColor READ getSelectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QString selectedBGColor READ getSelectedBGColor WRITE setSelectedBGColor NOTIFY selectedBGColorChanged)

    Q_PROPERTY(QString currentSelectedColor READ getCurrentSelectedColor WRITE setCurrentSelectedColor NOTIFY currentSelectedColorChanged)
    Q_PROPERTY(QString currentSelectedBGColor READ getCurrentSelectedBGColor WRITE setCurrentSelectedBGColor NOTIFY currentSelectedBGColorChanged)

    Q_PROPERTY(QString hiddenColor READ getHiddenColor WRITE setHiddenColor NOTIFY hiddenColorChanged)
    Q_PROPERTY(QString hiddenBGColor READ getHiddenBGColor WRITE setHiddenBGColor NOTIFY hiddenBGColorChanged)

    Q_PROPERTY(QString linkColor READ getLinkColor WRITE setLinkColor NOTIFY linkColorChanged)
    Q_PROPERTY(QString linkBGColor READ getLinkBGColor WRITE setLinkBGColor NOTIFY linkBGColorChanged)

    Q_PROPERTY(QString fontName READ getFontName WRITE setFontName NOTIFY fontNameChanged)
    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool fontBold READ getFontBold WRITE setFontBold NOTIFY fontBoldChanged)
    Q_PROPERTY(bool fontItalic READ getFontItalic WRITE setFontItalic NOTIFY fontItalicChanged)

    Q_PROPERTY(int thumbWidth READ getThumbWidth WRITE setThumbWidth NOTIFY thumbWidthChanged)
    Q_PROPERTY(int thumbHeight READ getThumbHeight WRITE setThumbHeight NOTIFY thumbHeightChanged)

    Q_PROPERTY(QString terminalPath READ getTerminalPath WRITE setTerminalPath NOTIFY terminalPathChanged)
    Q_PROPERTY(QString viewerPath READ getViewerPath WRITE setViewerPath NOTIFY viewerPathChanged)
    Q_PROPERTY(QString editorPath READ getEditorPath WRITE setEditorPath NOTIFY editorPathChanged)
    Q_PROPERTY(QString comparePath READ getComparePath WRITE setComparePath NOTIFY comparePathChanged)
    Q_PROPERTY(QString packerPath READ getPackerPath WRITE setPackerPath NOTIFY packerPathChanged)
    Q_PROPERTY(QString unPackerPath READ getUnPackerPath WRITE setUnPackerPath NOTIFY unpackerPathChanged)

    Q_PROPERTY(bool dirty READ getDirty NOTIFY dirtyChanged)

public:

    // Get Instance - Static Constructor
    static SettingsController* getInstance();

    // Release
    void release();

    // Get Value
    QVariant value(const QString& aKey, const QVariant& aDefaultValue = QVariant());

    // Set Value
    void setValue(const QString& aKey, const QVariant& aValue);

    // Get Dirty
    bool getDirty();

    // Restore Defaults
    void restoreDefaults();

    // Get Show Function Keys
    bool getShowFunctionKeys();
    // Set Show Function Keys
    void setShowFunctionKeys(const bool& aShow);

    // Get Show Directory Hot Keys
    bool getShowDirHotKeys();
    // Set Show Directory Hot Keys
    void setShowDirHotKeys(const bool& aShow);

    // Get Show Drive Buttons
    bool getShowDriveButtons();
    // Set Show Drive Buttons
    void setShowDriveButtons(const bool& aShow);

    // Get Close When Finished
    bool getCloseWhenFinished();
    // Set Close When Finished
    void setCloseWhenFinished(const bool& aClose);

    // Get Select Directories
    bool getSelectDirectories();
    // Set Select Directories
    void setSelectDirectories(const bool& aSelect);

    // Get Show Hidden Files
    bool getShowHiddenFiles();
    // Set Show Hidden Files
    void setShowHiddenFiles(const bool& aHidden);

    // Get Show Directories First
    bool getShowDirsFirst();
    // Set Show Directories First
    void setShowDirsFirst(const bool& aShow);

    // Get Case Sensitive Sorting
    bool getCaseSensitiveSort();
    // Set Case Sensitive Sorting
    void setCaseSensitiveSort(const bool& aCaseSensitive);

    // Get Use Default Icons
    bool getUseDefaultIcons();
    // Set Use Default Icons
    void setUseDefaultIcons(const bool& aUseDefaultIcons);

    // Get Show Full File Sizes
    bool getShowFullSizes();
    // Set Show Full File Sizes
    void setShowFullSizes(const bool& aShowFullSizes);

    // Get Copy Hidden Files
    bool getCopyHiddenFiles();
    // Set Copy Hidden Files
    void setCopyHiddenFiles(const bool& aCopyHiddenFile);

    // Get Follow Links
    bool getFollowLinks();
    // Set Follow Links
    void setFollowLinks(const bool& aFollowLinks);

    // Get Normal Text Color
    QString getTextColor();
    // Set Normal Text Color
    void setTextColor(const QString& aColor);

    // Get Normal Text BG Color
    QString gettextBGColor();
    // Set Normal Text BG Color
    void setTextBGColor(const QString& aColor);

    // Get Current Text Color
    QString getCurrentColor();
    // Set Current Text Color
    void setCurrentColor(const QString& aColor);

    // Get Current Text BG Color
    QString getCurrentBGColor();
    // Set Current Text BG Color
    void setCurrentBGColor(const QString& aColor);

    // Get Selected Text Color
    QString getSelectedColor();
    // Set Selected Text Color
    void setSelectedColor(const QString& aColor);

    // Get Selected Text BG Color
    QString getSelectedBGColor();
    // Set Selected Text BG Color
    void setSelectedBGColor(const QString& aColor);

    // Get Current Selected Text Color
    QString getCurrentSelectedColor();
    // Set Current Selected Text Color
    void setCurrentSelectedColor(const QString& aColor);

    // Get Current Selected Text BG Color
    QString getCurrentSelectedBGColor();
    // Set Current Selected Text BG Color
    void setCurrentSelectedBGColor(const QString& aColor);

    // Get Hidden Text Color
    QString getHiddenColor();
    // Set Hidden Text Color
    void setHiddenColor(const QString& aColor);

    // Get Hidden Text BG Color
    QString getHiddenBGColor();
    // Set Hidden Text BG Color
    void setHiddenBGColor(const QString& aColor);

    // Get Link Text Color
    QString getLinkColor();
    // Set Link Text Color
    void setLinkColor(const QString& aColor);

    // Get Link Text BG Color
    QString getLinkBGColor();
    // Set Link Text BG Color
    void setLinkBGColor(const QString& aColor);

    // Get Font Name
    QString getFontName();
    // Set Font Name
    void setFontName(const QString& aFontName);

    // Get Font Size
    int getFontSize();
    // Set Font Size
    void setFontSize(const int& aSize);

    // Get Font Bold
    bool getFontBold();
    // Set Font Bold
    void setFontBold(const bool& aBold);

    // Get Font Italic
    bool getFontItalic();
    // Set Font Italic
    void setFontItalic(const bool& aItalic);

    // Get Thumb Width
    int getThumbWidth();
    // Set Thumb Width
    void setThumbWidth(const int& aWidth);

    // Get Thumb Height
    int getThumbHeight();
    // Set Thumb height
    void setThumbHeight(const int& aheight);

    // Get Terminal Path
    QString getTerminalPath();
    // Set Terminal Path
    void setTerminalPath(const QString& aTerminalPath);

    // Get Viewer Path
    QString getViewerPath();
    // Set Viewer Path
    void setViewerPath(const QString& aViewerPath);

    // Get Editor Path
    QString getEditorPath();
    // Set Editor Path
    void setEditorPath(const QString& aEditorPath);

    // Get Compare Path
    QString getComparePath();
    // Set Compare Path
    void setComparePath(const QString& aComparePath);

    // Get Packer Path
    QString getPackerPath();
    // Set Packer Path
    void setPackerPath(const QString& aPackerPath);

    // Get Unpacker Path
    QString getUnPackerPath();
    // Set Unpacker Path
    void setUnPackerPath(const QString& aUnpackerPath);

signals:

    // Dirty Changed
    void dirtyChanged(const bool& aDirty);

    // Begin Global Settings Update Signal
    void globalSettingsUpdateBegin();
    // Finish Global Settings Update Signal
    void globalSettingsUpdateFinished();

    // Show Function Keys Changed Signal
    void showFunctionKeysChanged(const bool& aShow);
    // Show Directory Hot Keys Changed Signal
    void showDirHotKeysChanged(const bool& aShow);
    // Show Drive Buttons changed Signal
    void showDriveButtonsChanged(const bool& aShow);
    // Close When Finished Signal
    void closeWhenFinishedChanged(const bool& aClose);

    // Select Directries Changed Signal
    void selectDirectoriesChanged(const bool& aSelect);
    // Show Hidden Changed Signal
    void showHiddenFilesChanged(const bool& aHidden);
    // Show Directories First Changed Signal
    void showDirsFirstChanged(const bool& aShow);
    // Case Sensitive Sorting Changed Signal
    void caseSensitiveSortChanged(const bool& aCaseSensitive);

    // Use Default Icons Changed Signal
    void useDefaultIconsChanged(const bool& aUseDefaultIcons);
    // Show Full File Sizes Changed Signal
    void showFullSizesChanged(const bool& aShowFullSizes);
    // Copy Hidden Files Changed Signal
    void copyHiddenFilesChanged(const bool& aCopyHidden);
    // Follow Links Changed Signal
    void followLinksChanged(const bool& aFollowLinks);

    // Text Color Changed Signal
    void textColorChanged(const QString& aColor);
    // BG Color Changed Signal
    void textBGColorChanged(const QString& aColor);
    // Current Text Color Changed Signal
    void currentColorChanged(const QString& aColor);
    // Current BG Color Changed Signal
    void currentBGColorChanged(const QString& aColor);
    // Selected Color Changed Signal
    void selectedColorChanged(const QString& aColor);
    // Selected BG Color Changed Signal
    void selectedBGColorChanged(const QString& aColor);
    // Current Selected Color Changed Signal
    void currentSelectedColorChanged(const QString& aColor);
    // Current Selected BG Color Changed Signal
    void currentSelectedBGColorChanged(const QString& aColor);
    // Hidden Color Changed Signal
    void hiddenColorChanged(const QString& aColor);
    // Hidden BG Color Changed Signal
    void hiddenBGColorChanged(const QString& aColor);
    // Link Color Changed Signal
    void linkColorChanged(const QString& aColor);
    // Link BG Color Changed Signal
    void linkBGColorChanged(const QString& aColor);

    // Font Name Changed Signal
    void fontNameChanged(const QString& aFontName);
    // Font Size Changed Signal
    void fontSizeChanged(const int& aSize);
    // Font Bold Changed Signal
    void fontBoldChanged(const bool& aBold);
    // Font italic Changed Signal
    void fontItalicChanged(const bool& aItalic);

    // Thumb Width Changed Signal
    void thumbWidthChanged(const int& aWidth);
    // Thumb Height Changed Signal
    void thumbHeightChanged(const int& aHeight);

    // Terminal Path Changed Signal
    void terminalPathChanged(const QString& aTerminalPath);
    // Viewer Path Changed Signal
    void viewerPathChanged(const QString& aViewerPath);
    // Editor Path Changed Signal
    void editorPathChanged(const QString& aEditorPath);
    // Compare Path Changed Signal
    void comparePathChanged(const QString& aComparePath);
    // Packer Path Changed Signal
    void packerPathChanged(const QString& aPackerPath);
    // Unpacker Path Changed Signal
    void unpackerPathChanged(const QString& aUnpackerPath);

protected: // Constructor/Destructor

    // Constructor
    explicit SettingsController(QObject* aParent = NULL);

    // Load Settings
    void loadSettings();
    // Save Settings
    void saveSettings();

    // Destructor
    virtual ~SettingsController();

protected:

    // Set Dirty
    void setDirty(const bool& aDirty);

protected:

    // Int Ref Counter
    int                     refCount;

    // Settings
    QSettings               settings;
    // Dirty
    bool                    dirty;


    // Show Function Keys
    bool                    showFunctionKeys;
    // Show Directory Hot Keys
    bool                    showDirHotKeys;
    // Show Drive Buttons
    bool                    showDriveButtons;
    // Close When Finished
    bool                    closeWhenFinished;
    // Select Directories
    bool                    selectDirectories;

    // Show Hidden Files
    bool                    showHiddenFiles;
    // Show Dirs First
    bool                    showDirsFirst;
    // Case Sensitive Sorting
    bool                    caseSensitiveSort;

    // Use Default Icons
    bool                    useDefaultIcons;
    // Show Full File Sizes
    bool                    showFullSizes;
    // Copy Hidden Files
    bool                    copyHiddenFiles;
    // Follow Symbolic Links
    bool                    followLinks;

    // Normal Text Color
    QString                 textColor;
    // Normal Text BG Color
    QString                 textBGColor;
    // Current Item Text Color
    QString                 currentColor;
    // Current Item BG Color
    QString                 currentBGColor;
    // Selected Item Text Color
    QString                 selectedColor;
    // Selected Item BG Color
    QString                 selectedBGColor;
    // Current Selected Item Text Color
    QString                 currentSelectedColor;
    // Current Selected Item BG Color
    QString                 currentSelectedBGColor;
    // Hidden Item Text Color
    QString                 hiddenColor;
    // Hidden Item BG Color
    QString                 hiddenBGColor;
    // Link Item Text Color
    QString                 linkColor;
    // Link Item BG Color
    QString                 linkBGColor;
    // Font Name
    QString                 fontName;
    // Font Size
    int                     fontSize;
    // Font Bold
    bool                    fontBold;
    // Font Italic
    bool                    fontItalic;

    // Thumb Width
    int                     thumbWidth;
    // Thumb Height
    int                     thumbHeight;


    // Terminal App Path
    QString                 terminalPath;
    // Viewer App Path
    QString                 viewerPath;
    // Editor App Path
    QString                 editorPath;
    // Compare Path
    QString                 comparePath;
    // Packer Path
    QString                 packerPath;
    // Unpacker Path
    QString                 unPackerPath;


};

#endif // SETTINGSCONTROLLER_H
