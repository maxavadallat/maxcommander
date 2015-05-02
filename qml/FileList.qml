import QtQuick 2.0
import QtGraphicalEffects 1.0
import MyCustomComponents 1.0

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
        onRightClicked: {
            console.log("fileListHeader.onRightClicked - posX: " + posX + " - posY: " + posY);

            // Set File List header Popup Pos
            fileListHeaderPopup.x = Math.min(posX, fileListRoot.width - fileListHeaderPopup.width - 1);
            fileListHeaderPopup.y = posY - 4;

            // Show Header Popup
            fileListHeaderPopup.showPopup();
        }
    }

    // File List Header Popup
    FileListHeaderPopup {
        id: fileListHeaderPopup
        opacity: 0.0
        z: 1.0
    }

    // List View
    ListView {
        id: fileListView
        anchors.fill: parent
        anchors.topMargin: fileListHeader.height
        anchors.bottomMargin: 1 // I don't no why the fuck fileListRoot is not properly sized.... X (
        spacing: 1
        clip: true

        focus: true

        highlightFollowsCurrentItem: true
        highlightMoveDuration: 0
        highlightResizeDuration: 0

        snapMode: ListView.SnapToItem

        property int delegateHeight: 32
        property int visualItemsCount: Math.floor((fileListView.height + fileListView.spacing) / fileListView.delegateHeight);
        property int prevIndex: -1

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
            fileSizeText: dirSize > 0 ? dirSize : fileSize
            fileDateText: fileDate
            fileHidden  : fileIsHidden
            fileSelected: fileIsSelected
            fileDirSize : dirSize

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

            // Mouse Area
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                property bool pressedInside: false
                // On Clicked
                onClicked: {
                    //console.log("fileListDelegateRoot.MouseArea.onClicked - index: " + index);

                    // Check Button
                    if (mouse.button === Qt.LeftButton) {
                        // Set Current Index
                        mainController.currentIndex = index;
                        // Set Panel Has Focus
                        mainController.panelHasFocus = true;
                    }
                }
                // On Pressed
                onPressed: {
                    //console.log("fileListDelegateRoot.MouseArea.onPressed - index: " + index);

                    // Check Mouse Button
                    if (mouse.button === Qt.RightButton) {
                        // Set Pressed Inside
                        delegateMouseArea.pressedInside = true;
                        // Set Prevent Stealing
                        delegateMouseArea.preventStealing = true;
                    }

                    // ...
                }
                // On Released
                onReleased: {
                    //console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index);

                    // Check Pressed Inside
                    if (delegateMouseArea.pressedInside && mouse.button === Qt.RightButton) {
                        // Reset Pressed Inside
                        delegateMouseArea.pressedInside = false;

                        // Check Mouse Position
                        if (mouse.x >= 0 && mouse.x < fileListDelegateRoot.width && mouse.y >= 0 && mouse.y < fileListDelegateRoot.height) {

                            //console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index + " - pos:[" + mouse.x + ":" + mouse.y + "]");

                            // Toggle File Selected
                            fileIsSelected = !fileIsSelected;
                        }
                    }

                    // Reset Prevent Stealing
                    delegateMouseArea.preventStealing = false;

                    // ...
                }

                // Double Clicked
                onDoubleClicked: {
                    // Check Pressed Buttons
                    if (delegateMouseArea.pressedButtons === Qt.LeftButton) {
                        //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index + " - pressedButtons: " + delegateMouseArea.pressedButtons);
                        // Handle Item Select
                        mainController.handleItemExec();
                    }
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

            // Check Loading
            if (mainController.loading) {
                // Reset List View Current Index - Because ListView Fucked Up!!! Sets Weird Values Sometimes for now fucking Reason... X (((
                fileListView.currentIndex = mainController.currentIndex;
                // Reset Loading
                mainController.loading = false;
            } else {
                // Check If Current Index Matches
                if (mainController.currentIndex != fileListView.currentIndex) {
                    // Set Main Controller Current Index
                    mainController.currentIndex = fileListView.currentIndex;
                }
            }

            // Check If Shift Pressed
            if (mainController.modifierKeys & Qt.ShiftModifier) {
                // Check Previous Index
                if (fileListView.prevIndex === -1) {
                    // Set Prev Index
                    fileListView.prevIndex = 0;
                }

                //console.log("fileListView.onCurrentIndexChanged - currentIndex: " + currentIndex + " - prevIndex: " + fileListView.prevIndex);

                // Check Previous Index
                if (fileListView.prevIndex < fileListView.currentIndex) {

                    // Go Thru Items From Previous Index To Current Index
                    for (var i=fileListView.prevIndex; i < fileListView.currentIndex && i >= 0 && i < fileListView.count;  i++) {
                        // Set Selected
                        fileListModel.setSelected(i, !fileListModel.getSelected(i));
                    }

                    // Check The Difference
                    if (fileListView.currentIndex - fileListView.prevIndex > 1) {
                        // Set Selected
                        fileListModel.setSelected(fileListView.currentIndex, !fileListModel.getSelected(fileListView.currentIndex));
                    }

                } else {

                    // Go Thru Items From Previous Index To Current Index
                    for (var i=fileListView.prevIndex; i > fileListView.currentIndex && i >= 0 && i < fileListView.count;  i--) {
                        // Set Selected
                        fileListModel.setSelected(i, !fileListModel.getSelected(i));
                    }

                    // Check The Difference
                    if (fileListView.prevIndex - fileListView.currentIndex > 1) {
                        // Set Selected
                        fileListModel.setSelected(fileListView.currentIndex, !fileListModel.getSelected(fileListView.currentIndex));
                    }

                }
            }

            // Set Previous Index
            fileListView.prevIndex = fileListView.currentIndex;
        }

        // On Visual Items Count Changed
        onVisualItemsCountChanged: {
            //console.log("fileListView.onVisualItemCountChanged - visualItemsCount: " + fileListView.visualItemsCount);
            // Set Main Controller Visual Items Count
            mainController.visualItemsCount = fileListView.visualItemsCount;
        }

        // On Width Changed
        onWidthChanged: {
            //console.log("fileListView.onWidthChanged - width: " + fileListView.width);

            // Init Remaining Width
            var remainingWidth = fileListView.width;

            // Check Header Visibility
            if (fileListHeader.extVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.extWidth;
            }

            // Check Header Visibility
            if (fileListHeader.typeVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.typeWidth;
            }

            // Check Header Visibility
            if (fileListHeader.sizeVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.sizeWidth;
            }

            // Check Header Visibility
            if (fileListHeader.dateVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.dateWidth;
            }

            // Check Header Visibility
            if (fileListHeader.ownerVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.ownerWidth;
            }

            // Check Header Visibility
            if (fileListHeader.permsVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.permsWidth;
            }

            // Check Header Visibility
            if (fileListHeader.attrVisible) {
                // Adjust Remaining Width
                remainingWidth -= fileListHeader.attrWidth;
            }

            //console.log("fileListView.onWidthChanged - width: " + fileListView.width + " - remainingWidth: " + remainingWidth);

            // Set Name Header Sizer Position
            fileListHeader.setNameHeaderSeparatorPosition(remainingWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
        }
    }

    // Shadow For File Renamer
    Rectangle {
       width: fileRenamer.width
       height: fileRenamer.height
       opacity: fileRenamer.opacity
       anchors.centerIn: fileRenamer
       anchors.horizontalCenterOffset: 4
       anchors.verticalCenterOffset: 4
       radius: fileRenamer.radius
       color: Const.DEFAULT_FILE_LIST_ITEM_RENAMER_SHADOW_COLOR
   }

    // File Renamer
    FileListItemRenamer {
        id: fileRenamer
        opacity: 0.0
        // On Accepted
        onAccepted: {
            // Check File Name
            if (fileName.length > 0) {
                console.log("fileRenamer.onAccepted - originalFileName: " + fileRenamer.originalFileName + " - fileName: " + fileRenamer.fileName);
                // Rename File
                mainController.renameFile(fileRenamer.originalFileName, fileRenamer.fileName);
            }

            // Set Focus
            fileListView.focus = true;
        }
        // On Rejected
        onRejected: {
            // Set Focus
            fileListView.focus = true;
        }
    }

    // Busy Indicator
    BusyIndicator {
        id: busyIndicator
        width: Const.DEFAULT_BUSY_INDICATOR_WIDTH
        height: Const.DEFAULT_BUSY_INDICATOR_HEIGHT
        anchors.right: parent.right
        anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Const.DEFAULT_MARGIN_WIDTH
        running: mainController.busy
        opacity: running ? 1.0 : 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    // Helper Timer
    Timer {
        id: helperTimer
        repeat: false
        interval: 100
        // On Triggered
        onTriggered: {
            // Set Highlight Move Duration
            fileListView.highlightMoveDuration = 100;
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

        // Set Prev Index
        fileListView.prevIndex = fileListView.currentIndex;

        // ...

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
            // Check List View Current Index
            if (fileListView.currentIndex != aIndex) {
                //console.log("fileListRoot.Connections.mainController.onCurrentIndexChanged - aIndex: " + aIndex);
                // Set File List View Current Index
                fileListView.currentIndex = aIndex;
                // Position View
                fileListView.positionViewAtIndex(aIndex + 1, ListView.Center);
            } else {
                //console.log("fileListRoot.Connections.mainController.onCurrentIndexChanged - aIndex: " + aIndex + " - MATCHES!");
            }

            // Loading Finishes When Proper Indexes Are Set.
            // For Some reason the Fucking ListView does set some weird value for currentIndex after loading

            // Reset Loading
            //mainController.loading = false;
        }

        // On Loading Changed
        onLoadingChanged: {
            //console.log("#### fileListRoot.Connections.mainController.onLoadingChanged - loading: " + mainController.loading);

            // Check Loading
            if (mainController.loading) {
                // Reset Highlight Move Duration
                fileListView.highlightMoveDuration = 0;
            } else {
                // Restart Helper Timer
                helperTimer.restart();
            }

            // ...
        }

        // On Modifier Keys Pressed
        onModifierKeysChanged: {
            // Check Modifier Keys
            if (aModifierKeys === 0) {
                //console.log("fileListRoot.Connections.mainController.onModifierKeysChanged - NO MODIFIER");
            }

            // Check Modifier Keys
            if (aModifierKeys & Qt.ShiftModifier) {
                //console.log("fileListRoot.Connections.mainController.onModifierKeysChanged - SHIFT");
            }

            // Check Modifier Keys
            if (aModifierKeys & Qt.AltModifier) {
                //console.log("fileListRoot.Connections.mainController.onModifierKeysChanged - ALT");
            }

            // Check Modifier Keys
            if (aModifierKeys & Qt.ControlModifier) {
                //console.log("fileListRoot.Connections.mainController.onModifierKeysChanged - CONTROL");
            }

            // Check Modifier Keys
            if (aModifierKeys & Qt.MetaModifier) {
                //console.log("fileListRoot.Connections.mainController.onModifierKeysChanged - META");
            }

            // ...
        }

        // On Launch File Rename
        onLaunchFileRename: {
            console.log("fileListRoot.Connections.mainController.onLaunchFileRename");

            // Set File Renamer Pos & Size
            fileRenamer.x = fileListView.currentItem.mapToItem(fileListRoot).x;
            fileRenamer.y = fileListView.currentItem.mapToItem(fileListRoot).y;
            fileRenamer.width = fileListView.width - 4;
            fileRenamer.height = fileListView.currentItem.height;

            // Set Original File Name
            fileRenamer.originalFileName = fileListModel.getFileName(fileListView.currentIndex);
            // Set File Name
            fileRenamer.fileName = fileListModel.getFileName(fileListView.currentIndex);

            // Show Renamer
            fileRenamer.showRenamer();
        }
    }
}

