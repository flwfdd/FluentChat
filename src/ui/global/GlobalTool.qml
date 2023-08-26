pragma Singleton

import QtQuick

QtObject{
    function formatTime(t){
        let date=new Date(t*1000)
        let d1=new Date(t*1000)
        let d2=new Date()
        d1.setHours(0,0,0,0)
        d2.setHours(0,0,0,0)
        let diffDays=Math.round((d2-d1)/(1000*60*60*24))

        if(diffDays==0)return Qt.formatTime(date,"HH:mm:ss")
        if(diffDays==1)return "昨天 "+Qt.formatTime(date,"HH:mm")
        if(diffDays==2)return "前天 "+Qt.formatTime(date,"HH:mm")
        if(diffDays<7)return Qt.formatDate(date,['周日','周一','周二','周三','周四','周五','周六'][date.getDay()]+" "+Qt.formatTime(date,"HH:mm"))
        if(d1.getFullYear()==d2.getFullYear())return Qt.formatDate(date,"MM-dd")
        return Qt.formatDate(date,"yy-MM-dd")
    }
}
