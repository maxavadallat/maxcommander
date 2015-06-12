import QtQuick 2.0
import QtGraphicalEffects 1.0
import MyCustomComponents 1.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileListRoot

    color: globalSettings.textBGColor

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
        // On Item Visibility Changed
        onItemVisibilityChanged: {
            // Update Header Layout
            updateFileListHeaderLayout();
        }
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

        cacheBuffer: globalSettings.thumbHeight * 8

        property int delegateHeight: globalSettings.thumbHeight
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
                // Check Global Settings
                if (globalSettings.useDefaultIcons) {
                    // Check If Is Dir
                    if (fileIsDir) {
                       return Const.DEFAULT_FILE_LIST_ICON_DIR;
                    }

                    return Const.DEFAULT_FILE_LIST_ICON_FILE;

                } else {
                    // Check File Name
                    if (Utility.isImage(fileFullName, mainController)) {
                        return Const.DEFAULT_FILE_PREFIX + mainController.currentDir + "/" + fileFullName;
                    }

                    // Image Provider
                    return Const.DEFAULT_FILE_ICON_PREFIX + mainController.currentDir + "/" + fileFullName;
                }
            }

            // Behavior
            Behavior on fileIconSource {
            }

            fileNameText  : fileName
            fileExtText   : fileExt
            fileTypeText  : fileType
            fileSizeText  : dirSize > 0 ? dirSize : fileSize
            fileDateText  : fileDate
            fileOwnerText : fileOwner
            filePermsText : filePerms
            fileHidden    : fileIsHidden
            fileSelected  : fileIsSelected
            fileDirSize   : dirSize
            fileSymLink   : fileIsLink

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

            itemIndex   : index
            listIndex   : fileListView.currentIndex

            // Mouse Area
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
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
                }
                // On Released
                onReleased: {
                    //console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index);
                    // Reset Prevent Stealing
                    delegateMouseArea.preventStealing = false;
                }

                // Double Clicked
                onDoubleClicked: {
                    //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index);
                    // Check Pressed Buttons
                    if (delegateMouseArea.pressedButtons === Qt.LeftButton) {
                        //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index + " - pressedButtons: " + delegateMouseArea.pressedButtons);
                        // Handle Item Selection
                        mainController.handleItemSelection();
                    }
                }
            }
        }

        // Highlight
        highlight: FileListHightLight {
            id: itemHighlight
            width: fileListView.delegate.width
            height: fileListView.delegate.height
        }

        // On Current Index Changed
        onCurrentIndexChanged: {
            // Check Loading
            if (mainController.loading) {
                //console.log("fileListView.onCurrentIndexChanged - currentIndex: " + fileListView.currentIndex + " - LOADING IS ON!!");
                // Reset List View Current Index - Because ListView Fucked Up!!! Sets Weird Values Sometimes for now fucking Reason... X (((
                fileListView.currentIndex = mainController.currentIndex;
                // Reset Loading
                mainController.loading = false;

            } else {
                //console.log("fileListView.onCurrentIndexChanged - currentIndex: " + fileListView.currentIndex);

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

            // Update File List Header Layout
            updateFileListHeaderLayout();
        }

        // Scroll Bar
        FileListScrollBar {
            id: scrollBar
            parentVisibleSize:  fileListView.height - 4;
            parentContentSize:  fileListView.contentHeight;
            parentContentPos:   fileListView.contentY;
            // On Scroll Y Changed
            onScrollYChanged: {
                //console.log("fileListRoot.scrollBar.onScrollYChanged - posY: " + posY);
                // Calculate Content Position
                var contentPos = posY * (fileListView.contentHeight - (fileListView.height - 4)) / (fileListView.height - 4 - scrollBar.height);

                //console.log("fileListRoot.scrollBar.onScrollYChanged - contentPos: " + contentPos);

                // ...

                // Set Content Y
                fileListView.contentY = Math.max(0, Math.min(contentPos, fileListView.contentHeight - fileListView.height));
            }
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

    // Right Mouse Area
    MouseArea {
        id: selectionMouseArea
        anchors.fill: fileListView
        acceptedButtons: Qt.RightButton
        hoverEnabled: false
        preventStealing: true

        property variant pressedItem: undefined
        property variant releasedItem: undefined

        property int justSelected: -1

        property int lastHoverIndex: -1
        property int hoverIndex: -1

        // On Pressed
        onPressed: {
            // Get Pressed Item
            pressedItem = fileListView.itemAt(mouseX, mouseY);
            //console.log("selectionMouseArea.onPressed - index: " + pressedItem.itemIndex);
        }

        // On Released
        onReleased: {
            // Get Released Item
            releasedItem = fileListView.itemAt(mouseX, mouseY);
            //console.log("selectionMouseArea.onReleased - index: " + releasedItem.itemIndex);
            // Check Pressed Item
            if (pressedItem === releasedItem && pressedItem != undefined ) {
                // Check Just Selected
                if (justSelected !== releasedItem.itemIndex) {
                    // Set Selected
                    fileListModel.setSelected(releasedItem.itemIndex, false);
                }
            }

            // Reset Just Selected
            justSelected = -1;
            // Reset Last Hover Index
            lastHoverIndex = -1;
            // Reset Hover Index
            hoverIndex = -1;
        }

        // On Mouse Y Changed
        onMouseYChanged: {
            // Check Mouse X
            if (mouseX >= 0 && mouseX < fileListView.width && mouseY >= 0 && mouseY < fileListView.height) {
                //console.log("selectionMouseArea.onMouseYChanged - mouseY: " + mouseY);
                // Get Hover Item
                var hoverItem = fileListView.itemAt(mouseX, mouseY);
                // Get Hover Item Index
                hoverIndex = hoverItem ? hoverItem.itemIndex : -1;
                //console.log("selectionMouseArea.onMouseYChanged - hoverIndex: " + hoverIndex);
                // Check Last Hover Index
                if (lastHoverIndex != hoverIndex) {
                    // Set Last Hover Index
                    lastHoverIndex = hoverIndex;
                    // Check Hover Index
                    if (hoverIndex != -1) {
                        //console.log("selectionMouseArea.onMouseYChanged - hoverIndex: " + hoverIndex);

                        // Check If Selected
                        if (justSelected === -1 && fileListModel.getSelected(hoverIndex)) {
                            // Set Just Selected Index
                            justSelected = hoverIndex;
                        } else {
                            // Set Selected
                            fileListModel.setSelected(hoverIndex, !fileListModel.getSelected(hoverIndex));
                            // Check Selected
                            if (fileListModel.getSelected(hoverIndex)) {
                                // Set Just Selected Index
                                justSelected = hoverIndex;
                            }
                        }
                    }
                }
            }
        }
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

        // ...

    }

    // Update File List Header Layout
    function updateFileListHeaderLayout() {
        //console.log("fileListRoot.updateFileListHeaderLayout");

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

        // Set Name Header Size Position
        fileListHeader.setNameHeaderSeparatorPosition(remainingWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
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
                //fileListView.positionViewAtIndex(aIndex + 1, ListView.Center);
            }
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
            fileRenamer.width = fileListView.width - 4;
            fileRenamer.height = Math.max(fileListView.currentItem.height, Const.DEFAULT_FILE_LIST_ITEM_RENAMER_MINIMUM_HEIGHT);

            fileRenamer.x = fileListView.currentItem.mapToItem(fileListRoot).x;
            fileRenamer.y = Math.min(fileListView.currentItem.mapToItem(fileListRoot).y - (fileRenamer.height - fileListView.currentItem.height) / 2, fileListRoot.height - fileRenamer.height);

            // Set Original File Name
            fileRenamer.originalFileName = fileListModel.getFileName(fileListView.currentIndex);
            // Set File Name
            fileRenamer.fileName = fileListModel.getFileName(fileListView.currentIndex);

            // Show Renamer
            fileRenamer.showRenamer();
        }
    }
}

