#include "ServerOperate.h"
#include "data/database.h"
#include "global/store.h"

#include <QFileInfo>
#include <QThreadPool>

ServerOperate::ServerOperate(QObject *parent)
        : QTcpServer(parent) {
    initOperate();
}

ServerOperate::~ServerOperate() {
    dislisten();
}

QString ServerOperate::getFilePath() const {
    QMutexLocker locker(&dataMutex);
    return fileUrl;
}

void ServerOperate::setFilePath(const QString &path) {
    QMutexLocker locker(&dataMutex);
    fileUrl = path;
}

bool ServerOperate::wisListening() const {
    qDebug() << "operate26";
    return this->isListening();
}

QString ServerOperate::fromHash(const QString &fileHash) {
    return Store::instance()->getFileHash2Path(fileHash);
}

void ServerOperate::hash2url(const QString &fileHash, std::function<QString(const QString &)> &fromHash) {
    QString url = fromHash(fileHash);
    setFilePath(url);
}

void ServerOperate::doListen(const QString &address, quint16 port) {
    if (this->isListening()) {
        doDislisten();
    }
    //启动监听
    const bool result = this->listen(QHostAddress(address), port);
    qDebug() << "op36";
    emit listenStateChanged(result);
    emit logMessage(result ? "服务启动成功" : "服务启动失败");
}

void ServerOperate::dislisten() {
    qDebug() << "op42";
    doDislisten();
    emit listenStateChanged(false);
    emit logMessage("服务关闭");
}

void ServerOperate::cancelFileTransfer(qintptr socketDescriptor) {
    //关闭文件
    qDebug() << "op51";
    doCancel(socketDescriptor);
    //发送停止传输指令
    sendData(0x04, QByteArray(), socketDescriptor);
}

void ServerOperate::initOperate() {
    // 获取调试信息
    connect(this, &ServerOperate::logMessage, [](const QString &msg) {
        qDebug() << "server"<<msg;
    });
}

void ServerOperate::incomingConnection(qintptr socketDescriptor) {

    QTcpSocket *clientSocket = new QTcpSocket();
    qDebug() << "operate99";

    if (!clientSocket->setSocketDescriptor(socketDescriptor)) {
        // 处理套接字描述符设置失败的情况
        qDebug() << "Failed to set socket descriptor:" << clientSocket->errorString();
        delete clientSocket;
        return;
    }
    qDebug() << socketDescriptor << " " << clientSocket->socketDescriptor();
    emit logMessage(QString("新的客户端连接 [%1:%2]")
                            .arg(clientSocket->peerAddress().toString())
                            .arg(clientSocket->peerPort()));
    clientSockets.append(clientSocket);
    addPendingConnection(clientSocket);

    qDebug() << "operate106";

    //收到数据，触发readyRead
    connect(clientSocket, &QTcpSocket::readyRead, [=] {
        //没有可读的数据就返回
        if (clientSocket->bytesAvailable() <= 0)
            return;
        //读取数据
        operateReceiveData(clientSocket->readAll(), socketDescriptor);
    });


    qDebug() << "operate131";
    connect(clientSocket, &QTcpSocket::disconnected, [=] {
        emit logMessage(QString("客户端连接已断开 [%1:%2]")
                                .arg(clientSocket->peerAddress().toString())
                                .arg(clientSocket->peerPort()));

        qDebug() << QString("客户端连接已断开 [%1:%2]")
                .arg(clientSocket->peerAddress().toString())
                .arg(clientSocket->peerPort());
        closeTimer(socketDescriptor);
    });

    qDebug() << "New client connected:" << clientSocket->peerAddress().toString()
             << clientSocket->peerPort();
    TimeControl(socketDescriptor);
}

bool ServerOperate::inMap(qintptr socketDescriptor) {
    return socket_timer.contains(socketDescriptor);
}

QTcpSocket *ServerOperate::getSocket(qintptr socketDescriptor) {
    QTcpSocket *socket = nullptr;
    for (int i = 0; i < clientSockets.size(); ++i) {
        if (clientSockets.at(i)->socketDescriptor() == socketDescriptor) {
            socket = clientSockets.at(i);
            return socket;
        }
    }
    emit logMessage("列表中不存在Socket对象,发送终止");
    return nullptr;
}


void ServerOperate::TimeControl(qintptr socketDescriptor) {

    QTimer *timer = nullptr;
    if (!inMap(socketDescriptor)) {
        QTimer *timer = new QTimer(this);
        timers.append(timer);
        socket_timer[socketDescriptor] = timer;
        qDebug("op169");
    }
//    Q_ASSERT(inMap(socketDescriptor));

    timer = socket_timer[socketDescriptor];

    // socket 对应的 timer* 已经创建

    qDebug() << "创建定时器:" << timer;

    //通过定时器来控制数据发送
    connect(timer, &QTimer::timeout, [=] {

        QTcpSocket *socket = getSocket(socketDescriptor);

        QFile *file = nullptr;

        file = socket_file.value(socketDescriptor);

        // socket 对应的 file* 已经创建

        if (!socket->isValid()) {
            doCancel(socketDescriptor);
            emit logMessage(QString("Socket不可操作，发送终止[%1:%2]")
                                    .arg(socket->peerAddress().toString())
                                    .arg(socket->peerPort()));
            return;
        }
        if (!file || !file->isOpen()) {
            doCancel(socketDescriptor);
//            emit logMessage("文件操作失败，发送终止");
            return;
        }

        qint64 sendSize = socket_sendsize.value(socketDescriptor);

        const qint64 read_size = file->read(fileBuffer, 4096);
        //socket->write(fileBuffer,read_size);
        sendFile(fileBuffer, read_size, socketDescriptor);
        sendSize += read_size;
        file->seek(sendSize);
        if (!socket->waitForBytesWritten()) {
            doCancel(socketDescriptor);
            emit logMessage("文件发送超时，发送终止");
            return;
        }

        socket_sendsize.insert(socketDescriptor, sendSize);
        //避免除零
        if (socket_filesize.value(socketDescriptor) > 0) {
            emit progressChanged(sendSize * 100 / socket_filesize.value(socketDescriptor));
        }
        if (sendSize >= socket_filesize.value(socketDescriptor)) {
            doCancel(socketDescriptor);
            emit logMessage("文件发送完成");
            emit progressChanged(100);
            sendData(0x03, QByteArray(), socketDescriptor);
            return;
        }
    });
}


void ServerOperate::doDislisten() {
    //关闭服务
    //由于 socket 放入 pendingsocket 池, 故不释放避免重复释放
    this->close();

    socket_file.clear();
    socket_timer.clear();
    socket_filesize.clear();
    socket_sendsize.clear();
    clientSockets.clear();
}

void ServerOperate::doCloseFile(qintptr socketDescriptor) {
    if (!socket_file.contains(socketDescriptor)) {
        qDebug() << "不存在该文件，关闭错误 [socketDescriptor:" << socketDescriptor;
        return;
    }
    QFile *file = socket_file.value(socketDescriptor);
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }
    socket_file.remove(socketDescriptor);
}

void ServerOperate::closeTimer(qintptr socketDescriptor) {
    if (!socket_timer.contains(socketDescriptor)) {
        qDebug() << "不存在该定时器，关闭错误 [socketDescriptor:" << socketDescriptor;
        return;
    }
    QTimer *timer = socket_timer.value(socketDescriptor);
    qDebug() << "删除定时器:" << timer;
    if (timer) {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }
    socket_timer.remove(socketDescriptor);
}


void ServerOperate::doCancel(qintptr socketDescriptor) {
    QFile *file = socket_file.value(socketDescriptor);
    if (file) {
        //关闭文件
        doCloseFile(socketDescriptor);
    }
}

// 获得当前 socket 传输的 url 及创建相应的 sendSize, fileSize
bool ServerOperate::isUrl(const QString &fileurl, qintptr socketDescriptor) {
    QFile *file = nullptr;
    if (socket_file.contains(socketDescriptor)) {
        QFile *file = socket_file.value(socketDescriptor);
    }
    if (file) {
        doCloseFile(socketDescriptor);
    }
    // 现在该套接字对象的 file 已经删除

    socket_sendsize[socketDescriptor] = 0;


    //创建qfile用于写文件
    QString file_path = fileurl;
    //无效路径
    if (file_path.isEmpty() || !QFile::exists(file_path)) {
        emit logMessage("无效的文件路径" + file_path);
        return false;
    }
    file = new QFile(this);
    file->setFileName(file_path);
    socket_file[socketDescriptor] = file;
    qDebug() << file_path << file;
    //打开失败
    if (!file->open(QIODevice::ReadOnly)) {
        doCloseFile(socketDescriptor);
        emit logMessage("打开文件失败" + file_path);
        return false;
    }
    qint64 fileSize = file->size();
    qDebug() << file << fileSize;
    socket_filesize[socketDescriptor] = fileSize;
    return true;
}

bool ServerOperate::readySendFile(qint64 size, qintptr socketDescriptor) {
    // 若发送回的文件大小和目前的大小不一样，则报错
    if (socket_filesize.value(socketDescriptor) != size) {
        emit logMessage("所需文件与目前不一致" + fileUrl);
        emit logMessage(QString("大小为[%1]").arg(size));
        return false;
    }
    return true;
}

void ServerOperate::sendFile(const char *data, int size, qintptr socketDescriptor) {
    QTcpSocket *socket = getSocket(socketDescriptor);
    if (!socket->isValid()) {
        return;
    }
    frameHead[6] = (char) 0x02;
    const quint64 data_size = size;
    frameHead[5] = data_size % 0x100;
    frameHead[4] = data_size / 0x100;

    //发送头+数据+尾
    socket->write(frameHead, 7);
    socket->write(data, size);
    socket->write(frameTail, 2);
}

void ServerOperate::sendData(char type, const QByteArray &data, qintptr socketDescriptor) {
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
    QTcpSocket *socket = getSocket(socketDescriptor);
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

void ServerOperate::operateReceiveData(const QByteArray &data, qintptr socketDescriptor) {
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
                //接收文件 Url
                //发送文件大小
//                fileUrl = QString::fromUtf8(dataTemp.constData() + 7, data_size);
//                emit showLineUrl(fileUrl);
                fileHash = QString::fromUtf8(dataTemp.constData() + 7, data_size);
                fileUrl = fromHash(fileHash);
                if (isUrl(fileUrl, socketDescriptor)) {
                    emit logMessage(QString("存在文件,大小为[%1]").arg(socket_filesize.value(socketDescriptor)));
                    //应答
                    char file_size[4] = {0};
                    const quint64 data_size = socket_filesize.value(socketDescriptor); //有符号转无符号，会被截断
                    file_size[3] = data_size >> 0 % 0x100;
                    file_size[2] = data_size >> 8 % 0x100;
                    file_size[1] = data_size >> 16 % 0x100;
                    file_size[0] = data_size >> 24;
                    QTimer *timer = nullptr;
                    if (!inMap(socketDescriptor)) {
                        qDebug("取消接收之后再接收");
                    }
                    sendData(0x01, QByteArray(file_size, 4), socketDescriptor);
                } else {
                    emit logMessage("搜索服务器文件失败");
                }
            }
                break;
            case 0x02: { //准备发送数据
                //直接发送数据能执行

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

                qDebug() << "Server端filesize:" <<
                         socket_filesize.value(socketDescriptor) << "  ||  "
                         << "Client端filesize:" << file_size;
                if (readySendFile(file_size, socketDescriptor)) {
                    QTimer *timer = socket_timer.value(socketDescriptor);
                    qDebug() << "开始传数据:" << timer;
                    timer->start(0);
                    emit logMessage("客户端已准备好发送数据，开始发送" + getFilePath());
                } else {
                    cancelFileTransfer(socketDescriptor);
                    emit logMessage("准备发送客户端文件失败");
                }
                break;
            }
            case 0x03: { //发送数据完成应答
                //1成功，0失败
                const bool result = (dataTemp[7] == (char) 0x01);
                emit logMessage(QString("服务器文件发送完毕，发送") + (result ? "成功" : "失败"));

                // 发送完重置对应的filesize 和 sendsize, 关闭文件
                socket_filesize.insert(socketDescriptor, 0);
                socket_sendsize.insert(socketDescriptor, 0);
                doCancel(socketDescriptor);
            }
                break;
            case 0x04: //客户端取消发送
                doCancel(socketDescriptor);
                emit logMessage("客户端取消发送，发送终止");
                // 重置对应的filesize 和 sendsize
                closeTimer(socketDescriptor);
                socket_filesize.insert(socketDescriptor, 0);
                socket_sendsize.insert(socketDescriptor, 0);
                break;
            default:
                break;
        }
        //移除处理完的字节
        dataTemp.remove(0, 7 + 2 + data_size);
    }
}


