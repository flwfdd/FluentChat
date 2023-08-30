import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtMultimedia
import FluentUI
import "qrc:/FluentChat/ui/component/"


FluPage {

    Column {
        anchors.centerIn: parent
        spacing: 10
        ChatAvatar {
            id: avatar
            bgColor: FluTheme.primaryColor.lightest
            avatar: "🥳"
            online: true
            size: 50
            anchors.horizontalCenter: parent.horizontalCenter
        }
        FluText {
            text: "FluentChat"
            font.pixelSize: 24
        }

        // FluText {
        //     text: "<a href='http://bit101.cn'>点我</a>"
        //     textFormat: Text.RichText
        //     onLinkActivated: {
        //         Qt.openUrlExternally("http://bit101.cn")
        //     }
        // }
    }
}
