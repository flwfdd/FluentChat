import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import FluentUI

Item {
    property FluObject items
    property FluObject footerItems
    property Component autoSuggestBox
    property int navWidth: 300
    id:control
    Item{
        id:d
        property bool animDisabled:false
        function handleItems(){
            var _idx = 0
            var data = []
            if(items){
                for(var i=0;i<items.children.length;i++){
                    var item = items.children[i]
                    item._idx = _idx
                    data.push(item)
                    _idx++
                }
                if(footerItems){
                    for(var k=0;k<footerItems.children.length;k++){
                        var itemFooter = footerItems.children[k]
                            itemFooter._idx = _idx;
                            _idx++
                    }
                }
            }
            return data
        }
    }

    Component.onCompleted: {
        timer_anim_delay.restart()
    }
    Timer{
        id:timer_anim_delay
        interval: 200
        onTriggered: {
            d.animDisabled = true
        }
    }

    Component{
        id:com_panel_item_separatorr
        FluDivider{
            width: layout_list.width
            spacing: model.spacing
            separatorHeight: model.size
        }
    }

    Component{
        id:chat_item
        Item{
            clip: true
            height: 66
            visible: true
            width: layout_list.width
            FluControl{
                id:item_control
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 2
                    bottomMargin: 2
                    leftMargin: 6
                    rightMargin: 6
                }
                MouseArea{
                    anchors.fill: parent
                    acceptedButtons:  Qt.RightButton
                    onClicked: function(mouse){
                        if (mouse.button === Qt.RightButton) {
                            if(model.menuDelegate){
                                loader_item_menu.sourceComponent = model.menuDelegate
                                loader_item_menu.item.popup();
                            }
                        }
                    }
                    z:-100
                }
                onClicked: {
                    nav_list.currentIndex = _idx
                    layout_footer.currentIndex = -1
                    model.onClick()
                }
                Rectangle{
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if(FluTheme.dark){
                            if(nav_list.currentIndex === _idx){
                                return Qt.rgba(1,1,1,0.06)
                            }
                            if(item_control.hovered){
                                return Qt.rgba(1,1,1,0.03)
                            }
                            return Qt.rgba(0,0,0,0)
                        }else{
                            if(nav_list.currentIndex === _idx){
                                return Qt.rgba(0,0,0,0.06)
                            }
                            if(item_control.hovered){
                                return Qt.rgba(0,0,0,0.03)
                            }
                            return Qt.rgba(0,0,0,0)
                        }
                    }

                    Rectangle{
                        id:item_avatar
                        width: 42
                        height: width
                        radius: width/2
                        color: model.avatar_color
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:parent.left
                            leftMargin: 10
                        }

                        FluText {
                            anchors.centerIn: parent
                            text: model.avatar_text?[...model.avatar_text][0]:"" //截取可能含有emoji的字符串的第一个字符
                            color: "white"
                            font.pixelSize: 24
                        }
                    }

                    Column {
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:item_avatar.right
                            leftMargin:10
                            right: unread_badge.left
                        }
                        FluText{
                            id:item_title
                            text:model.title
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            font.pixelSize: 16
                            color:{
                                if(item_control.pressed){
                                    return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                                }
                                return FluTheme.dark ? FluColors.White : FluColors.Grey220
                            }
                            anchors{
                                left:parent.left
                                right:parent.right
                            }
                        }
                        FluText{
                            id:item_text
                            text:model.text
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            font.pixelSize: 12
                            color:{
                                if(item_control.pressed){
                                    return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                                }
                                return FluTheme.dark ? FluColors.White : FluColors.Grey220
                            }
                            anchors{
                                left:parent.left
                                right:parent.right
                            }
                        }
                    }

                    FluBadge{
                        id: unread_badge
                        anchors{
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: 10
                        }
                        count: model.unread_num
                    }
                }
            }
        }
    }

    Component{
        id:com_footer_item
        Item{
            clip: true
            height: 38
            visible: true
            width: layout_list.width
            FluControl{
                id:item_control
                anchors{
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
                    if(model.tapFunc){
                        model.tapFunc()
                    }else{
                        layout_footer.currentIndex = _idx
                        model.tap()
                    }
                }
                Rectangle{
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if(FluTheme.dark){
                            if(item_control.hovered){
                                return Qt.rgba(1,1,1,0.03)
                            }
                            return Qt.rgba(0,0,0,0)
                        }else{
                            if(item_control.hovered){
                                return Qt.rgba(0,0,0,0.03)
                            }
                            return Qt.rgba(0,0,0,0)
                        }
                    }

                    Item{
                        id:item_icon
                        width: 30
                        height: 30
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:parent.left
                            leftMargin: 3
                        }
                        FluIcon{
                            anchors.centerIn: parent
                            iconSource: {
                                if(model.icon){
                                    return model.icon
                                }
                                return 0
                            }
                            iconSize: 15
                        }
                    }
                    FluText{
                        id:item_title
                        text:model.title
                        visible: {
                            if(d.isCompactAndNotPanel){
                                return false
                            }
                            return true
                        }
                        elide: Text.ElideRight
                        color:{
                            if(item_control.pressed){
                                return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
                            }
                            return FluTheme.dark ? FluColors.White : FluColors.Grey220
                        }
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:item_icon.right
                            right: item_dot_loader.left
                        }
                    }
                    Loader{
                        id:item_dot_loader
                        property bool isDot: (item_dot_loader.item&&item_dot_loader.item.isDot)
                        anchors{
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: isDot ? 3 : 10
                            verticalCenterOffset: isDot ? -8 : 0
                        }
                        sourceComponent: {
                            if(model.infoBadge){
                                return model.infoBadge
                            }
                            return undefined
                        }
                    }
                }
            }
        }
    }



    Component{
        id:main_content
        Rectangle{
            anchors.fill: parent
            color: "lightblue"
        }
    }

    Loader{
        id:loader_content
        anchors{
            left: parent.left
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            leftMargin: control.navWidth
        }
        sourceComponent: main_content
    }

    Rectangle{
        id:layout_list
        width: control.navWidth
        anchors{
            top: parent.top
            bottom: parent.bottom
        }
        border.color: FluTheme.dark ? Qt.rgba(45/255,45/255,45/255,1) : Qt.rgba(226/255,230/255,234/255,1)
        border.width: 1
        Item{
            id:layout_header
            width: layout_list.width
            clip: true
            y: 10
            height: autoSuggestBox ? 38 : 0
            Loader{
                id:loader_auto_suggest_box
                anchors.centerIn: parent
                sourceComponent: autoSuggestBox
            }
        }
        Flickable{
            id:layout_flickable
            anchors{
                top: layout_header.bottom
                topMargin: 6
                left: parent.left
                right: parent.right
                bottom: layout_footer.top
            }
            boundsBehavior: ListView.StopAtBounds
            clip: true
            contentHeight: nav_list.contentHeight
            ScrollBar.vertical: FluScrollBar {}
            ListView{
                id:nav_list
                clip: true
                anchors.fill: parent
                model:d.handleItems()
                boundsBehavior: ListView.StopAtBounds
                highlightMoveDuration: 0
                highlight: Item{
                    id:highlight_item
                    onYChanged: {
                        highlight_rectangle.y=y+(66-42)/2
                    }
                }
                Rectangle{
                    id: highlight_rectangle
                    height: 42
                    color: FluTheme.primaryColor.dark
                    width: 4
                    radius: width/2
                    y: (66-42)/2
                    anchors{
                        left: parent.left
                        leftMargin: 6
                    }

                    Behavior on y {
                        NumberAnimation{
                            easing.period: 0.75
                            easing.amplitude: 1
                            duration: 666
                            easing.type: Easing.OutElastic
                        }
                    }
                }
                currentIndex: -1

                delegate: Loader{
                    property var model: modelData
                    property var _idx: index
                    property int type: 0
                    sourceComponent: chat_item
                }
            }
        }

        ListView{
            id:layout_footer
            clip: true
            width: layout_list.width
            height: childrenRect.height
            anchors.bottom: parent.bottom
            interactive: false
            boundsBehavior: ListView.StopAtBounds
            currentIndex: -1
            model: {
                if(footerItems){
                    return footerItems.children
                }
            }
            delegate: Loader{
                property var model: modelData
                property var _idx: index
                property int type: 1
                sourceComponent: {
                    if(modelData instanceof FluPaneItem){
                        return com_footer_item
                    }
                    if(modelData instanceof FluPaneItemSeparator){
                        return com_panel_item_separatorr
                    }
                }
            }
        }
    }

    Popup{
        property var childModel
        id:control_popup
        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from:0
                to:1
                duration: 66
            }
        }
        padding: 0
        focus: true
        contentItem: Item{
            ListView{
                id:list_view
                anchors.fill: parent
                clip: true
                currentIndex: -1
                model: control_popup.childModel
                ScrollBar.vertical: FluScrollBar {}
                delegate:Button{
                    id:item_button
                    width: 180
                    height: 66
                    focusPolicy:Qt.TabFocus
                    background: Rectangle{
                        color:  {
                            if(FluTheme.dark){
                                if(item_button.hovered){
                                    return Qt.rgba(1,1,1,0.06)
                                }
                                return Qt.rgba(0,0,0,0)
                            }else{
                                if(item_button.hovered){
                                    return Qt.rgba(0,0,0,0.03)
                                }
                                return Qt.rgba(0,0,0,0)
                            }
                        }
                        FluFocusRectangle{
                            visible: item_button.activeFocus
                            radius:4
                        }

                        Loader{
                            id:item_dot_loader
                            anchors{
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                                rightMargin: 10
                            }
                            sourceComponent: {
                                if(model.infoBadge){
                                    return model.infoBadge
                                }
                                return undefined
                            }
                        }

                    }
                    contentItem: FluText{
                        text:modelData.title
                        elide: Text.ElideRight
                        rightPadding: item_dot_loader.width
                        verticalAlignment: Qt.AlignVCenter
                        anchors{
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    onClicked: {
                        if(modelData.tapFunc){
                            modelData.tapFunc()
                        }else{
                            modelData.tap()
                            nav_list.currentIndex = _idx
                            layout_footer.currentIndex = -1
                        }
                        control_popup.close()
                    }
                }
            }
        }
        background: FluRectangle{
            implicitWidth: 180
            implicitHeight: 66*Math.min(Math.max(list_view.count,1),8)
            radius: [4,4,4,4]
            FluShadow{
                radius: 4
            }
            color: FluTheme.dark ? Qt.rgba(51/255,48/255,48/255,1) : Qt.rgba(248/255,250/255,253/255,1)
        }
        function showPopup(pos,model){
            control_popup.x = pos.x
            control_popup.y = pos.y
            control_popup.childModel = model
            control_popup.open()
        }
    }
    Loader{
        id:loader_item_menu
    }
    Component{
        id:com_placeholder
        Item{
            property int launchMode: FluPageType.SingleInstance
            property string url
        }
    }

    function setCurrentIndex(index){
        nav_list.currentIndex = index
        var item = nav_list.model[index]
        if(item instanceof FluPaneItem){
            item.tap()
        }
    }
    function getItems(){
        return nav_list.model
    }
    function getCurrentIndex(){
        return nav_list.currentIndex
    }
    function getCurrentUrl(){
        return loader_content.source.toString()
    }

    function startPageByItem(data){
        var items = getItems()
        for(var i=0;i<items.length;i++){
            var item =  items[i]
            if(item.key === data.key){
                if(getCurrentIndex() === i){
                    return
                }
                setCurrentIndex(i)
                if(item.parent && !d.isCompactAndNotPanel){
                    item.parent.isExpand = true
                }
                return
            }
        }
    }
    function backButton(){
        return btn_back
    }
    function navButton(){
        return btn_nav
    }
    function logoButton(){
        return image_logo
    }
}
