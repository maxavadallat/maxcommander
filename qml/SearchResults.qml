import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: searchResultsRoot

    width: 400
    height: 600

    color: "transparent"

    // Search Result List
    ListView {
        id: searchResultList

        anchors.fill: parent
        anchors.margins: 1
        spacing: 1
        clip: true
        focus: true
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 0
        highlightResizeDuration: 0
        snapMode: ListView.SnapToItem

        property int visualItemCount: Math.min(count, Math.ceil(searchResultList.height / Const.DEFAULT_SEARCH_RESULTS_DELEGATE_HEIGHT))

        // Model
        model: searchResultModel

        // Delegate
        delegate: Rectangle {
            id: delegateRoot
            color: "transparent"

            width: searchResultList.width
            height: Const.DEFAULT_SEARCH_RESULTS_DELEGATE_HEIGHT

            Row {
                id: resultContainer
                anchors.fill: parent

                // Icon
                Image {
                    id: resultItemIcon
                    width: parent.height
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: {
                        // Check File Name
                        if (Utility.isImage(filePath, searchResultController)) {
                            Const.DEFAULT_FILE_PREFIX + filePath
                        } else {
                            // Image Provider
                            Const.DEFAULT_FILE_ICON_PREFIX + filePath
                        }
                    }
                }

                // Spacer
                Rectangle {
                    id: spacer
                    width: 4
                    height: parent.height
                    color: "transparent"
                }

                // Text
                Text {
                    id: resultItemText
                    width: parent.width - resultItemIcon.width - spacer.width * 2
                    height: parent.height
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideMiddle
                    text: filePath
                }
            }

            // Mouse Area
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: {
                    // Set Current Index
                    searchResultList.currentIndex = index;
                }
                onDoubleClicked: {
                    // Item Selected
                    searchResultController.itemSelected();
                }
            }
        }

        // Highlight
        highlight: Rectangle {
            width: searchResultList.width
            height: Const.DEFAULT_SEARCH_RESULTS_DELEGATE_HEIGHT
            color: "#44444477"
        }

        // On Visual Item Coun Changed
        onVisualItemCountChanged: {
            //console.log("searchResultList.onVisualItemCountChanged - visualItemCount: " + searchResultList.visualItemCount);

            // Set Visual Item Count
            searchResultController.visualItemCount = searchResultList.visualItemCount;
        }

        // On Current Index Changed
        onCurrentIndexChanged: {
            // Check Search Result Controller Current Index
            if (searchResultController.currentIndex != searchResultList.currentIndex) {
                // Set Current Index
                searchResultController.currentIndex = searchResultList.currentIndex;
            }
        }
    }


    // On Completed
    Component.onCompleted: {
        //console.log("searchResultsRoot.onCompleted");

    }

    // On Destruction
    Component.onDestruction: {
        //console.log("searchResultsRoot.onDestruction");

    }

    // Connections
    Connections {
        target: searchResultController
        // On Current index Changed
        onCurrentIndexChanged: {
            //console.log("searchResultsRoot.Connections.searchResultController.onCurrentIndexChanged - currentIndex: " + searchResultController.currentIndex);

            // Check Result List View Current Index
            if (searchResultList.currentIndex != searchResultController.currentIndex) {
                // Set Current Index
                searchResultList.currentIndex = searchResultController.currentIndex;
            }
        }
    }
}

