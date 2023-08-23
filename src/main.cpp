#include "qqmlcontext.h"
#include "viewmodel/chat_list.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>


void inject(QQmlApplicationEngine &engine){
    ChatListModel* chatListModel=new ChatListModel();
    for(int i=0;i<10;i++){
        chatListModel->append(new ChatListModelItem(i,"小苏"+QString::number(i) ,"啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊",i,QColor("#aef"),"🤔"));
    }
    engine.rootContext()->setContextProperty("chatListModel", chatListModel);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    inject(engine);
    const QUrl url(u"qrc:/FluentChat/ui/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
