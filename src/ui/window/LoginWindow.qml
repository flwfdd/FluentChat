import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI
import "qrc:/FluentChat/ui/component/"


FluWindow {
    id: window_login
    title: "FluentChat"
    width: 400
    height: 600
    minimumWidth: 400
    minimumHeight: 600
    visible: true
    launchMode: FluWindowType.SingleTask

    property bool hasCookie: false
    property bool register: false
    property var loginUsername

    Column {
        spacing: 10
        visible: hasCookie
        width: 200
        anchors.centerIn: parent

        FluText {
            text: "Fluent Chat"
            font.pixelSize: 33
            color: FluTheme.primaryColor.normal
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.baseline
                anchors.verticalCenterOffset: height / 3
                color: FluTheme.primaryColor.normal
                opacity: 0.5
                width: parent.width
                height: 6
                radius: height / 2
                z: -1
            }
        }

        FluText {
            text: window_login.loginUsername + " 已经登陆"
            color: FluTheme.primaryColor.normal
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            anchors.topMargin: 20
        }


        FluFilledButton {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: "直接登录"
            onClicked: {
                store.control.init()
                timer.start()
            }
        }
        FluTextButton {
            id: delete_cookie_button
            width: parent.width
            text: "退出登录"
            onClicked: {
                store.setConfig("cookie", "")
                store.setConfig("loginUid", "")
                store.setConfig("loginUsername", "")
                window_login.hasCookie = false
            }
        }
    }

    Column {
        spacing: 10
        visible: !window_login.register && !hasCookie
        width: 200
        anchors.centerIn: parent

        FluText {
            id: login_title
            text: "Fluent Chat"
            font.pixelSize: 33
            color: FluTheme.primaryColor.normal
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.baseline
                anchors.verticalCenterOffset: height / 3
                color: FluTheme.primaryColor.normal
                opacity: 0.5
                width: parent.width
                height: 6
                radius: height / 2
                z: -1
            }
        }

        FluTextBox {
            id: login_username
            width: parent.width
            placeholderText: "用户名"
        }
        FluPasswordBox {
            id: login_password
            width: parent.width
            placeholderText: "密码"
        }
        FluFilledButton {
            id: login_button
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: "登录"
            onClicked: {
                if (check()) {
                    var salt = "FluentChat"
                    var ori_password = login_password.text + salt
                    var hash = Qt.md5(ori_password)
                    store.control.login(login_username.text, hash)
                }
            }

            function check() {
                if (login_username.text.length === 0) {
                    showError("用户名不能为空")
                    return false
                }
                var regex = /^[a-zA-Z0-9]+$/
                if (!regex.test(login_username.text)) {
                    showError("用户名只能包含字母和数字")
                    return false
                }
                if (login_password.text.length === 0) {
                    showError("密码不能为空")
                    return false
                }
                return true
            }
        }
        FluTextButton {
            id: to_register_button
            width: parent.width
            text: "注册"
            onClicked: {
                window_login.register = true
            }
        }
    }

    Column {
        spacing: 10
        visible: window_login.register
        width: 200
        anchors.centerIn: parent

        ChatAvatar {
            id: avatar_preview
            bgColor: register_color.colorValue
            avatar: register_avatar.text
            online: true
            size: 50
            anchors.horizontalCenter: parent.horizontalCenter
        }

        FluTextBox {
            id: register_username
            width: parent.width
            placeholderText: "用户名"
        }
        FluPasswordBox {
            id: regiter_password
            width: parent.width
            placeholderText: "密码"
        }
        FluPasswordBox {
            id: regiter_password_confirm
            width: parent.width
            placeholderText: "确认密码"
        }
        FluTextBox {
            id: register_nickname
            width: parent.width
            placeholderText: "昵称"
        }
        FluTextBox {
            id: register_avatar
            width: parent.width
            placeholderText: "头像字（可为Emoji）"
        }
        FluColorPicker {
            id: register_color
            width: parent.width

            FluText {
                text: "头像色"
                color: "white"
                anchors.centerIn: parent
            }

            Component.onCompleted: {
                register_color.colorValue = FluTheme.primaryColor.normal
            }
        }
        FluFilledButton {
            id: register_button
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: "注册"
            onClicked: {
                if (check()) {
                    var salt = "FluentChat"
                    var ori_password = regiter_password.text + salt
                    var hash = Qt.md5(ori_password)
                    store.control.registerUser(register_username.text, hash, register_nickname.text, register_color.colorValue, register_avatar.text)
                }
            }

            function check() {
                if (register_username.text.length === 0) {
                    showError("用户名不能为空")
                    return false
                }
                var regex = /^[a-zA-Z0-9]+$/
                if (!regex.test(register_username.text)) {
                    showError("用户名只能包含字母和数字")
                    return false
                }
                if (regiter_password.text.length === 0) {
                    showError("密码不能为空")
                    return false
                }
                if (regiter_password.text !== regiter_password_confirm.text) {
                    showError("两次输入的密码不一致")
                    return false
                }
                if (register_nickname.text.length === 0) {
                    showError("昵称不能为空")
                    return false
                }
                if (register_avatar.text.length === 0) {
                    showError("头像字不能为空")
                    return false
                }
                return true
            }
        }
        FluTextButton {
            id: to_login_register_button
            width: parent.width
            text: "登录"
            onClicked: {
                window_login.register = false
            }
        }
    }

    Component.onCompleted: {
        if (store.getConfig("cookie") !== "") {
            window_login.hasCookie = true
            window_login.loginUsername = store.getConfig("loginUsername")
        }
    }

    Connections {
        target: store

        function onErrorMsgChanged() {
            showError(store.errorMsg)
        }

        function onSuccessMsgChanged() {
            showSuccess(store.successMsg)
        }

        function onIsLoginChanged() {
            if (store.isLogin) {
                timer.start()
            }
        }
    }

    Timer {
        id: timer
        interval: 424
        repeat: false
        onTriggered: {
            FluApp.navigate("/")
            window_login.close()
        }
    }

}

