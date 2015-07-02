import QtQuick 2.0

import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: dirHistoryRoot

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    radius: Const.DEFAULT_POPUP_RADIUS

    border.color: Const.DEFAULT_POPUP_BORDER_COLOR
    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH

    focus: true

    // Goto Prev Item
    function gotoPrev() {
        // Set Current Index
        dirHistoryList.currentIndex = Math.max(0, dirHistoryList.currentIndex - 1);
    }

    // Next Item
    function gotoNext() {
        // Set Current Index
        dirHistoryList.currentIndex = Math.min(dirHistoryList.count - 1, dirHistoryList.currentIndex + 1);
    }

    // Goto Home
    function gotoHome() {
        // Set Current Index
        dirHistoryList.currentIndex = 0;
    }

    // Goto End
    function gotoEnd() {
        // Set Current Index
        dirHistoryList.currentIndex = dirHistoryList.count - 1;
    }

    // Goto Page Up
    function gotoPageUp() {
        // Set Current Index
        dirHistoryList.currentIndex = Math.max(0, dirHistoryList.currentIndex - dirHistoryList.visualCount);
    }

    // Goto Page Down
    function gotoPageDown() {
        // Set Current Index
        dirHistoryList.currentIndex = Math.min(dirHistoryList.count - 1, dirHistoryList.currentIndex + dirHistoryList.visualCount);
    }

    Keys.onPressed: {
        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:
                if (event.isAutoRepeat) {
                    // Goto Prev
                    gotoPrev();
                }
            break;

            case Qt.Key_Down:
                if (event.isAutoRepeat) {
                    // Goto Next
                    gotoNext();
                }
            break;

            case Qt.Key_PageUp:
                if (event.isAutoRepeat) {
                    // Goto Page Up
                    gotoPageUp();
                }
            break;

            case Qt.Key_PageDown:
                if (event.isAutoRepeat) {
                    // Goto Page Down
                    gotoPageDown();
                }
            break;
        }
    }

    Keys.onReleased: {
        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:
                // Goto Prev
                gotoPrev();
            break;

            case Qt.Key_Down:
                // Goto Next
                gotoNext();
            break;

            case Qt.Key_Home:
                // Goto Home
                gotoHome();
            break;

            case Qt.Key_End:
                // Goto End
                gotoEnd();
            break;

            case Qt.Key_PageUp:
                // Goto Page Up
                gotoPageUp();
            break;

            case Qt.Key_PageDown:
                // Goto Page Down
                gotoPageDown();
            break;

            case Qt.Key_Return:
            case Qt.Key_Enter:
                // Check Count
                if (dirHistoryList.count > 0) {
                    // Select Item
                    dirHistoryListController.dirHistoryListItemSelected(dirHistoryList.currentIndex);
                }
            break;

            case Qt.Key_Escape:
                // Select Item
                dirHistoryListController.dirHistoryListItemSelected(-1);
            break;
        }
    }

    // Dir History List View
    ListView {
        id: dirHistoryList

        anchors.fill: parent
        anchors.leftMargin: parent.border.width
        anchors.topMargin: parent.radius
        anchors.rightMargin: parent.border.width
        anchors.bottomMargin: parent.radius + clearButton.height
        interactive: true
        highlightFollowsCurrentItem: true
        highlightMoveDuration: Const.DEFAULT_LIST_HIGHLIGHT_MOVE_DURATION
        snapMode: ListView.SnapToItem
        clip: true

        property int visualCount: Math.min(dirHistoryList.count, Math.floor(dirHistoryList.height / Const.DEFAULT_DIR_HISTORY_LIST_POPUP_ITEM_HEIGHT))

        model: dirHistoryListModel

        // Delegate
        delegate: Rectangle {
            id: delegateRoot

            color: mousePressed ?  Const.DEFAULT_POPUP_ITEM_SELECT_COLOR : mouseHovered ? "#11FFFFFF" : "transparent"

            width: dirHistoryList.width
            height: Const.DEFAULT_DIR_HISTORY_LIST_POPUP_ITEM_HEIGHT

            property bool mousePressed: false
            property bool mouseHovered: false

            // Item Text
            Text {
                id: delegateText
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: Const.DEFAULT_DIR_HISTORY_LIST_POPUP_ITEM_FONT_SIZE
                color: Const.DEFAULT_FONT_COLOR
                elide: Text.ElideLeft
                text: dirPath
            }

            // Mouse Area
            MouseArea {
                anchors.fill: parent
                preventStealing: true
                hoverEnabled: true

                // On Pressed
                onPressed: {
                    // Set Accepted
                    mouse.accepted = true;
                    // Set Mouse Pressed
                    mousePressed = true;
                    // Set Current Index
                    dirHistoryList.currentIndex = index;
                }

                // On Clicked
                onReleased: {
                    // Check Mouse Pressed
                    if (mousePressed) {
                        // Reset Mouse Pressed
                        mousePressed = false;
                        // Set Accepted
                        mouse.accepted = true;
                        // Select Item
                        dirHistoryListController.dirHistoryListItemSelected(index);
                        // Set Current Index
                        dirHistoryList.currentIndex = 0;
                    }
                }

                // On Entered
                onEntered: {
                    // Set Mouse Hovered
                    delegateRoot.mouseHovered = true;
                }

                // On Exited
                onExited: {
                    // Reset Mouse Hovered
                    delegateRoot.mouseHovered = false;
                }
            }
        }

        // Highlight
        highlight: Rectangle {
            width: dirHistoryList.width
            height: Const.DEFAULT_DIR_HISTORY_LIST_POPUP_ITEM_HEIGHT
            color: Const.DEFAULT_POPUP_ITEM_HIGHLIGHT_COLOR
            visible: !dirHistoryList.delegateMousePressed
        }

        // On Current index Changed
        onCurrentIndexChanged: {
            // Set Controller Current Index
            dirHistoryListController.currentIndex = dirHistoryList.currentIndex;
        }
    }

    // Empty Text
    Text {
        id: emptyText
        visible: dirHistoryList.count <= 0
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Dir History List Is Empty"
        color: Const.DEFAULT_FONT_COLOR

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Clear Clicked
                dirHistoryListController.dirHistoryClearClicked();
            }
        }
    }

    // Clear Button
    Rectangle {
        id: clearButton

        //width: parent.width
        height: Const.DEFAULT_DIR_HISTORY_LIST_POPUP_CLEAR_BUTTON_HEIGHT

        anchors.left: parent.left
        anchors.leftMargin: parent.border.width

        anchors.right: parent.right
        anchors.rightMargin: parent.border.width

        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.radius + parent.border.width

        color: clearButtonMouseArea.pressed ? Const.DEFAULT_POPUP_ITEM_SELECT_COLOR : "transparent"

        visible: dirHistoryList.count > 0

        // Button Text
        Text {
            id: clearButtonText
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Clear"
            color: Const.DEFAULT_FONT_COLOR
        }

        // Button Mouse Area
        MouseArea {
            id: clearButtonMouseArea
            anchors.fill: parent
            property bool pressedInside: false
            onPressed: {
                // Set Pressed Inside
                pressedInside = true;
            }
            onReleased: {
                // Check PRessed Inside
                if (pressedInside) {
                    // Reset Pressed Inside
                    pressedInside = false;
                    // Clear Clicked
                    dirHistoryListController.dirHistoryClearClicked();
                    // Reset Current Index
                    dirHistoryList.currentIndex = 0;
                }
            }
        }
    }
}

