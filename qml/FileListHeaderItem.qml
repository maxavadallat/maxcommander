import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHeaderItemRoot

    property alias itemText: headerItemLabel.text

    property int itemSorting: Const.DEFAULT_SORT_NAME

    property bool reverseOrder: mainController.reverseOrder

    property bool sortingVisible: mainController.sorting === fileListHeaderItemRoot.itemSorting

    width: 100
    height: parent.height

    color: "transparent"

    border.color: Const.DEFAULT_FILE_LIST_HEADER_ITEM_BORDER_COLOR
    border.width: 1

    clip: true

    // Set Separator Position
    function setSeparatorPosition(xPos) {
        // Set Separator Position
        headerItemSeparator.setPositionX(xPos);
    }

    // Text
    FileListText {
        id: headerItemLabel
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
    }

    //
    Image {
        id: sortingIndicator
        height: parent.height
        width: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: headerItemSeparator.left
        cache: true
        fillMode: Image.PreserveAspectFit
        source: fileListHeaderItemRoot.reverseOrder ? "/resources/images/arrow-up-128.png" : "/resources/images/arrow-down-128.png"
        visible: fileListHeaderItemRoot.sortingVisible
    }

    MouseArea {
        id: sortingSelectionMouseArea
        anchors.fill: parent
        onReleased: {
            // Check Sorting Visible
            if (fileListHeaderItemRoot.sortingVisible) {
                // Toggle Reverse Mode
                mainController.reverseOrder = !mainController.reverseOrder;
            } else {
                // Set Sorting
                mainController.sorting = fileListHeaderItemRoot.itemSorting;
            }
        }
    }

    // Separator
    FileListHeaderItemSeparator {
        id: headerItemSeparator
    }
}

