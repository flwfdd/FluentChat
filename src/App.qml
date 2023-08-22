import QtQuick
import QtQuick.Window
import FluentUI


FluWindow{
    id:page_front
    title: "FluentUI"
    width: 1000
    height: 640
    minimumWidth: 520
    minimumHeight: 200
    visible: true
    launchMode: FluWindowType.SingleTask

    ChatListNavigationView{
        property int clickCount: 0
        id:nav_view
        width: parent.width
        height: parent.height
        z:999
        items: FluObject{

            function rename(item, newName){
                if(newName && newName.trim().length>0){
                    item.title = newName;
                }
            }

            ChatListItem{
                id: chat_list_item_1
                title: "小苏啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊"
                unread_num: 1
                text: "欸嘿嘿啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊"
                avatar_color: "#aef"
                avatar_text: "苏"
                onClick: ()=>{
                    console.log(title)
                }
                menuDelegate: FluMenu{
                    id:nav_item_right_menu
                    width: 120
                    FluMenuItem{
                        text: "重命名"
                        visible: true
                        onClicked: {
                            item_home.showEdit = true
                        }
                    }
                }
            }

            ChatListItem{
                id: chat_list_item_2
                title: "小苏"
                unread_num: 1
                text: "欸嘿嘿"
                avatar_color: "#aef"
                avatar_text: "🤔"
                onClick: ()=>{
                    unread_num=0
                }
            }

            function getRecentlyAddedData(){
                var arr = []
                for(var i=0;i<children.length;i++){
                    var item = children[i]
                    if(item instanceof FluPaneItem && item.recentlyAdded){
                        arr.push(item)
                    }
                    if(item instanceof FluPaneItemExpander){
                        for(var j=0;j<item.children.length;j++){
                            var itemChild = item.children[j]
                            if(itemChild instanceof FluPaneItem && itemChild.recentlyAdded){
                                arr.push(itemChild)
                            }
                        }
                    }
                }
                arr.sort(function(o1,o2){ return o2.order-o1.order })
                return arr
            }

            function getRecentlyUpdatedData(){
                var arr = []
                var items = navigationView.getItems();
                for(var i=0;i<items.length;i++){
                    var item = items[i]
                    if(item instanceof FluPaneItem && item.recentlyUpdated){
                        arr.push(item)
                    }
                }
                return arr
            }

            function getSearchData(){
                var arr = []
                var items = navigationView.getItems();
                for(var i=0;i<items.length;i++){
                    var item = items[i]
                    if(item instanceof FluPaneItem){
                        if (item.parent instanceof FluPaneItemExpander)
                        {
                            arr.push({title:`${item.parent.title} -> ${item.title}`,key:item.key})
                        }
                        else
                            arr.push({title:item.title,key:item.key})
                    }
                }
                return arr
            }

            function startPageByItem(data){
                navigationView.startPageByItem(data)
            }

        }
        footerItems: FluObject{

            property var navigationView

            id:footer_items

            FluPaneItemSeparator{}

            FluPaneItem{
                title:"关于"
                icon:FluentIcons.Contact
                tapFunc:function(){
//                    FluApp.navigate("/about")
                }
            }

            FluPaneItem{
                title:"设置"
                icon:FluentIcons.Settings
                onTap:{
//                    navigationView.push("qrc:/example/qml/page/T_Settings.qml")
                }
            }

        }
        autoSuggestBox:FluAutoSuggestBox{
            width: 280
            anchors.centerIn: parent
            iconSource: FluentIcons.Search
//            items: ItemsOriginal.getSearchData()
            placeholderText: "搜索"
            onItemClicked:
                (data)=>{
//                    ItemsOriginal.startPageByItem(data)
                }
        }
        Component.onCompleted: {
//            ItemsOriginal.navigationView = nav_view
//            ItemsFooter.navigationView = nav_view
            setCurrentIndex(0)
        }
    }
}

