import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

FileListPopup {
    id: fileListDropPopupRoot

    property string dropItemList: ""
    property string dropKeyList: ""

    height: popupItemsContainer.height + radius * 2

    // On Completed
    Component.onCompleted: {
        //console.log("fileListDropPopupRoot.onCompleted");

        // ...
    }

    // Show Popup
    function showPopup(aFocus) {
        // Show File List Popup
        showFileListPopup(aFocus, popupItemsContainer);
    }

    // Popup Items Container
    Column {
        id: popupItemsContainer

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.top: parent.top
        anchors.topMargin: parent.radius

        clip: true

        // Items

        FileListPopupItem {
            id: copyItem
            title: "Copy Here"
            command: 1
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();

                // Drop Operation Selected
                mainController.dropOperationSelected(dropItemList, copyItem.command);
            }
        }

        FileListPopupItem {
            id: moveItem
            title: "Move Here"
            command: 2
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();

                // Drop Operation Selected
                mainController.dropOperationSelected(dropItemList, moveItem.command);
            }
        }
    }
}

