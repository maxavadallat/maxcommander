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
    property bool isImage: mainController.isImage(fileListItemPopupRoot.currentItemPath)

    // On Completed
    Component.onCompleted: {
        //console.log("fileListItemPopupRoot.onCompleted");

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
            visible: !isVolume && isImage
        }

        FileListPopupItem {
            id: setDesktopImage
            title: "Set as Wallpaper"
            command: isVolume ? 0 : 8
            visible: !isVolume && isImage
            active: index == currentIndex
            onItemClicked: {
                // Hide
                hidePopup();
                // Set Desktop Image
                mainController.setAsWallPaper(currentItemPath);
            }
        }

        FileListPopupItem {
            title: "-"
            visible: !isVolume
        }

        FileListPopupItem {
            id: propertiesItem
            title: "Properties"
            command: isVolume ? 0 : 9
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

