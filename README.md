# FluentChat
基于FluentUI的大三小学期东软实训即时通讯软件Linpop

克隆仓库：
```shell
git clone --recursive https://github.com/flwfdd/FluentChat.git
```

然后使用`Qt Creator`打开根目录下的`CMakeList.txt`即可。

## 引入FluentUI

```shell
# 添加子模块
git submodule add https://github.com/zhuzichu520/FluentUI.git FluentUI
# 下载子模块
git submodule update --init --recursive
```

之后就可以直接在`.qml`中使用[FluentUI](https://github.com/zhuzichu520/FluentUI)了。一个引入例子可见[这个issue](https://github.com/zhuzichu520/FluentUI)。
