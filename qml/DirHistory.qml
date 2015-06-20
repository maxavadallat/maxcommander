import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: dirHistoryRoot

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    radius: Const.DEFAULT_POPUP_RADIUS

    border.color: Const.DEFAULT_POPUP_BORDER_COLOR
    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH

    // Dir History List View
    ListView {
        id: dirHistoryList
        cacheBuffer: 0
        anchors.fill: parent
        anchors.leftMargin: parent.border.width
        anchors.topMargin: parent.radius
        anchors.rightMargin: parent.border.width
        anchors.bottomMargin: parent.radius + clearButton.height
        interactive: true
        focus: true
        highlightFollowsCurrentItem: true
        snapMode: ListView.SnapToItem
        clip: true

        model: dirHistoryListModel

        // Delegate
        delegate: Rectangle {
            id: delegateRoot

            color: mousePressed ?  Const.DEFAULT_POPUP_ITEM_SELECT_COLOR : "transparent"

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
                //hoverEnabled: true

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
            onClicked: {

            }
        }
    }
}

