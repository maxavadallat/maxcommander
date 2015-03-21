#ifndef CONSTANTS_H
#define CONSTANTS_H


// Default Values
#define MAX_NUMBER_OF_FILE_THREADS                          8


// Application Info

#define DEFAULT_APPLICATION_NAME                            "MaxCommander"
#define DEFAULT_ORGANIZATION_NAME                           "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                         "mysoft.com"


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


// Settings Keys

// Main

#define SETTINGS_KEY_MAIN_MAXIMIZED                         SETTINGS_GROUP_MAIN"/maximized"

#define SETTINGS_KEY_MAIN_POSX                              SETTINGS_GROUP_MAIN"/posx"
#define SETTINGS_KEY_MAIN_POSY                              SETTINGS_GROUP_MAIN"/posy"

#define SETTINGS_KEY_MAIN_WIDTH                             SETTINGS_GROUP_MAIN"/width"
#define SETTINGS_KEY_MAIN_HEIGHT                            SETTINGS_GROUP_MAIN"/height"


// UI

#define SETTINGS_KEY_MAIN_SPLITTER                          SETTINGS_GROUP_UI"/mainSplitter"

#define SETTINGS_KEY_THUMBS_WIDTH                           SETTINGS_GROUP_UI"/thumbsWidth"
#define SETTINGS_KEY_THUMBS_HEIGHT                          SETTINGS_GROUP_UI"/thumbsHeiight"

#define SETTINGS_KEY_FOCUSED_PANEL                          SETTINGS_GROUP_UI"/focusedPanel"



// Common

#define SETTINGS_KEY_SHOW_HIDDEN_FILES                      SETTINGS_GROUP_PANEL_COMMON"/showHidden"
#define SETTINGS_KEY_DIRFIRST                               SETTINGS_GROUP_PANEL_COMMON"/dirFirst"
#define SETTINGS_KEY_CASE_SENSITIVE                         SETTINGS_GROUP_PANEL_COMMON"/caseSensitive"

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



// Default Settings






// Constants


#define DEFAULT_PANEL_FOCUSED_STYLE_SHEET                   "background-color: #11000077"
#define DEFAULT_PANEL_TRASPARENT_STYLE_SHEET                "background-color: #00000000"

#define DEFAULT_AVAILABLE_SPACE_FORMAT_STRING               "%1 of %2 Kbytes free"

#define DEFAULT_MAIN_CONTROLLER_NAME                        "mainController"
#define DEFAULT_FILE_LIST_MODEL_NAME                        "fileListModel"

#define DEFAULT_FILE_ICON_PROVIDER_ID                       "fileicon"

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







#endif // CONSTANTS_H

