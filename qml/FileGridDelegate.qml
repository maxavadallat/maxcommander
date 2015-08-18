import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileGridDelegateRoot

    width: Const.DEFAULT_FILE_GRID_DELEGATE_WIDTH
    height: Const.DEFAULT_FILE_GRID_DELEGATE_HEIGHT

    property alias fileIconSource: fileIconImage.source
    property alias fileNameText: fileNameLabel.text

    property bool fileHidden: false
    property bool fileSelected: false
    property bool fileSymLink: false

    property bool mouseHovered: false

    property int itemIndex: 0
    property int listIndex: -1

    property string textColor: {
        // Check If Selected
        if (fileGridDelegateRoot.fileSelected) {
            // Check Item Index
            if (itemIndex == listIndex) {
                return globalSettings.currentSelectedColor;
            }

            return globalSettings.selectedColor;
        }

        // Check If Hidden
        if (fileGridDelegateRoot.fileHidden) {
            return globalSettings.hiddenColor;
        }

        // Check If Link
        if (fileGridDelegateRoot.fileSymLink) {
            return globalSettings.linkColor;
        }

        // Check If Current
        if (itemIndex == listIndex) {
            return globalSettings.currentColor;
        }

        return globalSettings.textColor;
    }

    color: "transparent"

    clip: true

    // File Thumb
    Image {
        id: fileIconImage
        anchors.fill: parent
        anchors.leftMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.topMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.bottomMargin: Const.DEFAULT_MARGIN_WIDTH + fileNameLabel.height + Const.DEFAULT_MARGIN_WIDTH
        fillMode: Image.PreserveAspectFit
        cache: true
        smooth: false
        asynchronous: true
        opacity: fileIconImage.status == Image.Ready ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        visible: opacity > 0.0
    }

    // Loading Text
    Text {
        id: loadingText
        anchors.fill: parent
        anchors.leftMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.topMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.bottomMargin: Const.DEFAULT_MARGIN_WIDTH + fileNameLabel.height + Const.DEFAULT_MARGIN_WIDTH
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: "loading..."
        color: Const.DEFAULT_POPUP_BACKGROUND_COLOR
        opacity: fileIconImage.status != Image.Ready ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        visible: opacity > 0.0
    }

    // File Text
    Text {
        id: fileNameLabel
        anchors.left: parent.left
        anchors.leftMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.right: parent.right
        anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Const.DEFAULT_MARGIN_WIDTH
        height: Const.DEFAULT_FILE_GRID_DELEGATE_TEXT_SIZE
        verticalAlignment: Text.AlignBottom
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideMiddle
        color: fileGridDelegateRoot.textColor
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
}

