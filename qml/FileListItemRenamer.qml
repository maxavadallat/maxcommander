import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListItemRenamerRoot

    property string originalFileName: ""

    property alias fileName: renamerEditor.text

    width: 160
    height: 62

    color: Const.DEFAULT_POPUP_BACKGROUND_COLOR

    border.width: Const.DEFAULT_POPUP_BORDER_WIDTH
    border.color: Const.DEFAULT_POPUP_BORDER_COLOR

    radius: Const.DEFAULT_POPUP_RADIUS

    signal accepted
    signal rejected

    visible: opacity > 0.0

    Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }

    //focus: true

    // Accept
    function accept() {
        console.log("fileListItemRenamerRoot.accept - fileName: " + fileListItemRenamerRoot.fileName);
        // Hide Renamer
        hideRenamer();
        // Emit Accepted
        accepted();
    }

    // Reject
    function reject() {
        console.log("fileListItemRenamerRoot.reject");
        // Hide Renamer
        hideRenamer();
        // Emit Rejected
        rejected();
    }

    // Show Renamer
    function showRenamer() {
        // Set Cursor Position
        renamerEditor.cursorPosition = renamerEditor.text.length;
        // Select All
        renamerEditor.selectAll();
        // Set Opacity
        fileListItemRenamerRoot.opacity = 1.0;
        // Set Focus
        renamerEditor.focus = true;

        // Set File Renamer Active
        mainController.fileRenameActive = true;
    }

    // Hide Renamer
    function hideRenamer() {
        // Set Opacity
        fileListItemRenamerRoot.opacity = 0.0;
        // Set Focus
        renamerEditor.focus = false;

        // Reset File Renamer Active
        mainController.fileRenameActive = false;
    }

    Component.onCompleted: {
        //console.log("fileListItemRenamerRoot.onCompleted");

    }

    Component.onDestruction: {
        //console.log("fileListItemRenamerRoot.onDestruction");

    }

    Row {
        anchors.fill: parent
        anchors.margins: fileListItemRenamerRoot.border.width

        // Spacer
        Rectangle {
            width: fileListItemRenamerRoot.radius
            height: parent.height
            color: "transparent"
        }

        // Editor
        TextInput {
            id: renamerEditor
            width: parent.width - 2 * parent.height - 2 * fileListItemRenamerRoot.radius
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            cursorVisible: true
            text: "Testing Testing Testing..."
            color: "white"
            selectionColor: Const.DEFAULT_FILE_LIST_ITEM_RENAMER_SELECTION_COLOR

            Keys.onReleased: {
                // Check Event Key - RETURN
                if (event.key === Qt.Key_Return) {
                    console.log("renamerEditor.Keys.onReleased - RETURN");

                    // Accept
                    fileListItemRenamerRoot.accept();

                // Check Event Key - ESC
                } else if (event.key === Qt.Key_Escape) {
                    console.log("renamerEditor.Keys.onReleased - ESC");

                    // Reject
                    fileListItemRenamerRoot.reject();
                }
            }
        }

        // Spacer
        Rectangle {
            width: fileListItemRenamerRoot.radius
            height: parent.height
            color: "transparent"
        }

        // OK Button
        Rectangle {
            id: okButton
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            clip: true

            Image {
                id: okImage
                width: Math.min(Const.DEFAULT_FILE_LIST_ITEM_RENAMER_BUTTON_WIDTH, parent.height)
                height: Math.min(Const.DEFAULT_FILE_LIST_ITEM_RENAMER_BUTTON_HEIGHT, parent.height)
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: "/resources/images/check_256.png"
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    okImage.anchors.horizontalCenterOffset = 1;
                    okImage.anchors.verticalCenterOffset = 1;
                }
                onReleased: {
                    okImage.anchors.horizontalCenterOffset = 0;
                    okImage.anchors.verticalCenterOffset = 0;
                }
                onClicked: {
                    console.log("okButton.onClicked");
                    // Accept
                    accept();
                }
            }
        }

        // Cancel Button
        Rectangle {
            id: cancelButton
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            clip: true

            Image {
                id: cancelImage
                width: Math.min(Const.DEFAULT_FILE_LIST_ITEM_RENAMER_BUTTON_WIDTH, parent.height)
                height: Math.min(Const.DEFAULT_FILE_LIST_ITEM_RENAMER_BUTTON_HEIGHT, parent.height)
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: "/resources/images/delete_256.png"
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    cancelImage.anchors.horizontalCenterOffset = 1;
                    cancelImage.anchors.verticalCenterOffset = 1;
                }
                onReleased: {
                    cancelImage.anchors.horizontalCenterOffset = 0;
                    cancelImage.anchors.verticalCenterOffset = 0;
                }
                onClicked: {
                    console.log("cancelButton.onClicked");
                    // Reject
                    reject();
                }
            }
        }
    }
}

