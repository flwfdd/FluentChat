import QtQuick
import QtQuick.Window
import FluentUI

Window {
    id: app
    flags: Qt.SplashScreen

    Component.onCompleted: {
        FluApp.init(app)
        FluTheme.darkMode = FluThemeType.System
        FluTheme.enableAnimation = true
        FluApp.routes = {
            "/login": "qrc:/FluentChat/ui/window/LoginWindow.qml",
            "/": "qrc:/FluentChat/ui/window/MainWindow.qml",
        }
        FluApp.initialRoute = "/login"
        // FluApp.initialRoute = "/"
        //        FluApp.httpInterceptor = interceptor

        //加载设置
        var darkMode = store.getConfig("darkMode", "System")
        if (darkMode === "Light") {
            FluTheme.darkMode = FluThemeType.Light
        } else if (darkMode === "Dark") {
            FluTheme.darkMode = FluThemeType.Dark
        } else {
            FluTheme.darkMode = FluThemeType.System
        }
        var enableAnimation = store.getConfig("enableAnimation", "true")
        FluTheme.enableAnimation = Boolean(enableAnimation)
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
        var primaryColor = store.getConfig("primaryColor", FluColors.Blue)
        FluTheme.primaryColor = map[primaryColor] ? map[primaryColor] : FluColors.Blue

        FluApp.run()
    }
}
