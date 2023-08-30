#ifndef SERVERFILETRANS_H
#define SERVERFILETRANS_H

#include <QObject>
#include <QThread>
#include "ServerOperate.h"

class ServerFileTrans : public QObject {
    Q_OBJECT
  public:
    explicit ServerFileTrans(QString ip, quint16 port, QObject *parent = nullptr);
    ~ServerFileTrans();


  signals:
    //使用信号槽操作线程中的ServerOperate
    void listen(const QString &address, quint16 port);
    void dislisten();

  private:
    //线程
    QThread *thread;
    //server处理放到线程中
    ServerOperate *operate;
    // ip 地址
    QString m_ip;
    // port
    quint16 m_port;
};

#endif // SERVERFILETRANS_H
