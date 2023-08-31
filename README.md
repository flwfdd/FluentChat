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

然后使用`Qt Creator`打开根目录下的`CMakeList.txt`即可。启动后程序运行目录会有一个`.ini`可以配置服务端链接。

这里使用的版本是Qt 6.5.2的MinGW，安装时除了Android和WASM其他基本都选了，已在Windows和Linux上测试成功。

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
了。一个引入例子可见[这个issue](https://github.com/zhuzichu520/FluentUI)。
