import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileListRoot

    color: Const.DEFAULT_FILE_LIST_BACKGROUND_COLOR

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
        anchors.bottomMargin: 1 // I don't no why the fuck fileListRoot is not properly sized X (
        spacing: 1
        clip: true
        snapMode: ListView.SnapToItem
        focus: true
        highlightMoveDuration: 50
        highlightFollowsCurrentItem: true

        property int delegateHeight: 32
        property int visualItemsCount: Math.floor((fileListView.height + fileListView.spacing) / fileListView.delegateHeight);

        // Model
        model: fileListModel

        // Delegate
        delegate: FileListDelegate {
            id: fileListDelegateRoot
            width: fileListView.width
            height: fileListView.delegateHeight

            fileIconSource: {
                // Check File Name
                if (Utility.isImage(fileFullName)) {
                    Const.DEFAULT_FILE_PREFIX + mainController.currentDir + "/" + fileFullName
                } else {
                    // Image Provider
                    Const.DEFAULT_FILE_ICON_PREFIX + mainController.currentDir + "/" + fileFullName
                }
            }

            fileNameText: fileName
            fileExtText : fileExt
            fileTypeText: fileType
            fileSizeText: fileSize
            fileDateText: fileDate
            fileHidden  : fileIsHidden
            fileSelected: fileIsSelected

            nameWidth   : fileListHeader.nameWidth
            extWidth    : fileListHeader.extWidth
            typeWidth   : fileListHeader.typeWidth
            sizeWidth   : fileListHeader.sizeWidth
            dateWidth   : fileListHeader.dateWidth
            ownerWidth  : fileListHeader.ownerWidth
            permsWidth  : fileListHeader.permsWidth
            attrWidth   : fileListHeader.attrWidth

            nameVisible : fileListHeader.nameVisible
            extVisible  : fileListHeader.extVisible
            typeVisible : fileListHeader.typeVisible
            sizeVisible : fileListHeader.sizeVisible
            dateVisible : fileListHeader.dateVisible
            ownerVisible: fileListHeader.ownerVisible
            permsVisible: fileListHeader.permsVisible
            attrVisible : fileListHeader.attrVisible

            //color: (Math.floor(index / 2) == index / 2) ? "transparent" : "#77FFFFFF"

            // Mouse Area
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    //console.log("fileListDelegateRoot.MouseArea.onClicked - index: " + index);

                    // Check Button
                    if (mouse.button === Qt.LeftButton) {
                        // Set Current Index
                        fileListView.currentIndex = index;
                        // Set Panel Has Focus
                        mainController.panelHasFocus = true;
                    }
                }

                onPressed: {
                    //console.log("fileListDelegateRoot.MouseArea.onPressed - index: " + index);

                    // ...
                }

                onReleased: {
                    //console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index);

                    // ...
                }

                onDoubleClicked: {
                    //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index);

                    // ...

                    // Handle Item Select
                    mainController.handleItemSelect();
                }
            }
        }

        // Highlight
        highlight: FileListHightLight {
            width: fileListView.delegate.width
            height: fileListView.delegate.height
        }

        // On Current Index Changed
        onCurrentIndexChanged: {
            //console.log("fileListView.onCurrentIndexChanged - currentIndex: " + fileListView.currentIndex);
            // Set Main Controller Current Index
            mainController.currentIndex = fileListView.currentIndex;
        }

        // On Visual Items Count Changed
        onVisualItemsCountChanged: {
            //console.log("fileListView.onVisualItemCountChanged - visualItemsCount: " + fileListView.visualItemsCount);
            // Set Main Controller Visual Items Count
            mainController.visualItemsCount = fileListView.visualItemsCount;
        }

        // On Width Changed
        onWidthChanged: {
            console.log("fileListView.onWidthChanged - width: " + fileListView.width);
            // Init Rest Of The Header Items Width
            //var restWidth = 0;
            // Set Name Width
            //fileListHeader.nameWidth = fileListView.width - restWidth;
        }
    }

    // On Focus Changed
    onFocusChanged: {
        //console.log("fileListRoot.onFocusChanged");
    }

    // On Completed
    Component.onCompleted: {
        //console.log("fileListRoot.onCompleted - visualItemsCount: " + fileListView.visualItemsCount);
        // Set Visual Items Count
        mainController.visualItemsCount = fileListView.visualItemsCount;
    }

    // On Destruction
    Component.onDestruction: {
        //console.log("fileListRoot.onDestruction");

    }

    // Connections
    Connections {
        target: mainController
        // On Current Index Changed
        onCurrentIndexChanged: {
            //console.log("fileListRoot.Connections.mainController.onCurrentIndexChanged - aIndex: " + aIndex);
            // Check List View Current Index
            if (fileListView.currentIndex != aIndex) {
                // Set File List View Current Index
                fileListView.currentIndex = aIndex;
                // Position View
                fileListView.positionViewAtIndex(aIndex + 1, ListView.Center);
            }
        }
    }
}

