import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileListScrollBarRoot

    width: Const.DEFAULT_FILE_LIST_SCROLLBAR_WIDTH
    height: parentContentSize > parentVisibleSize ? Math.max(parentVisibleSize * parentVisibleSize / parentContentSize, Const.DEFAULT_FILE_LIST_SCROLLBAR_MINIMAL_HEIGHT) : 0

    property int parentContentPos: 0

    property int parentVisibleSize: 0

    property int parentContentSize: 0

    property int scrollPos: parentContentPos * (parentVisibleSize - height) / (parentContentSize - parentVisibleSize)

    anchors.right: parent.right
    anchors.rightMargin: 2
    anchors.top: scrollBarMouseArea.pressed ? undefined : parent.top
    anchors.topMargin: 2 + scrollPos

    color: "transparent"
    opacity: 0.0
    visible: (parentContentSize > parentVisibleSize) && (opacity > 0.0)

    signal scrollYChanged(var posY)

    // On Y Pos Changed
    onYChanged: {
        // Set Opacity
        opacity = 1.0;
        // Restart Hide Timer
        hideTimer.restart();

        // Check If Dragging
        if (scrollBarMouseArea.pressed) {
            // Emit Scroll Y Changed
            scrollYChanged(y);
        }
    }

    // On Height Changed
    onHeightChanged: {
        // Set Opacity
        opacity = 1.0;
        // Restart Hide Timer
        hideTimer.restart();
    }

    // Behavior On Opacity
    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    // Scroll Bar
    Rectangle {
        id: bar
        anchors.fill: parent
        radius: Const.DEFAULT_FILE_LIST_SCROLLBAR_RADIUS
        color: Const.DEFAULT_FILE_LIST_SCROLLBAR_COLOR
        border.width: 1
        border.color: Const.DEFAULT_FILE_LIST_SCROLLBAR_BORDER_COLOR
    }

    // Mouse Area
    MouseArea {
        id: scrollBarMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        drag.target: parent
        drag.axis: Drag.YAxis
        cursorShape: pressed ? Qt.ClosedHandCursor : Qt.OpenHandCursor
        // On Entered
        onEntered: {
            // Set Color
            bar.color = Const.DEFAULT_FILE_LIST_SCROLLBAR_COLOR_HOVER;
        }
        // On Exited
        onExited: {
            // Set Color
            bar.color = Const.DEFAULT_FILE_LIST_SCROLLBAR_COLOR;
        }
    }

    Timer {
        id: hideTimer
        repeat: false
        interval: Const.DEFAULT_FILE_LIST_SCROLLBAR_VISIBILITY_TIMEOUT
        onTriggered: {
            // Set Opacity
            opacity = 0.0;
        }
    }
}

