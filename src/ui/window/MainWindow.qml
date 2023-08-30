import QtQuick
import QtQuick.Window
import FluentUI
import "qrc:/FluentChat/ui/view"


FluWindow {
    id: page_front
    title: "FluentChat"
    width: 1000
    height: 618
    minimumWidth: 666
    minimumHeight: 424
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
                    title: "设置"
                    icon: FluentIcons.Settings
                    onTap: {
                        loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/ConfigView.qml")
                    }
                }

                FluPaneItem {
                    title: "关于"
                    icon: FluentIcons.Contact
                    onTap: {
                        loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/AboutView.qml")
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
            sourceComponent: Qt.createComponent("qrc:/FluentChat/ui/view/AboutView.qml")
        }

        Connections {
            target: store
            property bool lastIsChatView : false

            function onCurrentGroupChanged() {
                if (store.currentGroup !== null) {
                    if(!lastIsChatView){
                        lastIsChatView = true
                        loader_content.sourceComponent = Qt.createComponent("qrc:/FluentChat/ui/view/ChatView.qml")
                    }

                } else {
                    lastIsChatView = false
                }

            }
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
    }
}

