import QtQuick
import FluentUI

Rectangle {
    property var user
    property int size
    id: avatar
    width: size
    height: size
    radius: size / 2
    color: user.color

    FluText {
        anchors.centerIn: parent
        text: user.avatar ? [...user.avatar][0] : "" //截取可能含有emoji的字符串的第一个字符
        color: "white"
        font.pixelSize: avatar.size * 0.75
    }

    FluBadge{
        count: model.user.online
        isDot: true
        color: FluTheme.primaryColor.lighter
        anchors{
            right: parent.right
            bottom: parent.bottom
        }
    }
}
