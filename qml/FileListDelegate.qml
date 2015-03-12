import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListDelegateRoot

    property alias fileIconSource: fileIconImage.source
    property alias fileNameText: fileNameLabel.text
    property alias fileExtText:  fileExtLabel.text
    property alias fileTypeText: fileTypeLabel.text
    property alias fileSizeText: fileSizeLabel.text
    property alias fileDateText: fileDateLabel.text

    width: 320
    height: 32

    color: "transparent"

    Row {
        id: fileListDelegateContainerRow
        anchors.fill: parent
        Image {
            id: fileIconImage
            width: parent.height
            height: parent.height
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }
        FileListText {
            id: fileNameLabel
            width: 120
        }
        FileListText {
            id: fileExtLabel
            width: 40
        }
        FileListText {
            id: fileTypeLabel
            width: 120
            visible: false
        }
        FileListText {
            id: fileSizeLabel
            width: 60
        }
        FileListText {
            id: fileDateLabel
            width: 120
        }
    }
}

