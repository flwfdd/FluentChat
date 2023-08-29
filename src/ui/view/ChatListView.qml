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
                    model.tap()
                    layout_footer.currentIndex = _idx
                    chat_list.currentIndex = -1
                    store.currentGroup = null
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
                items: {
                    var groups = store.groupList.items
                    var result = []
                    for (var i = 0; i < groups.length; i++) {
                        var group = groups[i]
                        if (group.type === "twin") {
                            var user = group.owner
                            if (user.nickname.indexOf(text) !== -1 || user.remark.indexOf(text) !== -1 || user.username.indexOf(text) !== -1) {
                                result.push({title: user.remark ? user.remark : user.nickname, key: group.id})
                            }
                        } else {
                            if (group.name.indexOf(text) !== -1 || group.remark.indexOf(text) !== -1) {
                                result.push({title: group.remark ? user.remark : group.name, key: group.id})
                            }
                        }
                    }
                    return result
                }
                placeholderText: "搜索"
                onItemClicked: (data) => {
                    for (var i = 0; i < store.groupList.items.length; i++) {
                        if (store.groupList.items[i].id === data.key) {
                            chat_list.currentIndex = i
                            layout_footer.currentIndex = -1
                            store.control.openGroup(store.groupList.items[i])
                            return
                        }
                    }
                }
            }
            FluIconButton {
                id: add_button
                anchors {
                    right: parent.right
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                iconSource: FluentIcons.AddBold
                iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                onClicked: {
                    add_popup.visible = true
                }
            }


            Popup {
                id: add_popup
                modal: true
                visible: false
                width: 150
                x: add_button.x - (add_popup.width - add_button.width) / 2
                y: add_button.y + add_button.height
                topInset: 5
                bottomInset: 5
                leftInset: 5
                rightInset: 5
                clip: true

                enter: Transition {
                    NumberAnimation {
                        property: "height"
                        from: 0
                        to: 155
                        duration: 233
                        easing.type: Easing.InOutExpo
                    }

                    NumberAnimation {
                        property:"opacity"
                        from:0
                        to: 1
                        duration: 233
                    }
                }
                exit: Transition {
                    NumberAnimation {
                        property: "height"
                        from: 155
                        to: 0
                        duration: 233
                        easing.type: Easing.InOutExpo
                    }

                    NumberAnimation {
                        property:"opacity"
                        from:1
                        to: 0
                        duration: 233
                    }
                }

                background: FluArea {
                    radius: 5
                    border.width: 0
                }


                Column {
                    id: add_popup_column
                    width: parent.width
                    spacing: 5

                    Repeater {
                        model: [
                            {
                                text: "添加好友", icon: FluentIcons.AddFriend, onClick: () => {
                                    add_user_dialog.visible = true
                                }
                            },
                            {
                                text: "加入群聊", icon: FluentIcons.ChatBubbles, onClick: () => {
                                    add_group_dialog.visible = true
                                }
                            },
                            {
                                text: "创建群聊", icon: FluentIcons.VideoChat, onClick: () => {
                                    create_group_dialog.visible = true
                                }
                            },
                        ]

                        delegate: Item {
                            width: add_popup_column.width
                            height: 40
                            Rectangle {
                                id: add_popup_item
                                property bool hoverd: false
                                width: parent.width
                                height: parent.height
                                radius: 5
                                color: hoverd ? (FluTheme.dark ? "#11FFFFFF" : "#11000000") : "transparent"

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        add_popup.visible = false
                                        modelData.onClick()
                                    }
                                    onEntered: {
                                        add_popup_item.hoverd = true
                                    }
                                    onExited: {
                                        add_popup_item.hoverd = false
                                    }
                                }

                                FluIcon {
                                    id: add_popup_icon
                                    anchors {
                                        left: parent.left
                                        leftMargin: 10
                                        verticalCenter: parent.verticalCenter
                                    }
                                    iconSource: modelData.icon
                                    iconColor: FluTheme.primaryColor.normal
                                    iconSize: 20
                                }

                                FluText {
                                    id: add_popup_text
                                    text: modelData.text
                                    font.pixelSize: 14
                                    anchors {
                                        left: add_popup_icon.right
                                        leftMargin: 10
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }
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

    Popup {
        id: add_user_dialog
        modal: true
        width: 300
        height: 250
        visible: false
        opacity: 0
        anchors.centerIn: Overlay.overlay
        background: Rectangle {
            color: "transparent"
        }
        enter: Transition {
            NumberAnimation {
                property: "opacity";
                from: 0.0;
                to: 1.0
            }
        }
        exit: Transition {
            NumberAnimation {
                property: "opacity";
                from: 1.0;
                to: 0.0
            }
        }

        FluArea {
            anchors.fill: parent
            radius: 10

            Column {
                id: add_user_dialog_column
                spacing: 10
                anchors.centerIn: parent
                width: 200

                ChatAvatar {
                    id: add_user_avatar
                    bgColor: "#aef"
                    avatar: "🤗"
                    online: true
                    size: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                FluTextBox {
                    id: add_user_textbox
                    placeholderText: "用户名"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                FluButton {
                    text: "添加好友"
                    width: parent.width
                    onClicked: {
                        if (add_user_textbox.text) {
                            store.control.requestUser(add_user_textbox.text)
                            add_user_dialog.visible = false
                        } else {
                            showError("用户名不能为空")
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: add_group_dialog
        modal: true
        width: 300
        height: 250
        visible: false
        opacity: 0
        anchors.centerIn: Overlay.overlay
        background: Rectangle {
            color: "transparent"
        }
        enter: Transition {
            NumberAnimation {
                property: "opacity";
                from: 0.0;
                to: 1.0
            }
        }
        exit: Transition {
            NumberAnimation {
                property: "opacity";
                from: 1.0;
                to: 0.0
            }
        }

        FluArea {
            anchors.fill: parent
            radius: 10

            Column {
                id: add_group_dialog_column
                spacing: 10
                anchors.centerIn: parent
                width: 200

                ChatAvatar {
                    id: add_group_avatar
                    bgColor: "#aef"
                    avatar: "🥳"
                    online: true
                    size: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                FluTextBox {
                    id: add_group_textbox
                    placeholderText: "群号"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                FluButton {
                    text: "加入群组"
                    width: parent.width
                    onClicked: {
                        if (add_group_textbox.text && !isNaN(add_group_textbox.text)) {
                            store.control.requestGroup(add_group_textbox.text)
                            add_group_dialog.visible = false
                        } else {
                            showError("群号不对哦")
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: create_group_dialog
        modal: true
        width: 300
        height: 350
        visible: false
        opacity: 0
        anchors.centerIn: Overlay.overlay
        background: Rectangle {
            color: "transparent"
        }
        enter: Transition {
            NumberAnimation {
                property: "opacity";
                from: 0.0;
                to: 1.0
            }
        }
        exit: Transition {
            NumberAnimation {
                property: "opacity";
                from: 1.0;
                to: 0.0
            }
        }

        FluArea {
            anchors.fill: parent
            radius: 10

            Column {
                id: create_group_dialog_column
                spacing: 10
                anchors.centerIn: parent
                width: 200

                ChatAvatar {
                    bgColor: create_group_color.colorValue
                    avatar: create_group_avatar.text
                    online: true
                    size: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                FluTextBox {
                    id: create_group_name
                    width: parent.width
                    placeholderText: "群名"
                }

                FluTextBox {
                    id: create_group_avatar
                    width: parent.width
                    placeholderText: "头像字（可为Emoji）"
                }
                FluColorPicker {
                    id: create_group_color
                    width: parent.width

                    FluText {
                        text: "头像色"
                        color: "white"
                        anchors.centerIn: parent
                    }

                    Component.onCompleted: {
                        create_group_color.colorValue = FluTheme.primaryColor.normal
                    }
                }

                FluButton {
                    text: "创建群组"
                    width: parent.width
                    onClicked: {
                        if (create_group_name.text) {
                            store.control.createGroup(create_group_name.text, create_group_avatar.text, create_group_color.colorValue)
                            create_group_dialog.visible = false
                        } else {
                            showError("群名不能为空")
                        }
                    }
                }
            }
        }
    }

}
