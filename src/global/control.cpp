//
// Created by flwfdd on 2023/8/27.
//

#include "control.h"
#include "global/store.h"
#include <QDateTime>

Control::Control(QObject *parent) : QObject(parent) {
    net = Net::instance();
    db = Database::instance();
}

Control *Control::instance() {
    static Control *control = new Control();
    return control;
}

void Control::initConnect() {
    connect(Store::instance(), &Store::currentGroupChanged, this, &Control::initMessageList);
}

void Control::sendMessage(int gid, const QString &type, const QString &content) {
    if (content.isEmpty())
        return;
    auto messageList = Store::instance()->messageList();
    auto item = new MessageModel{int(messageList->items().size()), type, content,
                                 QDateTime::currentMSecsSinceEpoch() / 1000,
                                 (*(Store::instance()->users()))[1], 0, 0, false};
    messageList->append(item);
}

void Control::openGroup(GroupModel *item) {
    if (item->last())item->setRead(item->last()->mid());
    Store::instance()->setCurrentGroup(item);
}

void Control::initMessageList() {
    auto group = Store::instance()->currentGroup();
    auto mlist = QList<MessageModel *>();
    Net::instance()->getMessages(group->id(), -1, -1, [=](const QList<MessageModel *> messages) {
        // 加载到messageList
        if (group == Store::instance()->currentGroup())Store::instance()->messageList()->setItems(messages);
    });
}


QList<UserModel *> Control::getUsers(QList<int> ids) {
    auto idSet = QSet<int>(ids.begin(), ids.end()); // 去重
    auto users = Store::instance()->users();
    auto userList = QList<UserModel *>();
    auto unloaded = QList<UserModel *>();
    auto unloadedSet = QSet<UserModel *>();
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
    if (Store::instance()->currentUser() == nullptr)return;
    auto db = Database::instance();
    auto net = Net::instance();
    auto groupListModel = Store::instance()->groupList();
    groupListModel->setItems(db->getGroups());
    net->loadGroups();
}

void Control::loadMessages() {
    if(Store::instance()->currentGroup() == nullptr)return;
    auto group = Store::instance()->currentGroup();
    auto mlist = QList<MessageModel *>();
    Net::instance()->getMessages(group->id(), -1, -1, [=](const QList<MessageModel *> messages) {
        // 加载到messageList
        if (group == Store::instance()->currentGroup())Store::instance()->messageList()->prepend(messages);
    });
}

