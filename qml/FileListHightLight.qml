import QtQuick 2.0

import "qrc:/qml"
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: fileListHighLightRoot

    width: 320
    height: 32

    color: mainController.panelHasFocus ? "#44444477" : "#00000000"
}

