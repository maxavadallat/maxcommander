import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHeaderItemRoot

    property alias itemText: headerItemLabel.text

    width: 100
    height: parent.height

    color: "transparent"

    border.color: Const.DEFAULT_FILE_LIST_HEADER_ITEM_BORDER_COLOR
    border.width: 1

    clip: true

    // Text
    FileListText {
        id: headerItemLabel
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
    }

    // Separator
    FileListHeaderItemSeparator {
        id: headerItemSeparator
    }
}

