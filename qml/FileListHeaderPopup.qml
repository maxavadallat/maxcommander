import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHeaderPopupRoot

    width: Const.DEFAULT_POPUP_WIDTH
    height: popupItemsContainer.height + radius * 2

    property alias extChecked: extMenuItem.itemChecked
    property alias typeChecked: typeMenuItem.itemChecked
    property alias sizeChecked: sizeMenuItem.itemChecked
    property alias dateChecked: dateMenuItem.itemChecked
    property alias ownerChecked: ownerMenuItem.itemChecked
    property alias permsChecked: permsMenuItem.itemChecked
    property alias attrsChecked: attrsMenuItem.itemChecked

    radius: Const.DEFAULT_POPUP_RADIUS

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    border.color: Const.DEFAULT_POPUP_BORDER_COLOR
    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH

    opacity: 0.0

    visible: opacity > 0.0

    Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }

    // Show
    function showPopup() {
        // Set Up File List Header Popup Checkboxes
        fileListHeaderPopupRoot.extChecked = mainController.extVisible;
        fileListHeaderPopupRoot.typeChecked = mainController.typeVisible;
        fileListHeaderPopupRoot.sizeChecked = mainController.sizeVisible;
        fileListHeaderPopupRoot.dateChecked = mainController.dateVisible;
        fileListHeaderPopupRoot.ownerChecked = mainController.ownerVisible;
        fileListHeaderPopupRoot.permsChecked = mainController.permsVisible;
        fileListHeaderPopupRoot.attrsChecked = mainController.attrsVisible;

        // Set Opacity
        opacity = 1.0;
    }

    // Hide
    function hidePopup() {
        // Set Opacity
        opacity = 0.0;
    }

    // Popup Items Container
    Column {
        id: popupItemsContainer

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.top: parent.top
        anchors.topMargin: parent.radius

        clip: true

        FileListHeaderPopupItem {
            id: extMenuItem
            title: "Extension"
            onItemCheckedChanged: {
                // Set Extensions Visible
                mainController.extVisible = extMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: typeMenuItem
            title: "Type"
            onItemCheckedChanged: {
                // Set Type Visible
                mainController.typeVisible = typeMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: sizeMenuItem
            title: "Size"
            onItemCheckedChanged: {
                // Set Size Visible
                mainController.sizeVisible = sizeMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: dateMenuItem
            title: "Date"
            onItemCheckedChanged: {
                // Set Date Visible
                mainController.dateVisible = dateMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: ownerMenuItem
            title: "Owner"
            onItemCheckedChanged: {
                // Set Owner Visible
                mainController.ownerVisible = ownerMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: permsMenuItem
            title: "Permissions"
            onItemCheckedChanged: {
                // Set Permissions Visible
                mainController.permsVisible = permsMenuItem.itemChecked;
            }
        }

        FileListHeaderPopupItem {
            id: attrsMenuItem
            title: "Attributes"
            onItemCheckedChanged: {
                // Set Attributes Visible
                mainController.attrsVisible = attrsMenuItem.itemChecked;
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

