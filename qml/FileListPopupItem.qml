import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: popupItemRoot

    property alias title: titleLabel.text

    property int command: 0

    property bool mousePressed: false

    property bool mouseHovered: false

    property int titleLeftMargin: Const.DEFAULT_MARGIN_WIDTH

    property alias textAlignment: titleLabel.horizontalAlignment

    width: Const.DEFAULT_POPUP_WIDTH - 4

    height: popupItemRoot.title === "-" ? Const.DEFAULT_POPUP_ITEM_HEIGHT * 0.5 : Const.DEFAULT_POPUP_ITEM_HEIGHT

    anchors.horizontalCenter: parent.horizontalCenter

    color: mouseHovered ? Const.DEFAULT_POPUP_ITEM_HIGHLIGHT_COLOR : "transparent"

    signal itemClicked(var aCommand);

    // Title Label
    Text {
        id: titleLabel
        anchors.fill: parent
        anchors.leftMargin: popupItemRoot.titleLeftMargin //Const.DEFAULT_MARGIN_WIDTH
        anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
        verticalAlignment: Text.AlignVCenter
        visible: popupItemRoot.title !== "-"
        color: Const.DEFAULT_FONT_COLOR

    }

    // Separator
    Rectangle {
        id: separator
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: Const.DEFAULT_POPUP_BORDER_COLOR
        visible: popupItemRoot.title === "-"
    }

    // Mouse Area
    MouseArea {
        anchors.fill: parent
        hoverEnabled: popupItemRoot.title !== "-"
        preventStealing: true

        // On Pressed
        onPressed: {
            // Set Accepted
            mouse.accepted = true;
            // Set Mouse Pressed
            popupItemRoot.mousePressed = true;
        }

        // On Clicked
        onReleased: {
            // Check Mouse Pressed
            if (mousePressed) {
                // Reset Mouse Pressed
                popupItemRoot.mousePressed = false;
                // Set Accepted
                mouse.accepted = true;
                // Emit Item Clicked Signal
                itemClicked(popupItemRoot.command);
            }
        }

        // On Entered
        onEntered: {
            // Set Color
            // Check Title
            if (popupItemRoot.title !== "-") {
                // Set Mouse Hovered
                mouseHovered = true;
            }
        }

        // On Exited
        onExited: {
            // Reset Color
            // Reset Mouse Hovered
            mouseHovered = false;
        }
    }
}

