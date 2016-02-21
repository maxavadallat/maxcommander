import QtQuick 2.0

import MyCustomComponents 1.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileDeleteListDelegateRoot

    property alias fileIconSource: fileIconImage.source
    property alias fileNameText: fileTextLabel.text

    property bool fileDirectory: false
    property bool fileHidden: false
    property bool fileSelected: false
    property bool fileSymLink: false
    property bool fileArchive: false

    property int itemIndex: 0
    property int listIndex: -1

    property int status: 0

    property string textColor: {
        // Check If Hidden
        if (fileDeleteListDelegateRoot.fileHidden) {
            return globalSettings.hiddenColor;
        }

        // Check If Link
        if (fileDeleteListDelegateRoot.fileSymLink) {
            return globalSettings.linkColor;
        }

        // Check If Archive
        if (fileDeleteListDelegateRoot.fileArchive) {
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
            width: fileDeleteListDelegateRoot.height
            height: fileDeleteListDelegateRoot.height
            fillMode: Image.PreserveAspectFit
            asynchronous: false
            source: ""
        }

        // Source
        Text {
            id: fileTextLabel
            width: fileDeleteListDelegateRoot.width - fileIconImage.width - statusIndicatorContainer.width
            height: fileDeleteListDelegateRoot.height
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: fileDeleteListDelegateRoot.height * 0.4
            elide: Text.ElideMiddle
            color: textColor
        }

        Item {
            id: statusIndicatorContainer
            width: fileDeleteListDelegateRoot.height
            height: fileDeleteListDelegateRoot.height

            // Status Image
            Image {
                id: statusIndicatorImage
                width: parent.width * 0.5
                height: parent.height * 0.5
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                asynchronous: false
                cache: true
                opacity: fileDeleteListDelegateRoot.status == 1 ? 0.0 : 1.0
                visible: opacity > 0.0
                Behavior on opacity { NumberAnimation { duration: 500 } }

                source: {
                    switch (fileDeleteListDelegateRoot.status) {
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
                running: fileDeleteListDelegateRoot.status == 1
                opacity: running ? 1.0 : 0.0
                visible: opacity > 0.0
                Behavior on opacity { NumberAnimation { duration: 500 } }
            }
        }
    }
}
