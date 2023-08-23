import QtQuick
import QtQuick.Window
import FluentUI

Window {
    id: app
    flags: Qt.SplashScreen

    //    FluFilledButton {
    //        width: 125
    //        height: 35
    //        text: qsTr("Hello FluentIM!")
    //        anchors.centerIn: parent
    //    }

    Component.onCompleted: {
        FluApp.init(app)
        FluTheme.darkMode = FluThemeType.System
        FluTheme.enableAnimation = true
        FluApp.routes = {
            "/": "qrc:/FluentChat/ui/MainWindow.qml",
        }
        FluApp.initialRoute = "/"
        //        FluApp.httpInterceptor = interceptor
        FluApp.run()
    }
}
