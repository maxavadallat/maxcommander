import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

Rectangle {
    id: searchResultsRoot

    width: 400
    height: 600

    color: "transparent"

    focus: true

    property bool ownKeyPress: false

    // Goto Prev Item
    function gotoPrev() {
        // Set Current Index
        searchResultList.currentIndex = Math.max(0, searchResultList.currentIndex - 1);
    }

    // Next Item
    function gotoNext() {
        // Set Current Index
        searchResultList.currentIndex = Math.min(searchResultList.count - 1, searchResultList.currentIndex + 1);
    }

    // Goto Home
    function gotoHome() {
        // Set Current Index
        searchResultList.currentIndex = 0;
    }

    // Goto End
    function gotoEnd() {
        // Set Current Index
        searchResultList.currentIndex = searchResultList.count - 1;
    }

    // Goto Page Up
    function gotoPageUp() {
        // Set Current Index
        searchResultList.currentIndex = Math.max(0, searchResultList.currentIndex - searchResultList.visualCount);
    }

    // Goto Page Down
    function gotoPageDown() {
        // Set Current Index
        searchResultList.currentIndex = Math.min(searchResultList.count - 1, searchResultList.currentIndex + searchResultList.visualCount);
    }

    Keys.onPressed: {

        // Set Own Key Press
        searchResultsRoot.ownKeyPress = true;

        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:
                if (event.isAutoRepeat) {
                    // Goto Prev
                    gotoPrev();
                }
            break;

            case Qt.Key_Down:
                if (event.isAutoRepeat) {
                    // Goto Next
                    gotoNext();
                }
            break;

            case Qt.Key_PageUp:
                if (event.isAutoRepeat) {
                    // Goto Page Up
                    gotoPageUp();
                }
            break;

            case Qt.Key_PageDown:
                if (event.isAutoRepeat) {
                    // Goto Page Down
                    gotoPageDown();
                }
            break;

            default:
            break;
        }
    }

    Keys.onReleased: {

        // Check Own Key Press
        if (!searchResultsRoot.ownKeyPress)
            return;

        // reset Own Key Press
        searchResultsRoot.ownKeyPress = false;

        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:
                // Goto Prev
                gotoPrev();
            break;

            case Qt.Key_Down:
                // Goto Next
                gotoNext();
            break;

            case Qt.Key_Home:
                // Goto Home
                gotoHome();
            break;

            case Qt.Key_End:
                // Goto End
                gotoEnd();
            break;

            case Qt.Key_PageUp:
                // Goto Page Up
                gotoPageUp();
            break;

            case Qt.Key_PageDown:
                // Goto Page Down
                gotoPageDown();
            break;

            case Qt.Key_Return:
            case Qt.Key_Enter:
                // Item Selected
                searchResultController.itemSelected();
            break;

            case Qt.Key_F3:
                // Item View
                searchResultController.itemView(false);
            break;

            case Qt.Key_F4:
                // Item Edit
                searchResultController.itemView(true);
            break;

            case Qt.Key_Escape:
                // reject Dialog
                searchResultController.reject();
            break;

            default:
            break;
        }
    }

    // Search Result List
    ListView {
        id: searchResultList

        anchors.fill: parent
        anchors.margins: 1
        spacing: 1
        clip: true
        highlightFollowsCurrentItem: true
        highlightMoveDuration: Const.DEFAULT_LIST_HIGHLIGHT_MOVE_DURATION
        highlightResizeDuration: 0
        snapMode: ListView.SnapToItem

        property int visualCount: Math.min(count, Math.ceil(searchResultList.height / Const.DEFAULT_SEARCH_RESULTS_DELEGATE_HEIGHT))

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
                    cache: false
                    smooth: false
                    source: {
                        // Check Settings
                        if (globalSettings.useDefaultIcons) {
                            // Check If Is Dir
                            if (fileIsDir) {
                               return Const.DEFAULT_FILE_LIST_ICON_DIR;
                            }

                            return Const.DEFAULT_FILE_LIST_ICON_FILE;

                        } else {
                            // Check File Name
                            if (Utility.isImage(filePath, searchResultController)) {
                                return Const.DEFAULT_FILE_PREFIX + filePath;
                            }

                            // Image Provider
                            return Const.DEFAULT_FILE_ICON_PREFIX + filePath;
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
            color: Const.DEFAULT_SEARCH_RESULTS_HIGLIGHT_COLOR
        }

        // On Visual Item Coun Changed
        onVisualCountChanged: {
            //console.log("searchResultList.onVisualItemCountChanged - visualCount: " + searchResultList.visualCount);

            // Set Visual Item Count
            searchResultController.visualItemCount = searchResultList.visualCount;
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

