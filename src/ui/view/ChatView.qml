import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import Qt5Compat.GraphicalEffects
import "qrc:/FluentChat/ui/component/"

Item{
    anchors.fill: parent

    Component{
        id: message_text
        Item{
            id: message_text_item
            width: message_view.width-10
            height: {
                return Math.max(message_text_avatar.height,message_text_rectangle.height)+message_text_time.height+5
            }

            ChatAvatar{
                id: message_text_avatar
                user:model.user
                size: 35
                anchors{
                    top: parent.top
                    left: isSender?undefined:parent.left
                    right: isSender?parent.right:undefined
                }
            }

            Rectangle{
                id: message_text_rectangle
                width: {
                    let max_width=(message_text_item.width-message_text_avatar.width-10)*0.75
                    let need_width=message_text_metrics.width+20
                    return need_width>max_width?max_width:need_width
                }
                height: message_text_content.contentHeight+20
                anchors{
                    top:parent.top
                    left: isSender?undefined:message_text_avatar.right
                    leftMargin: 10
                    right: isSender?message_text_avatar.left:undefined
                    rightMargin: 10
                }
                radius: 10
//                color: FluTheme.primaryColor.lighter
                color: {
                    if(isSender)
                        return FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    return FluTheme.dark ? Window.active ?  Qt.rgba(38/255,44/255,54/255,1) : Qt.rgba(39/255,39/255,39/255,1) : Qt.rgba(251/255,251/255,253/255,1)
                }

                FluCopyableText{
                    id: message_text_content
                    text: model.content
                    wrapMode: Text.Wrap
                    color: FluTheme.dark^isSender ? FluColors.White : FluColors.Black
                    font.pixelSize: 14
                    anchors{
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        margins: 10
                    }
                }

                FluCopyableText{
                    id: message_text_metrics
                    text: message_text_content.text
                    font: message_text_content.font
                    visible: false
                }
            }

            FluText{
                id: message_text_time
                text: Qt.formatDateTime(new Date(model.time * 1000), "yyyy-MM-dd hh:mm:ss")
                color: FluTheme.dark ? FluColors.Grey120 : FluColors.Grey80
                font.pixelSize: 10
                anchors{
                    top: message_text_rectangle.bottom
                    topMargin: 5
                    left: isSender?undefined:message_text_rectangle.left
                    right: isSender?message_text_rectangle.right:undefined
                }
            }


        }
    }

    Rectangle{
        id: header
        width: parent.width
        height: 45
        z:1
        radius: 10
        color: FluTheme.dark ? Window.active ?  Qt.rgba(38/255,44/255,54/255,1) : Qt.rgba(39/255,39/255,39/255,1) : Qt.rgba(251/255,251/255,253/255,1)
        anchors{
            top: parent.top
            topMargin: 5
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
        }

        FluText{
            text: store.currentUser.nickname
            color: FluTheme.dark ? FluColors.White : FluColors.Black
            font.pixelSize: 20
            elide: Text.ElideRight
            anchors{
                left: parent.left
                leftMargin: 20
                right: header_icon.left
                verticalCenter: parent.verticalCenter
            }
        }

        FluIconButton{
            id: header_icon
            iconSource: FluentIcons.Info
            iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
            anchors{
                right: parent.right
                rightMargin: 20
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Item{
        id: message_view_mask //防止最上面和最下面的空白显示消息
        clip:true
        anchors{
            top: header.bottom
            topMargin: -10
            left: parent.left
            leftMargin: 20
            right: parent.right
            rightMargin: 10
            bottom: input_area.top
            bottomMargin: -10
        }
        ListView{
            id: message_view
            spacing: 20
            orientation: ListView.Vertical
            ScrollBar.vertical: FluScrollBar {}
            boundsBehavior: Flickable.DragOverBounds
            anchors{
                fill: parent
                topMargin: 10
                bottomMargin: 10
            }

            model: store.messageList.items

            header: Rectangle{
                width: message_view.width
                height: 20
                visible: false
            }

            footer: Rectangle{
                width: message_view.width
                height: 20
                 visible: false
            }

            delegate: Loader{
                id:message_loader
                property var model: modelData
                property bool isSender: modelData.user===store.currentUser
                sourceComponent: message_text
            }
        }
    }



    FluArea{
        id: input_area
        height: editExpand?parent.height*0.5:150
        paddings: 15
        radius: 10
        border.width: 0
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            bottomMargin: 10
        }

        property bool editExpand: false
        Behavior on height{
            enabled: FluTheme.enableAnimation
            NumberAnimation{
                duration: 666
                easing.type: Easing.InOutExpo
            }
        }

        RowLayout{
            id: button_group
            width: parent.width

            Row{
                spacing: 5
                FluIconButton{
                    iconSource: input_area.editExpand?FluentIcons.ChevronDown:FluentIcons.ChevronUp
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                        input_area.editExpand = !input_area.editExpand
                    }
                }

                FluIconButton{
                    iconSource:FluentIcons.Folder
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                    }
                }

                FluIconButton{
                    iconSource:FluentIcons.Photo
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                    }
                }

                FluIconButton{
                    iconSource:FluentIcons.Cut
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                    }
                }
            }

            Row{
                spacing: 5
                Layout.alignment: Qt.AlignRight
                FluIconButton{
                    iconSource:FluentIcons.Send
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                        input_area.sendMessage()
                    }
                }
            }


        }

        Item{
            id: text_box_container
            anchors{
                top:button_group.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 10
            }

            Flickable{
                clip: true
                anchors.fill: parent
                contentWidth: parent.width
                contentHeight: text_box.height
                ScrollBar.vertical: FluScrollBar {}
                boundsBehavior: Flickable.StopAtBounds

                FluMultilineTextBox{
                    id:text_box
                    placeholderText: "Tips：使用Ctrl+Enter换行OvO"
                    width: parent.width
                    height: contentHeight+20<text_box_container.height?text_box_container.height:contentHeight+20
                    padding: 0
                    background: Rectangle{
                        color:"transparent"
                    }
                    onCommit: {
                        input_area.sendMessage()
                    }
                }
            }
        }

        function sendMessage(){
            if(text_box.text==="")return
            showInfo(text_box.text)
            store.messageList.send(text_box.text)
            text_box.text=""
            message_view.contentY=message_view.contentHeight-message_view.height
        }
    }

    // 底部边框效果
    Rectangle{
        id: bottom_line_mask
        anchors.fill: input_area
        color: "transparent"

        layer.enabled: true
        layer.effect:OpacityMask {
            maskSource: Rectangle {
                width: bottom_line_mask.width
                height: bottom_line_mask.height
                radius: 10
            }
        }

        Rectangle{
            width: text_box.activeFocus?parent.width:0
            height: text_box.activeFocus ? 5 : 1
            anchors{
                bottom: parent.bottom
                left: parent.left
                leftMargin: text_box.activeFocus?0:parent.width/2

                Behavior on leftMargin{
                    enabled: FluTheme.enableAnimation
                    NumberAnimation{
                        duration: 666
                        easing.type: Easing.InOutExpo
                    }
                }
            }

            color: {
                if(FluTheme.dark){
                    text_box.activeFocus ? FluTheme.primaryColor.lighter  : Qt.rgba(166/255,166/255,166/255,1)
                }else{
                    return text_box.activeFocus ? FluTheme.primaryColor.dark  : Qt.rgba(183/255,183/255,183/255,1)
                }
            }
            Behavior on height{
                enabled: FluTheme.enableAnimation
                NumberAnimation{
                    duration: 666
                    easing.type: Easing.OutExpo
                }
            }
            Behavior on width{
                enabled: FluTheme.enableAnimation
                NumberAnimation{
                    duration: 666
                    easing.type: Easing.InOutExpo
                }
            }

        }
    }
}
