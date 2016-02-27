import QtQuick 2.0
import MyCustomComponents 1.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileDeleteListRoot

    color: mainController.archiveMode ? Utility.colorWithAlpha(globalSettings.archiveColor, Const.DEFAULT_ARCHIVE_MODE_BACKGROUND_ALPHA) : globalSettings.textBGColor

    width: 480
    height: 640

    Connections {
        target: mainController

        onCurrentIndexChanged: {
            if (fileDeleteListView.currentIndex != mainController.currentIndex) {
                // Set Current Index
                fileDeleteListView.currentIndex = mainController.currentIndex;
            }
        }
    }

    ListView {
        id: fileDeleteListView
        anchors.fill: parent

        spacing: 1
        clip: true

        snapMode: ListView.SnapToItem

        focus: true

        property int delegateHeight: globalSettings.thumbHeight
        property int visualItemsCount: Math.floor((fileDeleteListView.height + fileDeleteListView.spacing) / fileDeleteListView.delegateHeight);
        property int prevIndex: -1

        highlightMoveDuration: 0

        model: fileDeleteListModel

        delegate: FileDeleteListDelegate {
            id: fileTransferListDelegate

            width: fileDeleteListView.width
            height: fileDeleteListView.delegateHeight

            fileNameText: fileName

            fileDirectory: fileIsDir
            fileHidden: fileIsHidden
            fileSymLink: fileIsLink
            fileArchive: fileIsArchive

            status: fileOpStatus

            itemIndex: index
            listIndex: fileDeleteListView.currentIndex

            // File Icon Source
            fileIconSource: {
                // Check Global Settings
                if (globalSettings.useDefaultIcons) {
                    // Check If Is Dir
                    if (fileIsDir) {
                       // Check Size
                       if (fileDeleteListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                           return Const.DEFAULT_FILE_LIST_ICON_DIR_SMALL;
                       }

                       return Const.DEFAULT_FILE_LIST_ICON_DIR;
                    }

                    // Check Size
                    if (fileDeleteListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                        return Const.DEFAULT_FILE_LIST_ICON_FILE_SMALL;
                    }

                    return Const.DEFAULT_FILE_LIST_ICON_FILE;
                }

                // Check Archive Mode
                if (mainController.archiveMode) {
                    // Check If File Is Dir
                    if (fileIsDir) {
                        return Const.DEFAULT_FILE_ICON_PREFIX + Const.DEFAULT_FILE_ICON_DEFAULT_DIR;
                    }

                    return Const.DEFAULT_FILE_ICON_PREFIX + Const.DEFAULT_FILE_ICON_DEFAULT_FILE;
                }

                // Check File Name
                if (Utility.isImage(fileName, mainController)) {
                    return Const.DEFAULT_FILE_PREFIX + fileName;
                }

                // Image Provider
                return Const.DEFAULT_FILE_ICON_PREFIX + fileName;
            }

            // ...

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    mainController.panelHasFocus = true;
                }

                onReleased: {

                }

                onClicked: {
                    //mainController.currentIndex = index;
                }
            }
        }

        // Highlight
        highlight: FileListHightLight {
            width: fileDeleteListView.delegate.width
            height: fileDeleteListView.delegate.height
        }

        Keys.onPressed: {
            //console.log("fileDeleteListView.Keys.onPressed - key: " + event.key);

        }

        Keys.onReleased: {
            //console.log("fileDeleteListView.Keys.onReleased - key: " + event.key);

        }
    }
}
