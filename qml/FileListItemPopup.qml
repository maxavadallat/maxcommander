import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

FileListPopup {
    id: fileListHeaderPopupRoot

    height: popupItemsContainer.height + radius * 2

    property bool isDir: false
    property bool isLink: false
    property bool isVolume: fileListModel.isVolume(fileListHeaderPopupRoot.currentItemPath)

    property string currentItemPath: ""

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
            visible: !isVolume
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
            visible: fileListModel.selectedCount > 0 && !isVolume;
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
            visible: !isVolume
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
            visible: !isVolume
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
            visible: !isVolume
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
            visible: !isVolume
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
            visible: isVolume
            onItemClicked: {
                // Hide
                hidePopup();
                // Emit Launch Unmount Drive
                mainController.launchUnmountDrive();
            }
        }

        FileListPopupItem {
            title: "-"
        }

        FileListPopupItem {
            title: "Close"
            textAlignment: Text.AlignHCenter
            onItemClicked: {
                // Hide
                hidePopup();
            }
        }
    }
}

