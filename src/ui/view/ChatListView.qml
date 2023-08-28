import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import FluentUI
import "qrc:/FluentChat/ui/global"
import "qrc:/FluentChat/ui/component"

Item {
    id: control
    property var chatList
    property FluObject footerItems
    property Component autoSuggestBox
    property double chat_item_height: 66
    property double footer_item_height: 42

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
                    chat_list.currentIndex = _idx
                    layout_footer.currentIndex = -1
                    store.control.openGroup(model)
                }
                Rectangle {
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if (FluTheme.dark) {
                            if (chat_list.currentIndex === _idx) {
                                return Qt.rgba(1, 1, 1, 0.06)
                            }
                            if (item_control.hovered) {
                                return Qt.rgba(1, 1, 1, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        } else {
                            if (chat_list.currentIndex === _idx) {
                                return Qt.rgba(0, 0, 0, 0.06)
                            }
                            if (item_control.hovered) {
                                return Qt.rgba(0, 0, 0, 0.03)
                            }
                            return Qt.rgba(0, 0, 0, 0)
                        }
                    }

                    ChatAvatar {
                        id: item_avatar
                        avatar: model.type === "twin" ? model.owner.avatar : model.avatar
                        bgColor: model.type === "twin" ? model.owner.color : model.color
                        online: model.type === "twin" ? model.owner.online : false
                        size: 42
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 10
                        }
                    }

                    FluText {
                        id: item_title
                        text: model.type === "twin" ? (model.owner.remark ? model.owner.remark : model.owner.nickname) : (model.remark ? model.remark : model.name)
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
                            left: item_avatar.right
                            leftMargin: 10
                            right: time_text.left
                            rightMargin: 5
                            verticalCenter: parent.verticalCenter
                            verticalCenterOffset: -10
                        }
                    }
                    FluText {
                        id: item_text
                        text: {
                            if (!model.last) return ""
                            let text = ""
                            if (model.type !== "twin") text = model.last.user.remark ? model.last.user.remark : model.last.user.nickname + "："
                            switch (model.last.type) {
                                case "text":
                                    text += model.last.content
                                    break
                            }
                            return text
                        }
                        elide: Text.ElideRight
                        maximumLineCount: 1
                        font.pixelSize: 12
                        color: {
                            if (item_control.pressed) {
                                return FluTheme.dark ? FluColors.Grey120 : FluColors.Grey80
                            }
                            return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                        }
                        anchors {
                            left: item_avatar.right
                            leftMargin: 10
                            right: unread_badge.left
                            rightMargin: 5
                            verticalCenter: parent.verticalCenter
                            verticalCenterOffset: 10
                        }
                    }

                    FluText {
                        id: time_text
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            verticalCenterOffset: -10
                            rightMargin: 10
                        }
                        text: model.last ? GlobalTool.formatTime(model.last.time) : ""
                        font.pixelSize: 10
                        color: item_text.color
                    }

                    Rectangle {
                        id: unread_badge
                        color: Qt.rgba(255 / 255, 77 / 255, 79 / 255, 1)
                        width: {
                            if (model.unreadNum < 10) {
                                return 20
                            } else if (model.unreadNum < 100) {
                                return 30
                            }
                            return 35
                        }
                        height: 20
                        radius: 10
                        border.width: 0
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            verticalCenterOffset: 10
                            rightMargin: 10
                        }
                        visible: model.unreadNum !== 0
                        Text {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
                                verticalCenterOffset: 1
                            }

                            color: Qt.rgba(1, 1, 1, 1)
                            text: {
                                if (model.unreadNum < 100)
                                    return model.unreadNum
                                return "99+"
                            }
                            font.pixelSize: 10
                            font.bold: true
                        }
                    }
                }
            }
        }
    }

    Component {
        id: footer_item
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
                    chat_list.currentIndex = -1
                    chatList.currentItem = null
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
                            right: parent.right
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
        color: FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
        border.color: FluTheme.dark ? Qt.rgba(45 / 255, 45 / 255, 45 / 255, 1) : Qt.rgba(226 / 255, 230 / 255, 234 / 255, 1)
        border.width: 1
        Item {
            id: layout_header
            width: layout_list.width
            height: 50
            FluAutoSuggestBox {
                anchors {
                    left: parent.left
                    leftMargin: 15
                    right: add_button.left
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                iconSource: FluentIcons.Search
                //            items: ItemsOriginal.getSearchData()
                placeholderText: "搜索"
                onItemClicked:
                        (data) => {
                    // ItemsOriginal.startPageByItem(data)
                }
            }
            FluIconButton {
                id: add_button
                anchors {
                    right: parent.right
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                iconSource: FluentIcons.Add
                iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                onClicked: {
                    // ItemsOriginal.startPageByItem(data)
                }
            }
        }

        Item {
            id: highlight_clip
            anchors.fill: chat_list
            clip: true

            Rectangle {
                id: highlight_rectangle
                height: chat_item_height
                color: FluTheme.primaryColor.normal
                width: 4
                radius: width / 2
                anchors {
                    left: parent.left
                    leftMargin: 6
                }
                property bool enableAnimation: true

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


        ListView {
            id: chat_list
            clip: true
            anchors {
                top: layout_header.bottom
                left: parent.left
                right: parent.right
                bottom: layout_footer.top
            }
            model: chatList.items
            ScrollBar.vertical: FluScrollBar {
            }
            boundsBehavior: Flickable.DragOverBounds

            onCurrentIndexChanged: {
                if (chat_list.currentIndex !== -1) {
                    highlight_clip.clip = true
                    highlight_rectangle.height = chat_item_height * 0.5
                    highlight_rectangle.y = chat_list.currentItem.y - chat_list.contentY + (chat_item_height - chat_item_height * 0.5) / 2
                }
            }

            Connections {
                // 监听数据源变化
                target: chatList

                function onItemsChanged() {
                    updateList()
                }

                function updateList() {
                    for (var i = 0; i < chatList.items.length; i++) {
                        if (chatList.items[i] === chatList.currentItem) {
                            chat_list.currentIndex = i
                        }
                    }

                    // 防止列表更新时滚轮自动移动到选中项
                    chat_list.contentY = chat_list.lastContentY
                }
            }

            property var lastTopItem
            property double lastContentY: 0
            onContentYChanged: {
                var imm = (lastContentY - chat_list.contentY != 0.0) // 高亮是否关闭动画 用于滚动跟随
                if (chat_list.lastTopItem === chatList.items[0]) {
                    lastContentY = chat_list.contentY
                } else chat_list.lastTopItem = chatList.items[0]


                if (chat_list.currentIndex !== -1 && chat_list.currentItem) {
                    highlight_clip.clip = true
                    if (imm) highlight_rectangle.enableAnimation = false
                    highlight_rectangle.height = chat_item_height * 0.5
                    highlight_rectangle.y = chat_list.currentItem.y - chat_list.contentY + (chat_item_height - chat_item_height * 0.5) / 2
                    if (imm) highlight_rectangle.enableAnimation = true
                }
            }

            delegate: Loader {
                property var model: modelData
                property var _idx: index
                property int type: 0
                sourceComponent: chat_item
            }
        }

        // 底部菜单
        ListView {
            id: layout_footer
            clip: true
            width: parent.width
            height: contentHeight
            anchors.bottom: parent.bottom
            boundsBehavior: ListView.StopAtBounds
            currentIndex: -1
            model: {
                if (footerItems) {
                    return footerItems.children
                }
            }

            // 顶部分割线
            header: Rectangle {
                color: FluTheme.dark ? Qt.rgba(80 / 255, 80 / 255, 80 / 255, 1) : Qt.rgba(210 / 255, 210 / 255, 210 / 255, 1)
                width: layout_list.width
                height: 1
                z: -1
            }

            delegate: Loader {
                property var model: modelData
                property var _idx: index
                property int type: 1
                sourceComponent: footer_item
            }

            onCurrentIndexChanged: {
                if (layout_footer.currentIndex !== -1) {
                    highlight_clip.clip = false
                    highlight_rectangle.height = footer_item_height * 0.5
                    highlight_rectangle.y = layout_footer.y - chat_list.y + layout_footer.currentItem.y + (footer_item_height - footer_item_height * 0.5) / 2
                }
            }

            onYChanged: {
                if (layout_footer.currentIndex !== -1) {
                    highlight_clip.clip = false
                    highlight_rectangle.enableAnimation = false
                    highlight_rectangle.height = footer_item_height * 0.5
                    highlight_rectangle.y = layout_footer.y - chat_list.y + layout_footer.currentItem.y + (footer_item_height - footer_item_height * 0.5) / 2
                    highlight_rectangle.enableAnimation = true
                }
            }
        }
    }
}
