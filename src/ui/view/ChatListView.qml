import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import FluentUI

Item {
    id: control
    property var chatListM
    property FluObject footerItems
    property Component autoSuggestBox
    property double chat_item_height:66
    property double footer_item_height:42

    Component {
        id: chat_item
        Item {
            clip: true
            height: chat_item_height
            visible: true
            width: layout_list.width
            FluControl {
                id: item_control
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 2
                    bottomMargin: 2
                    leftMargin: 6
                    rightMargin: 6
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: function (mouse) {
                        if (mouse.button === Qt.RightButton) {
                            if (model.menuDelegate) {
                                loader_item_menu.sourceComponent = model.menuDelegate
                                loader_item_menu.item.popup();
                            }
                        }
                    }
                    z: -100
                }
                onClicked: {
                    nav_list.currentIndex = _idx
                    layout_footer.currentIndex = -1
                    chatListM.onClick(model)
                }
                Rectangle {
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if (FluTheme.dark) {
                            if (nav_list.currentIndex === _idx) {
                                return Qt.rgba(1, 1, 1, 0.06)
                            }
                            if (item_control.hovered) {
                                return Qt.rgba(1, 1, 1, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        } else {
                            if (nav_list.currentIndex === _idx) {
                                return Qt.rgba(0, 0, 0, 0.06)
                            }
                            if (item_control.hovered) {
                                return Qt.rgba(0, 0, 0, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        }
                    }

                    Rectangle {
                        id: item_avatar
                        width: 42
                        height: width
                        radius: width / 2
                        color: model.color
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 10
                        }

                        FluText {
                            anchors.centerIn: parent
                            text: model.avatar ? [...model.avatar][0] : "" //截取可能含有emoji的字符串的第一个字符
                            color: "white"
                            font.pixelSize: 24
                        }
                    }

                    Column {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: item_avatar.right
                            leftMargin: 10
                            right: unread_badge.left
                        }
                        FluText {
                            id: item_title
                            text: model.title
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            font.pixelSize: 16
                            color: {
                                if (item_control.pressed) {
                                    return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                                }
                                return FluTheme.dark ? FluColors.White : FluColors.Grey220
                            }
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                        FluText {
                            id: item_text
                            text: model.text
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            font.pixelSize: 12
                            color: {
                                if (item_control.pressed) {
                                    return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                                }
                                return FluTheme.dark ? FluColors.White : FluColors.Grey220
                            }
                            anchors {
                                left: parent.left
                                right: parent.right
                            }
                        }
                    }

                    FluBadge {
                        id: unread_badge
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: 10
                        }
                        count: model.unreadNum
                    }
                }
            }
        }
    }

    Component {
        id: com_footer_item
        Item {
            clip: true
            height: footer_item_height
            visible: true
            width: layout_list.width
            FluControl {
                id: item_control
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 2
                    bottomMargin: 2
                    leftMargin: 6
                    rightMargin: 6
                }
                onClicked: {
                    model.tapFunc()
                    layout_footer.currentIndex = _idx
                    nav_list.currentIndex = -1
                    chatListM.currentItem=null
                }
                Rectangle {
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if (FluTheme.dark) {
                            if (item_control.hovered) {
                                return Qt.rgba(1, 1, 1, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        } else {
                            if (item_control.hovered) {
                                return Qt.rgba(0, 0, 0, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        }
                    }

                    Item {
                        id: item_icon
                        width: 30
                        height: 30
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 3
                        }
                        FluIcon {
                            anchors.centerIn: parent
                            iconSource: {
                                if (model.icon) {
                                    return model.icon
                                }
                                return 0
                            }
                            iconSize: 15
                        }
                    }
                    FluText {
                        id: item_title
                        text: model.title
                        elide: Text.ElideRight
                        color: {
                            if (item_control.pressed) {
                                return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                            }
                            return FluTheme.dark ? FluColors.White : FluColors.Grey220
                        }
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: item_icon.right
                            right: item_dot_loader.left
                        }
                    }
                    Loader {
                        id: item_dot_loader
                        property bool isDot: (item_dot_loader.item && item_dot_loader.item.isDot)
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: isDot ? 3 : 10
                            verticalCenterOffset: isDot ? -8 : 0
                        }
                        sourceComponent: {
                            if (model.infoBadge) {
                                return model.infoBadge
                            }
                            return undefined
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: layout_list
        width: parent.width
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        border.color: FluTheme.dark ? Qt.rgba(45 / 255, 45 / 255, 45 / 255, 1) : Qt.rgba(226 / 255, 230 / 255, 234 / 255, 1)
        border.width: 1
        color: "transparent"
        Item {
            id: layout_header
            width: layout_list.width
            clip: true
            y: 10
            height: autoSuggestBox ? 38 : 0
            Loader {
                id: loader_auto_suggest_box
                anchors.centerIn: parent
                sourceComponent: autoSuggestBox
            }
        }

        Item{
            id: highlight_clip
            anchors.fill: layout_flickable
            clip: true

            Rectangle {
                id: highlight_rectangle
                height: chat_item_height
                color: FluTheme.primaryColor.dark
                width: 4
                radius: width / 2
                anchors {
                    left: parent.left
                    leftMargin: 6
                }
                property bool enableAnimation:true

                Behavior on y {
                    enabled: highlight_rectangle.enableAnimation & FluTheme.enableAnimation
                    NumberAnimation {
                        easing.period: 0.75
                        easing.amplitude: 1
                        duration: 666
                        easing.type: Easing.OutElastic
                    }
                }

                Behavior on height {
                    enabled: highlight_rectangle.enableAnimation & FluTheme.enableAnimation
                    NumberAnimation {
                        easing.period: 0.75
                        easing.amplitude: 1
                        duration: 666
                        easing.type: Easing.OutElastic
                    }
                }
            }
        }



        Flickable {
            id: layout_flickable
            anchors {
                top: layout_header.bottom
                topMargin: 6
                left: parent.left
                right: parent.right
                bottom: layout_footer.top
            }
            boundsBehavior: ListView.StopAtBounds
            clip: true
            contentHeight: nav_list.contentHeight
            ScrollBar.vertical: FluScrollBar {
            }
            ListView {
                id: nav_list
                clip: true
                anchors.fill: parent
                model: chatListM.items
                boundsBehavior: ListView.StopAtBounds

                onCurrentIndexChanged: {
                    if(nav_list.currentIndex!==-1){
                        highlight_clip.clip=true
                        highlight_rectangle.height=chat_item_height*0.5
                        highlight_rectangle.y= nav_list.currentItem.y-layout_flickable.contentY + (chat_item_height - chat_item_height*0.5) / 2
                    }
                }

                Connections{
                    // 监听使得高亮位置跟随当前聊天窗口
                    target: chatListM
                    function onItemsChanged(){
                        updateIndex()
                    }
                    function updateIndex(){
                        for(var i=0;i<chatListM.items.length;i++){
                            if(chatListM.items[i]===chatListM.currentItem){
                                nav_list.currentIndex=i
                                break
                            }
                        }
                    }
                }

                delegate: Loader {
                    property var model: modelData
                    property var _idx: index
                    property int type: 0
                    sourceComponent: chat_item
                }
            }

            onContentYChanged: {
                if(nav_list.currentIndex!==-1){
                    highlight_clip.clip=true
                    highlight_rectangle.enableAnimation=false
                    highlight_rectangle.height=chat_item_height*0.5
                    highlight_rectangle.y= nav_list.currentItem.y-layout_flickable.contentY + (chat_item_height - chat_item_height*0.5) / 2
                    highlight_rectangle.enableAnimation=true
                }
            }
        }

        // 分割线
        Rectangle{
            color: FluTheme.dark ? Qt.rgba(80/255,80/255,80/255,1) : Qt.rgba(210/255,210/255,210/255,1)
            width: layout_list.width
            height: 1
            anchors.top:layout_flickable.bottom
            z:-1
        }

        ListView {
            id: layout_footer
            clip: true
            width: parent.width
            height: childrenRect.height
            anchors.bottom: parent.bottom
            boundsBehavior: ListView.StopAtBounds
            currentIndex: -1
            model: {
                if (footerItems) {
                    return footerItems.children
                }
            }
            delegate: Loader {
                property var model: modelData
                property var _idx: index
                property int type: 1
                sourceComponent: com_footer_item
            }
            onCurrentIndexChanged: {
                if(layout_footer.currentIndex!==-1){
                    highlight_clip.clip=false
                    highlight_rectangle.height=footer_item_height*0.5
                    highlight_rectangle.y= layout_footer.y-layout_flickable.y+layout_footer.currentItem.y + (footer_item_height - footer_item_height*0.5) / 2
                }
            }

            onYChanged: {
                if(layout_footer.currentIndex!==-1){
                    highlight_clip.clip=false
                    highlight_rectangle.enableAnimation=false
                    highlight_rectangle.height=footer_item_height*0.5
                    highlight_rectangle.y= layout_footer.y-layout_flickable.y+layout_footer.currentItem.y + (footer_item_height - footer_item_height*0.5) / 2
                    highlight_rectangle.enableAnimation=true
                }
            }
        }
    }
}
