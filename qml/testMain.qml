import QtQuick 2.0

Rectangle {
    id: testMainRoot

    width: 640
    height: 480

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#FF000000" }
        GradientStop { position: 0.5; color: "#FF000044" }
        GradientStop { position: 1.0; color: "#FF000000" }
    }

    ListView {
        id: clientsListView
        anchors.fill: parent
        spacing: 1
        clip: true
        snapMode: ListView.SnapToItem
        focus: true
        highlightMoveDuration: 50
        highlightFollowsCurrentItem: true

        model: clientListModel

        delegate: Rectangle {
            id: delegateRoot

            width: clientsListView.width
            height: 32

            property int transitionDuration: clientBusy ? 400 : 100

            property bool busyOn: false

            property string busyColor: busyOn ? "#FFAA00AA" : "#77770077"

            color: clientConnected ? (clientBusy ? delegateRoot.busyColor : "#77007700") : "#77770000"

            Behavior on color { PropertyAnimation { duration: delegateRoot.transitionDuration } }

            Row {
                id: itemContainer
                anchors.fill: parent

                Rectangle {
                    id: spacer
                    width: 8
                    height: parent.height
                    color: "transparent"
                }

                Text {
                    id: clientIDText
                    width: parent.width - parent.height - spacer.width
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: clientID === 0 ? "Disconnected" : "Client ID: " + clientID
                    color: "#FFEFEFEF"

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            // Check Button
                            if (mouse.button === Qt.LeftButton) {
                                // Set Current Index
                                clientsListView.currentIndex = index;
                            }
                        }
                    }
                }

                Rectangle {
                    id: deleteButton
                    width: parent.height
                    height: parent.height
                    color: "transparent"

                    Image {
                        width: 16
                        height: 16
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: "../resources/images/delete_256.png"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mainController.deleteClient(index);
                        }
                    }
                }
            }

            Timer {
                id: busyAnimTimer
                repeat: true
                running: clientBusy
                interval: delegateRoot.transitionDuration
                onTriggered: {
                    // Toggle Busy On
                    delegateRoot.busyOn = !delegateRoot.busyOn
                }
            }
        }

        highlight: Rectangle {
            color: "#44FFFFFF"
            width: clientsListView.delegate.width
            height: clientsListView.delegate.height
            border.color: "white"
            border.width: 1
        }

        // On Current Index Changed
        onCurrentIndexChanged: {
            // Check Current Index
            if (mainController.currentIndex != clientsListView.currentIndex) {
                // Set Main Controller Current Index
                mainController.currentIndex = clientsListView.currentIndex;
            }
        }
    }

    // Connections - Main Controller
    Connections {
        target: mainController

        // On Current Index Changed
        onCurrentIndexChanged: {
            // Check List View Current Index
            if (clientsListView.currentIndex != mainController.currentIndex) {
                // Set Current Index
                clientsListView.currentIndex = mainController.currentIndex;
            }
        }
    }
}

