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

    // On Completed
    Component.onCompleted: {
        //console.log("imageViewerRoot.onCompleted - contentSource: " + viewerContent);

        // ...
    }
}

