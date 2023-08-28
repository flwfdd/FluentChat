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

void Control::sendMessage(int gid, QString type, QString content) {
    if (content.isEmpty())
        return;
    auto group = Store::instance()->currentGroup();
    if (group == nullptr || group->id() != gid)return;
    auto mid = group->last() ? group->last()->mid() + 1 : 1;
    auto item = new MessageModel{int(gid * 1000 + mid), type, content,
                                 QDateTime::currentMSecsSinceEpoch() / 1000,
                                 Store::instance()->currentUser(), gid, mid, false};
    receiveMessage(item);
}

void Control::receiveMessage(MessageModel *message) {
    Database::instance()->saveMessages(QList<MessageModel *>() << message);
    auto group = Store::instance()->currentGroup();
    if (group == nullptr || group->id() != message->gid())return;
    auto *message2 = new MessageModel(message->id(), message->type(), message->content(), message->time(),
                                      message->user(), message->gid(), message->mid(), message->recall());
    group->setLast(message2);
    Store::instance()->groupList()->sortItems();
    Database::instance()->saveGroups(QList<GroupModel *>() << group);
    mergeMessageList(QList<MessageModel *>() << message, false, false);
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
    Store::instance()->setCurrentGroupUsers(QList<UserModel *>());
    messageList->setHasMore(false);
    auto group = Store::instance()->currentGroup();
    if (group == nullptr) {
        messageList->setItems(QList<MessageModel *>());
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
        messageList->setItems(QList<MessageModel *>());
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
    auto m_list = replace ? QList<MessageModel *>() : Store::instance()->messageList()->items();
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
//    net->loadGroups();
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

