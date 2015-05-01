
# Target
TARGET                  = MaxCommander

# Application Version
VERSION                 = 0.0.1

# Template
TEMPLATE                = app

# Qt Modules/Config
QT                      += core gui
QT                      += quickwidgets
QT                      += qml quick
QT                      += widgets
QT                      += network
QT                      += webkitwidgets
QT                      += multimedia

macx: {
# Icon
ICON                    = resources/images/icons/MaxCommander.icns
} else {

}

# Sources
SOURCES                 += src/main.cpp \
                        src/mainwindow.cpp \
                        src/filepanel.cpp \
                        src/filelistmodel.cpp \
                        src/utility.cpp \
                        src/filelistwidget.cpp \
                        src/filelistimageprovider.cpp \
                        src/remotefileutilclient.cpp \
                        src/busyindicator.cpp \
                        src/createdirdialog.cpp \
                        src/deletefiledialog.cpp \
                        src/transferfiledialog.cpp \
                        src/confirmdialog.cpp \
                        src/transferprogressdialog.cpp \
                        src/deleteprogressdialog.cpp \
                        src/deleteprogressmodel.cpp \
                        src/transferprogressmodel.cpp \
                        src/aboutdialog.cpp \
                        src/preferencesdialog.cpp \
                        src/helpwindow.cpp \
                        src/audiotagimageprovider.cpp \
                        src/viewerwindow.cpp \
                        src/testdialog.cpp \
                        src/infodialog.cpp \
                        src/passworddialog.cpp \
                        src/colorpickerbutton.cpp

# Heders
HEADERS                 += src/constants.h \
                        src/mainwindow.h \
                        src/filepanel.h \
                        src/filelistmodel.h \
                        src/utility.h \
                        src/filelistwidget.h \
                        src/filelistimageprovider.h \
                        src/remotefileutilclient.h \
                        src/busyindicator.h \
                        src/createdirdialog.h \
                        src/deletefiledialog.h \
                        src/transferfiledialog.h \
                        src/confirmdialog.h \
                        src/transferprogressdialog.h \
                        src/deleteprogressdialog.h \
                        src/deleteprogressmodel.h \
                        src/transferprogressmodel.h \
                        src/aboutdialog.h \
                        src/preferencesdialog.h \
                        src/helpwindow.h \
                        src/audiotagimageprovider.h \
                        src/viewerwindow.h \
                        src/testdialog.h \
                        src/infodialog.h \
                        src/passworddialog.h \
                        src/colorpickerbutton.h \
                        src/defaultsettings.h

# Include Path
INCLUDEPATH             += \
                        /usr/local/include/mcw

# Forms
FORMS                   += \
                        ui/mainwindow.ui \
                        ui/filepanel.ui \
                        ui/createdirdialog.ui \
                        ui/deletefiledialog.ui \
                        ui/transferfiledialog.ui \
                        ui/confirmdialog.ui \
                        ui/transferprogressdialog.ui \
                        ui/deleteprogressdialog.ui \
                        ui/aboutdialog.ui \
                        ui/preferencesdialog.ui \
                        ui/helpwindow.ui \
                        ui/viewerwindow.ui \
                        ui/testdialog.ui \
                        ui/infodialog.ui \
                        ui/passworddialog.ui

# Resources
RESOURCES               += \
                        maxcommander.qrc \
                        maxcommanderimages.qrc

# Other Files
OTHER_FILES             += \
                        createdmg.sh

MCWORKERFILES.files     += mcworker
MCWORKERFILES.path      = Contents/MacOS

QMAKE_BUNDLE_DATA       += MCWORKERFILES


macx: {
# Libs
LIBS                    += -framework Carbon
} else {

}

# Output/Intermediate Dirs
OBJECTS_DIR             = ./objs
OBJMOC                  = ./objs
MOC_DIR                 = ./objs
UI_DIR                  = ./objs
RCC_DIR                 = ./objs


