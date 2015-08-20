import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: imageViewerRoot

    width: Const.DEFAULT_IMAGE_VIEWER_WIDTH
    height: Const.DEFAULT_IMAGE_VIEWER_HEIGHT

    color: "black"

    // Preview Image
    AnimatedImage {
        id: previewImage
        anchors.fill: parent
        source: Const.DEFAULT_FILE_PREFIX + viewerContent
        asynchronous: true
        fillMode: Image.PreserveAspectFit
    }

    // Mouse Area
    MouseArea {
        id: viewerMouseArea
        anchors.fill: previewImage
        // On Wheel Event
        onWheel: {
            //console.log("imageViewerRoot.viewerMouseArea.onWheel - wheel: " + wheel.angleDelta.y);
            // Check Angle Delta
            if (wheel.angleDelta.y < 0) {
                // Next Image
                imageBrowser.gotoNext();
            } else {
                // Prev Image
                imageBrowser.gotoPrev();
            }
        }
        // On Double Clicked
        onDoubleClicked: {
            // Emit Select Current
            imageBrowser.selectCurrent();
        }
    }

    // On Completed
    Component.onCompleted: {
        //console.log("imageViewerRoot.onCompleted - contentSource: " + viewerContent);

        // ...
    }

    // Focus Changed
    onFocusChanged: {
        console.log("imageViewerRoot.onFocusChanged - focus: " + imageViewerRoot.focus);


    }
}

