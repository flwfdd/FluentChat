import QtQuick
import QtQuick.Window
import FluentUI
import "qrc:/FluentChat/ui/view"


FluWindow {
    id: page_front
    title: "FluentChat"
    width: 1000
    height: 640
    minimumWidth: 520
    minimumHeight: 200
    visible: true
    launchMode: FluWindowType.SingleTask

    Item {
        anchors.fill: parent
        ChatListView {
            id: nav_view
            z: 999
            height: parent.height
            width: 300
            chatList: store.groupList
            footerItems: FluObject {

                property var navigationView

                id: footer_items

                FluPaneItem {
                    title: "关于"
                    icon: FluentIcons.Contact
                    tapFunc: function () {
                        loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/ChatView.qml")
                    }
                }

                FluPaneItem {
                    title: "设置"
                    icon: FluentIcons.Settings
                    tapFunc: function () {
                        loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/ConfigView.qml")
                    }
                }

            }
        }

        Loader {
            id: loader_content
            anchors {
                left: nav_view.right
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            sourceComponent: Qt.createComponent("qrc:/FluentChat/ui/view/ChatView.qml")
        }

        Connections {
            target: store
            function onCurrentGroupChanged() {
                loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/ChatView.qml")
            }
        }
    }

}

