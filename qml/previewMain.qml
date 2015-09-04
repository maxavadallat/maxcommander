import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: previewRoot

    width: 180
    height: 300

    //color: Const.DEFAULT_FILE_LIST_BACKGROUND_COLOR
    color: previewController.textBGColor

    // Preview List Model
    ListModel {
        id: previewModel

        ListElement {
            fileIcon: "/tmp"
            fileName: "Normal Item"
            fileSelected: false
            fileHidden: false
            fileCurrent: false
            fileLink: false
            fileDir: true
            fileArchive: false
        }

        ListElement {
            fileIcon: "/tmp"
            fileName: "Link Item"
            fileSelected: false
            fileHidden: false
            fileCurrent: false
            fileLink: true
            fileDir: true
            fileArchive: false
        }

        ListElement {
            fileIcon: "/.file"
            fileName: "Current Item"
            fileSelected: false
            fileHidden: false
            fileCurrent: true
            fileLink: false
            fileDir: false
            fileArchive: false
        }

        ListElement {
            fileIcon: "/.file"
            fileName: "Selected Item"
            fileSelected: true
            fileHidden: false
            fileCurrent: false
            fileLink: false
            fileDir: false
            fileArchive: false
        }

        ListElement {
            fileIcon: "/.file"
            fileName: "Current Selected Item"
            fileSelected: true
            fileHidden: false
            fileCurrent: true
            fileLink: false
            fileDir: false
            fileArchive: false
        }

        ListElement {
            fileIcon: "/.file"
            fileName: "Hidden Item"
            fileSelected: false
            fileHidden: true
            fileCurrent: false
            fileLink: false
            fileDir: false
            fileArchive: false
        }

        ListElement {
            fileIcon: "/.file"
            fileName: "Archive Item"
            fileSelected: false
            fileHidden: false
            fileCurrent: false
            fileLink: false
            fileDir: false
            fileArchive: true
        }

    }

    // Font Loader
    FontLoader {
        id: delegateFont
        name: previewController.fontName
    }

    // Preview List
    ListView {
        id: previewList
        anchors.fill: parent
        spacing: 1
        clip: true
        snapMode: ListView.SnapToItem

        //focus: true

        //highlightMoveDuration: 50
        highlightFollowsCurrentItem: true

        currentIndex: 2

        model: previewModel

        delegate: Rectangle {
            id: delegateRoot

            width: previewList.width
            height: previewController.thumbHeight

            color: "transparent"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 4
                anchors.rightMargin: 4

                // Icon
                Image {
                    id: delegateImage
                    anchors.verticalCenter: parent.verticalCenter
                    width: previewController.thumbWidth
                    height: previewController.thumbHeight
                    fillMode: Image.PreserveAspectFit
                    smooth: false
                    source: {
                        // Check Use Default Icons
                        if (previewController.useDefaultIcons) {
                            // Check If Dir
                            if (fileDir) {
                                // Check Size
                                if (previewController.thumbHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                                    return Const.DEFAULT_FILE_LIST_ICON_DIR_SMALL;
                                }

                                return Const.DEFAULT_FILE_LIST_ICON_DIR;
                            }

                            // Check Size
                            if (previewController.thumbHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                                return Const.DEFAULT_FILE_LIST_ICON_FILE_SMALL;
                            }

                            return Const.DEFAULT_FILE_LIST_ICON_FILE;
                        }

                        return Const.DEFAULT_FILE_ICON_PREFIX + fileIcon;
                    }
                }

                Rectangle {
                    id: spacer
                    width: 4
                    height: previewController.thumbHeight
                    color: "transparent"
                }

                // File Name
                Text {
                    id: delegate
                    width: previewList.width - 4 - 4 - 4
                    height: previewController.thumbHeight
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: fileName
                    font.family: delegateFont.name
                    font.pixelSize: previewController.fontSize
                    font.bold: previewController.fontBold
                    font.italic: previewController.fontItalic
                    color: {
                        // Check If File Selected
                        if (fileSelected && fileCurrent) {
                            return previewController.currentSelectedColor;
                        }

                        if (fileSelected) {
                            return previewController.selectedColor;
                        }

                        if (fileCurrent || index == previewList.currentIndex) {
                            return previewController.currentColor;
                        }

                        if (fileLink) {
                            return previewController.linkColor;
                        }

                        if (fileHidden) {
                            return previewController.hiddenColor;
                        }

                        if (fileArchive) {
                            return previewController.archiveColor;
                        }

                        return previewController.textColor;
                    }
                }
            }
        }

        highlight: Rectangle {
            width: previewList.width
            height: previewController.thumbHeight
            //color: "#44444400"
            color: previewController.currentBGColor
        }
    }
}

