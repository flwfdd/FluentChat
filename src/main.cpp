#include "qqmlcontext.h"
#include "model/chat_list.h"
#include "model/message_list.h"
#include "global/store.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>


void inject(QQmlApplicationEngine &engine){
    auto store=Store::instance();
    auto users=Store::instance()->users();
    for(int i=0;i<111;i++){
        auto user=new UserModel(i,"su"+QString::number(i),"啊啊",QColor("#aef"),"🤔","啊啊啊",rand()%2);
        users->insert(i,user);
        store->chatList()->append(new ChatListModelItem(i,user ,"啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊",1692841710-100000*i,rand()%111));
    }

    store->setCurrentUser(users->value(1));

    auto messageListModel= store->messageList();
    for(int i=0;i<111;i++){
        messageListModel->append(new MessageListModelItem(i,"text","啊\n啊啊\n\n啊啊啊",1692841710-100000*i,users->value(i),0,0,false));
    }

    engine.rootContext()->setContextProperty("store", store);
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
