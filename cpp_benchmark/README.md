## Google BenchMark

## 1、概述

性能对于C++来说至关重要，需要对C++写出的算法进行性能测试。本篇就是使用Google开源的`benchmark`工具进行性能测试的简单使用说明。

- 安装

```cpp
//In mac
brew install googlebenchmark
//In other
//克隆项目，编译安装
```

- 简单使用

```cpp
#include <benchmark/benchmark.h>
#include <chrono>
#inlcude <sstream>
#include <iomainip>
#include <string>

std::string GetNow()
{
  	auto now = std::chrono::system_clock::now();
  	auto t = std::chrono::system_clock::to_time_t(now);
  	std::stringstream ss;
  	ss << std::put_time(localtime(&t), "%Y-%m-%d %X");
  	return ss.str();
}

static void BM_TimerStringGet(benchmark::State& state)
{
  	for(auto _: state)
    {
      	auto it = GetNow();
    }
}

BENCHMARK(BM_TimerStringGet);
BENCHMARK_MAIN();
```

```bash
clang++ testBench.cpp -o testBench -std=c++11 -lbenchmark
./a.out

Running ./a.out
Run on (8 X 24.1215 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x8)
Load Average: 1.27, 1.51, 1.49
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
BM_TimeGet       1665 ns         1665 ns       412624
```

- 集成CMAKE中

先在系统中安装`Google benchmark`,然后链接到我们需要的`target`中。

```cmake
find_package(benchmark REQUIRED)

add_executable(MyTarget *.CPP)
target_link_libraries(MyTarget benchmark::benchmark)
```

### 参考

[user_guide](https://github.com/google/benchmark/blob/main/docs/user_guide.md)

[benchmark Git官网](https://github.com/google/benchmark)

[google benchmark](https://zhuanlan.zhihu.com/p/492920760)