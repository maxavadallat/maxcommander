import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListRoot

    color: "transparent"

    width: 480
    height: 640

    // List Header
    FileListHeader {
        id: fileListHeader
        width: parent.width
        height: Const.DEFAULT_FILE_LIST_HEADER_HEIGHT
    }

    // List View
    ListView {
        id: fileListView
        anchors.fill: parent
        anchors.topMargin: fileListHeader.height
        spacing: 1
        clip: true
        snapMode: ListView.SnapToItem
        focus: true
        highlightMoveDuration: 50

        model: ListModel {
            ListElement {
                fileName  : "FileName1"
                fileExt   : "ext1"
                fileType  : "Document"
                fileSize  : 123456
                fileDate  : "2015.03.12 14:35"
                fileOwner : "max"
            }

            ListElement {
                fileName  : "FileName2"
                fileExt   : "ext2"
                fileType  : "Document"
                fileSize  : 123456
                fileDate  : "2015.03.12 14:35"
                fileOwner : "max"
            }

            ListElement {
                fileName  : "FileName3"
                fileExt   : "ext3"
                fileType  : "Document"
                fileSize  : 123456
                fileDate  : "2015.03.12 14:35"
                fileOwner : "max"
            }

            ListElement {
                fileName  : "FileName4"
                fileExt   : "ext4"
                fileType  : "Document"
                fileSize  : 123456
                fileDate  : "2015.03.12 14:35"
                fileOwner : "max"
            }
        }

        delegate: FileListDelegate {
            id: fileListDelegateRoot
            width: fileListView.width
            height: 32
            fileIconSource: Const.DEFAULT_FILE_ICON_PREFIX + fileName
            fileNameText: fileName
            fileExtText:  fileExt
            fileTypeText: fileType
            fileSizeText: fileSize
            fileDateText: fileDate

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    console.log("fileListDelegateRoot.MouseArea.onClicked - index: " + index);

                    // Check Button
                    if (mouse.button === Qt.LeftButton) {
                        // Set Current Index
                        fileListView.currentIndex = index;
                        // Set Panel Has Focus
                        mainController.panelHasFocus = true;
                    }
                }

                onPressed: {
                    console.log("fileListDelegateRoot.MouseArea.onPressed - index: " + index);

                    // ...
                }

                onReleased: {
                    console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index);

                    // ...
                }
            }
        }

        highlight: FileListHightLight {
            width: fileListView.delegate.width
            height: fileListView.delegate.height
        }
    }


    // On Focus Changed
    onFocusChanged: {
        //console.log("fileListRoot.onFocusChanged");
    }


    // On Completed
    Component.onCompleted: {
        //console.log("fileListRoot.onCompleted");

    }

    // On Destruction
    Component.onDestruction: {
        //console.log("fileListRoot.onDestruction");

    }

    // Connections
    Connections {
        target: mainController

    }
}

