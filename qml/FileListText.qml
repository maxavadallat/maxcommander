import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Text {
    id: fileListTextRoot
    height: parent.height
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    anchors.fill: parent
    anchors.topMargin: 2
    anchors.rightMargin: Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH + 1
    visible: parent.width > Const.DEFAULT_FILE_LIST_HEADER_SEPARATOR_WIDTH
    elide: Text.ElideMiddle

    color: globalSettings.textColor

    font.family: delegateFont.name
    font.pixelSize: globalSettings.fontSize
    font.bold: globalSettings.fontBold
    font.italic: globalSettings.fontItalic

    // Font Loader
    FontLoader {
        id: delegateFont
        name: globalSettings.fontName
    }
}

