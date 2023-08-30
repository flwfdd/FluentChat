//
// Created by flwfdd on 2023/8/27.
//

#include "control.h"
#include "global/store.h"
#include <QDateTime>

Control::Control(QObject *parent) : QObject(parent) {
    net = Net::instance();
    db = Database::instance();
    auto onlineStatusTimer = new QTimer(this);
    connect(onlineStatusTimer, &QTimer::timeout, this, &Control::updateOnlineStatus);
    onlineStatusTimer->start(onlineStatusTimerInterval);
}

Control *Control::instance() {
    static Control *control = new Control();
    return control;
}

void Control::initConnect() {
    connect(Store::instance(), &Store::currentGroupChanged, this, &Control::initMessageList);
}

void Control::sendMessage(int gid, QString type, QString content) {
    if (content.isEmpty())
        return;
    Net::instance()->sendMessage(gid, type, content, [=]() {
        showSuccess("发送成功");
    });
//    auto group = Store::instance()->currentGroup();
//    if (group == nullptr || group->id() != gid)return;
//    auto mid = group->last() ? group->last()->mid() + 1 : 1;
//    auto item = new MessageModel{int(gid * 1000 + mid), type, content,
//                                 QDateTime::currentMSecsSinceEpoch() / 1000,
//                                 Store::instance()->currentUser(), gid, mid, false};
//    receiveMessage(item);
}

void Control::receiveMessage(MessageModel *message) {
    Database::instance()->saveMessages(QList < MessageModel * > () << message);
    auto group = Store::instance()->currentGroup();
    if (group == nullptr || group->id() != message->gid())return;
    auto *message2 = new MessageModel(message->id(), message->type(), message->content(), message->time(),
                                      message->user(), message->gid(), message->mid(), message->recall());
    group->setLast(message2);
    Store::instance()->groupList()->sortItems();
    Database::instance()->saveGroups(QList < GroupModel * > () << group);
    mergeMessageList(QList < MessageModel * > () << message, false, false);
}

void Control::openGroup(GroupModel *item) {
    if (item->last()) {
        item->setRead(item->last()->mid());
        Database::instance()->saveRead(item->id(), item->last()->mid());
    }
    Store::instance()->setCurrentGroup(item);
}

void Control::initMessageList() {
    auto messageList = Store::instance()->messageList();
    Store::instance()->setCurrentGroupUsers(QList < UserModel * > ());
    messageList->setHasMore(false);
    auto group = Store::instance()->currentGroup();
    if (group == nullptr) {
        messageList->setItems(QList < MessageModel * > ());
        return;
    }

    // 加载群成员
    Net::instance()->getGroupUsers(group->id(), [=](const QList<UserModel *> users) {
        if (group == Store::instance()->currentGroup()) {
            Store::instance()->setCurrentGroupUsers(users);
        }
    });

    // 加载群消息
    if (group->last() == nullptr) {
        messageList->setItems(QList < MessageModel * > ());
        return;
    } else {
        auto endMid = group->last()->mid();
        auto startMid = std::max(1, endMid - pageSize + 1);
        auto list = db->getMessages(group->id(), startMid, endMid);
        if (list.size() != endMid - startMid + 1) {
            // 数据不完整 从网络加载
            Net::instance()->getMessages(group->id(), startMid, endMid,
                                         [=](const QList<MessageModel *> messages) {
                                             // 加载到messageList
                                             if (group == Store::instance()->currentGroup())
                                                 mergeMessageList(messages, false, true);
                                         });
        } else {
            // 加载到messageList
            mergeMessageList(list, false, true);
        }
    }
}

void Control::mergeMessageList(QList<MessageModel *> list, bool front, bool replace) {
    auto m_list = replace ? QList < MessageModel * > () : Store::instance()->messageList()->items();
    std::sort(list.begin(), list.end(), [](MessageModel *a, MessageModel *b) {
        return a->mid() < b->mid();
    });
    if (front) {
        for (auto it = list.rbegin(); it != list.rend(); it++) {
            m_list.prepend(*it);
        }
    } else {
        for (auto item: list) {
            m_list.append(item);
        }
    }
    Store::instance()->messageList()->setHasMore(!m_list.empty() && m_list.first()->mid() != 1);
    Store::instance()->messageList()->setItems(m_list);
}


QList<UserModel *> Control::getUsers(QList<int> ids) {
    auto idSet = QSet < int > (ids.begin(), ids.end()); // 去重
    auto users = Store::instance()->users();
    auto userList = QList < UserModel * > ();
    auto unloaded = QList < UserModel * > ();
    auto unloadedSet = QSet < UserModel * > ();
    for (auto id: idSet) {
        if (!users->contains(id)) {
            // 不存在则先新建占位
            auto user = new UserModel(id, "", "", "", "", "", false);
            users->insert(id, user);
            unloaded.append(user);
            unloadedSet.insert(user);
            userList.append(user);
        } else userList.append(users->value(id));
    }

    // 加入在线检查队列
    for (auto user: userList) {
        checkOnlineUids.insert(user);
    }

    // 从数据库加载
    auto loaded = db->loadUsers(unloaded);
    for (auto user: loaded) {
        users->insert(user->id(), user);
        unloadedSet.remove(user);
    }

    // 从网络加载
    unloaded.clear();
    for (auto user: unloadedSet) {
        unloaded.append(user);
    }
    Net::instance()->loadUsers(unloaded);

    return userList;
}

void Control::init() {
    Ws::instance()->init();
    if (Store::instance()->currentUser() == nullptr) {
        QString uid = Store::instance()->getConfig("loginUid");
        if (uid.isEmpty())return;
        Store::instance()->setCurrentUser(getUsers(QList < int > () << uid.toInt()).first());
    }
    Store::instance()->groupList()->setItems(db->getGroups());
    Net::instance()->loadGroups();

    updateOnlineStatus();
}

void Control::loadMessages() {
    if (Store::instance()->currentGroup() == nullptr || !Store::instance()->messageList()->hasMore())return;
    auto group = Store::instance()->currentGroup();
    auto endMid = Store::instance()->messageList()->items().first()->mid() - 1;
    auto startMid = std::max(1, endMid - pageSize + 1);
    auto list = db->getMessages(group->id(), startMid, endMid);
    if (list.size() != endMid - startMid + 1) {
        // 数据不完整 从网络加载
        Net::instance()->getMessages(group->id(), startMid, endMid,
                                     [=](const QList<MessageModel *> messages) {
                                         // 加载到messageList
                                         if (group == Store::instance()->currentGroup())
                                             mergeMessageList(messages, true);
                                     });
    } else {
        // 加载到messageList
        mergeMessageList(list, true);
    }
}

void Control::showSuccess(const QString &message) {
    Store::instance()->setSuccessMsg(message);
}

void Control::showError(const QString &message) {
    Store::instance()->setErrorMsg(message);
}

void Control::login(const QString &username, const QString &password) {
    Net::instance()->login(username, password, [=](UserModel *user) {
        Store::instance()->setCurrentUser(user);
        init();
        showSuccess("登录成功");
        Store::instance()->setIsLogin(true);
    });
}

void Control::registerUser(const QString &username, const QString &password, const QString &nickname,
                           const QString &color, const QString &avatar) {
    Net::instance()->resgisterUser(username, password, nickname, color, avatar, [=](UserModel *user) {
        Store::instance()->setCurrentUser(user);
        init();
        showSuccess("注册成功");
        Store::instance()->setIsLogin(true);
    });
}

void Control::requestUser(QString username) {
    Net::instance()->requestUser(username, [=]() {
        Net::instance()->loadGroups();
        showSuccess("添加成功");
    });
}

void Control::requestGroup(int gid) {
    Net::instance()->requestGroup(gid, [=]() {
        Net::instance()->loadGroups();
        showSuccess("加入成功");
    });
}

void Control::createGroup(const QString &name, const QString &avatar, const QString &color) {
    Net::instance()->createGroup(name, avatar, color, [=]() {
        Net::instance()->loadGroups();
        showSuccess("创建成功");
    });
}

void Control::updateOnlineStatus() {
    // 加入群组列表中的
    auto groupList = Store::instance()->groupList();
    for (auto group: groupList->items()) {
        if (group->type() != "twin")continue;
        auto users = group->owner();
        if (users == nullptr)continue;
        checkOnlineUids.insert(users);
    }
    if (checkOnlineUids.empty())return;
    auto uids = QList < int > ();
    for (auto user: checkOnlineUids) {
        uids.append(user->id());
    }
    checkOnlineUids.clear();
    Net::instance()->getOnlineStatus(uids, [=](QList<bool> status) {
        if (uids.size() != status.size())return;
        for (int i = 0; i < uids.size(); i++) {
            auto user = Store::instance()->users()->value(uids[i]);
            if (user == nullptr)continue;
            user->setOnline(status[i]);
        }
    });
}

void Control::uploadFile(QString path, QJSValue callable) {
    // 获取后缀名
    auto suffix = path.mid(path.lastIndexOf('.') + 1);
    Net::instance()->uploadFile(path, suffix);
}

