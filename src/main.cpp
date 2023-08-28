#include "qqmlcontext.h"
#include "model/group.h"
#include "model/message.h"
#include "global/store.h"
#include "data/database.h"
#include "data/net.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>


void inject(QQmlApplicationEngine & engine) {
    auto store = Store::instance();
    store->control()->initConnect();
    auto users = Store::instance()->users();
    auto groupListModel = store->groupList();
    auto messageListModel = store->messageList();
    auto groups=new QList<GroupModel*>();
//    auto uids = QList<int>();
//    for (int i = 1; i < 111; i++) {
//        uids << i;
//    }
//    Control::instance()->getUsers(uids);
//    for (int i = 1; i < 111; i++) {
//        auto sn = QString::number(i);
////        auto user = new UserModel(i, "su" + sn, "小苏" + sn, QColor("#aef"), "🤔", "", rand() % 2);
//        auto user = users->value(i);
//        auto message = new MessageModel(i, "text", "啊\n啊啊\n\n啊啊啊", 1292841710 + 100000 * i, user, i, 1, false);
//        messageListModel->append(message);
//        if (rand() % 2)
//            groups->append(
//                    new GroupModel(i, "public", "群组" + QString::number(i), "急", "#aef", "", user, nullptr, 0));
//        else
//            groups->append(
//                    new GroupModel(i, "twin", "私聊" + QString::number(i), "急", "#aef", "", user, nullptr, 0));
//    }

    store->setCurrentUser(Control::instance()->getUsers(QList<int>() << 1)[0]);
    groupListModel->setItems(*groups);

    Control::instance()->init();

//    auto usersList = QList<UserModel *>();
//    for (int i = 0; i < 123; i++) {
//        auto user = new UserModel(i, "", "", QColor(), "", "", false);
//        usersList.append(user);
//    }
//    auto loadedUsers = db->loadUsers(usersList);
//    qDebug() << "loaded users:" << loadedUsers.length();


//    Net::instance()->get("/group/list", QMap<QString, QString>(), [&](QJsonDocument data) {
//        qDebug() << data;
//    });

    engine.rootContext()->setContextProperty("store", store);
}

int main(int argc, char *argv[]) {
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
