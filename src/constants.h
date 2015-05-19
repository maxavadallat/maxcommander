#ifndef CONSTANTS_H
#define CONSTANTS_H


// Default Values
#define MAX_NUMBER_OF_FILE_THREADS                          10


// Application Info

#define DEFAULT_APPLICATION_NAME                            "MaxCommander"
#define DEFAULT_ORGANIZATION_NAME                           "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                         "mysoft.com"


#define DEFAULT_WORKER_HOST_NAME                            "localhost"

// QML Context Properties

#define MAIN_VIEW_CONTROLLER                                "mainViewController"

#define LIST_MODEL_LEFT_PANE                                "leftPaneModel"
#define LIST_MODEL_RIGHT_PANE                               "rightPaneModel"


// Settings Groups

#define SETTINGS_GROUP_MAIN                                 "main"
#define SETTINGS_GROUP_LEFTPANEL                            "leftpanel"
#define SETTINGS_GROUP_RIGHTPANEL                           "rightpanel"
#define SETTINGS_GROUP_PANEL_COMMON                         "panelCommon"
#define SETTINGS_GROUP_UI                                   "ui"
#define SETTINGS_GROUP_VIEWER                               "viewer"
#define SETTINGS_GROUP_APPS                                 "apps"


// Settings Keys

// Main

#define SETTINGS_KEY_EXEC_PATH                              SETTINGS_GROUP_MAIN"/execpath"

#define SETTINGS_KEY_MAIN_MAXIMIZED                         SETTINGS_GROUP_MAIN"/maximized"

#define SETTINGS_KEY_MAIN_POSX                              SETTINGS_GROUP_MAIN"/posx"
#define SETTINGS_KEY_MAIN_POSY                              SETTINGS_GROUP_MAIN"/posy"

#define SETTINGS_KEY_MAIN_WIDTH                             SETTINGS_GROUP_MAIN"/width"
#define SETTINGS_KEY_MAIN_HEIGHT                            SETTINGS_GROUP_MAIN"/height"

#define SETTINGS_KEY_MAIN_ACTIVEPANEL                       SETTINGS_GROUP_MAIN"/activePanel"


// UI

#define SETTINGS_KEY_MAIN_SPLITTER                          SETTINGS_GROUP_UI"/mainSplitter"

#define SETTINGS_KEY_SHOW_FUNCTION_KEYS                     SETTINGS_GROUP_UI"/showFunctionKeys"
#define SETTINGS_KEY_SHOW_DIR_HOT_KEYS                      SETTINGS_GROUP_UI"/showDirHotKeys"
#define SETTINGS_KEY_SHOW_DRIVE_BUTTONS                     SETTINGS_GROUP_UI"/showDriveButtons"


#define SETTINGS_KEY_THUMBS_WIDTH                           SETTINGS_GROUP_UI"/thumbsWidth"
#define SETTINGS_KEY_THUMBS_HEIGHT                          SETTINGS_GROUP_UI"/thumbsHeiight"

#define SETTINGS_KEY_FOCUSED_PANEL                          SETTINGS_GROUP_UI"/focusedPanel"

#define SETTINGS_KEY_CLOSE_WHEN_FINISHED                    SETTINGS_GROUP_UI"/closeWhenFinished"


#define SETTINGS_KEY_PANEL_COLOR_TEXT                       SETTINGS_GROUP_UI"/textColor"
#define SETTINGS_KEY_PANEL_COLOR_TEXT_BG                    SETTINGS_GROUP_UI"/textBGColor"

#define SETTINGS_KEY_PANEL_COLOR_CURRENT                    SETTINGS_GROUP_UI"/currentColor"
#define SETTINGS_KEY_PANEL_COLOR_CURRENT_BG                 SETTINGS_GROUP_UI"/currentBGColor"

#define SETTINGS_KEY_PANEL_COLOR_SELECTED                   SETTINGS_GROUP_UI"/selectedColor"
#define SETTINGS_KEY_PANEL_COLOR_SELECTED_BG                SETTINGS_GROUP_UI"/selectedBGColor"

#define SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED           SETTINGS_GROUP_UI"/currentSelectedColor"
#define SETTINGS_KEY_PANEL_COLOR_CURRENT_SELECTED_BG        SETTINGS_GROUP_UI"/currentSelectedBGColor"

#define SETTINGS_KEY_PANEL_COLOR_HIDDEN                     SETTINGS_GROUP_UI"/hiddenColor"
#define SETTINGS_KEY_PANEL_COLOR_HIDDEN_BG                  SETTINGS_GROUP_UI"/hiddenBGColor"

#define SETTINGS_KEY_PANEL_COLOR_LINK                       SETTINGS_GROUP_UI"/linkColor"
#define SETTINGS_KEY_PANEL_COLOR_LINK_BG                    SETTINGS_GROUP_UI"/linkBGColor"

#define SETTINGS_KEY_PANEL_FONT_NAME                        SETTINGS_GROUP_UI"/fontName"
#define SETTINGS_KEY_PANEL_FONT_SIZE                        SETTINGS_GROUP_UI"/fontSize"
#define SETTINGS_KEY_PANEL_FONT_BOLD                        SETTINGS_GROUP_UI"/fontBold"
#define SETTINGS_KEY_PANEL_FONT_ITALIC                      SETTINGS_GROUP_UI"/fontItalic"

#define SETTINGS_KEY_APPS_TERMINAL                          SETTINGS_GROUP_APPS"/terminal"
#define SETTINGS_KEY_APPS_VIEWER                            SETTINGS_GROUP_APPS"/viewer"
#define SETTINGS_KEY_APPS_EDITOR                            SETTINGS_GROUP_APPS"/editor"
#define SETTINGS_KEY_APPS_COMPARE                           SETTINGS_GROUP_APPS"/compare"
#define SETTINGS_KEY_APPS_PACKER                            SETTINGS_GROUP_APPS"/packer"
#define SETTINGS_KEY_APPS_UNPACKER                          SETTINGS_GROUP_APPS"/unpacker"


// Common

#define SETTINGS_KEY_SHOW_HIDDEN_FILES                      SETTINGS_GROUP_PANEL_COMMON"/showHidden"
#define SETTINGS_KEY_DIRFIRST                               SETTINGS_GROUP_PANEL_COMMON"/dirFirst"
#define SETTINGS_KEY_CASE_SENSITIVE                         SETTINGS_GROUP_PANEL_COMMON"/caseSensitive"
#define SETTINGS_KEY_SELECT_DIRS                            SETTINGS_GROUP_PANEL_COMMON"/selectDirs"


// File Panels

#define DEFAULT_PANEL_NAME_LEFT                             "LEFT_PANEL"
#define DEFAULT_PANEL_NAME_RIGHT                            "RIGHT_PANEL"


// Panel Settings - Use With Panel Name!!

#define SETTINGS_KEY_PANEL_DIR                              "/dir"
#define SETTINGS_KEY_PANEL_SORTTYPE                         "/sortType"
#define SETTINGS_KEY_PANEL_REVERSE                          "/reverse"

#define SETTINGS_KEY_PANEL_COL_ENABLED_EXT                  "/columnExt"
#define SETTINGS_KEY_PANEL_COL_ENABLED_TYPE                 "/columnType"
#define SETTINGS_KEY_PANEL_COL_ENABLED_SIZE                 "/columnSize"
#define SETTINGS_KEY_PANEL_COL_ENABLED_DATE                 "/columnDate"
#define SETTINGS_KEY_PANEL_COL_ENABLED_OWNER                "/columnOwner"
#define SETTINGS_KEY_PANEL_COL_ENABLED_PERMS                "/columnPerms"
#define SETTINGS_KEY_PANEL_COL_ENABLED_ATTR                 "/columnAttr"

#define SETTINGS_KEY_PANEL_COL_WIDTH_EXT                    "/widthExt"
#define SETTINGS_KEY_PANEL_COL_WIDTH_TYPE                   "/widthType"
#define SETTINGS_KEY_PANEL_COL_WIDTH_SIZE                   "/widthSize"
#define SETTINGS_KEY_PANEL_COL_WIDTH_DATE                   "/widthDate"
#define SETTINGS_KEY_PANEL_COL_WIDTH_OWNER                  "/widthOwner"
#define SETTINGS_KEY_PANEL_COL_WIDTH_PERMS                  "/widthPerms"
#define SETTINGS_KEY_PANEL_COL_WIDTH_ATTR                   "/widthAttr"



// Viewer

#define SETTINGS_KEY_VIEWER_WIDTH                           SETTINGS_GROUP_VIEWER"/width"
#define SETTINGS_KEY_VIEWER_HEIGHT                          SETTINGS_GROUP_VIEWER"/height"

#define SETTINGS_KEY_VIEWER_WORDWRAP                        SETTINGS_GROUP_VIEWER"/wordwrap"


#define DEFAULT_MIME_PREFIX_TEXT                            "text/"
#define DEFAULT_MIME_PREFIX_IMAGE                           "image/"
#define DEFAULT_MIME_PREFIX_AUDIO                           "audio/"
#define DEFAULT_MIME_PREFIX_VIDEO                           "video/"
#define DEFAULT_MIME_PREFIX_APP                             "application/"

#define DEFAULT_MIME_TEXT                                   "text"
#define DEFAULT_MIME_XML                                    "xml"
#define DEFAULT_MIME_SHELLSCRIPT                            "x-shellscript"
#define DEFAULT_MIME_JAVASCRIPT                             "javascript"
#define DEFAULT_MIME_SUBRIP                                 "x-subrip"


// Apps

#define SETTINGS_KEY_APPS_TERMINAL                          SETTINGS_GROUP_APPS"/terminal"
#define SETTINGS_KEY_APPS_VIEWER                            SETTINGS_GROUP_APPS"/viewer"
#define SETTINGS_KEY_APPS_EDITOR                            SETTINGS_GROUP_APPS"/editor"
#define SETTINGS_KEY_APPS_COMPARE                           SETTINGS_GROUP_APPS"/compare"
#define SETTINGS_KEY_APPS_PACKER                            SETTINGS_GROUP_APPS"/packer"
#define SETTINGS_KEY_APPS_UNPACKER                          SETTINGS_GROUP_APPS"/unpacker"

// Default Settings


// Constants

// Default Transparent Color
#define DEFAULT_TRANSPARENT_COLOR                           "#00000000"

#define DEFAULT_PANEL_FOCUSED_STYLE_SHEET                   "background-color: #11000077"
#define DEFAULT_PANEL_TRASPARENT_STYLE_SHEET                "background-color: "DEFAULT_TRANSPARENT_COLOR

#define DEFAULT_AVAILABLE_SPACE_FORMAT_STRING               "%1 files, %2 of %3 Kbytes free"

#define DEFAULT_MAIN_CONTROLLER_NAME                        "mainController"
#define DEFAULT_FILE_LIST_MODEL_NAME                        "fileListModel"
#define DEFAULT_IMAGE_VIEWER_CONTROLLER                     "viewerController"
#define DEFAULT_IMAGE_VIEWER_CONTENT                        "viewerContent"
#define DEFAULT_IMAGE_BROWSER                               "imageBrowser"
#define DEFAULT_CLIENT_LIST_MODEL_NAME                      "clientListModel"
#define DEFAULT_PREVIEW_CONTROLLER                          "previewController"
#define DEFAULT_SEARCH_RESULT_CONTROLLER                    "searchResultController"
#define DEFAULT_SEARCH_RESULT_MODEL                         "searchResultModel"

#define DEFAULT_FILE_ICON_PROVIDER_ID                       "fileicon"
#define DEFAULT_AUDIO_TAG_PROVIDER_ID                       "audiotag"

// Bits Per Color Components
#define DEFAULT_BITS_PER_COMPONENTS                         8

// Default Icon Get Retry Count Max
#define DEFAULT_ICOM_GET_RETRY_COUNT_MAX                    5

// Default Root Shell Command Template
#define DEFAULT_ROOT_SHELL_COMMAND_TEMPLATE                 "echo \"%1\" | sudo -S %2 &"

// Default PS Command Line To Check If File Serverr Is Running
#define DEFAULT_PS_COMMAND_CHECK_FILESERVER                 "ps x"

// Default File Server Launch Delay
#define DEFAULT_FILE_SERVER_LAUNCH_DELAY                    200

// Date Format String
#define DEFAULT_DATE_FORMAT_STRING                          "%1-%2-%3 %4:%5:%6"

// Default Icon Sizes
#define DEFAULT_ICON_WIDTH_16                               16
#define DEFAULT_ICON_HEIGHT_16                              16

#define DEFAULT_ICON_WIDTH_32                               32
#define DEFAULT_ICON_HEIGHT_32                              32

#define DEFAULT_ICON_WIDTH_64                               64
#define DEFAULT_ICON_HEIGHT_64                              64


#define DEFAULT_ONE_SEC                                     1000
#define DEFAULT_ANIM_FRAME_RATE                             30
#define DEFAULT_ANIM_PIECES_MAX                             3


#define DEFAULT_AUDIO_TAG_IMAGE_WIDTH                       256
#define DEFAULT_AUDIO_TAG_IMAGE_HEIGHT                      256


#define DEFAULT_STATUS_BAR_MESSAGE_TIMEOUT                  3000

// Progress Dialogs

#define DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_OP             30
#define DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE           40
#define DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN                 4
#define DEFAULT_PROGRESS_DIALOG_SCROLLBAR_WIDTH             10

#define DEFAULT_PROGRESS_DIALOG_SEMI_TRANSPARENCY           0.4


#define DEFAULT_CONFIRM_BUTTON_MINIMAL_WIDTH                80


// Icons
#define DEFAULT_ICON_PATH_OK                                ":/resources/images/check_256.png"
#define DEFAULT_ICON_PATH_CANCEL                            ":/resources/images/delete_256.png"
#define DEFAULT_ICON_PATH_PROGRESS                          ":/resources/images/synchronize_256.png"


// Exec Command
#define DEFAULT_EXEC_APP_SYSTEM_COMMAND_MAC_OSX             "open -a \"%1\" &"

// Open Files
#define DEFAULT_OPEN_FILE_SYSTEM_COMMAND_MAC_OSX            "open \"%1\" &"


// Localization Constants

#define DEFAULT_ERROR_TITLE_DELETE_FILE                     "File Deleting Error"
#define DEFAULT_ERROR_TITLE_COPY_FILE                       "File Copy Error"
#define DEFAULT_ERROR_TITLE_MOVE_FILE                       "File Rename/Move Error"
#define DEFAULT_ERROR_TITLE_SCAN_DIR                        "Dir Scanning Error"

#define DEFAULT_ERROR_TEXT_FILE_DOESNT_EXIST                "File Doesn't Exist:"

#define DEFAULT_TITLE_CONFIRMATION                          "Confirmation"
#define DEFAULT_TITLE_COPY_FILES                            "Copy File(s)"
#define DEFAULT_TITLE_MOVE_FILES                            "Move/Rename File(s)"

#define DEFAULT_CONFIRM_BUTTON_TEXT_YES                     "Yes"
#define DEFAULT_CONFIRM_BUTTON_TEXT_YESTOALL                "Yes to All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_NO                      "No"
#define DEFAULT_CONFIRM_BUTTON_TEXT_NOTOALL                 "No to All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_SKIP                    "Skip"
#define DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL                 "Skip All"
#define DEFAULT_CONFIRM_BUTTON_TEXT_RETRY                   "Retry"
#define DEFAULT_CONFIRM_BUTTON_TEXT_ASADMIN                 "As Admin"


#define DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS               "Directory Exists:"
#define DEFAULT_CONFIRM_TEXT_DIRECTORY_NOT_EMPTY            "Directory: %1 Is Not Empty. Delete All?"
#define DEFAULT_CONFIRM_TEXT_DIRECTORY_EXISTS_MERGE         "Target Directory Exists, Merge?"
#define DEFAULT_CONFIRM_TEXT_TARGET_FILE_EXISTS             "Target File Exists: %1 Overwrite?"
#define DEFAULT_ERROR_TEXT_CANT_CREATE_DIRECTORY            "Can Not Create Directory:"
#define DEFAULT_ERROR_TEXT_CANT_DELETE_FILE                 "Can Not Delete File:"

#define DEFAULT_ERROR_TEXT_CANT_COPY_FILE                   "Can Not Copy File:"
#define DEFAULT_ERROR_TEXT_CANT_MOVE_FILE                   "Can Not Rename/Move File:"

#define DEFAULT_ERROR_TEXT_CANT_SCAN_DIR                    "Can Not Scan Directory:"

#define DEFAULT_ERROR_TEXT_CANT_DELETE_SOURCE               "Can Not Delete Source:"
#define DEFAULT_ERROR_TEXT_CANT_DELETE_TARGET               "Can Not Delete Target:"

#define DEFAULT_CONFIRM_TEXT_TARGET_DIR_DOESNT_EXIST        "Target Dir Doesn\'t Exist. Create?"

#define DEFAULT_PROGRESS_DIALOG_FILENAME_TEXT_TEMPLATE      "%1 @ %2 KB/s"


#define DEFAULT_TITLE_VIEWER                                "Viewer - "

#define DEFAULT_TITLE_SAVE_NEW_FILE                         "Save New File"
#define DEFAULT_TITLE_SAVE_FILE                             "Save File"

#define DEFAULT_TITLE_INFO                                  "Information"
#define DEFAULT_TITLE_WARNING                               "Warning"
#define DEFAULT_TITLE_ERROR                                 "Error"


#define DEFAULT_TEXT_WORD_WRAP_ON                           "Wrap On"
#define DEFAULT_TEXT_WORD_WRAP_OFF                          "Wrap Off"


#define DEFAULT_WARNING_TEXT_CANT_CREATE_NEW_FILE           "Could not Create New File!"
#define DEFAULT_WARNING_TEXT_UNSUPPORTED_FILE_FORMAT        "Unsupported File Format!"


#define DEFAULT_TITLE_FONT_SIZE                             "size: "

#define DEFAULT_BUTTON_TEXT_START                           "Start"

#define DEFAULT_LABEL_CURRENT_FILE_TITLE                    "Current File:"
#define DEFAULT_LABEL_CURRENT_FILE_TITLE_FINISHED           "Finished"


#endif // CONSTANTS_H

