import QtQuick
import QtQuick.Controls
import FluentUI

QtObject {
    readonly property string key : FluTools.uuid()
    property int _idx
    property string title
    property int unread_num
    property string text
    property color avatar_color
    property string avatar_text
    property var parent
    property var onClick
    property Component menuDelegate
}
