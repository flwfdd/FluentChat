#ifndef CLIENTFILETRANS_H
#define CLIENTFILETRANS_H

#include <QObject>
#include <QThread>
#include "ClientOperate.h"

class ClientFileTrans : public QObject {
    Q_OBJECT
  public:
    explicit ClientFileTrans(QString ip, quint16 port, QObject *parent = nullptr);
    ~ClientFileTrans();

    // 开始文件传输
    void startTrans(const QString &fileHash, const QString &savePath);

    // 取消文件传输
    void cancelTrans();

    int getProgressValue() const;

    qint64 getFileSize() const;

    qint64 getReceiveSize() const;
    void setReceiveSize(qint64 newReceiveSize);

    void onSizeChanged(const std::function<void(qint64, qint64)>& callback);

  signals:
    //使用信号槽操作线程中的ClientOperate
    void connectTcp(const QString &address, quint16 port);
    void disconnectTcp();

    void receiveSizeChanged();

  private:
    //线程
    QThread *thread;
    //server处理放到线程中
    ClientOperate *operate;
    // 接收方 ip 地址
    QString m_ip;
    // port
    quint16 m_port;
    // progress
    int progressValue;
    Q_PROPERTY(int progressValue READ getProgressValue CONSTANT FINAL)
    //文件大小
    qint64 fileSize = 0;
    //已接收文件长度
    qint64 receiveSize = 0;
    Q_PROPERTY(qint64 receiveSize READ getReceiveSize WRITE setReceiveSize NOTIFY receiveSizeChanged FINAL)
    Q_PROPERTY(qint64 fileSize READ getFileSize CONSTANT FINAL)
    bool isCancel = false;
};

#endif // CLIENTFILETRANS_H
