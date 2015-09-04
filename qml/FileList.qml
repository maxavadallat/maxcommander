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
        opacity: mainController.gridMode ? 0.0 : 1.0
        onRightClicked: {
            // Check If In Search Result Mode
            if (!mainController.searchResultsMode) {
                //console.log("fileListHeader.onRightClicked - posX: " + posX + " - posY: " + posY);

                // Set File List header Popup Pos
                fileListHeaderPopup.x = Math.min(posX, fileListRoot.width - fileListHeaderPopup.width - 1);
                fileListHeaderPopup.y = posY - 4;

                // Show Header Popup
                fileListHeaderPopup.showPopup();
            }
        }
    }

    // File List Header Popup
    FileListHeaderPopup {
        id: fileListHeaderPopup
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
        focus: !mainController.gridMode
        highlightFollowsCurrentItem: true
        highlightMoveDuration: Const.DEFAULT_LIST_HIGHLIGHT_MOVE_DURATION
        highlightResizeDuration: 0
        snapMode: ListView.SnapToItem
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION * 2 } }
        opacity: mainController.gridMode ? 0.0 : 1.0
        //cacheBuffer: globalSettings.thumbHeight * 8
        property int delegateHeight: globalSettings.thumbHeight
        property int visualItemsCount: Math.floor((fileListView.height + fileListView.spacing) / fileListView.delegateHeight);
        property int prevIndex: -1

        Keys.onPressed: {
            //console.log("fileListView.Keys.onPressed - key: " + event.key);
        }

        Keys.onReleased: {
            // Switch Key
            switch (event.key) {
                case Qt.Key_Escape:
                    // Hide File List Header Popup
                    fileListHeaderPopup.hidePopup();
                    // Hide File List Item Popup
                    fileListItemPopup.hidePopup();
                break;

                case 16:
                    //console.log("fileListView.Keys.onReleased - key: POPUP - currentItem: " + fileListView.currentItem.y);

                    // Set File List Item Popup Pos
                    fileListItemPopup.x = Const.DEFAULT_FILE_LIST_ITEM_POPUP_POSX;
                    fileListItemPopup.y = Math.min(fileListView.currentItem.y, fileListRoot.height - fileListItemPopup.height - 1);

                    // Set Current Path
                    fileListItemPopup.currentItemPath = fileListModel.getFullPath(fileListView.currentIndex);

                    // Show Item Popup
                    fileListItemPopup.showPopup(true);

                break;

                default:
                    //console.log("fileListView.Keys.onReleased - key: " + event.key);

                break;
            }
        }

        // Model
        model: fileListModel

        // Delegate
        delegate: FileListDelegate {
            id: fileListDelegateRoot

            width: fileListView.width
            height: fileListView.delegateHeight

            fileIconSource: {
                // Check Grid Mode
                if (mainController.gridMode) {
                    return "";
                }

                // Check Global Settings
                if (globalSettings.useDefaultIcons) {
                    // Check If Is Dir
                    if (fileIsDir) {
                       // Check Size
                       if (fileListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                           return Const.DEFAULT_FILE_LIST_ICON_DIR_SMALL;
                       }

                       return Const.DEFAULT_FILE_LIST_ICON_DIR;
                    }

                    // Check Size
                    if (fileListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
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

                // Check Main Controller Search Result Mode
                if (mainController.searchResultsMode) {

                    // Check File Name
                    if (Utility.isImage(fileFullName, mainController)) {
                        return Const.DEFAULT_FILE_PREFIX + fileListModel.getFullPath(index);
                    }

                    // Image Provider
                    return Const.DEFAULT_FILE_ICON_PREFIX + fileListModel.getFullPath(index);

                } else {
                    // Check File Name
                    if (Utility.isImage(fileFullName, mainController)) {
                        return Const.DEFAULT_FILE_PREFIX + mainController.currentDir + "/" + fileFullName;
                    }

                    // Image Provider
                    return Const.DEFAULT_FILE_ICON_PREFIX + mainController.currentDir + "/" + fileFullName;
                }
            }

            fileNameText  : fileName
            fileExtText   : fileExt
            fileTypeText  : fileType
            fileSizeText  : dirSize != "0 B" ? dirSize : fileSize
            fileDateText  : fileDate
            fileOwnerText : fileOwner
            filePermsText : filePerms
            fileHidden    : fileIsHidden
            fileSelected  : fileIsSelected
            fileSymLink   : fileIsLink
            fileDirectory : fileIsDir
            fileArchive   : fileIsArchive

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
                id: listDelegateMouseArea
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
                    // Set Pressed Index
                    mainController.pressedIndex = index;

                    // Hide File List Header Popup
                    fileListHeaderPopup.hidePopup();
                    // Hide File List Item Popup
                    fileListItemPopup.hidePopup();
                }
                // On Released
                onReleased: {
                    //console.log("fileListDelegateRoot.MouseArea.onReleased - index: " + index);
                    // Reset Prevent Stealing
                    listDelegateMouseArea.preventStealing = false;
                }

                // Double Clicked
                onDoubleClicked: {
                    //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index);
                    // Check Pressed Buttons
                    if (listDelegateMouseArea.pressedButtons === Qt.LeftButton) {
                        //console.log("fileListDelegateRoot.MouseArea.onDoubleClicked - index: " + index + " - pressedButtons: " + delegateMouseArea.pressedButtons);
                        // Handle Item Selection
                        mainController.handleItemSelection();
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

    // Grid View
    GridView {
        id: fileGridView
        anchors.fill: parent
        clip: true
        focus: mainController.gridMode
        highlightFollowsCurrentItem: true
        highlightMoveDuration: Const.DEFAULT_LIST_HIGHLIGHT_MOVE_DURATION
        snapMode: GridView.SnapToRow
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION * 2 } }
        opacity: mainController.gridMode ? 1.0 : 0.0

        property int delegateWidth: globalSettings.gridThumbWidth
        property int delegateHeight: globalSettings.gridThumbHeight

        property int visibleRowCount: Math.floor(fileGridView.height / fileGridView.delegateHeight)
        property int visibleColCount: Math.floor(fileGridView.width / fileGridView.delegateWidth)

        cellWidth: delegateWidth
        cellHeight: delegateHeight

        // Grid Model
        model: fileListModel

        // Grid Delegate
        delegate: FileGridDelegate {
            id: fileGridDelegateRoot

            width: fileGridView.delegateWidth
            height: fileGridView.delegateHeight

            // File Name Text
            fileNameText: {
                // Check If Dir
                if (fileIsDir) {
                    return fileFullName;
                }

                return fileFullName + " - " + fileSize;

            }

            // File Icon Source
            fileIconSource: {
                // Check Grid Mode
                if (!mainController.gridMode) {
                    return "";
                }

                // Check Global Settings
                if (globalSettings.useDefaultIcons) {
                    // Check If Is Dir
                    if (fileIsDir) {
                       // Check Size
                       if (fileListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
                           return Const.DEFAULT_FILE_LIST_ICON_DIR_SMALL;
                       }

                       return Const.DEFAULT_FILE_LIST_ICON_DIR;
                    }

                    // Check Size
                    if (fileListView.delegateHeight < Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT) {
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

                // Check Main Controller Search Result Mode
                if (mainController.searchResultsMode) {

                    // Check File Name
                    if (Utility.isImage(fileFullName, mainController)) {
                        return Const.DEFAULT_FILE_PREFIX + fileListModel.getFullPath(index);
                    }

                    // Image Provider
                    return Const.DEFAULT_FILE_ICON_PREFIX + fileListModel.getFullPath(index);

                } else {
                    // Check File Name
                    if (Utility.isImage(fileFullName, mainController)) {
                        return Const.DEFAULT_FILE_PREFIX + mainController.currentDir + "/" + fileFullName;
                    }

                    // Image Provider
                    return Const.DEFAULT_FILE_ICON_PREFIX + mainController.currentDir + "/" + fileFullName;
                }
            }

            // Delegate Mouse Area
            MouseArea {
                id: gridDelegateMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                // On Clicked
                onClicked: {
                    //console.log("fileGridDelegateRoot.MouseArea.onClicked - index: " + index);
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
                    //console.log("fileGridDelegateRoot.MouseArea.onPressed - index: " + index);
                    // Set Pressed Index
                    mainController.pressedIndex = index;

                    // Hide File List Header Popup
                    fileListHeaderPopup.hidePopup();
                    // Hide File List Item Popup
                    fileListItemPopup.hidePopup();
                }
                // On Released
                onReleased: {
                    //console.log("fileGridDelegateRoot.MouseArea.onReleased - index: " + index);
                    // Reset Prevent Stealing
                    gridDelegateMouseArea.preventStealing = false;
                }

                // Double Clicked
                onDoubleClicked: {
                    //console.log("fileGridDelegateRoot.MouseArea.onDoubleClicked - index: " + index);
                    // Check Pressed Buttons
                    if (gridDelegateMouseArea.pressedButtons === Qt.LeftButton) {
                        //console.log("fileGridDelegateRoot.MouseArea.onDoubleClicked - index: " + index + " - pressedButtons: " + delegateMouseArea.pressedButtons);
                        // Handle Item Selection
                        mainController.handleItemSelection();
                    }
                }
            }
        }

        // Highlight
        highlight: FileListHightLight {
            width: fileGridView.delegate.width
            height: fileGridView.delegate.height
            radius: Const.DEFAULT_RADIUS
        }

        // Keys Pressed
        Keys.onPressed: {
            // Check Auto Repeat
            if (event.isAutoRepeat) {
                // Switch Key
                switch (event.key) {
                    default:
                    break;

                    case Qt.Key_PageUp: {
                        //console.log("fileGridView.Key.onPressed - PAGEUP");
                        // Loop
                        for (var i=0; i<fileGridView.visibleRowCount; i++) {
                            // Move Index Up
                            fileGridView.moveCurrentIndexUp();
                        }

                        // Position View
                        fileGridView.positionViewAtIndex(fileGridView.currentIndex, GridView.Beginning);

                    } break;

                    case Qt.Key_PageDown: {
                        //console.log("fileGridView.Key.onPressed - PAGEDOWN");
                        // Loop
                        for (var i=0; i<fileGridView.visibleRowCount; i++) {
                            // Move Index Down
                            fileGridView.moveCurrentIndexDown();
                        }

                        // Position View
                        fileGridView.positionViewAtIndex(fileGridView.currentIndex, GridView.End);

                    } break;
                }
            }
        }

        // Keys Released
        Keys.onReleased: {
            // Switch Key
            switch (event.key) {
                default:
                break;

                case Qt.Key_PageUp: {
                    //console.log("fileGridView.Key.onReleased - PAGEUP");
                    // Loop
                    for (var i=0; i<fileGridView.visibleRowCount; i++) {
                        // Move Index Up
                        fileGridView.moveCurrentIndexUp();
                    }

                    // Position View
                    fileGridView.positionViewAtIndex(fileGridView.currentIndex, GridView.Beginning);

                } break;

                case Qt.Key_PageDown: {
                    //console.log("fileGridView.Key.onReleased - PAGEDOWN");
                    // Loop
                    for (var i=0; i<fileGridView.visibleRowCount; i++) {
                        // Move Index Down
                        fileGridView.moveCurrentIndexDown();
                    }

                    // Position View
                    fileGridView.positionViewAtIndex(fileGridView.currentIndex, GridView.End);

                } break;
            }
        }

        // On Current Index Changed
        onCurrentIndexChanged: {
            // Check Loading
            if (mainController.loading) {
                // Reset Loading
                mainController.loading = false;

            } else {
                //console.log("fileGridView.onCurrentIndexChanged - currentIndex: " + fileGridView.currentIndex);

                // Check If Current Index Matches
                if (mainController.currentIndex != fileGridView.currentIndex) {
                    // Set Main Controller Current Index
                    mainController.currentIndex = fileGridView.currentIndex;
                }
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
                //console.log("fileRenamer.onAccepted - originalFileName: " + fileRenamer.originalFileName + " - fileName: " + fileRenamer.fileName);
                // Rename File
                mainController.renameFile(fileRenamer.originalFileName, fileRenamer.fileName);
            }

            // Check Grid Mode
            if (mainController.gridMode) {
                // Set Focus
                fileGridView.focus = true;
            } else {
                // Set Focus
                fileListView.focus = true;
            }
        }

        // On Rejected
        onRejected: {
            // Check Grid Mode
            if (mainController.gridMode) {
                // Set Focus
                fileGridView.focus = true;
            } else {
                // Set Focus
                fileListView.focus = true;
            }
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

    // Drop Area
    DropArea {
        id: dragArea
        anchors.fill: fileListView
        // On Entered
        onEntered: {
            //console.log("dragArea.onEntered");
            // Accept Event
            //drag.accept();

            // ...
        }

        // On Position Changed
        onPositionChanged: {
            //console.log("dragArea.onPositionChanged - pos:[ " + drag.x + ":" + drag.y + "]");

            // ...
        }

        // On Dropped
        onDropped: {
            // Accept
            drop.accept();
            //console.log("dragArea.onDropped - drop.source: " + drop.text);
            // Drag Dropped
            mainController.dragDropped(drop.text, drop.keys);

            // ...
        }

        // On Exited
        onExited: {
            //console.log("dragArea.onExited");

            // ...
        }
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

        property bool popupShown: false

        property int justSelected: -1

        property int lastHoverIndex: -1
        property int hoverIndex: -1

        property bool firstChange: false

        enabled: !mainController.gridMode

        // On Pressed
        onPressed: {
            // Get Pressed Item
            pressedItem = fileListView.itemAt(mouseX, mouseY + fileListView.contentY);

            //console.log("selectionMouseArea.onPressed - index: " + pressedItem.itemIndex);

            // Set First Change
            firstChange = true;
            // Reset Just Selected
            justSelected = -1;

            // Check Pressed Item
            if (pressedItem) {
                // Check If Selected
                if (!fileListModel.getSelected(pressedItem.itemIndex)) {
                    // Set selected
                    fileListModel.setSelected(pressedItem.itemIndex, true);
                    // set just Selected Index
                    justSelected = pressedItem.itemIndex;
                }

                // Set Hover Index
                hoverIndex = justSelected;
                // Set Last Hover Index
                lastHoverIndex = hoverIndex;
            }

            // Hide File List Item Popup
            fileListItemPopup.hidePopup();
        }

        // On Released
        onReleased: {
            // Get Released Item
            releasedItem = fileListView.itemAt(mouseX, mouseY + fileListView.contentY);

            //console.log("selectionMouseArea.onReleased - index: " + releasedItem.itemIndex);

            // Check Pressed Item
            if (pressedItem === releasedItem && pressedItem != undefined) {
                // Check Just Selected
                if (justSelected !== releasedItem.itemIndex && !fileListItemPopup.visible) {
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
            // Chekc If File List Popup Visible
            if (fileListItemPopup.visible) {
                return;
            }

            // Check Mouse X
            if (mouseX >= 0 && mouseX < fileListView.width && mouseY >= 0 && mouseY < fileListView.height) {
                // Check First Change
                if (firstChange) {
                    // Reset First Change
                    firstChange = false;

                    return;
                }

                //console.log("selectionMouseArea.onMouseYChanged - mouseY: " + mouseY);
                // Get Hover Item
                var hoverItem = fileListView.itemAt(mouseX, mouseY + fileListView.contentY);
                // Get Hover Item Index
                hoverIndex = hoverItem ? hoverItem.itemIndex : -1;

                // Check Last Hover Index
                if (lastHoverIndex != hoverIndex && hoverIndex != -1) {
                    //console.log("selectionMouseArea.onMouseYChanged - hoverIndex: " + hoverIndex);
                    // Set Last Hover Index
                    lastHoverIndex = hoverIndex;
                    // Set Selected
                    fileListModel.setSelected(hoverIndex, !fileListModel.getSelected(hoverIndex));
                }
            }
        }

        // On Press And Hold
        onPressAndHold: {
            // Check Pressed Item
            if (pressedItem != undefined && mouse.button === Qt.RightButton) {
                //console.log("selectionMouseArea.onPressAndHold - index: " + pressedItem.itemIndex + " - path: " + fileListModel.getFullPath(pressedItem.itemIndex));

                // Set File List Item Popup Pos
                fileListItemPopup.x = Math.min(mouseX, fileListRoot.width - fileListItemPopup.width - 1);
                fileListItemPopup.y = Math.min(mouseY, fileListRoot.height - fileListItemPopup.height - 1);

                // Set Current Path
                fileListItemPopup.currentItemPath = fileListModel.getFullPath(pressedItem.itemIndex);

                // Show Item Popup
                fileListItemPopup.showPopup(false);

                // Set Selected
                //fileListModel.setSelected(pressedItem.itemIndex, false);

                // ...


            }
        }
    }
/*
    // Item Popup Mouse Eater
    MouseArea {
        id: popupMouseEater
        anchors.fill: fileListView
        enabled: false
        visible: false
    }
*/
    // File List Item Popup
    FileListItemPopup {
        id: fileListItemPopup
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
            //console.log("fileListRoot.Connections.mainController.onCurrentIndexChanged - aIndex: " + aIndex);
            // Check List View Current Index
            if (fileListView.currentIndex != aIndex) {
                // Set File List View Current Index
                fileListView.currentIndex = aIndex;
            }

            // Check Grid View
            if (fileGridView.currentIndex != aIndex) {
                // Set Current Index
                fileGridView.currentIndex = aIndex;
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

        // On Launch File Rename
        onLaunchFileRename: {
            //console.log("fileListRoot.Connections.mainController.onLaunchFileRename");

            // Check Grid Mode
            if (mainController.gridMode) {
                // Set File Renamer Pos & Size
                fileRenamer.width = fileGridView.cellWidth;
                fileRenamer.height = Const.DEFAULT_FILE_LIST_ITEM_RENAMER_MINIMUM_HEIGHT;

                // Get Current Grid Item
                var currentGridItem = fileGridView.currentItem;

                fileRenamer.x = currentGridItem.mapToItem(fileListRoot).x + currentGridItem.width / 2 - fileRenamer.width / 2;
                fileRenamer.y = Math.min(currentGridItem.mapToItem(fileListRoot).y + currentGridItem.height - fileRenamer.height, fileListRoot.height - fileRenamer.height);
            } else {
                // Set File Renamer Pos & Size
                fileRenamer.width = fileListView.width - 4;
                fileRenamer.height = Math.max(fileListView.currentItem.height, Const.DEFAULT_FILE_LIST_ITEM_RENAMER_MINIMUM_HEIGHT);

                fileRenamer.x = fileListView.currentItem.mapToItem(fileListRoot).x;
                fileRenamer.y = Math.min(fileListView.currentItem.mapToItem(fileListRoot).y - (fileRenamer.height - fileListView.currentItem.height) / 2, fileListRoot.height - fileRenamer.height);
            }

            // Set Original File Name
            fileRenamer.originalFileName = fileListModel.getFileName(fileListView.currentIndex);
            // Set File Name
            fileRenamer.fileName = fileListModel.getFileName(fileListView.currentIndex);

            // Show Renamer
            fileRenamer.showRenamer();
        }

        // On Search Result Mode Changed
        onSearchResultsModeChanged: {
            //console.log("fileListRoot.Connections.mainController.onLaunchFileRename");

            // Update File List Header Layout
            updateFileListHeaderLayout();
        }

        // On Set List View Interactive
        onSetListViewInteractive: {
            //console.log("fileListRoot.Connections.mainController.onSetListViewInteractive - aInteractive: " + aInteractive);

            // Set Interactive
            fileListView.interactive = aInteractive;
        }

        // On Hide Popups
        onHidePopups: {
            //console.log("fileListRoot.Connections.mainController.onHidePopups");

            // Hide File List Header Popup
            fileListHeaderPopup.hidePopup();
            // Hide File List Item Popup
            fileListItemPopup.hidePopup();
        }

        // On Grid Mode Changed
        onGridModeChanged: {
            //console.log("fileListRoot.Connections.mainController.onGridModeChanged - gridMode: " + mainController.gridMode);

            // Check Grid Mode
            if (mainController.gridMode) {
                // Set Focus
                fileGridView.focus = true;
                // Set Focus
                fileListView.focus = false;
            } else {
                // Set Focus
                fileGridView.focus = false;
                // Set Focus
                fileListView.focus = true;
            }

            // ...
        }
    }
}

