import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI

FluScrollablePage{
    title:"设置"
    topPadding:10

    FluArea{
        Layout.fillWidth: true
        Layout.topMargin: 20
        height: 128
        paddings: 10

        ColumnLayout{
            spacing: 5
            anchors{
                top: parent.top
                left: parent.left
            }
            FluText{
                text: "深色模式"
                font: FluTextStyle.BodyStrong
                Layout.bottomMargin: 4
            }
            Repeater{
                model: [{title:"跟随系统",mode:FluThemeType.System},{title:"始终关闭",mode:FluThemeType.Light},{title:"始终开启",mode:FluThemeType.Dark}]
                delegate:  FluRadioButton{
                    checked : FluTheme.darkMode === modelData.mode
                    text:modelData.title
                    clickListener:function(){
                        FluTheme.darkMode = modelData.mode
                    }
                }
            }
        }
    }

    Repeater {

        model: chatListModel.items

        delegate:FluButton{
            text: modelData.title
            onClicked: {
                chatListModel.top(modelData)
            }
        }
    }

}
