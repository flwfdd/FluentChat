import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import Qt5Compat.GraphicalEffects
import "qrc:/FluentChat/ui/component/"

FluPage {
    anchors.fill: parent

    Component {
        id: message_text
        Item {
            id: message_text_item
            width: message_view.width - 10 //避开滚动条
            height: {
                return Math.max(message_text_avatar.height, message_text_rectangle.height + message_text_name.height) + message_text_time.height + 10
            }
            clip: true

            ChatAvatar {
                id: message_text_avatar
                bgColor: model.user.color
                avatar: model.user.avatar
                online: model.user.online
                size: 35
                anchors {
                    top: parent.top
                    left: isSender ? undefined : parent.left
                    right: isSender ? parent.right : undefined
                }
            }

            FluText {
                id: message_text_name
                text: model.user.remark ? model.user.remark : model.user.nickname
                maximumLineCount: 1
                horizontalAlignment: isSender ? Text.AlignRight : Text.AlignLeft
                elide: Text.ElideRight
                color: FluTheme.dark ? FluColors.Grey100 : FluColors.Grey100
                font.pixelSize: 12
                anchors {
                    top: parent.top
                    left: isSender ? message_text_item.left : message_text_avatar.right
                    leftMargin: 10
                    right: isSender ? message_text_avatar.left : message_text_item.right
                    rightMargin: 10
                }
            }

            Rectangle {
                id: message_text_rectangle
                width: {
                    let max_width = message_text_item.width * 0.75
                    let need_width = message_text_metrics.width + 20
                    return need_width > max_width ? max_width : need_width
                }
                height: message_text_content.contentHeight + 20
                anchors {
                    top: message_text_name.bottom
                    topMargin: 5
                    left: isSender ? undefined : message_text_avatar.right
                    leftMargin: 10
                    right: isSender ? message_text_avatar.left : undefined
                    rightMargin: 10
                }
                radius: 10
                color: {
                    if (isSender)
                        return FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    return FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
                }

                FluCopyableText {
                    id: message_text_content
                    text: model.content
                    wrapMode: Text.Wrap
                    color: FluTheme.dark ^ isSender ? FluColors.White : FluColors.Black
                    font.pixelSize: 14
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        margins: 10
                    }
                }

                FluCopyableText {
                    id: message_text_metrics
                    text: message_text_content.text
                    font: message_text_content.font
                    visible: false
                }
            }

            FluText {
                id: message_text_time
                text: Qt.formatDateTime(new Date(model.time * 1000), "yyyy-MM-dd hh:mm:ss") + " · #" + model.mid
                color: FluTheme.dark ? FluColors.Grey120 : FluColors.Grey80
                font.pixelSize: 10
                anchors {
                    top: message_text_rectangle.bottom
                    topMargin: 5
                    left: isSender ? undefined : message_text_rectangle.left
                    right: isSender ? message_text_rectangle.right : undefined
                }
            }
        }
    }

    Component {
        id: message_image
        Item {
            id: message_image_item
            width: message_view.width - 10 //避开滚动条
            height: {
                return Math.max(message_image_avatar.height, message_image_rectangle.height + message_image_name.height) + message_image_time.height + 10
            }
            clip: true

            ChatAvatar {
                id: message_image_avatar
                bgColor: model.user.color
                avatar: model.user.avatar
                online: model.user.online
                size: 35
                anchors {
                    top: parent.top
                    left: isSender ? undefined : parent.left
                    right: isSender ? parent.right : undefined
                }
            }

            FluText {
                id: message_image_name
                text: model.user.remark ? model.user.remark : model.user.nickname
                maximumLineCount: 1
                horizontalAlignment: isSender ? Text.AlignRight : Text.AlignLeft
                elide: Text.ElideRight
                color: FluTheme.dark ? FluColors.Grey100 : FluColors.Grey100
                font.pixelSize: 12
                anchors {
                    top: parent.top
                    left: isSender ? message_image_item.left : message_image_avatar.right
                    leftMargin: 10
                    right: isSender ? message_image_avatar.left : message_image_item.right
                    rightMargin: 10
                }
            }

            // Rectangle {
            //     id: message_image_rectangle
            //     width: {
            //         let max_width = message_text_item.width * 0.75
            //         let need_width = message_text_metrics.width + 20
            //         return need_width > max_width ? max_width : need_width
            //     }
            //     height: message_text_content.contentHeight + 20
            //     anchors {
            //         top: message_text_name.bottom
            //         topMargin: 5
            //         left: isSender ? undefined : message_text_avatar.right
            //         leftMargin: 10
            //         right: isSender ? message_text_avatar.left : undefined
            //         rightMargin: 10
            //     }
            //     radius: 10
            //     color: {
            //         if (isSender)
            //             return FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
            //         return FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
            //     }
            // }


            FluRectangle {
                id: message_image_rectangle
                width: 200
                height: 200
                radius: [10, 10, 10, 10]
                anchors {
                    top: message_image_name.bottom
                    topMargin: 5
                    left: isSender ? undefined : message_image_avatar.right
                    leftMargin: 10
                    right: isSender ? message_image_avatar.left : undefined
                    rightMargin: 10
                }
                Image {
                    id: message_image_image
                    asynchronous: true
                    source: model.content
                    anchors.fill: parent
                    onStatusChanged: {
                        if (status == Image.Ready) {
                            var max_size = Math.min(message_image_item.width * 0.5, 424)
                            var scale = max_size / Math.max(sourceSize.width, sourceSize.height)
                            message_image_rectangle.width = sourceSize.width * scale
                            message_image_rectangle.height = sourceSize.height * scale
                        }
                    }
                }
            }

            onWidthChanged: {
                if (message_image_image.status == Image.Ready) {
                    var max_size = Math.min(message_image_item.width * 0.5, 666)
                    var scale = max_size / Math.max(message_image_image.sourceSize.width, message_image_image.sourceSize.height)
                    message_image_rectangle.width = message_image_image.sourceSize.width * scale
                    message_image_rectangle.height = message_image_image.sourceSize.height * scale
                }
            }

            FluText {
                id: message_image_time
                text: Qt.formatDateTime(new Date(model.time * 1000), "yyyy-MM-dd hh:mm:ss") + " · #" + model.mid
                color: FluTheme.dark ? FluColors.Grey120 : FluColors.Grey80
                font.pixelSize: 10
                anchors {
                    top: message_image_rectangle.bottom
                    topMargin: 5
                    left: isSender ? undefined : message_image_rectangle.left
                    right: isSender ? message_image_rectangle.right : undefined
                }
            }
        }
    }

    Rectangle {
        id: header
        width: parent.width
        height: 45
        z: 1
        radius: 10
        color: FluTheme.dark ? Window.active ? Qt.rgba(38 / 255, 44 / 255, 54 / 255, 1) : Qt.rgba(39 / 255, 39 / 255, 39 / 255, 1) : Qt.rgba(251 / 255, 251 / 255, 253 / 255, 1)
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
        }

        FluText {
            text: {
                if (!store.currentGroup) return ""
                let s = ""
                if (store.currentGroup.type === "twin") {
                    s += store.currentGroup.owner.nickname
                } else {
                    s += store.currentGroup.remark ? store.currentGroup.remark : store.currentGroup.name
                    if (store.currentGroupUsers.length > 0)
                        s += "（" + store.currentGroupUsers.length + "）"
                }
                return s
            }
            color: FluTheme.dark ? FluColors.White : FluColors.Black
            font.pixelSize: 20
            elide: Text.ElideRight
            anchors {
                left: parent.left
                leftMargin: 20
                right: header_icon.left
                verticalCenter: parent.verticalCenter
            }
        }

        FluIconButton {
            id: header_icon
            iconSource: FluentIcons.Info
            iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
            anchors {
                right: parent.right
                rightMargin: 20
                verticalCenter: parent.verticalCenter
            }
            onClicked: {
                info_popup.visible = true
            }
        }

        Popup {
            id: info_popup
            modal: true
            visible: false
            width: 200
            height: 200
            x: parent.width - width
            y: parent.height + 10
            clip: true

            enter: Transition {
                NumberAnimation {
                    property: "height"
                    from: 0
                    to: 200
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
                    from: 200
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
                radius: 10
                border.width: 0
            }


            ListView {
                id: info_list
                width: parent.width
                height: parent.height
                spacing: 10
                orientation: ListView.Vertical
                ScrollBar.vertical: FluScrollBar {
                }
                boundsBehavior: Flickable.DragOverBounds
                model: store.currentGroupUsers

                header: Item {
                    visible: store.currentGroup && store.currentGroup.type !== "twin"
                    height: store.currentGroup && store.currentGroup.type !== "twin" ? info_text.height + 20 : 0
                    width: info_list.width
                    FluText {
                        id: info_text
                        text: store.currentGroup ? "群号：#" + store.currentGroup.id + "\n群主：" + store.currentGroup.owner.username + "\n人数：" + store.currentGroupUsers.length + "\n群名：" + store.currentGroup.name : ""
                        color: FluTheme.dark ? FluColors.Grey100 : FluColors.Grey100
                        wrapMode: Text.WrapAnywhere
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                            margins: 10
                        }
                    }
                }

                delegate: Item {
                    width: info_list.width
                    height: 50
                    Rectangle {
                        id: info_item
                        property bool hoverd: false
                        width: parent.width
                        height: parent.height
                        radius: 10
                        color: hoverd ? (FluTheme.dark ? "#11FFFFFF" : "#11000000") : "transparent"

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                info_item.hoverd = true
                            }
                            onExited: {
                                info_item.hoverd = false
                            }
                        }

                        FluTooltip {
                            visible: info_item.hoverd
                            text: modelData.username
                            delay: 1000
                        }

                        ChatAvatar {
                            id: info_avatar
                            bgColor: modelData.color
                            avatar: modelData.avatar
                            online: modelData.online
                            size: 30
                            anchors {
                                left: parent.left
                                leftMargin: 5
                                verticalCenter: parent.verticalCenter
                            }
                        }

                        FluText {
                            text: modelData.remark ? modelData.remark : modelData.nickname
                            color: FluTheme.dark ? FluColors.White : FluColors.Black
                            font.pixelSize: 14
                            elide: Text.ElideRight
                            anchors {
                                left: info_avatar.right
                                leftMargin: 5
                                right: parent.right
                                rightMargin: 5
                                top: info_avatar.top
                            }
                        }

                        FluText {
                            text: modelData.username
                            color: FluTheme.dark ? FluColors.Grey100 : FluColors.Grey100
                            font.pixelSize: 10
                            elide: Text.ElideRight
                            anchors {
                                left: info_avatar.right
                                leftMargin: 5
                                right: parent.right
                                rightMargin: 5
                                bottom: info_avatar.bottom
                            }
                        }
                    }
                }
            }
        }

    }

    Item {
        id: message_view_mask //防止最上面和最下面的空白显示消息
        clip: true
        anchors {
            top: header.bottom
            topMargin: -10
            left: parent.left
            leftMargin: 20
            right: parent.right
            rightMargin: 10 //这里不是20是因为有个滚动条
            bottom: input_area.top
            bottomMargin: -10
        }
        ListView {
            id: message_view
            spacing: 20
            orientation: ListView.Vertical
            ScrollBar.vertical: FluScrollBar {
            }
            boundsBehavior: Flickable.DragOverBounds
            anchors {
                fill: parent
                topMargin: 10
                bottomMargin: 10
            }

            model: store.messageList.items

            header: Rectangle {
                width: message_view.width
                height: 60
                color: "transparent"
                FluTextButton {
                    text: store.messageList.hasMore ? "加载更多" : "没有更多了QwQ"
                    anchors.centerIn: parent
                    onClicked: {
                        message_view.loading = true
                        store.control.loadMessages()
                    }
                    disabled: message_view.loading || !store.messageList.hasMore
                }
            }

            footer: Rectangle {
                width: message_view.width
                height: 20
                visible: false
            }

            delegate: Loader {
                id: message_loader
                property var model: modelData
                property bool isSender: modelData.user === store.currentUser
                sourceComponent: {
                    if (model.type === "text") {
                        return message_text
                    }
                    if (model.type === "image") {
                        return message_image
                    }
                }
            }

            property bool loading: true
            property var lastGroupWhenBottom: null
            property var oldFirst: 0
            onModelChanged: {
                if (lastGroupWhenBottom !== store.currentGroup && message_view.contentHeight > message_view.height) {
                    lastGroupWhenBottom = store.currentGroup
                    message_view.positionViewAtEnd()
                    if (message_view.model.length) {
                        console.log(message_view.model[0])
                        oldFirst = message_view.model[0].id
                    }
                    loading = false
                } else {
                    if (message_view.model.length && oldFirst !== message_view.model[0].id) {
                        console.log(message_view.model[0], oldFirst)
                        loading = false
                        var idx = 0
                        for (var i = 0; i < message_view.model.length; i++) {
                            if (message_view.model[i].id === oldFirst) {
                                idx = i
                                break
                            }
                        }
                        message_view.positionViewAtIndex(Math.max(0, idx - 1), ListView.Beginning)
                        oldFirst = message_view.model[0].id
                    } else {
                        message_view.positionViewAtEnd()
                    }
                }
            }
        }
    }


    FluArea {
        id: input_area
        height: editExpand ? parent.height * 0.5 : 150
        paddings: 15
        radius: 10
        border.width: 0
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            bottomMargin: 10
        }

        property bool editExpand: false
        Behavior on height {
            enabled: FluTheme.enableAnimation
            NumberAnimation {
                duration: 666
                easing.type: Easing.InOutExpo
            }
        }

        RowLayout {
            id: button_group
            width: parent.width

            Row {
                spacing: 5
                FluIconButton {
                    iconSource: input_area.editExpand ? FluentIcons.ChevronDown : FluentIcons.ChevronUp
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                        input_area.editExpand = !input_area.editExpand
                    }
                }

                FluIconButton {
                    iconSource: FluentIcons.Folder
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                    }
                }

                FluIconButton {
                    iconSource: FluentIcons.Photo
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                    }
                }

                FluIconButton {
                    iconSource: FluentIcons.Cut
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                        message_view.positionViewAtEnd();
                    }
                }
            }

            Row {
                spacing: 5
                Layout.alignment: Qt.AlignRight
                FluIconButton {
                    iconSource: FluentIcons.Send
                    iconColor: FluTheme.dark ? FluTheme.primaryColor.lighter : FluTheme.primaryColor.dark
                    onClicked: {
                        input_area.sendMessage()
                    }
                }
            }


        }

        Item {
            id: text_box_container
            anchors {
                top: button_group.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 10
            }

            Flickable {
                clip: true
                anchors.fill: parent
                contentWidth: parent.width
                contentHeight: text_box.height
                ScrollBar.vertical: FluScrollBar {
                }
                boundsBehavior: Flickable.StopAtBounds

                FluMultilineTextBox {
                    id: text_box
                    placeholderText: "Tips：使用Ctrl+Enter换行OvO"
                    width: parent.width
                    height: contentHeight + 20 < text_box_container.height ? text_box_container.height : contentHeight + 20
                    padding: 0
                    background: Rectangle {
                        color: "transparent"
                    }
                    onCommit: {
                        input_area.sendMessage()
                    }
                }
            }
        }

        function sendMessage() {
            if (text_box.text === "") return
            store.control.sendMessage(store.currentGroup.id, "text", text_box.text)
            text_box.text = ""
            message_view.positionViewAtEnd()
        }
    }

    // 底部边框效果
    Rectangle {
        id: bottom_line_mask
        anchors.fill: input_area
        color: "transparent"

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: bottom_line_mask.width
                height: bottom_line_mask.height
                radius: 10
            }
        }

        Rectangle {
            width: text_box.activeFocus ? parent.width : 0
            height: text_box.activeFocus ? 5 : 1
            anchors {
                bottom: parent.bottom
                left: parent.left
                leftMargin: text_box.activeFocus ? 0 : parent.width / 2

                Behavior on leftMargin {
                    enabled: FluTheme.enableAnimation
                    NumberAnimation {
                        duration: 666
                        easing.type: Easing.InOutExpo
                    }
                }
            }

            color: {
                if (FluTheme.dark) {
                    text_box.activeFocus ? FluTheme.primaryColor.lighter : Qt.rgba(166 / 255, 166 / 255, 166 / 255, 1)
                } else {
                    return text_box.activeFocus ? FluTheme.primaryColor.dark : Qt.rgba(183 / 255, 183 / 255, 183 / 255, 1)
                }
            }
            Behavior on height {
                enabled: FluTheme.enableAnimation
                NumberAnimation {
                    duration: 666
                    easing.type: Easing.OutExpo
                }
            }
            Behavior on width {
                enabled: FluTheme.enableAnimation
                NumberAnimation {
                    duration: 666
                    easing.type: Easing.InOutExpo
                }
            }

        }
    }


}
