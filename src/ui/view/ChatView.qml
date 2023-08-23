import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import FluentUI

Item{
    anchors.fill: parent
    Flickable{
        id:flick_view
        clip: true
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: container.height
        ScrollBar.vertical: FluScrollBar {
            anchors.right: flick_view.right
            anchors.rightMargin: 2
        }
        boundsBehavior: Flickable.StopAtBounds
        property var items: [1,2,3,4,5,6,7,8,9,10]
        ListView{
            id:container
            clip: true
            width: parent.width
            orientation: ListView.Vertical
            height: contentItem.height
            model: flick_view.items

            delegate: Rectangle{
                width: modelData%2?100:200
                height: 100
                color: modelData%2?"red":"blue"
                FluText{
                    text: modelData
                    anchors.centerIn: parent
                }
            }
        }

        onContentYChanged: {
            if(contentY == 0){
                for(var i=0;i<10;i++)
                    items.unshift(items.length+1)
                container.model = items
            }
        }

        property int oldH: 0
        onContentHeightChanged: {
            contentY = contentHeight - oldH
            oldH=contentHeight
        }

        Component.onCompleted: {
            flick_view.contentY = flick_view.contentHeight - flick_view.height
        }
    }
}
