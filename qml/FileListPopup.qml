import QtQuick 2.0

import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: fileListPopupRoot

    property variant currentItem: undefined

    property int currentIndex: -1

    width: Const.DEFAULT_POPUP_WIDTH

    radius: Const.DEFAULT_POPUP_RADIUS

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    border.color: Const.DEFAULT_POPUP_BORDER_COLOR
    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH

    opacity: 0.0

    z: 1.0

    visible: opacity > 0.0

    Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }

    // Key Release Handling
    Keys.onReleased: {
        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:
                // Prev Item
                prevItem();
            break;

            case Qt.Key_Down:
                // Next Item
                nextItem();
            break;

            case Qt.Key_Enter:
            case Qt.Key_Return:
                // Exec Item
                execItem();
            break;

            case Qt.Key_Escape:
                // Hide Popup
                hidePopup();
            break;

            default:
                console.log("fileListPopupRoot.Keys.onReleased - key: " + event.key);
            break;
        }
    }

    // Show Popup
    function showFileListPopup(aFocus, container) {

        // Set Opacity
        opacity = 1.0;
        // Set Focus
        //focus = aFocus;
        focus = true;

        // Get Number Of Children
        var numChildren = container.children.length;
        var indexing = 0;

        //console.log("#### fileListItemPopupRoot.showPopup - numChildren: " + numChildren);

        // Go Thru Children
        for (var i=0; i<numChildren; i++) {

            // Get Child/Popup Item
            var popupItem = container.children[i];

            // Check Command
            if (popupItem.command !== 0 /*&& popupItem.visible*/) {
                //console.log("fileListItemPopupRoot.showPopup - command: " + popupItem.command + " - index: " + indexing);

                // Set Index
                popupItem.index = indexing;
                // Append Popup Item
                Utility.appendFileListPopupItem(popupItem);

                // Inc Indexing
                indexing++;
            }
        }

        // Reset Current Index
        currentIndex = -1;
        // Reset Current Item
        currentItem = undefined;

        // Set File List Item Popup Active
        mainController.fileListItemPopupActive = true;

    }

    // Hide Popup
    function hidePopup() {
        // Set Opacity
        opacity = 0.0;
        // Reset Focus
        focus = false;
        // Clear File List Popup Items
        Utility.clearFileListPopupItems();
        // Reset Current Index
        currentIndex = -1;
        // Reset Current Item
        currentItem = undefined;

        // Set File List Item Popup Active
        mainController.fileListItemPopupActive = false;
    }

    // Prev Item
    function prevItem() {
        // Check Current Index
        if (currentIndex > 0) {
            // Dec Current Index
            currentIndex--;
        }

        // Set Current Item
        currentItem = Utility.fileListPopupItems[currentIndex];
    }

    // Next Item
    function nextItem() {
        // Check Current Index
        if (currentIndex < Utility.fileListPopupItems.length-1) {
            // Inc Current Index
            currentIndex++;
        }

        // Set Current Item
        currentItem = Utility.fileListPopupItems[currentIndex];
    }

    // Exec Item
    function execItem() {
        // Check Current Item
        if (currentItem != undefined) {

            // Signal Item Clicked
            currentItem.itemClicked(currentItem.command);

        }
    }


}

