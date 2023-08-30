#ifndef SERVEROPERATE_H
#define SERVEROPERATE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QTimer>
#include <QRunnable>

//服务端socket处理--服务端作为发送
class ServerOperate : public QTcpServer {
    Q_OBJECT
  public:
    explicit ServerOperate(QObject *parent = nullptr);
    ~ServerOperate();

    //get/set 文件路径、监听状态等变量，使用了互斥锁
    QString getFilePath() const;

    void setFilePath(const QString &path);

    // 原本 QTcpServer 是成员，现在本身是 QTcpServer,
    // 把 widget 中 isListening 换成 wisListening, 否则无限递归且和 isListening 冲突
    bool wisListening() const;

    QString fromHash(const QString &fileHash);

    void hash2url(const QString &fileHash, std::function<QString(const QString &fileHash)>& fromHash);

  signals:
    //操作记录发送到ui显示
    void logMessage(const QString &msg);
    //服务端监听状态
    void listenStateChanged(bool isListen);
    //接收进度0-100
    void progressChanged(int value);
    // 发送文件 url 给主线程，以显示到 EditLine 上面
    void showLineUrl(const QString &url);
    // 发送析构信号,qt异步处理会造成重复析构?
    void closeAll(qintptr socketDescriptor);

  public slots:
    //server监听
    void doListen(const QString &address, quint16 port);
    //server取消监听
    void dislisten();
    //取消文件传输
    void cancelFileTransfer(qintptr socketDescriptor);

  private:
    //初始化
    void initOperate();
    //把槽对应的实际操作分离出来是为了复用，这样便于组合

    //关闭服务，断开socket连接，释放资源
    void doDislisten();

    // 如果socket对应的文件打开了执行关闭
    void doCloseFile(qintptr socketDescriptor);

    // 如果socket对应的定时器打开了执行关闭
    void closeTimer(qintptr socketDescriptor);

    // 取消传输的时候调用 doCloseFile
    void doCancel(qintptr socketDescriptor);

    // 获得当前 socket 传输的 url 及创建相应的 sendSize, fileSize
    // 后面对接的时候这个要改
    bool isUrl(const QString &fileurl, qintptr socketDescriptor);

    // 检测客户端发出的文件大小，若发送回的文件大小和目前的大小不一样，则报错
    bool readySendFile(qint64 size, qintptr socketDescriptor);

    /* 发送文件，包含
     * char* data:文件数据
     * int size:发送的文件大小
     * socketDescriptor: 标识符，区分不同 socket
     * 作业: 发送 0x02 帧类型，直接传输数据
    */
    void sendFile(const char *data, int size, qintptr socketDescriptor);

    /*传输协议
     * 帧结构：帧头4+帧长2+帧类型1+帧数据N+帧尾2（没有校验段，懒得写）
     * 帧头：4字节定值 0x0F 0xF0 0x00 0xFF
     * 帧长：2字节数据段长度值 arr[4]*0x100+arr[5] 前面为高位后面为低位
     * 帧类型：1字节
     * - 0x01 交流阶段，
     *        S->R:后跟四字节文件长度，长度计算同帧长一样前面为高位后面为低位
     *        R->S:后跟N字节文件 Url, 长度计算同帧长一样前面为高位后面为低位
     * - 0x02 发送者发送文件数据，接收者准备好接收数据
     * - 0x03 发送结束
     * - 0x04 取消发送
     * （服务端收到0x01 0x03开始和结束发送两个命令要进行应答，回同样的命令码无数据段）
     * 帧尾：2字节定值 0x0D 0x0A
     */
    void sendData(char type, const QByteArray &data, qintptr socketDescriptor);

    /* 处理接收的数据
     *收到0x01:接收文件 Url,  发送文件大小
     *收到0x02:接收文件大小进行校验, 发送文件数据
     *收到0x03:收到的数据段是 0x01则成功， 0x00 则失败
     *收到0x04:客户端取消发送
     */
    void operateReceiveData(const QByteArray &data, qintptr socketDescriptor);


    /*通过定时器来控制数据发送
     *如果map里面没 timer* 就 new 一个,有的话取出来用
     *取出 socketDescriptor 对应的 socket*, file*, sendsize
     *用 4096B 的 buffer 发送
     *并且更新进度条的值, 并发可以删除, 也可以加上一个下载列表
     */
    void TimeControl(qintptr socketDescriptor);

    /*之前用迭代处理是否在map中，后面发现有 contains 方法,懒得改了
     *返回map中是否有相应的timer*
     */
    bool inMap(qintptr socketDescriptor);

    /*获得列表中 socketDescriptor 对应的 socket*
     *没有的话返回空指针, 有风险
     */
    QTcpSocket* getSocket(qintptr socketDescriptor);

  protected:
    // 用来处理连接，有连接进来就会进入这个函数
    void incomingConnection(qintptr socketDescriptor) override;

  private:
    //用来锁文件路径、监听状态等变量
    mutable QMutex dataMutex;
    //文件存储路径
    QString fileUrl;
    // 文件 hash 值
    QString fileHash;
    //地址和端口
    QString address;
    quint16 port;

    //文件操作
    QFile* m_file = nullptr;
    // 使用队列存储套接字对象
    QList<QTcpSocket*> clientSockets;
    //发送数据的定时器
    QList<QTimer*> timers;
    // *socketDescriptor - *timer
    QMap<qintptr, QTimer*> socket_timer;
    // *socketDescriptor - *file
    QMap<qintptr, QFile*> socket_file;
    //发送的字节数，因为Qt接口是int64，本来想用无符号类型
    QMap<qintptr, qint64> socket_sendsize;
    //文件长度
    QMap<qintptr, qint64> socket_filesize;
    // 文件缓冲区, 每个人都设不同的缓冲区再用
    QMap<qintptr, char*> socket_fileBuffer;

//    //发送的字节数，因为Qt接口是int64，本来想用无符号类型
//    qint64 sendSize = 0;

    //接收缓存，实际操作时还是用char*好点
    QByteArray dataTemp;

    //读取文件到缓冲区
    char fileBuffer[4096] = {0};
    //帧头+长度+类型
    char frameHead[7] = { 0x0F, (char)0xF0, 0x00, (char)0xFF, 0x00, 0x00, 0x00 };
    //帧尾
    char frameTail[2] = { 0x0D, 0x0A };
};

#endif // SERVEROPERATE_H
