![](https://s2.loli.net/2023/08/31/rNUhg6yKeGQ7Jif.png)

# FluentChat

基于FluentUI的大三小学期东软实训即时通讯软件Linpop

客户端基于`Qt`+`QML`+`SQlite`，与后端通过`HTTP`和`WebSocket`进行通讯，客户端之间可以通过`TCP Sockete`进行`P2P`内网文件传输。

API请见[Apifox](https://apifox.com/apidoc/shared-db263d95-ca73-4423-868d-a06de529859b)。

* 可配置主题
* 大量非线性动效
* 离线消息记录
* 在线提示（头像旁的小点）
* 图片、文件、`P2P`文件同传、截屏

![](https://s2.loli.net/2023/08/31/QFRTnBpP4lZKIOE.png)

![](https://s2.loli.net/2023/08/31/meu7rbQp5XRc4oS.png)

![](https://s2.loli.net/2023/08/31/dQJZMcVKYHI87ra.png)

## 使用方法

克隆仓库：

```shell
git clone --recursive https://github.com/flwfdd/FluentChat.git
```

然后使用`Qt Creator`打开根目录下的`CMakeList.txt`即可。启动后程序运行目录下会有`fluentchat.ini`配置文件和`fluentchat.db`的`sqlite`数据库。

这里使用的版本是`Qt 6.5.2`的`MinGW`，安装时除了`Android`和`WASM`其他基本都选了，已在`Windows`和`Linux`上测试成功。

然后[服务端](https://github.com/3RepresentsTheory/IMback/tree/sqlite_ver)，启动！注意是`sqlite_ver`分支，启动后`WebSocket`监听`1234`端口，`HTTP`监听`1235`端口。 （服务端启动后也会自动生成一个`test.db`，不过是在可执行文件所在目录的再上一级目录）

如果更改了服务端监听端口或服务端不在本地，则需要更改（或新建）一个`fluentchat.ini`配置文件在客户端可执行文件的同目录下，内容形如：
```
[General]
httpURL=http://127.0.0.1:1235
wsURL=ws://127.0.0.1:1234
```

另外可以看看配套的[服务端仓库](https://github.com/3RepresentsTheory/IMback)
和内嵌的[P2P文件互传模块仓库](https://github.com/PulsarRX/Qtcptransfile)。

⚠️小学期最后的通宵夺走了我所有的优雅。懂不懂`BASE64`+`WebSocket`
传文件的含金量啊！这已然是一坨摇摇欲坠的屎山，上面有自由自在的野指针在欢乐地嬉戏。最后的最后，我注释了所有的`delete`
，而没有留下任何的自然语言注释。

## 引入FluentUI

```shell
# 添加子模块
git submodule add https://github.com/zhuzichu520/FluentUI.git FluentUI
# 下载子模块
git submodule update --init --recursive
```

之后就可以直接在`.qml`中使用[FluentUI](https://github.com/zhuzichu520/FluentUI)
了。一个引入例子可见[这个issue](https://github.com/zhuzichu520/FluentUI/issues/142)。
