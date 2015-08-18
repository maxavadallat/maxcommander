import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

FileListPopup {
    id: fileListItemPopupRoot

    height: popupItemsContainer.height + radius * 2

    property string currentItemPath: ""

    property bool isDir: false
    property bool isLink: false
    property bool isVolume: fileListModel.isVolume(fileListItemPopupRoot.currentItemPath)

    property variant currentItem: undefined
    property int currentIndex: -1

    // On Completed
    Component.onCompleted: {
        console.log("fileListItemPopupRoot.onCompleted");

        // ...
    }

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
                console.log("fileListItemPopupRoot.Keys.onReleased - key: " + event.key);
            break;
        }
    }

    // Show Popup
    function showPopup(aFocus) {

        // Set Opacity
        opacity = 1.0;
        // Set Focus
        //focus = aFocus;
        focus = true;

        // Get Number Of Children
        var numChildren = popupItemsContainer.children.length;
        var indexing = 0;

        //console.log("#### fileListItemPopupRoot.showPopup - numChildren: " + numChildren);

        // Go Thru Children
        for (var i=0; i<numChildren; i++) {

            // Get Child/Popup Item
            var popupItem = popupItemsContainer.children[i];

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
/*
        // Check Focus
        if (focus) {
            // Reset Current Index
            currentIndex = 0;
            // Set Current Item
            currentItem = Utility.fileListPopupItems[currentIndex];

        } else {
            // Reset Current Index
            currentIndex = -1;
            // Reset Current Item
            currentItem = undefined;
        }
*/
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
            id: selectAllItem
            title: "Select All"
            command: isVolume ? 0 : 1
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Select All
                fileListModel.selectAll();
            }
        }

        FileListPopupItem {
            id: selectNoneItem
            title: "Select None"
            command: isVolume ? 0 : 2
            visible: fileListModel.selectedCount > 0 && !isVolume;
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Deselect All
                fileListModel.deselectAll();
            }
        }

        FileListPopupItem {
            title: "-"
            visible: !isVolume
        }

        FileListPopupItem {
            id: copyItem
            title: "Copy"
            command: isVolume ? 0 : 3
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch File Copy Signal
                mainController.launchFileCopy();
            }
        }

        FileListPopupItem {
            id: moveItem
            title: "Move"
            command: isVolume ? 0 : 4
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch File Move Signal
                mainController.launchFileMove();
            }
        }

        FileListPopupItem {
            id: renameItem
            title: "Rename"
            command: isVolume ? 0 : 5
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch File Rename Signal
                mainController.launchFileRename();
            }
        }

        FileListPopupItem {
            visible: !isVolume
            title: "-"
        }

        FileListPopupItem {
            id: deleteItem
            title: "Delete"
            command: isVolume ? 0 : 6
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch Delete Signal
                mainController.launchDelete();
            }
        }

        FileListPopupItem {
            id: unMountItem
            title: "Eject/Unmount"
            command: isVolume ? 7 : 0
            visible: isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch Unmount Drive
                //mainController.launchUnmountDrive();
                // Eject Volume
                fileListModel.ejectVolume(currentItemPath);
            }
        }

        FileListPopupItem {
            title: "-"
            visible: !isVolume
        }

        FileListPopupItem {
            id: propertiesItem
            title: "Properties"
            command: isVolume ? 0 : 8
            visible: !isVolume
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch Properties Signal
                mainController.launchProperties();
            }
        }

/*
        FileListPopupItem {
            title: "-"
        }

        FileListPopupItem {
            title: "Close"
            command: 99
            textAlignment: Text.AlignHCenter
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
            }
        }
*/
    }
}

