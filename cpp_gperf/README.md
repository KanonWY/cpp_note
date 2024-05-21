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

使用 perf record 记录某一个进程的数据采样信息

```bash
# perf record -g --call-graph dwarf -F99 -p 进程ID -o 输出文件名
Warning:
PID/TID switch overriding SYSTEM
参数说明：
-a         		all cpus
-F99       		采样频率 99HZ
-g         		统计调用栈
-p         		指定进程ID
-o            输出文件的名称
--call-graph  dwarf 追踪链的格式 dward | fp(default)
```
#### perf stat

收集某一个进程的性能计数器信息

```bash
perf stat -p 326689

 Performance counter stats for process id '326689':

             50.11 msec task-clock                #    0.009 CPUs utilized
             1,132      context-switches          #   22.589 K/sec
               366      cpu-migrations            #    7.304 K/sec
                 0      page-faults               #    0.000 /sec
        52,774,050      cycles                    #    1.053 GHz
        14,508,135      instructions              #    0.27  insn per cycle
         3,069,012      branches                  #   61.243 M/sec
           284,633      branch-misses             #    9.27% of all branches
       214,713,190      slots                     #    4.285 G/sec
         8,785,139      topdown-retiring          #      4.0% retiring
        67,075,408      topdown-bad-spec          #     30.5% bad speculation
       129,425,654      topdown-fe-bound          #     58.9% frontend bound
        14,460,471      topdown-be-bound          #      6.6% backend bound

       5.373774924 seconds time elapsed
```



- `task-clock`：进程的总运行时间，以毫秒为单位。
- `context-switches`：上下文切换的次数，表示进程在调度器中被切换的次数。
- `cpu-migrations`：CPU 迁移的次数，表示进程在不同 CPU 核之间迁移的次数。
- `page-faults`: 页错误指标
- `cycles`：CPU 执行的时钟周期数。
- `instructions`：执行的指令数。
- `branches`：分支指令的数量。
- `branch-misses`：分支预测失败的次数，表示分支预测错误的次数。
- `slots`：总的微操作数，表示 CPU 中执行的微操作的数量。
- `topdown-retiring`：顶层向下模型中的“retiring”事件，表示因顶层模型限制而未执行的指令数。
- `topdown-bad-spec`：顶层向下模型中的“bad speculation”事件，表示因顶层模型限制而导致的错误预测的指令数。
- `topdown-fe-bound`：顶层向下模型中的“frontend bound”事件，表示因前端限制而未执行的指令数。
- `topdown-be-bound`：顶层向下模型中的“backend bound”事件，表示因后端限制而未执行的指令数。

##### page-faults

`page-faults` 是 `perf stat` 输出中的一个指标，表示进程在执行过程中发生的页面错误（Page Faults）的次数。页面错误是指当进程访问的内存页面不在物理内存中时发生的情况，需要将相应的页面从磁盘加载到内存中，或者从其他进程共享的内存区域复制到当前进程的内存中。

页面错误通常分为两种类型：

1. **缺页中断（Page Fault）**：当进程访问的页面不在物理内存中时，会触发缺页中断，操作系统会将相应的页面从磁盘加载到内存中。这种情况通常发生在第一次访问某个内存页面时，或者当操作系统决定将某个页面置换出物理内存时。
2. **写时复制（Copy-on-Write）**：当多个进程共享同一块内存时，如果其中一个进程尝试写入该内存，操作系统会触发写时复制机制，将该内存页面复制一份，并将写入操作应用到新复制的页面上，以保护其他进程不受影响。

#### topdown 分析

##### 1、topdown-retiring

`topdown-retiring` 是 `perf stat` 输出中的一个指标，它是顶层向下（Top-down）性能分析模型中的一个事件，用于衡量 CPU 执行指令的效率。

在顶层向下的性能分析模型中，CPU 的性能瓶颈被分为三个部分：前端（Frontend）、后端（Backend）和退休（Retiring）。其中，`topdown-retiring` 表示 CPU 中由于顶层性能模型的限制而未执行的指令数量。

具体来说，`topdown-retiring` 指标反映了由于顶层性能模型的限制（比如分支预测、缓存访问延迟等）而未能执行的指令数量。在顶层向下的性能分析模型中，`topdown-retiring` 事件的数量越多，表示 CPU 中由于性能瓶颈而未执行的指令越多，可能意味着存在性能瓶颈。

具体来说，`topdown-retiring` 事件可能反映以下一些潜在的性能瓶颈：

1. **分支预测失败（Branch Prediction Misses）**：分支预测是 CPU 中常见的一种技术，用于预测程序中的分支指令的执行路径。当分支预测失败时，CPU 需要重新计算分支目标地址，导致指令未能按预期执行，从而增加了 `topdown-retiring` 事件的数量。
2. **缓存访问延迟（Cache Access Latency）**：缓存是 CPU 中用于加速内存访问的重要组成部分。当 CPU 访问缓存中未命中的数据时，需要从主存中加载数据，这会导致额外的延迟，从而增加了 `topdown-retiring` 事件的数量。
3. **内存访问延迟（Memory Access Latency）**：当 CPU 访问主存时，由于内存层级结构和总线带宽等因素，可能会产生额外的访问延迟，从而影响指令的执行速度，增加 `topdown-retiring` 事件的数量。
4. **其他微架构限制（Microarchitectural Limitations）**：CPU 的微架构设计中可能存在其他一些限制，比如指令调度、乱序执行、执行单元资源竞争等，都有可能导致 `topdown-retiring` 事件的增加。

##### 2、topdown-bad-spec

`topdown-bad-spec` 指标反映了由于 CPU 预测错误而导致的指令未能按预期执行的次数。这些错误的预测可能涉及到分支预测、数据依赖预测、指令级别的并行执行等方面。

##### 3、topdown-fe-bound

`topdown-fe-bound` 是 `perf stat` 输出中的另一个指标，它是顶层向下（Top-down）性能分析模型中的一个事件，用于衡量 CPU 中由于前端（Frontend）限制而未执行的指令数量。

`topdown-fe-bound` 事件反映了由于 CPU 前端性能限制而导致的指令未能按预期执行的次数。这些限制可能包括分支预测失败、指令缓存未命中、解码器繁忙等

##### 4、topdown-be-bound

`topdown-be-bound` 是 `perf stat` 输出中的另一个指标，它是顶层向下（Top-down）性能分析模型中的一个事件，用于衡量 CPU 中由于后端（Backend）限制而未执行的指令数量。

CPU 的后端主要负责指令的执行、数据处理和结果写回，它包括执行单元、乱序引擎、寄存器文件等部件。`topdown-be-bound` 事件反映了由于 CPU 后端性能限制而导致的指令未能按预期执行的次数。这些限制可能包括执行单元资源竞争、寄存器文件写回延迟等。

### perf top

查看系用性能采集和指定进程的性能采集

```bash
perf top -p pid
```

如何只想看用户空间函数和只想看内核空间函数

```bash
-U
-K
仅查看用户空间函数
perf top -p pid -K
仅查看内核空间函数
perf top -p pid -U
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
