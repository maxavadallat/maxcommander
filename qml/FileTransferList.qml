import QtQuick 2.0
import MyCustomComponents 1.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileTransferListRoot

    color: mainController.archiveMode ? Utility.colorWithAlpha(globalSettings.archiveColor, Const.DEFAULT_ARCHIVE_MODE_BACKGROUND_ALPHA) : globalSettings.textBGColor

    width: 480
    height: 640

    Connections {
        target: mainController

        onCurrentIndexChanged: {
            if (fileTransferListView.currentIndex != mainController.currentIndex) {
                // Set Current Index
                fileTransferListView.currentIndex = mainController.currentIndex;
            }
        }
    }

    ListView {
        id: fileTransferListView
        anchors.fill: parent

        spacing: 1
        clip: true

        snapMode: ListView.SnapToItem

        focus: true

        property int delegateHeight: globalSettings.thumbHeight
        property int visualItemsCount: Math.floor((fileTransferListView.height + fileTransferListView.spacing) / fileTransferListView.delegateHeight);
        property int prevIndex: -1

        highlightMoveDuration: 0

        model: fileTransferListModel

        delegate: FileTransferListDelegate {
            id: fileTransferListDelegate

            width: fileTransferListView.width
            height: fileTransferListView.delegateHeight

            sourceText: fileSource
            targetText: "==> " + fileTarget

            fileDirectory: fileIsDir
            fileHidden: fileIsHidden
            fileSymLink: fileIsLink
            fileArchive: fileIsArchive

            opStatus: fileOpStatus

            itemIndex: index
            listIndex: fileTransferListView.currentIndex

            // File Icon Source
            fileIconSource: {
                // Check Global Settings
                if (globalSettings.useDefaultIcons) {
                    // Check If Is Dir
                    if (fileIsDir) {
                       // Check Size
                       if (fileTransferListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                           return Const.DEFAULT_FILE_LIST_ICON_DIR_SMALL;
                       }

                       return Const.DEFAULT_FILE_LIST_ICON_DIR;
                    }

                    // Check Size
                    if (fileTransferListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
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

                // TODO: Get Proper File Name, Source Or Target when Moving and Operation Status is Done

                // Check File Name
                if (Utility.isImage(fileSource, mainController)) {
                    return Const.DEFAULT_FILE_PREFIX + fileSource;
                }

                // Image Provider
                return Const.DEFAULT_FILE_ICON_PREFIX + fileSource;
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
            width: fileTransferListView.delegate.width
            height: fileTransferListView.delegate.height
        }

        Keys.onPressed: {
            //console.log("fileTransferListView.Keys.onPressed - key: " + event.key);

        }

        Keys.onReleased: {
            //console.log("fileTransferListView.Keys.onReleased - key: " + event.key);

        }
    }
}
