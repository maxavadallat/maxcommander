
# Target
TARGET = MaxCommander

# Application Version
VERSION = 0.0.1

# Application
TEMPLATE = app

# Qt Config
QT += core gui

macx: {
# Icon
ICON = resources/images/icons/maxcommander.icns
} else {
# Icon
ICON = resources/images/icons/maxcommander.png
}

# Defines
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# Sources
SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/headerresizearea.cpp \
    src/headeritem.cpp \
    src/headertitlearea.cpp \
    src/customheader.cpp \
    src/custompanel.cpp \
    src/customfilelist.cpp \
    src/listbox.cpp \
    src/listmodel.cpp \
    src/fileutils.cpp \
    src/settings.cpp \
    src/settingswindow.cpp \
    src/colorpickerbutton.cpp \
    src/createdirdialog.cpp \
    src/headerpopup.cpp \
    src/infodialog.cpp \
    src/confirmationdialog.cpp \
    src/searchdialog.cpp \
    src/copyprogressdialog.cpp \
    src/deleteprogressdialog.cpp \
    src/copydialog.cpp \
    src/viewerwindow.cpp

# Headers
HEADERS  += src/mainwindow.h \
    src/headerresizearea.h \
    src/headeritem.h \
    src/headertitlearea.h \
    src/customheader.h \
    src/constants.h \
    src/custompanel.h \
    src/customfilelist.h \
    src/listbox.h \
    src/listmodel.h \
    src/fileutils.h \
    src/settings.h \
    src/settingswindow.h \
    src/colorpickerbutton.h \
    src/createdirdialog.h \
    src/headerpopup.h \
    src/infodialog.h \
    src/confirmationdialog.h \
    src/searchdialog.h \
    src/copyprogressdialog.h \
    src/deleteprogressdialog.h \
    src/copydialog.h \
    src/viewerwindow.h

# Forms
FORMS    += ui/mainwindow.ui \
    ui/headerresizearea.ui \
    ui/headeritem.ui \
    ui/headertitlearea.ui \
    ui/customheader.ui \
    ui/custompanel.ui \
    ui/customfilelist.ui \
    ui/settingswindow.ui \
    ui/colorpickerbutton.ui \
    ui/createdirdialog.ui \
    ui/headerpopup.ui \
    ui/infodialog.ui \
    ui/confirmationdialog.ui \
    ui/searchdialog.ui \
    ui/copyprogressdialog.ui \
    ui/deleteprogressdialog.ui \
    ui/copydialog.ui \
    ui/viewerwindow.ui

macx: {
LIBS += -framework CoreServices -framework ApplicationServices
}

# Resources
RESOURCES += \
    maxcommander.qrc

RC_FILE = maxcommander.rc

UI_DIR = ./obj

MOC_DIR = ./obj

OBJECTS_DIR = ./obj

RCC_DIR = ./obj




