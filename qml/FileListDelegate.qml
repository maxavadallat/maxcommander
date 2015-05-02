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
    property alias fileOwnerText: fileOwnerLabel.text
    property alias filePermsText: filePermsLabel.text
    property alias fileAttrsText: fileAttrsLabel.text

    property alias nameVisible : fileNameContainer.visible
    property alias extVisible  : fileExtContainer.visible
    property alias typeVisible : fileTypeContainer.visible
    property alias sizeVisible : fileSizeContainer.visible
    property alias dateVisible : fileDateContainer.visible
    property alias ownerVisible: fileOwnerContainer.visible
    property alias permsVisible: filePermsContainer.visible
    property alias attrVisible : fileAttrContainer.visible

    property alias nameWidth : fileNameContainer.width
    property alias extWidth  : fileExtContainer.width
    property alias typeWidth : fileTypeContainer.width
    property alias sizeWidth : fileSizeContainer.width
    property alias dateWidth : fileDateContainer.width
    property alias ownerWidth: fileOwnerContainer.width
    property alias permsWidth: filePermsContainer.width
    property alias attrWidth : fileAttrContainer.width

    property bool fileHidden: false
    property bool fileSelected: false
    property bool fileSymLink: false

    property bool mouseHovered: false
    property int fileDirSize: 0

    property int itemIndex: 0
    property int listIndex: -1

    property string textColor: {
        // Check If Selected
        if (fileListDelegateRoot.fileSelected) {
            return Const.DEFAULT_FILE_LIST_SELECTED_TEXT_COLOR;
        }

        if (fileListDelegateRoot.fileHidden) {
            return Const.DEFAULT_FILE_LIST_HIDDEN_TEXT_COLOR;
        }

        if (fileListDelegateRoot.fileSymLink) {
            return Const.DEFAULT_FILE_LIST_LINK_TEXT_COLOR;
        }

        return Const.DEFAULT_FILE_LIST_TEXT_COLOR;
    }

    width: 320
    height: 32

    color: mouseHovered ? "#07000044" : "transparent"

    Row {
        id: fileListDelegateItemsContainerRow
        anchors.fill: parent

        FileListDelegateItem {
            id: fileNameContainer
            width: 152
            // Icon
            Image {
                id: fileIconImage
                width: parent.height
                height: parent.height
                fillMode: Image.PreserveAspectFit
                cache: true
                smooth: false
                asynchronous: true
            }
            // File Name
            FileListText {
                id: fileNameLabel
                anchors.leftMargin: fileIconImage.width + 2
                horizontalAlignment: Text.AlignLeft
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileExtContainer
            width: 40
            FileListText {
                id: fileExtLabel
                horizontalAlignment: Text.AlignRight
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileTypeContainer
            width: 120
            FileListText {
                id: fileTypeLabel
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileSizeContainer
            width: 60
            FileListText {
                id: fileSizeLabel
                horizontalAlignment: {
                    text === Const.DEFAULT_DIR_SIZE_TITLE_LABEL_TEXT_DIR || text === Const.DEFAULT_DIR_SIZE_TITLE_LABEL_TEXT_BUNDLE ? Text.AlignHCenter : Text.AlignRight
                }
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileDateContainer
            width: 120
            FileListText {
                id: fileDateLabel
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileOwnerContainer
            width: 60
            FileListText {
                id: fileOwnerLabel
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: filePermsContainer
            width: 100
            FileListText {
                id: filePermsLabel
                color: fileListDelegateRoot.textColor
            }
        }

        FileListDelegateItem {
            id: fileAttrContainer
            width: 60
            FileListText {
                id: fileAttrsLabel
                color: fileListDelegateRoot.textColor
            }
        }
    }
}

