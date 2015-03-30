import QtQuick 2.0
import QtMultimedia 5.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: videoViewerRoot

    width: Const.DEFAULT_VIDEO_VIEWER_WIDTH
    height: Const.DEFAULT_VIDEO_VIEWER_HEIGHT

    color: "black"

    focus: true

    // Video Player
    Video {
        id: mediaPlayer
        anchors.fill: parent
        autoLoad: true
        autoPlay: true
        source: Const.DEFAULT_FILE_PREFIX + viewerContent
        volume: 0.3

        onStatusChanged: {
            console.log("mediaPlayer.onStatusChanged: " + mediaPlayer.status + " - source: " + mediaPlayer.source);

        }

        onErrorStringChanged: {
            console.log("mediaPlayer.errorString: " + mediaPlayer.errorString);
        }
    }

    // Audio Image
    Image {
        id: audioImage
        anchors.centerIn: parent
        opacity: mediaPlayer.hasVideo ? 0.0 : 1.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: Const.DEFAULT_AUDIO_TAG_PREFIX + viewerContent
    }

    // Play Icon
    Image {
        id: playIcon
        anchors.centerIn: parent
        opacity: 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: ""
    }

    // Pause Icon
    Image {
        id: pauseIcon
        anchors.centerIn: parent
        opacity: 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: ""
    }

    Timer {
        id: iconHideTimer
        interval: 1000
        repeat: false
        onTriggered: {
            // Reset Opacity
            playIcon.opacity = 0.0;
            // Reset Opacity
            pauseIcon.opacity = 0.0;
        }
    }

    // Keys Pressed
    Keys.onPressed: {

    }

    // Keys Released
    Keys.onReleased: {
        // Switch Key
        switch (event.key) {
            case Qt.Key_P:
                // Check Video Player State
                if (mediaPlayer.playbackState == MediaPlayer.PlayingState) {
                    // Pause
                    mediaPlayer.pause();
                    // Set Pause Icon Opacity
                    pauseIcon.opacity = 1.0;
                    // Start Icon Hide Timer
                    iconHideTimer.restart();
                } else if (mediaPlayer.hasVideo || mediaPlayer.hasAudio) {
                    // Play
                    mediaPlayer.play();
                    // Set Play Icon Opacity
                    playIcon.opacity = 1.0;
                    // Start Icon Hide Timer
                    iconHideTimer.restart();
                }
            break;

            case Qt.Key_Equal:
            case Qt.Key_Plus:
                // Set Volume
                mediaPlayer.volume = Math.min(mediaPlayer.volume + 0.1, 1.0);
            break;

            case Qt.Key_Minus:
                // Set Volume
                mediaPlayer.volume = Math.max(mediaPlayer.volume - 0.1, 0.0);
            break;
        }
    }

    // On Completed
    Component.onCompleted: {
        console.log("videoViewerRoot.onCompleted - viewerContent: " + viewerContent);

        // ...
    }
}

