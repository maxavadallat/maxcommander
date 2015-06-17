import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHeaderRoot

    width: 480
    height: 62

    property alias nameVisible : nameHeader.visible
    property alias extVisible  : extHeader.visible
    property alias typeVisible : typeHeader.visible
    property alias sizeVisible : sizeHeader.visible
    property alias dateVisible : dateHeader.visible
    property alias ownerVisible: ownerHeader.visible
    property alias permsVisible: permsHeader.visible
    property alias attrVisible : attribsHeader.visible


    property alias nameWidth : nameHeader.width
    property alias extWidth  : extHeader.width
    property alias typeWidth : typeHeader.width
    property alias sizeWidth : sizeHeader.width
    property alias dateWidth : dateHeader.width
    property alias ownerWidth: ownerHeader.width
    property alias permsWidth: permsHeader.width
    property alias attrWidth : attribsHeader.width

    signal rightClicked(var posX, var posY);

    // Set Name Header Separator Position
    function setNameHeaderSeparatorPosition(xPos) {
        // Set Separator Position
        nameHeader.setSeparatorPosition(xPos);
    }

    color: Const.DEFAULT_FILE_LIST_HEADER_BACKGROUND_COLOR

    // Header Row
    Row {
        id: itemsContainer
        anchors.fill: parent

        // Header Item - Name
        FileListHeaderItem {
            id: nameHeader
            width: 152
            itemText: "Filename"
            itemSorting: Const.DEFAULT_SORT_NAME
        }

        // Header Item - Ext
        FileListHeaderItem {
            id: extHeader
            itemText: "Ext"
            itemSorting: Const.DEFAULT_SORT_EXT
            visible: mainController.extVisible && !mainController.searchResultsMode
            onWidthChanged: {
                // Set Extensions Column Width
                mainController.extWidth = extHeader.width;
            }
        }

        // Header Item - Type
        FileListHeaderItem {
            id: typeHeader
            itemText: "Type"
            itemSorting: Const.DEFAULT_SORT_TYPE
            visible: mainController.typeVisible && !mainController.searchResultsMode
            onWidthChanged: {
                // Set Type Column Width
                mainController.typeWidth = typeHeader.width;
            }
        }

        FileListHeaderItem {
            id: sizeHeader
            itemText: "Size"
            visible: mainController.sizeVisible /*&& !mainController.searchResultsMode*/
            itemSorting: Const.DEFAULT_SORT_SIZE
            onWidthChanged: {
                // Set Size Column Width
                mainController.sizeWidth = sizeHeader.width;
            }
        }

        FileListHeaderItem {
            id: dateHeader
            itemText: "Date"
            visible: mainController.dateVisible && !mainController.searchResultsMode
            itemSorting: Const.DEFAULT_SORT_DATE
            onWidthChanged: {
                // Set Date Column Width
                mainController.dateWidth = dateHeader.width;
            }
        }

        FileListHeaderItem {
            id: ownerHeader
            itemText: "Owner"
            visible: mainController.ownerVisible && !mainController.searchResultsMode
            itemSorting: Const.DEFAULT_SORT_OWNER
            onWidthChanged: {
                // Set Owner Column Width
                mainController.ownerWidth = ownerHeader.width;
            }
        }

        FileListHeaderItem {
            id: permsHeader
            itemText: "Perms"
            visible: mainController.permsVisible && !mainController.searchResultsMode
            itemSorting: Const.DEFAULT_SORT_PERMS
            onWidthChanged: {
                // Set Permissions Column Width
                mainController.permsWidth = permsHeader.width;
            }
        }

        FileListHeaderItem {
            id: attribsHeader
            itemText: "Attribs"
            visible: mainController.attrsVisible && !mainController.searchResultsMode
            itemSorting: Const.DEFAULT_SORT_ATTRS
            onWidthChanged: {
                // Set Attributes Column Width
                mainController.attrsWidth = attribsHeader.width;
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        preventStealing: true
        // On Clicked
        onClicked: {
            // Emit Right Clicked
            rightClicked(mouse.x, mouse.y);
        }
    }

    // Connections - Main Controller
    Connections {
        target: mainController
        // On Extension Column Width Changed
        onExtWidthChanged: {
            // Check Extensions Header Width
            if (extHeader.width != mainController.extWidth) {
                // Set Extensions Header Position - Will Trigger Column Width Change
                extHeader.setSeparatorPosition(mainController.extWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Type Column Width Changed
        onTypeWidthChanged: {
            // Check Type Header Width
            if (typeHeader.width != mainController.extWidth) {
                // Set Type Header Position - Will Trigger Column Width Change
                typeHeader.setSeparatorPosition(mainController.typeWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Size Column Width Changed
        onSizeWidthChanged: {
            // Check Size Header Width
            if (sizeHeader.width != mainController.extWidth) {
                // Set Size Header Position - Will Trigger Column Width Change
                sizeHeader.setSeparatorPosition(mainController.sizeWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Date Column Width Changed
        onDateWidthChanged: {
            // Check Date Header Width
            if (dateHeader.width != mainController.extWidth) {
                // Set Date Header Position - Will Trigger Column Width Change
                dateHeader.setSeparatorPosition(mainController.dateWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Owner Column Width Changed
        onOwnerWidthChanged: {
            // Check Owner Header Width
            if (ownerHeader.width != mainController.extWidth) {
                // Set Owner Header Position - Will Trigger Column Width Change
                ownerHeader.setSeparatorPosition(mainController.ownerWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Permissions Column Width Changed
        onPermsWidthChanged: {
            // Check Permissions Header Width
            if (permsHeader.width != mainController.extWidth) {
                // Set Permissions Header Position - Will Trigger Column Width Change
                permsHeader.setSeparatorPosition(mainController.permsWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Attributes Column Width Changed
        onAttrsWidthChanged: {
            // Check Attributes Header Width
            if (attribsHeader.width != mainController.extWidth) {
                // Set Attributes Header Position - Will Trigger Column Width Change
                attribsHeader.setSeparatorPosition(mainController.attrsWidth - Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH);
            }
        }

        // On Sorting Changed
        onSortingChanged: {

        }

        // On Reverse Order Changed
        onReverseOrderChanged: {

        }
    }

    // On Completed
    Component.onCompleted: {
        //console.log("fileListHeaderRoot.onCompleted - extWidth: " + mainController.extWidth);

        // ...

    }
}

