import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: separatorRoot

    width: Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH
    height: target.height

    property variant target: parent

    color: Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_COLOR

    border.color: Const.DEFAULT_FILE_LIST_HEADER_ITEM_BORDER_COLOR
    border.width: 1

    // Set Pos X
    function setPositionX(xPos) {
        //console.log("separatorRoot.setPositionX - xPos: " + xPos);
        // Set Position
        separatorRoot.x = xPos;
    }

    MouseArea {
        anchors.fill: parent
        drag.target: separatorRoot
        drag.axis: Drag.XAxis
        drag.minimumX: 0
        cursorShape: Qt.SplitHCursor
        hoverEnabled: true
        onEntered: {
            parent.color = "red";
        }
        onExited: {
            parent.color = Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_COLOR;
        }
    }

    // On X Changed
    onXChanged: {
        // Check Target
        if (target) {
            // Set Target Width
            target.width = x + separatorRoot.width;
        }
    }

    // N Completed
    Component.onCompleted: {
        separatorRoot.x = target.width - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH + 1;
    }
}

