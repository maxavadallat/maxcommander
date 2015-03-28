import QtQuick 2.0
import QtQuick.Controls 1.3
import "qrc:/qml/js/constants.js" as Const

FileListPopupItem {
    id: fileListHeaderPopupItemRoot

    property alias itemChecked: fileListHeaderPopupItemCheckBox.checked

    titleLeftMargin: fileListHeaderPopupItemCheckBox.width + Const.DEFAULT_MARGIN_WIDTH * 2

    // Check Box
    CheckBox {
        id: fileListHeaderPopupItemCheckBox
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: Const.DEFAULT_MARGIN_WIDTH
    }
}

