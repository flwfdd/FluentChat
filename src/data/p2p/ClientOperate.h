#ifndef CLIENTOPERATE_H
#define CLIENTOPERATE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QTimer>

//客户端socket处理--客户端作为接收
//帧没有数据校验字段
//没有流量控制
//没有错误数据段重发机制
//没有确认返回超时机制
//没有流程状态记录，如果当前正在发送却收到开始发送的错误命令，就乱套了
class ClientOperate : public QObject {
    Q_OBJECT
  public:
    explicit ClientOperate(QObject *parent = nullptr);
    ~ClientOperate();

    //get/set 文件路径、连接状态等变量，使用了互斥锁
    QString getFilePath() const;
    void setFilePath(const QString &path);

    QString getSavePath() const;
    void setSavePath(const QString &path);

    bool isConnected() const;
    void setConnected(bool connected);

    qint64 getFileSize() const;
    void setFileSize(qint64 newFileSize);

    qint64 getReceiveSize() const;
    void setReceiveSize(qint64 newReceiveSize);

signals:
    //操作记录发送到ui显示
    void logMessage(const QString &msg);
    //连接状态改变
    void connectStateChanged(bool isConnect);
    //接收进度0-100
    void progressChanged(int value);

    void fileSizeChanged(qint64 value);

    void receiveSizeChanged(qint64 value);

public slots:
    //连接
    void connectTcp(const QString &address, quint16 port);
    //断开连接
    void disconnectTcp();
    //传输文件
    bool startFileTransfer();
    //取消文件传输
    void cancelFileTransfer();

  private:
    //初始化
    void initOperate();
    //把槽对应的实际操作分离出来是为了复用，这样便于组合
    void doDisconnect();
    void doCloseFile();
    void doCancel();
    bool readyReceiveFile(qint64 size);
    void onReceiveFile(const char *data, qint64 size);
    void sendData(char type, const QByteArray &data);
    void operateReceiveData(const QByteArray &data);

  private:
    //用来锁文件路径、连接状态等
    mutable QMutex dataMutex;
    //所求文件路径
    QString filePath;
    // 存储文件路径,缺少文件名
    QString savePath;
    //地址和端口
    QString m_address;
    quint16 m_port;
    //连接状态
    bool connectState = false;
    //套接字
    QTcpSocket *socket = nullptr;
    //文件操作
    QFile *file = nullptr;
    //文件大小
    qint64 fileSize = 0;
    //已接收文件长度
    qint64 receiveSize = 0;
    //接收数据的缓存，实际操作时还是用char*好点
    QByteArray dataTemp;

    //帧头+长度+类型
    char frameHead[7] = { 0x0F, (char)0xF0, 0x00, (char)0xFF, 0x00, 0x00, 0x00 };
    //帧尾
    char frameTail[2] = { 0x0D, 0x0A };
    Q_PROPERTY(qint64 fileSize READ getFileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
    Q_PROPERTY(qint64 receiveSize READ getReceiveSize WRITE setReceiveSize NOTIFY receiveSizeChanged FINAL)
};

#endif // CLIENTOPERATE_H
