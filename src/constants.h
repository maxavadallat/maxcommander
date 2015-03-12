#ifndef CONSTANTS_H
#define CONSTANTS_H


// Default Values
#define MAX_NUMBER_OF_FILE_THREADS                      8


// Application Info

#define DEFAULT_APPLICATION_NAME                        "MaxCommander"
#define DEFAULT_ORGANIZATION_NAME                       "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                     "mysoft.com"


// QML Context Properties

#define MAIN_VIEW_CONTROLLER                            "mainViewController"

#define LIST_MODEL_LEFT_PANE                            "leftPaneModel"
#define LIST_MODEL_RIGHT_PANE                           "rightPaneModel"


// Settings Groups

#define SETTINGS_GROUP_MAIN                             "main"
#define SETTINGS_GROUP_LEFTPANEL                        "leftpanel"
#define SETTINGS_GROUP_RIGHTPANEL                       "rightpanel"
#define SETTINGS_GROUP_UI                               "ui"


// Settings Keys


#define SETTINGS_KEY_MAIN_MAXIMIZED                     SETTINGS_GROUP_MAIN"/maximized"

#define SETTINGS_KEY_MAIN_POSX                          SETTINGS_GROUP_MAIN"/posx"
#define SETTINGS_KEY_MAIN_POSY                          SETTINGS_GROUP_MAIN"/posy"

#define SETTINGS_KEY_MAIN_WIDTH                         SETTINGS_GROUP_MAIN"/width"
#define SETTINGS_KEY_MAIN_HEIGHT                        SETTINGS_GROUP_MAIN"/height"

#define SETTINGS_KEY_MAIN_SPLITTER                      SETTINGS_GROUP_UI"/mainSplitter"

#define SETTINGS_KEY_THUMBS_WIDTH                       SETTINGS_GROUP_UI"/thumbsWidth"
#define SETTINGS_KEY_THUMBS_HEIGHT                      SETTINGS_GROUP_UI"/thumbsHeiight"





// Default Settings




// Constants

#define DEFAULT_PANEL_NAME_LEFT                         "LEFT_PANEL"
#define DEFAULT_PANEL_NAME_RIGHT                        "RIGHT_PANEL"

#define DEFAULT_PANEL_FOCUSED_STYLE_SHEET               "background-color: #11000077"
#define DEFAULT_PANEL_TRASPARENT_STYLE_SHEET            "background-color: #00000000"

#define DEFAULT_AVAILABLE_SPACE_FORMAT_STRING           "%1 of %2 Kbytes free"

#define DEFAULT_MAIN_CONTROLLER_NAME                    "mainController"

#define DEFAULT_FILE_ICON_PROVIDER_ID                   "fileicon"


#endif // CONSTANTS_H

