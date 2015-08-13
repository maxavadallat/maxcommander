import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListPopupRoot

    width: Const.DEFAULT_POPUP_WIDTH

    radius: Const.DEFAULT_POPUP_RADIUS

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    border.color: Const.DEFAULT_POPUP_BORDER_COLOR
    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH

    opacity: 0.0

    z: 1.0

    visible: opacity > 0.0

    Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }

    // Show
    function showPopup() {

        // Set Opacity
        opacity = 1.0;
    }

    // Hide
    function hidePopup() {
        // Set Opacity
        opacity = 0.0;
    }

}

