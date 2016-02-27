import QtQuick 2.0

import MyCustomComponents 1.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileTransferListDelegateRoot

    property alias fileIconSource: fileIconImage.source
    property alias sourceText: sourceTextLabel.text
    property alias targetText: targetTextLabel.text

    property bool fileDirectory: false
    property bool fileHidden: false
    property bool fileSelected: false
    property bool fileSymLink: false
    property bool fileArchive: false

    property int itemIndex: 0
    property int listIndex: -1

    property int opStatus: 0

    property string textColor: {
        // Check If Hidden
        if (fileTransferListDelegateRoot.fileHidden) {
            return globalSettings.hiddenColor;
        }

        // Check If Link
        if (fileTransferListDelegateRoot.fileSymLink) {
            return globalSettings.linkColor;
        }

        // Check If Archive
        if (fileTransferListDelegateRoot.fileArchive) {
            return globalSettings.archiveColor;
        }

        // Check If Current
        if (itemIndex == listIndex) {
            return globalSettings.currentColor;
        }

        return globalSettings.textColor;
    }

    width: Const.DEFAULT_FILE_LIST_DELEGATE_WIDTH
    height: Const.DEFAULT_FILE_LIST_DELEGATE_HEIGHT

    color: "transparent"

    Row {
        anchors.fill: parent

        // Icon
        Image {
            id: fileIconImage
            width: fileTransferListDelegateRoot.height
            height: fileTransferListDelegateRoot.height
            fillMode: Image.PreserveAspectFit
            asynchronous: false
            source: ""
        }

        // Source/Target Column
        Column {
            width: fileTransferListDelegateRoot.width - fileIconImage.width - statusIndicatorContainer.width
            height: fileTransferListDelegateRoot.height

            // Source
            Text {
                id: sourceTextLabel
                anchors.left: parent.left
                anchors.right: parent.right
                height: fileTransferListDelegateRoot.height * 0.5
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: fileTransferListDelegateRoot.height * 0.3
                elide: Text.ElideMiddle
                color: textColor
            }

            // Target
            Text {
                id: targetTextLabel
                anchors.left: parent.left
                anchors.right: parent.right
                height: fileTransferListDelegateRoot.height * 0.5
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: fileTransferListDelegateRoot.height * 0.3
                elide: Text.ElideMiddle
                color: textColor
            }
        }

        Item {
            id: statusIndicatorContainer
            width: fileTransferListDelegateRoot.height
            height: fileTransferListDelegateRoot.height

            // Status Image
            Image {
                id: statusIndicatorImage
                width: parent.width * 0.5
                height: parent.height * 0.5
                anchors.centerIn: parent
                asynchronous: false
                cache: true
                fillMode: Image.PreserveAspectFit
                opacity: fileTransferListDelegateRoot.opStatus == 1 ? 0.0 : 1.0
                visible: opacity > 0.0
                Behavior on opacity { NumberAnimation { duration: 500 } }
                source: {
                    switch (fileTransferListDelegateRoot.opStatus) {
                        //case 1: return "resources/images/play-icon.png";
                        case 2: return "qrc:/resources/images/pause-icon.png";
                        case 3: return "qrc:/resources/images/check_256.png";
                        case 4: return "qrc:/resources/images/synchronize_256.png";
                        case 5: return "qrc:/resources/images/error.png";
                    }

                    return "";
                }
            }

            // Progress Indicator
            BusyIndicator {
                id: busyIndicator
                anchors.fill: parent
                running: fileTransferListDelegateRoot.opStatus == 1
                opacity: running ? 1.0 : 0.0
                visible: opacity > 0.0
                Behavior on opacity { NumberAnimation { duration: 500 } }
            }
        }
    }
}
