import QtQuick 2.0
import QtMultimedia 5.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml/js/utility.js" as Utility

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
        fillMode: VideoOutput.PreserveAspectFit
        autoLoad: true
        autoPlay: true
        source: Const.DEFAULT_FILE_PREFIX + viewerContent
        volume: 0.5

        onStatusChanged: {
            //console.log("mediaPlayer.onStatusChanged: " + mediaPlayer.status + " - source: " + mediaPlayer.source);
        }

        onErrorStringChanged: {
            console.log("mediaPlayer.errorString: " + mediaPlayer.errorString);
        }

        onHasAudioChanged: {
            //console.log("mediaPlayer.onHasAudioChanged - metadata: " + mediaPlayer.metaData.title);

            // ...
        }
    }

    // Audio Image
    Image {
        id: audioImage
        anchors.centerIn: parent
        opacity: mediaPlayer.hasAudio && !mediaPlayer.hasVideo ? 1.0 : 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: Const.DEFAULT_AUDIO_TAG_PREFIX + viewerContent
    }

    // Playback Progress
    Rectangle {
        id: playbackProgressIndicator
        width: parent.width * 0.8
        height: Const.DEFAULT_PROGRESS_INDICATOR_HEIGHT
        anchors.centerIn: parent
        anchors.verticalCenterOffset: Const.DEFAULT_PROGRESS_INDICATOR_VERTICAL_OFFSET

        color: "#44444444"

        // Position Label
        Text {
            id: posLabel
            width: Const.DEFAULT_PROGRESS_INDICATOR_TEXT_WIDTH
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Const.DEFAULT_PROGRESS_INDICATOR_TEXT_SIZE
            //text: Qt.formatTime(mediaPlayer.position, "mm:ss");
            text: Utility.formatSecs(mediaPlayer.position / 1000)
            color: Const.DEFAULT_FONT_COLOR
        }

        // Indicator Bar
        Rectangle {
            id: progressIndicatorBar
            height: Const.DEFAULT_PROGRESS_INDICATOR_BAR_HEIGHT
            anchors.left: posLabel.right
            anchors.leftMargin: Const.DEFAULT_MARGIN_WIDTH
            anchors.right: lengthLabel.left
            anchors.rightMargin: Const.DEFAULT_MARGIN_WIDTH
            anchors.verticalCenter: parent.verticalCenter
            color: Const.DEFAULT_PROGRESS_INDICATOR_BAR_COLOR
            radius: Const.DEFAULT_MARGIN_WIDTH / 2

            // Position Indicator
            Rectangle {
                id: positionIndicator
                height: progressIndicatorBar.height + 4
                width: Const.DEFAULT_PROGRESS_INDICATOR_POSITIONER_WIDTH
                color: Const.DEFAULT_PROGRESS_INDICATOR_POSITIONER_COLOR
                anchors.horizontalCenter: parent.left
                anchors.horizontalCenterOffset: parent.width * mediaPlayer.position / mediaPlayer.duration
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        // Length Label
        Text {
            id: lengthLabel
            width: Const.DEFAULT_PROGRESS_INDICATOR_TEXT_WIDTH
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Const.DEFAULT_PROGRESS_INDICATOR_TEXT_SIZE
            //text: Qt.formatTime(mediaPlayer.duration, "mm:ss");
            text: Utility.formatSecs(mediaPlayer.duration / 1000)
            color: Const.DEFAULT_FONT_COLOR
        }
    }


    // Play Icon
    Image {
        id: playIcon
        anchors.centerIn: parent
        opacity: 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: "qrc:/resources/images/play-icon.png"
    }

    // Pause Icon
    Image {
        id: pauseIcon
        anchors.centerIn: parent
        opacity: 0.0
        visible: opacity > 0.0
        Behavior on opacity { NumberAnimation { duration: Const.DEFAULT_TRANSITION_DURATION } }
        asynchronous: true
        source: "qrc:/resources/images/pause-icon.png"
    }

    // Icon Hide Timer
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
        //console.log("videoViewerRoot.onCompleted - viewerContent: " + viewerContent);

        // ...
    }
}

