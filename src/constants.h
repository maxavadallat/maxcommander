#ifndef CONSTANTS_H
#define CONSTANTS_H

// CONSTANTS

// Default Organization Name
#define DEFAULT_ORGANIZATION_NAME                           "Huge Company"
// Application Name
#define DEFAULT_APPLICATION_NAME                            "Max Commander"
// Server Name
#define DEFAULT_SERVER_NAME                                 "Max Commander SERVER"

// Exec Param Server Mode
#define PARAM_SERVER_MODE                                   "--server"
// Exec Param Operation
#define PARAM_OPERATION                                     "--operation="
// Exec Param Operation Read Dir
#define PARAM_OPERATION_READDIR                             "readdir"
// Exec Param Operation Size
#define PARAM_OPERATION_SIZE                                "size"
// Exec Param Operation Tree
#define PARAM_OPERATION_TREE                                "tree"
// Exec Param Operation Copy
#define PARAM_OPERATION_COPY                                "copy"
// Exec Param Operation Move
#define PARAM_OPERATION_MOVE                                "move"
// Exec Param Operation Delete
#define PARAM_OPERATION_DELETE                              "delete"
// Exec Param Operation Search
#define PARAM_OPERATION_SEARCH                              "search"

// Operation ID's
#define OPERATION_ID_READDIR                                0x0000
#define OPERATION_ID_SIZE                                   0x0001
#define OPERATION_ID_TREE                                   0x0002
#define OPERATION_ID_COPY                                   0x0003
#define OPERATION_ID_MOVE                                   0x0004
#define OPERATION_ID_DELETE                                 0x0005
#define OPERATION_ID_SEARCH                                 0x0006


// Icon Sizes
#define DEFAULT_ICON_SIZE_SMALL                             16
#define DEFAULT_ICON_SIZE_MEDIUM                            32
#define DEFAULT_ICON_SIZE_LARGE                             64

// Default Max Drives
#define DEFAULT_MAX_DRIVES                                  26  // ABCDEFGHIJKLMNOPQRSTUVWXYZ

// Default List Box Delegate/ItemCaching Overhead in percent
#define DEFAULT_LISTBOX_CACHE_OVERHEAD                      200
// Default List Box Item Spacing
#define DEFAULT_LISTBOX_ITEM_SPACING                        4
// Default List Box Item Size
#define DEFAULT_LISTBOX_ITEM_SIZE                           DEFAULT_ICON_SIZE_LARGE
// Default List Box Ease Position Property Name
#define DEFAULT_LISTBOX_EASPOS_PROPERTY_NAME                "easePos"
// Default List Box Ease Anim Interval In Millisecs
#define DEFAULT_LISTBOX_EASEANIM_DURATION                   1000
// Default List Box Repos Anim Interval In Millisecs
#define DEFAULT_LISTBOX_REPOSANIM_DURATION                  700
// Default List Box Snap To Anim Interval In Millisecs
#define DEFAULT_LISTBOX_SNAPTOANIM_DURATION                 300
// Default List Box Go To Anim Interval In Millisecs
#define DEFAULT_LISTBOX_GOTOANIM_DURATION                   200
// Default List Box Ease Acceleration
#define DEFAULT_LISTBOX_EASE_ACCELERATION                   -25.0
// Default List Box Drag Filter Timer Timeout
#define DEFAULT_LISTBOX_DRAGFILTER_TIMEOUT                  70
// Default List Box Wheel Repos Timer Timeout
#define DEFAULT_LISTBOX_WHEEL_REPOS_TIMEOUT                 1000
// Default List Box Wheel Speed
#define DEFAULT_LISTBOX_WHEEL_SPEED                         0.5
// Default List Box Item Header Minimum Width
#define DEFAULT_LISTBOX_ITEM_HEADER_MINIMUM_WIDTH           32


// File Size Consts
#define ONE_KILO                                            1024
#define ONE_MEGA                                            (ONE_KILO * ONE_KILO)
#define ONE_GIGA                                            (ONE_MEGA * ONE_KILO)
#define ONE_TERA                                            (ONE_GIGA * ONE_KILO)

// Default File Permission
#define DEFAULT_FILE_PERMISSION_NONE                        "----"

// Default File Size Constants
#define DEFAULT_FILE_SIZE_DIR                               "<DIR>"
#define DEFAULT_FILE_SIZE_LINK                              "<LINK>"
#define DEFAULT_FILE_SIZE_BUNDLE                            "<BUNDLE>"
#define DEFAULT_FILE_SIZE_APP                               "<APP>"


// Default Button ID OK
#define DEFAULT_BUTTON_INDEX_OK                             0X0001
// Default Button ID Cancel
#define DEFAULT_BUTTON_INDEX_CANCEL                         0X0000
// Default Button ID Close
#define DEFAULT_BUTTON_INDEX_CLOSE                          0X0000
// Default Button ID Retry
#define DEFAULT_BUTTON_INDEX_RETRY                          0X0001
// Default Button ID Abort
#define DEFAULT_BUTTON_INDEX_ABORT                          0X0000
// Default Button ID Ignore
#define DEFAULT_BUTTON_INDEX_IGNORE                         0X0002
// Default Button ID Overwrite
#define DEFAULT_BUTTON_INDEX_OVERWRITE                      0X0003
// Default Button ID Overwrite All
#define DEFAULT_BUTTON_INDEX_OVERWRITEALL                   0X0004
// Default Button ID Skip
#define DEFAULT_BUTTON_INDEX_SKIP                           0X0005
// Default Button ID Pause
#define DEFAULT_BUTTON_INDEX_PAUSE                          0X0003
// Default Button ID Continue
#define DEFAULT_BUTTON_INDEX_CONTINUE                       0X0004



// Default File List Item Height
#define DEFAULT_FILELIST_ITEM_HEIGHT                        DEFAULT_ICON_SIZE_MEDIUM
// Default File List Item Radius
#define DEFAULT_FILELIST_ITEM_RADIUS                        8

// Default Style Sheet Value Bold
#define STYLESHEET_VALUE_FONTBOLD                           "bold"
// Default Style Sheet Value Italic
#define STYLESHEET_VALUE_FONTITALIC                         "italic"


// Widget Style Sheet Text Template For Background Color
#define STYLE_SHEET_BACKGROUND_COLOR_TEMPLATE               "background-color: %1"
// Change font Button Example Text Template
#define CHANGE_FONT_BUTTON_EXAMPLE_TEXT_TEMPLATE            "Font: %1, size: %2"
// Preview Label Widget Style Sheet Template
#define STYLE_SHEET_ITEM_PREVIEW_TEMPLATE                   "color: %1; background-color: %2; font: %3 %4 %5pt \"%6\";"
// Preview Label Widget Style Sheet No Background Template
#define STYLE_SHEET_ITEM_PREVIEW_NO_BACKGROUND_TEMPLATE     "color: %1; font: %2 %3 %4pt \"%5\";"

// Default File List Panel Name1
#define DEFAULT_FILELIST_PANEL_NAME1                        "Panel1"
// Default File List Panel Name2
#define DEFAULT_FILELIST_PANEL_NAME2                        "Panel2"
// Default File List Panel Count
#define DEFAULT_FILELIST_PANEL_COUNT                        2


// Settings Keys

// Settings Group General
#define SETTINGS_GROUP_GENERAL                              "General"
// Settings Group Appearance
#define SETTINGS_GROUP_APPEARANCE                           "Appearance"
// Settings Group Tools
#define SETTINGS_GROUP_TOOLS                                "Tools"
// Settings Group Panel 1
#define SETTINGS_GROUP_PANEL1                               DEFAULT_FILELIST_PANEL_NAME1
// Settings Group Panel 2
#define SETTINGS_GROUP_PANEL2                               DEFAULT_FILELIST_PANEL_NAME2
// Settings Group Search
#define SETTINGS_GROUP_SEARCH                               "Search"
// Settings Group Shortcuts
#define SETTINGS_GROUP_SHORTCUTS                            "Shortcuts"
// Settings Group Viewer
#define SETTINGS_GROUP_VIEWER                               "Viewer"




// General

// Settings Key Show Splash
#define SETTINGS_KEY_SHOW_SPLASH                            "ShowSplash"
// Settings Key Show Command Button Bar
#define SETTINGS_KEY_SHOW_COMMAND_BAR                       "ShowCommandBar"
// Settings Key Show Tool Bar
#define SETTIGNS_KEY_SHOW_TOOL_BAR                          "ShowToolBar"
// Show Status Bar
#define SETTIGNS_KEY_SHOW_STATUS_BAR                        "ShowStatusBar"
// Settings Key Show Dir Buttons
#define SETTIGNS_KEY_SHOW_DIR_BUTTONS                       "ShowDirButtons"
// Settings Key Show Hidden Files
#define SETTIGNS_KEY_SHOW_HIDDEN_FILES                      "ShowHiddenFiles"
// Settings Key Execute Bundles
#define SETTIGNS_KEY_EXECUTE_BUNDLES                        "ExecuteBundles"
// Settings Key Use Recycle Bin
#define SETTIGNS_KEY_USE_RECYCLE_BIN                        "UseRecycleBin"
// Settings Key Save On Exit
#define SETTIGNS_KEY_SAVE_ON_EXIT                           "SaveOnExit"
// Settings Key Follow Links
#define SETTIGNS_KEY_FOLLOW_LINKS                           "FollowLinks"
// Treat Archves Like Dirs
#define SETTINGS_KEY_ARCHIVES_AS_DIRS                       "ArchivesAsDirs"
// Active Panel
#define SETTINGS_KEY_ACTIVE_PANEL                           "ActivePanel"


// Appearance

// Colors Keys

// Settings Key Color Normal Text Item Color
#define SETTINGS_KEY_NORMAL_TEXT_COLOR                      "TextColor"
// Settings Key Color Normal Text Item Background Color
#define SETTINGS_KEY_NORMAL_BACKGROUND_COLOR                "TextBackground"

// Settings Key Color Curent Text Item Color
#define SETTINGS_KEY_CURRENT_TEXT_COLOR                     "CurrentColor"
// Settings Key Color Current Text Item Background Color
#define SETTINGS_KEY_CURRENT_BACKGROUND_COLOR               "CurrentBackground"

// Settings Key Color Selected Text Item Color
#define SETTINGS_KEY_SELECTED_TEXT_COLOR                    "SelectedColor"
// Settings Key Color Selected Text Item Background Color
#define SETTINGS_KEY_SELECTED_BACKGROUND_COLOR              "SelectedBackground"

// Settings Key Color Current Selected Text Item Color
#define SETTINGS_KEY_CURRENT_SELECTED_TEXT_COLOR            "CurrentSelectedColor"
// Settings Key Color Current Selected Text Item Background Color
#define SETTINGS_KEY_CURRENT_SELECTED_BACKGROUND_COLOR      "CurrentSelectedBackground"

// Settings Key Color Link Text Item Color
#define SETTINGS_KEY_LINK_TEXT_COLOR                        "LinkColor"
// Settings Key Color Link Text Item Background Color
#define SETTINGS_KEY_LINK_BACKGROUND_COLOR                  "LinkBackground"


// Settings Key Icon Size
#define SETTINGS_KEY_ICONSIZE                               "IconSize"

// Settings Key Item Spacing
#define SETTINGS_KEY_ITEM_SPACING                           "Spacing"

// Settings Key Font Family
#define SETTINGS_KEY_FONTFAMILY                             "FontFamily"
// Settings Key Font Size
#define SETTINGS_KEY_FONTSIZE                               "FontSize"
// Settings Key Font Bold
#define SETTINGS_KEY_FONTBOLD                               "FontBold"
// Settings Key Font Italic
#define SETTINGS_KEY_FONTITALIC                             "FontItalic"



// Tools

// Settings Key Terminal Tool
#define SETTINGS_KEY_TOOL_TERMINAL                          "Terminal"
// Settings Key Viewer Tool
#define SETTINGS_KEY_TOOL_VIEWER                            "Viewer"
// Settings Key Editor Tool
#define SETTINGS_KEY_TOOL_EDITOR                            "Editor"
// Settings Key Comapre Tool
#define SETTINGS_KEY_TOOL_COMPARE                           "Compare"
// Settings Key Packe Tool
#define SETTINGS_KEY_TOOL_PACKER                            "Packer"


// Panel


// Header Widths

// Settings Key Header Width Name
#define SETTING_KEY_HEADER_WIDTH_NAME                       "NameWidth"
// Settings Key Header Width Ext
#define SETTING_KEY_HEADER_WIDTH_EXT                        "ExtWidth"
// Settings Key Header Width Type
#define SETTING_KEY_HEADER_WIDTH_TYPE                       "TypeWidth"
// Settings Key Header Width Size
#define SETTING_KEY_HEADER_WIDTH_SIZE                       "SizeWidth"
// Settings Key Header Width Date
#define SETTING_KEY_HEADER_WIDTH_DATE                       "DateWidth"
// Settings Key Header Width Owner
#define SETTING_KEY_HEADER_WIDTH_OWNER                      "OwnerWisth"
// Settings Key Header Width Perms
#define SETTING_KEY_HEADER_WIDTH_PERMS                      "PermsWidth"

// Settings Key Sort Type
#define SETTINGS_KEY_SORTTYPE                               "SortType"
// Settings Key Sort Reverse Order
#define SETTINGS_KEY_REVERSE                                "Reverse"
// Settings Key Last Dir
#define SETTINGS_KEY_LASTDIR                                "LastDir"




// Search

// Settigns Key Case Sensitive Search
#define SETTINGS_KEY_SEARCH_CASE_SENSITIVE                  "CaseSensitive"
// Settigns Key Whole word Search
#define SETTINGS_KEY_SEARCH_WHOLE_WORD                      "WholeWord"



// Viewer

// Settings Key Font Family
#define SETTINGS_KEY_VIEWER_FONTFAMILY                      "FontFamily"
// Settings Key Font Size
#define SETTINGS_KEY_VIEWER_FONTSIZE                        "FontSize"
// Settings Key Font Bold
#define SETTINGS_KEY_VIEWER_FONTBOLD                        "FontBold"
// Settings Key Font Italic
#define SETTINGS_KEY_VIEWER_FONTITALIC                      "FontItalic"
// Settigns Key Case Sensitive Search
#define SETTINGS_KEY_VIEWER_WORD_WRAP                       "WordWrap"
// Settigns Key Case Sensitive Search
#define SETTINGS_KEY_VIEWER_CASE_SENSITIVE                  "CaseSensitive"
// Settigns Key Whole word Search
#define SETTINGS_KEY_VIEWER_WHOLE_WORD                      "WholeWord"


// Default Setting Values

// Default Color Values
#define SETTINGS_VALUE_NORMAL_TEXT_COLOR                    QColor(0x00, 0x00, 0x00).rgba()
#define SETTINGS_VALUE_NORMAL_BACKGROUND_COLOR              -1

#define SETTINGS_VALUE_CURRENT_TEXT_COLOR                   QColor(0x00, 0x00, 0x00).rgba()
#define SETTINGS_VALUE_CURRENT_BACKGROUND_COLOR             QColor(0xCC, 0xCC, 0xCC).rgba()

#define SETTINGS_VALUE_SELECTED_TEXT_COLOR                  QColor(0xFF, 0x66, 0x00).rgba()
#define SETTINGS_VALUE_SELECTED_BACKGROUND_COLOR            -1

#define SETTINGS_VALUE_CURRENT_SELECTED_TEXT_COLOR          QColor(0xFF, 0x66, 0x00).rgba()
#define SETTINGS_VALUE_CURRENT_SELECTED_BACKGROUND_COLOR    QColor(0xCC, 0xCC, 0xCC).rgba()

#define SETTINGS_VALUE_LINK_TEXT_COLOR                      QColor(0x66, 0x66, 0xCC).rgba()
#define SETTINGS_VALUE_LINK_BACKGROUND_COLOR                -1

// Default Icon Size Values
#define SETTINGS_VALUE_ICONSIZE_16X16                       0
#define SETTINGS_VALUE_ICONSIZE_32X32                       1
#define SETTINGS_VALUE_ICONSIZE_64X32                       2


// Default Font Settings Values
#define SETTINGS_VALUE_FONTFAMILY                           "Tahoma"
#define SETTINGS_VALUE_FONTSIZE                             12
#define SETTINGS_VALUE_FONTBOLD                             false
#define SETTINGS_VALUE_FONTITALIC                           false

// Default Header Item Width
#define SETTINGS_VALUE_DEFAULT_HEADER_ITEM_WIDTH            32
// Default Item Spacing
#define SETTINGS_VALUE_DEFAULT_ITEM_SPACING                 2

// Status Panel Text Template
#define DEFAULT_STATUS_PANEL_TEXT_TEMPLATE                  "%1 File(s) - %L2K of %L3K bytes free"
// Selection Panel Text Template
#define DEFAULT_SELECTION_PANEL_TEXT_TEMPLATE               "%1 Selected items %L2 bytes"
// File Permissions Template
#define DEFAULT_PERMISSIONS_TEXT_TEMPLATE                   "%1%2%3%4"
// File List Delegate Date Format String
#define DEFAULT_FILELIST_DELEGATE_DATE_FORMAT_STRING        "yyyy-MM-dd hh:mm:ss"


// File List Headr Popup Menu Item Titles
#define DEFAULT_POPUP_MENU_ITEM_TITLE_NAME                  "Name"
#define DEFAULT_POPUP_MENU_ITEM_TITLE_EXT                   "Extension"
#define DEFAULT_POPUP_MENU_ITEM_TITLE_SIZE                  "Size"
#define DEFAULT_POPUP_MENU_ITEM_TITLE_DATE                  "Date"
#define DEFAULT_POPUP_MENU_ITEM_TITLE_OWNER                 "Owner"
#define DEFAULT_POPUP_MENU_ITEM_TITLE_PERMS                 "Permissions"



// Dir Size Scanner Update Timer Interval
#define DEFAULT_DIR_SIZE_SCANNER_UPDATE_TIMER_INTERVAL      250


// Bits Per Color Components
#define DEFAULT_BITS_PER_COMPONENTS                         8


// Function Key Texts
#define DEFAULT_FUNCTION_KEY_TEXT_F1                        "F1 Help"
#define DEFAULT_FUNCTION_KEY_TEXT_F2                        "F2 Tree"
#define DEFAULT_FUNCTION_KEY_TEXT_F3                        "F3 View"
#define DEFAULT_FUNCTION_KEY_TEXT_F4                        "F4 Edit"
#define DEFAULT_FUNCTION_KEY_TEXT_F5                        "F5 Copy"
#define DEFAULT_FUNCTION_KEY_TEXT_F6                        "F6 Move"
#define DEFAULT_FUNCTION_KEY_TEXT_F7                        "F7 Mk Dir"
#define DEFAULT_FUNCTION_KEY_TEXT_F8                        "F8 Delete"
#define DEFAULT_FUNCTION_KEY_TEXT_F9                        "F9 Options"
#define DEFAULT_FUNCTION_KEY_TEXT_F10                       "F10 Exit"

// Function Key Texts - Alt Pressed
#define DEFAULT_FUNCTION_KEY_TEXT_F1_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F2_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F3_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F4_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F5_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F6_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F7_ALT                    "F7 Search"
#define DEFAULT_FUNCTION_KEY_TEXT_F8_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F9_ALT                    ""
#define DEFAULT_FUNCTION_KEY_TEXT_F10_ALT                   ""

// Function Key Texts - Shift Pressed
#define DEFAULT_FUNCTION_KEY_TEXT_F1_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F2_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F3_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F4_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F5_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F6_SHIFT                  "F6 Rename"
#define DEFAULT_FUNCTION_KEY_TEXT_F7_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F8_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F9_SHIFT                  ""
#define DEFAULT_FUNCTION_KEY_TEXT_F10_SHIFT                 ""

// Function Key Texts - Control Pressed
#define DEFAULT_FUNCTION_KEY_TEXT_F1_CONTROL                ""
#define DEFAULT_FUNCTION_KEY_TEXT_F2_CONTROL                ""
#define DEFAULT_FUNCTION_KEY_TEXT_F3_CONTROL                "F3 Name"
#define DEFAULT_FUNCTION_KEY_TEXT_F4_CONTROL                "F4 Ext"
#define DEFAULT_FUNCTION_KEY_TEXT_F5_CONTROL                "F5 Size"
#define DEFAULT_FUNCTION_KEY_TEXT_F6_CONTROL                "F6 Date"
#define DEFAULT_FUNCTION_KEY_TEXT_F7_CONTROL                "F7 Owner"
#define DEFAULT_FUNCTION_KEY_TEXT_F8_CONTROL                "F8 Perms"
#define DEFAULT_FUNCTION_KEY_TEXT_F9_CONTROL                ""
#define DEFAULT_FUNCTION_KEY_TEXT_F10_CONTROL               ""


// Copy File Dialog Title Text - Multiple Files
#define DEFAULT_COPY_DIALOG_TITLE_TEXT_FILES                "Copy Files"
// Copy File Dialog Title Text - Single File
#define DEFAULT_COPY_DIALOG_TITLE_TEXT_FILE                 "Copy File"
// Copy File Dialog Text Template - Multiple Files
#define DEFAULT_COPY_DIALOG_TEXT_TEMPLATE_FILES             "Copy %1 Files To"
// Copy File Dialog Text Template - Single Files
#define DEFAULT_COPY_DIALOG_TEXT_TEMPLATE_FILE              "Copy %1 To"


// Move File Dialog Title Text - Multiple Files
#define DEFAULT_MOVE_DIALOG_TITLE_TEXT_FILES                "Move Files"
// Move File Dialog Title Text - Single File
#define DEFAULT_MOVE_DIALOG_TITLE_TEXT_FILE                 "Move File"
// Move File Dialog Text Template - Multiple Files
#define DEFAULT_MOVE_DIALOG_TEXT_TEMPLATE_FILES             "Move %1 Files To"
// Move File Dialog Text Template - Single Files
#define DEFAULT_MOVE_DIALOG_TEXT_TEMPLATE_FILE              "Move %1 To"


// Delete File Dialog Title Text - Multiple Files
#define DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILES              "Delete Files"
// Delete File Dialog Title Text - Single Files
#define DEFAULT_DELETE_DIALOG_TITLE_TEXT_FILE               "Delete File"
// Delete File Dialog Text - Multiple Files
#define DEFAULT_DELETE_DIALOG_TEXT_FILES                    "Are you sure to delete %1 files?"
// Delete File Dialog Text - Single Files
#define DEFAULT_DELETE_DIALOG_TEXT_FILE                     "Are you sure to delete %1?"



// File Viewer Max Size
#define DEFAULT_VIEWER_MAX_FILE_SIZE                        ONE_MEGA



#endif // CONSTANTS_H
