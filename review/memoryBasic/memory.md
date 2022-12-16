```bash
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc

// 查看对象布局

clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c model.cc

// 查看虚函数表布局 作者：沐歌爱编程 https://www.bilibili.com/read/cv7542132/ 出处：bilibili
```