```bash
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c model.cc

// 查看对象布局

clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c model.cc

// 查看虚函数表布局 作者：沐歌爱编程 https://www.bilibili.com/read/cv7542132/ 出处：bilibili
```

## 空基类优化对于智能指针的删除器的优化
[EBO](https://developer.aliyun.com/article/983223)  
