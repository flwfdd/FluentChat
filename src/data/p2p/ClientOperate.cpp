#include "ClientOperate.h"

#include <QFileInfo>
#include <QDebug>

ClientOperate::ClientOperate(QObject *parent)
        : QObject(parent) {
    initOperate();
}

ClientOperate::~ClientOperate() {
    doDisconnect();
}

QString ClientOperate::getFilePath() const {
    QMutexLocker locker(&dataMutex);
    return filePath;
}

void ClientOperate::setFilePath(const QString &path) {
    QMutexLocker locker(&dataMutex);
    filePath = path;
}

QString ClientOperate::getSavePath() const {
    QMutexLocker locker(&dataMutex);
    return savePath;
}

void ClientOperate::setSavePath(const QString &path) {
    QMutexLocker locker(&dataMutex);
    savePath = path;
}

bool ClientOperate::isConnected() const {
    QMutexLocker locker(&dataMutex);
    return connectState;
}

void ClientOperate::setConnected(bool connected) {
    QMutexLocker locker(&dataMutex);
    connectState = connected;
}

void ClientOperate::connectTcp(const QString &address, quint16 port) {
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        //连接服务器
        socket->connectToHost(QHostAddress(address), port);
        m_address = address;
        m_port = port;
    } else {
        emit logMessage("socket->state() != QAbstractSocket::UnconnectedState");
    }
}

void ClientOperate::disconnectTcp() {
    doDisconnect();
}

void ClientOperate::cancelFileTransfer() {
//    //清除文件
    if (file->exists() && !file->remove()) {
        emit logMessage("取消后删除文件失败" + file->fileName());
    }
    //关闭文件
    doCancel();

    //发送停止传输指令
    sendData(0x04, QByteArray());
    if (socket->state() == QAbstractSocket::ConnectedState) {
        doDisconnect();
    }
}

bool ClientOperate::startFileTransfer() {
    qDebug() << "startFileTransfer" << m_address << m_port;
    //之前如果打开了先释放
    doCloseFile();
    if (!socket->isValid()) {
//        qDebug("传输文件前没有socket");
//        return false;
        connectTcp(m_address, m_port);
    }


    QString save_path = getSavePath();
    file = new QFile(this);

    if (save_path.isEmpty()) {
        return false;
    }
    file->setFileName(save_path);
    //Truncate清掉原本内容
    qDebug() << "文件指针已经创建,path:" << file->fileName();

    // 如果文件已经存在
    qDebug() << "文件是否存在:" << file->exists();
//    if(file->exists()) {
//        if(!file->remove()) {
//            emit logMessage("已经存在文件,删除文件失败,无法进行接收" + file->fileName());
//        }
//    }
    if (!file->open(QIODevice::WriteOnly)) {
        emit logMessage("创建文件失败，无法进行接收" + file->fileName());
        return false;
    }
    emit logMessage("创建文件成功，准备接收" + file->fileName());
    filePath = getFilePath();

    //把文件 Url 发送给服务端，然后等待文件数据的传输
    sendData(0x01, filePath.toUtf8());

    return true;
}


void ClientOperate::initOperate() {
    socket = new QTcpSocket(this);

    //收到数据，触发readyRead
    connect(socket, &QTcpSocket::readyRead, [this] {
        //没有可读的数据就返回
        if (socket->bytesAvailable() <= 0)
            return;
        //读取数据
        operateReceiveData(socket->readAll());
    });

    //连接状态改变
    connect(socket, &QTcpSocket::connected, [this] {
        setConnected(true);
        emit connectStateChanged(true);
        emit logMessage(QString("已连接服务器 [%1:%2]")
                                .arg(socket->peerAddress().toString())
                                .arg(socket->peerPort()));
    });
    connect(socket, &QTcpSocket::disconnected, [this] {
        setConnected(false);
        emit connectStateChanged(false);
        emit logMessage(QString("与服务器连接已断开 [%1:%2]")
                                .arg(socket->peerAddress().toString())
                                .arg(socket->peerPort()));
    });

    // 获取调试信息
    connect(this, &ClientOperate::logMessage, [](const QString &msg) {
        qDebug() << "client" << msg;
    });
}

void ClientOperate::doDisconnect() {
    //断开socket连接，释放资源
    socket->abort();
    doCloseFile();
}

void ClientOperate::doCloseFile() {
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }
}

void ClientOperate::doCancel() {
    if (file) {
        //关闭文件
        doCloseFile();
    }
}

bool ClientOperate::readyReceiveFile(qint64 size) {
    //重置状态
    emit logMessage("创建文件成功，准备接收");
    return true;
}

void ClientOperate::onReceiveFile(const char *data, qint64 size) {
    if (!file || !file->isOpen()) {
        doCancel();
        //发送停止传输指令
        sendData(0x04, QByteArray());
//        emit logMessage("文件操作失败，取消接收");
        return;
    }
    if (size > 0) {
        const qint64 write_size = file->write(data, size);
        // waitForBytesWritten 解决断网问题，超过3s停止传输
        if (write_size != size && !file->waitForBytesWritten(3000)) {
            doCancel();
            //发送停止传输指令
            sendData(0x04, QByteArray());
            emit logMessage("文件写入超时，取消接收");
            return;
        }
    }
    setReceiveSize(receiveSize + size);
    //避免除零
    if (fileSize > 0) {
        emit progressChanged(receiveSize * 100 / fileSize);
    }
    if (receiveSize >= fileSize) {
        doCancel();
        emit logMessage("文件接收完成");
        emit progressChanged(100);
        return;
    }
}

void ClientOperate::sendData(char type, const QByteArray &data) {
    //传输协议
    //帧结构：帧头4+帧长2+帧类型1+帧数据N+帧尾2（没有校验段，懒得写）
    //帧头：4字节定值 0x0F 0xF0 0x00 0xFF
    //帧长：2字节数据段长度值 arr[4]*0x100+arr[5] 前面为高位后面为低位
    //帧类型：1字节
    //- 0x01 交流阶段，
    //       S->R:后跟四字节文件长度，长度计算同帧长一样前面为高位后面为低位
    //       R->S:后跟N字节文件 Url, 长度计算同帧长一样前面为高位后面为低位
    //- 0x02 发送者发送文件数据，接收者准备好接收数据
    //- 0x03 发送结束
    //- 0x04 取消发送
    //（服务端收到0x01 0x03开始和结束发送两个命令要进行应答，回同样的命令码无数据段）
    //帧尾：2字节定值 0x0D 0x0A
    if (!socket->isValid()) {
        return;
    }
    frameHead[6] = type;
    const quint64 data_size = data.size();
    frameHead[5] = data_size % 0x100;
    frameHead[4] = data_size / 0x100;

    //发送头+数据+尾
    socket->write(frameHead, 7);
    socket->write(data);
    socket->write(frameTail, 2);
}


void ClientOperate::operateReceiveData(const QByteArray &data) {
    static QByteArray frame_head = QByteArray(frameHead, 4);
    //这里只是简单的处理，所以用了QByteArray容器做缓存
    dataTemp += data;

    //处理数据
    while (true) {
        //保证以帧头为起始
        while (!dataTemp.startsWith(frame_head) && dataTemp.size() > 4) {
            dataTemp.remove(0, 1); //左边移除一字节
        }
        //小于最小帧长
        if (dataTemp.size() < 7 + 2) {
            return;
        }
        //取数据段长度，这里没有判断长度有效性
        const int data_size = uchar(dataTemp[4]) * 0x100 + uchar(dataTemp[5]);
        if (dataTemp.size() < 7 + 2 + data_size) {
            return;
        }
        //帧尾不一致，无效数据--这里懒得写校验位了
        if (memcmp(dataTemp.constData() + 7 + data_size, frameTail, 2) != 0) {
            dataTemp.clear();
            return;
        }
        //取数据类型
        const char type = dataTemp[6];
        switch (type) {
            case 0x01: {
                //接收文件大小数据段
                //应答数据，包括文件大小，返回校验大小是否一致
                //这里也可以做个弹框，询问是否接收数据
                //解析长度和文件名
                qint64 file_size = 0;
                //file_size+=uchar(dataTemp[7]);
                //file_size<<=8;
                //file_size+=uchar(dataTemp[8]);
                //file_size<<=8;
                //file_size+=uchar(dataTemp[9]);
                //file_size<<=8;
                //file_size+=uchar(dataTemp[10]);
                file_size = uchar(dataTemp[7]) * 0x1000000 +
                            uchar(dataTemp[8]) * 0x10000 +
                            uchar(dataTemp[9]) * 0x100 +
                            uchar(dataTemp[10]);
                emit logMessage(QString("接收到的文件大小为[%1]").arg(file_size));
                setFileSize(file_size);
                receiveSize = 0;
                if (readyReceiveFile(file_size)) {
                    emit logMessage("准备好接收服务器文件");
                    //应答
                    char cfile_size[4] = {0};
                    const quint64 data_size = fileSize; //有符号转无符号，会被截断
                    cfile_size[3] = data_size >> 0 % 0x100;
                    cfile_size[2] = data_size >> 8 % 0x100;
                    cfile_size[1] = data_size >> 16 % 0x100;
                    cfile_size[0] = data_size >> 24;
                    sendData(0x02, QByteArray(cfile_size, 4));
                } else {
                    emit logMessage("准备接收客户端文件失败");
                }
            }
                break;
            case 0x02: //数据传输
                onReceiveFile(dataTemp.constData() + 7, data_size);
                break;
            case 0x03: { //发送数据完成
                setReceiveSize(fileSize);
                doCloseFile();
                emit logMessage("客户端文件接收完毕");
                //应答
                sendData(0x03, QByteArray(1, (char) ((receiveSize == fileSize) ? 0x01 : 0x00)));
            }
                break;
            case 0x04: //服务器取消发送
                doCancel();
                emit logMessage("服务器取消发送，发送终止");
                break;
            default:
                break;
        }
        //移除处理完的字节
        dataTemp.remove(0, 7 + 2 + data_size);
    }
}

qint64 ClientOperate::getReceiveSize() const {
    return receiveSize;
}

void ClientOperate::setReceiveSize(qint64 newReceiveSize) {
    if (receiveSize == newReceiveSize)return;
    receiveSize = newReceiveSize;
    emit receiveSizeChanged(receiveSize);
}

qint64 ClientOperate::getFileSize() const {
    return fileSize;
}

void ClientOperate::setFileSize(qint64 newFileSize) {
    if (fileSize == newFileSize)
        return;
    fileSize = newFileSize;
    emit fileSizeChanged(fileSize);
}
