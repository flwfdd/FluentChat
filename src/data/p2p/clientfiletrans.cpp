#include "clientfiletrans.h"


ClientFileTrans::ClientFileTrans(QString ip, quint16 port, QObject *parent)
        : QObject{parent}, m_ip(ip), m_port(port) {
//    thread = new QThread(this);
    operate = new ClientOperate();
//    operate->moveToThread(thread);
//    connect(thread, &QThread::finished, operate, &ClientOperate::deleteLater);

    qDebug() << "ClientFileTrans::ClientFileTrans()" << m_ip << m_port;


    connect(this, &ClientFileTrans::connectTcp, operate, &ClientOperate::connectTcp);
    connect(this, &ClientFileTrans::disconnectTcp, operate, &ClientOperate::disconnectTcp);

    // 获取 progressValue
    connect(operate, &ClientOperate::progressChanged, this, [this](int value) {
        progressValue = value;
    });


    // 获取接收大小
    connect(operate, &ClientOperate::receiveSizeChanged, this, [this](qint64 value) {
        setReceiveSize(value);
    });


    // 获取文件大小
    connect(operate, &ClientOperate::fileSizeChanged, this, [this](qint64 value) {
        fileSize = value;
    });

    // 信号槽安全传递
    if (operate->isConnected()) {
        emit disconnectTcp();
    } else {
        emit connectTcp(m_ip, m_port);
    }

    // 还有什么加的?
//    thread->start();
}

ClientFileTrans::~ClientFileTrans() {
//    thread->quit();
//    thread->wait();
}


void ClientFileTrans::startTrans(const QString &fileHash, const QString &savePath) {

    operate->setFilePath(fileHash);
    operate->setSavePath(savePath);
    operate->startFileTransfer();
}

void ClientFileTrans::cancelTrans() {
    if (isCancel)return;
    isCancel = true;
    operate->cancelFileTransfer();
}

int ClientFileTrans::getProgressValue() const {
    return progressValue;
}

qint64 ClientFileTrans::getFileSize() const {
    return fileSize;
}

qint64 ClientFileTrans::getReceiveSize() const {
    return receiveSize;
}

void ClientFileTrans::setReceiveSize(qint64 newReceiveSize) {
    if (receiveSize == newReceiveSize) {
        return;
    }
    receiveSize = newReceiveSize;
    emit receiveSizeChanged();
}

void ClientFileTrans::onSizeChanged(const std::function<void(qint64, qint64)> &callback) {
    connect(this, &ClientFileTrans::receiveSizeChanged, this, [=]() {
        callback(receiveSize, fileSize);
    });
}
