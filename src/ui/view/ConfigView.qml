import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtMultimedia
import FluentUI


FluScrollablePage {
    title: "设置"
    topPadding: 20
    leftPadding: 20
    rightPadding: 20
    spacing: 20

    FluArea {
        Layout.topMargin: 20
        Layout.fillWidth: true
        height: dark_layout.height + 30
        paddings: 15
        border.width: 0
        radius: 10

        ColumnLayout {
            id: dark_layout
            spacing: 5
            anchors {
                top: parent.top
                left: parent.left
            }
            FluText {
                text: "深色模式"
                font: FluTextStyle.BodyStrong
                Layout.bottomMargin: 4
            }
            Repeater {
                model: [{title: "跟随系统", mode: FluThemeType.System}, {
                    title: "始终关闭",
                    mode: FluThemeType.Light
                }, {title: "始终开启", mode: FluThemeType.Dark}]
                delegate: FluRadioButton {
                    checked: FluTheme.darkMode === modelData.mode
                    text: modelData.title
                    clickListener: function () {
                        FluTheme.darkMode = modelData.mode
                        if (modelData.mode === FluThemeType.System) {
                            store.setConfig("darkMode", "System")
                        } else if (modelData.mode === FluThemeType.Light) {
                            store.setConfig("darkMode", "Light")
                        } else {
                            store.setConfig("darkMode", "Dark")
                        }
                    }
                }
            }
        }
    }

    FluArea {
        Layout.fillWidth: true
        height: color_layout.height + 30
        paddings: 15
        border.width: 0
        radius: 10

        ColumnLayout {
            id: color_layout
            spacing: 5
            anchors {
                top: parent.top
                left: parent.left
            }
            FluText {
                text: "主题色"
                font: FluTextStyle.BodyStrong
                Layout.bottomMargin: 4
            }

            RowLayout {
                Layout.topMargin: 10
                Repeater {
                    model: [FluColors.Yellow, FluColors.Orange, FluColors.Red, FluColors.Magenta, FluColors.Purple, FluColors.Blue, FluColors.Teal, FluColors.Green]
                    delegate: FluRectangle {
                        width: 42
                        height: 42
                        radius: [4, 4, 4, 4]
                        color: mouse_item.containsMouse ? Qt.lighter(modelData.normal, 1.1) : modelData.normal
                        FluIcon {
                            anchors.centerIn: parent
                            iconSource: FluentIcons.AcceptMedium
                            iconSize: 15
                            visible: modelData === FluTheme.primaryColor
                            color: FluTheme.dark ? Qt.rgba(0, 0, 0, 1) : Qt.rgba(1, 1, 1, 1)
                        }
                        MouseArea {
                            id: mouse_item
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                FluTheme.primaryColor = modelData
                                var map = {
                                    "Yello": FluColors.Yellow,
                                    "Orange": FluColors.Orange,
                                    "Red": FluColors.Red,
                                    "Magenta": FluColors.Magenta,
                                    "Purple": FluColors.Purple,
                                    "Blue": FluColors.Blue,
                                    "Teal": FluColors.Teal,
                                    "Green": FluColors.Green
                                }
                                for (var key in map) {
                                    if (map[key] === modelData) {
                                        store.setConfig("primaryColor", key)
                                        break
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    FluArea {
        Layout.fillWidth: true
        height: anime_layout.height + 30
        paddings: 15
        border.width: 0
        radius: 10

        ColumnLayout {
            id: anime_layout
            spacing: 5
            anchors {
                top: parent.top
                left: parent.left
            }
            FluText {
                text: "开启动画"
                font: FluTextStyle.BodyStrong
                Layout.bottomMargin: 4
            }

            FluToggleSwitch {
                Layout.topMargin: 5
                checked: FluTheme.enableAnimation
                onClicked: {
                    FluTheme.enableAnimation = !FluTheme.enableAnimation
                    store.setConfig("enableAnimation", FluTheme.enableAnimation ? "true" : "")
                }
            }
        }
    }
}
