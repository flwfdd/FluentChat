import QtQuick
import QtQuick.Window
import FluentUI

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("FluentIM")

    FluFilledButton {
        width: 125
        height: 35
        text: qsTr("Hello FluentIM!")
        anchors.centerIn: parent
    }
}
