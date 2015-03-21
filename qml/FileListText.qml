import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Text {
    id: fileListTextRoot
    height: parent.height
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.pixelSize: Const.DEFAULT_FILE_LIST_FONT_SIZE
    anchors.fill: parent
    anchors.rightMargin: Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH + 1
    visible: parent.width > Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH
}

