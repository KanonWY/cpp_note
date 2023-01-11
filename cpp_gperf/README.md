## Use gperftools

[gperftools](https://xusenqi.site/2020/12/06/C++Profile%E7%9A%84%E5%A4%A7%E6%9D%80%E5%99%A8_gperftools%E7%9A%84%E4%BD%BF%E7%94%A8/)  
[Doc](https://gperftools.github.io/gperftools/tcmalloc.html)  
[collection](https://zhuanlan.zhihu.com/p/399999297)  
[tcmalloc大空闲内存不释放问题](https://blog.csdn.net/qq_16097611/article/details/118999271)  


### 1.prepare
```bash
sudo apt-get install -y libunwind-dev
git clone https://github.com/gperftools/gperftools.git
cd gperftools
cmake -S . -B build -G "Ninja"
cmake --build build
```
### 2.cpu
```cpp
ProfilerStart(filename);

other code

ProfilerStop();
```

### 3.heap
- profile info
more detail in [heapprofile.html](https://gperftools.github.io/gperftools/heapprofile.html) and source code.
```cpp
#include <gperftools/malloc_extension.h>

MallocExtension::instance()->GetStats(buffer, buffer_length);
MallocExtension::instance()->GetHeapSample(&string);
MallocExtension::instance()->GetHeapGrowthStacks(&string);
```
- check
more detail in [heap_checker.html](https://gperftools.github.io/gperftools/heap_checker.html) and source code.
```cpp
#include <gperftools/heap-checker.h>
HeapLeakChecker heap_checker("test_foo");
{
    code that exercises some foo functionality;
    this code should not leak memory;
}
if (!heap_checker.NoLeaks()) assert(NULL == "heap memory leak");
```

### 4.Important environment variables
#### 4.1 TCMALLOC_SAMPLE_PARAMETER
采样操作之间的近似间隔。It musr be set!也就是说，我们大约每分配tcmalloc_sample_parmeter字节取一次样本。`default: 0`, A reasonable value is 524288(512k).
Get This env var By `ReadStackTraces(&Nums)`.
```cpp
//Byte
int Nums = 0;
MallocExtension::instance()->ReadStackTraces(&Nums);
```

#### 4.2 TCMALLOC_RELEASE_RATE
`default = 1.0` ,Rate at which we release unused memory to the system, via madvise(MADV_DONTNEED), on systems that support it. Zero means we never release memory back to the system. Increase this flag to return memory faster; decrease it to return memory slower. Reasonable rates are in the range [0,10].
```cpp
//force a release at a given point in the progam execution like so:
MallocExtension::instance()->ReleaseFreeMemory();
```
You can also call `SetMemoryReleaseRate()` to change the tcmalloc_release_rate value at runtime, or `GetMemoryReleaseRate() `to see what the current release rate is.
```cpp
auto rate = MallocExtension::instance()->GetMemoryReleaseRate();
std::cout << "rate = " << rate << std::endl;

double rate_2 = 2;
MallocExtension::instance()->SetMemoryReleaseRate(rate_2);
```

#### 4.3 TCMALLOC_MAX_TOTAL_THREAD_CACHE_BYTES
**绑定到分配给线程缓存的总字节数**。此绑定并不严格，因此缓存在某些情况下可以跳过此绑定。此值默认为16MB。对于具有许多线程的应用程序，这可能不是足够大的缓存，这会影响性能。如果您怀疑应用程序由于TCMalloc中的锁争用而无法扩展到多个线程，可以尝试增加该值。这可能会提高性能，代价是TCMalloc使用额外的内存。
```cpp
default: 16777216 ====> 16M
```


### 5.获取当前内存使用情况
```cpp
//human-readable string!
MallocExtension::instance()->GetStats(buffer, buffer_length);

//get information from tcmalloc system!
// format as the heap-profiler, It can write to prof file which can use pprof anysis!
std::string str1
MallocExtension::instance()->GetHeapSample(&str1);

std::string str2
MallocExtension::instance()->GetHeapGrowthStacks(&str2);


// make string content to pprof format file!
static bool WriteProfFile(const char* filePath, std::string&& content)
{
    FILE* file = fopen(filePath, "w+");
    if (!file) {
        std::cout << "fopen error" << std::endl;
        return false;
    }
    bool ret = true;
    if (fwrite(content.data(), content.size(), 1UL, file) != 1UL) {
        std::cout << "fwrite error" << std::endl;
        ret = false;
    }
    fclose(file);
    return ret;
} 
```
### 6. Tcmalloc属性
TCMalloc has support for setting and retrieving arbitrary 'properties':
```cpp
MallocExtension::instance()->SetNumericProperty(property_name, value);
MallocExtension::instance()->GetNumericProperty(property_name, &value);
```
**Easy Wrap**:
```cpp
static std::vector<std::string> TcmallocProperty{
    "generic.current_allocated_bytes",
    "generic.heap_size",
    "tcmalloc.pageheap_free_bytes",
    "tcmalloc.pageheap_unmapped_bytes",
    "tcmalloc.slack_bytes",
    "tcmalloc.max_total_thread_cache_bytes",
    "tcmalloc.current_total_thread_cache_bytes"};

void DumpTcmallocProperty()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext == nullptr) {
        std::cout << "error malloc_ext is empty" << std::endl;
    }
    std::map<std::string, size_t> Property_Map;
    for (auto it : TcmallocProperty) {
        size_t value;
        malloc_ext->GetNumericProperty(it.c_str(), &value);
        Property_Map[it] = value;
    }
    for (auto it : Property_Map) {
        printf("%45s %ld\n", it.first.c_str(), it.second);
    }
}
```
### 7、perf火焰图
#### 7.1 分析脚本下载
火焰图是用图形化的方式来展现perf等工具采集的性能数据，对数据进行统计和分析，方便找出性能热点。这里选择使用Brendan D. Gregg大神开发的火焰图工具FlameGraph。  
[flamgraphs官网](https://www.brendangregg.com/flamegraphs.html)  
```bash
git clone https://github.com/brendangregg/FlameGraph.git
#将这些脚本export到PATH中。
```
#### 7.2 记录指定进程的数据信息
```bash
# perf record -a -g -F99 -p 进程ID sleep 20
Warning:
PID/TID switch overriding SYSTEM
参数说明：
-a         all cpus
-F99       采样频率99HZ
-g         统计调用栈
-p         指定进程ID
sleep      统计时间，单位秒
```
#### 7.3 perf report分析数据
`perf report`简单分析数据
```bash
perf report -i perf.data
Samples: 1K of event 'cpu-clock:pppH', Event count (approx.): 12323232200
  Children      Self  Command  Shared Object      Symbol
+  100.00%     0.00%  main     main               [.] _start
+  100.00%     0.00%  main     libc.so.6          [.] __libc_start_main_impl (inlined)
+  100.00%     0.00%  main     libc.so.6          [.] __libc_start_call_main
+  100.00%     0.00%  main     main               [.] main
+  100.00%     0.00%  main     main               [.] example_test_heap_growth_profile
+  100.00%     0.00%  main     main               [.] func3
+   67.38%    67.30%  main     main               [.] func2
+   32.62%    32.62%  main     main               [.] func1
```
`perf report | more`查看函数调用
```bash
perf report -i perf.data | more
# Children      Self  Command  Shared Object      Symbol
# ........  ........  .......  .................  ....................................
#
   100.00%     0.00%  main     main               [.] _start
            |
            ---_start
               __libc_start_main_impl (inlined)
               __libc_start_call_main
               main
               example_test_heap_growth_profile
               func3
```
#### 7.4 火焰图生成
`perf record`的默认采集方式为：`event task-clock`,可以使用`ctrl+c`来结束采样;也可以使用sleep time使用
时间采样。  
使用`dwarf`格式函数展开可以更好的显示调用函数名，以及调用栈。
```bash
--call-graph dwarf        避免后面生成的火焰图有大量unknown函数名
```
```bash
采样结果为perf.data
（1）
```
